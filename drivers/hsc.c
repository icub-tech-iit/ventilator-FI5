/* driver for honeywell HSC sensors */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include "hsc.h"

#define HSC_STATUS_OK 0
#define HSC_STATUS_STALE 2

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
	int tmp;
	*hsc_status = hsc_word[0] >> 6;
	*hsc_pressure = ((hsc_word[0] & 0x3f) << 8) | hsc_word[1];
	*hsc_temperature = (hsc_word[2] << 3) | (hsc_word[3] >> 5);
}


int hsc_read(hsc_handle_t *h, float *pressure_bar, float *temperature_c)
{
	int ret;
	float pres;
	uint8_t hsc_word[4];
	int hsc_pressure, hsc_temperature, hsc_status;
	i2c_xfer_t xfer = {
		.direction = READ,
		.len = 4,
		.buf = hsc_word,
	};

	i2c_xfer_list_t xfers = {
		.xfer_num = 1,
		.xfers = &xfer
	};

	ret = h->i2c_xfer(&xfers, h->addr);
	if (ret)
		return ret;

	hsc_unpack(hsc_word, &hsc_status, &hsc_pressure, &hsc_temperature);

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
				return -EIO;
			}
			break;
		default:
			return -EIO;
		}

		h->last_temperature = hsc_temperature;
		h->last_pressure = hsc_pressure;
		h->first_data = false;
	}

	/* sanity check for max/min expected code from sensor */
	if ((hsc_pressure > h->cfg.output_max) ||
	    (hsc_pressure < h->cfg.output_min))
		return -EIO;

	*pressure_bar = hsc_calc_pressure(h, hsc_pressure);
	*temperature_c = hsc_calc_temperature(h, hsc_temperature);

	return 0;
}


int hsc_config_device(hsc_handle_t *h, hsc_sensor_t *cfg)
{
	h->cfg = *cfg;
}


int hsc_init(hsc_handle_t *h,
	     int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr), int addr)
{
	h->i2c_xfer = i2c_xfer;
	h->addr = addr;
	h->first_data = true;
}
