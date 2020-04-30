/* driver for honeywell HSC sensors */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include "hsc.h"

#define HSC_STATUS_OK 0
#define HSC_STATUS_STALE 2

#if defined(__ARMCOMPILER_VERSION)
    #if !defined(EIO)
    #define EIO 5
    #endif
#endif

static float psi2bar(float psi);

hsc_sensor_t HSCDANN150PG2A5 = {
	.has_diagnostic = false,
	.pressure_max = 150,
	.pressure_min = 0,
	.output_max = 0x3999,
	.output_min = 0x666,
	.conv_fn = psi2bar,
};

hsc_sensor_t HSCMAND160MD2A5 = {
	.has_diagnostic = true,
	.pressure_max = 0.160,
	.pressure_min = -0.160,
	.output_max = 0x3999,
	.output_min = 0x666,
	.conv_fn = NULL,
};

hsc_sensor_t HSCSANN100PA2A5 = {
	.has_diagnostic = false,
	.pressure_max = 100,
	.pressure_min = 0,
	.output_max = 0x3999,
	.output_min = 0x666,
	.conv_fn = psi2bar,
};

static float psi2bar(float psi)
{
	return psi * 0.0689476;
}

/* convert pressure code read from sensor to pressure in bar */
static float hsc_calc_pressure(hsc_handle_t *h, int code)
{
	float n = ((float)(code - h->cfg.output_min) *
		   (float)(h->cfg.pressure_max - h->cfg.pressure_min));
	float d = (h->cfg.output_max - h->cfg.output_min);
	float pres = n / d + h->cfg.pressure_min;
	if (!h->cfg.conv_fn)
		return pres;
	return h->cfg.conv_fn(pres);
}

/* convert temperature code read from sensor to temperature in Celsius */
static float hsc_calc_temperature(hsc_handle_t *h, int code)
{
	return (float)code / 2047 * 200 - 50;
}

static void hsc_unpack(uint8_t hsc_word[4],
		       int *hsc_status, int *hsc_pressure, int *hsc_temperature)
{
	*hsc_status = hsc_word[0] >> 6;
	*hsc_pressure = ((hsc_word[0] & 0x3f) << 8) | hsc_word[1];
	*hsc_temperature = (hsc_word[2] << 3) | (hsc_word[3] >> 5);
}

/* internal cb called by the I2C layer upon async xfer completion */
static void hsc_read_cb(int ret, void *arg)
{
	hsc_handle_t *h = arg;
	int hsc_pressure, hsc_temperature, hsc_status;

	if (ret)
		goto exit;

	hsc_unpack(h->hsc_word, &hsc_status, &hsc_pressure, &hsc_temperature);

	/* if sensor sends diagnostic status then check it */
	if (h->cfg.has_diagnostic) {
		switch (hsc_status) {
		case HSC_STATUS_OK:
			/* good data */
			break;

		case HSC_STATUS_STALE:
			/* old data flag */
			if (h->first_data)
				break;
			/* we can do consistency check wrt last data */
			if ((hsc_temperature != h->last_temperature) ||
			    (hsc_pressure != h->last_pressure)) {
				/*
				 * We don't know which read was incorrect, so
				 * clear any assumption on last good value and
				 * start over..
				 */
				h->first_data = true;
				ret = -EIO;
				goto exit;
			}
			break;
		default:
			ret = -EIO;
			goto exit;
		}

		h->last_temperature = hsc_temperature;
		h->last_pressure = hsc_pressure;
		h->first_data = false;
	}

	/* sanity check for max/min expected code from sensor */
	if ((hsc_pressure > h->cfg.output_max) ||
	    (hsc_pressure < h->cfg.output_min)) {
		ret = -EIO;
		goto exit;
	}

	/* success */
	if (h->async_raw_pressure_ptr)
		*h->async_raw_pressure_ptr = hsc_pressure;
	if (h->async_raw_temperature_ptr)
		*h->async_raw_temperature_ptr = hsc_temperature;
	if (h->async_pressure_ptr)
		*h->async_pressure_ptr = hsc_calc_pressure(h, hsc_pressure);
	if (h->async_temperature_ptr)
		*h->async_temperature_ptr =
			hsc_calc_temperature(h, hsc_temperature);

exit:
	if(h->async_read_cb)
		h->async_read_cb(ret);

}


/* public API for triggering read */
int hsc_read(hsc_handle_t *h, float *pressure_bar, float *temperature_c,
	     uint16_t *pressure_raw, uint16_t *temperature_raw,
	     void (*read_cb)(int status))
{
	i2c_xfer_list_t xfers = {
		.xfer_num = 1,
		.xfers = &h->read_xfer
	};

	h->async_pressure_ptr = pressure_bar;
	h->async_temperature_ptr = temperature_c;
	h->async_raw_pressure_ptr = pressure_raw;
	h->async_raw_temperature_ptr = temperature_raw;

	h->async_read_cb = read_cb;

	return h->i2c_xfer(&xfers, h->addr, hsc_read_cb, h);
}

int hsc_config_device(hsc_handle_t *h, hsc_sensor_t *cfg)
{
	h->cfg = *cfg;

	return 0;
}


int hsc_init(hsc_handle_t *h,
	     int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
			    i2c_xfer_cb_t i2c_xfer_cb, void *arg),
	     int addr)
{
	h->i2c_xfer = i2c_xfer;
	h->addr = addr;
	h->first_data = true;

	/* initialize read xfer descriptor: we can recycle this one forever */
	h->read_xfer.direction = READ;
	h->read_xfer.len = 4;
	h->read_xfer.buf = h->hsc_word;

	return 0;
}
