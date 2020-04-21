#ifndef __HSC_H__
#define __HSC_H__

#include <stdbool.h>
#include "i2c_api.h"

typedef struct {
	bool has_diagnostic;
	float pressure_max;
	float pressure_min;
	int output_max;
	int output_min;
	float(*conv_fn)(float);
} hsc_sensor_t;

hsc_sensor_t HSCDANN150PG2A5;
hsc_sensor_t HSCMAND160MD2A5;

typedef struct {
	hsc_sensor_t cfg;
	int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
		       i2c_xfer_cb_t i2c_xfer_cb, void *arg);
	int addr;
	bool first_data;
	int last_temperature;
	int last_pressure;

	/* buffers for async I2C xfers */
	float *async_temperature_ptr;
	float *async_pressure_ptr;
	void (*async_read_cb)(int status);
	uint8_t hsc_word[4];
	i2c_xfer_t read_xfer;
} hsc_handle_t;

int hsc_read(hsc_handle_t *h, float *pressure_bar, float *temperature_c, void (*read_cb)(int status));
int hsc_config_device(hsc_handle_t *h, hsc_sensor_t *cfg);
int hsc_init(hsc_handle_t *h,
	     int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
			     i2c_xfer_cb_t i2c_xfer_cb, void *arg), int addr);
#endif
