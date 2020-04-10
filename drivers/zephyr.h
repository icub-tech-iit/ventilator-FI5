#ifndef __ZEPHYR_H__
#define __ZEPHYR_H__

#include <stdbool.h>
#include <stdint.h>
#include "i2c_api.h"

typedef struct {
	int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr);
	int addr;
	bool init_ok;
	uint32_t sn;
} zephyr_handle_t;

int zephyr_get_sn(zephyr_handle_t *h, uint32_t *sn);
int zephyr_read(zephyr_handle_t *h, uint16_t *flow);
int zephyr_init(zephyr_handle_t *h,
		int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr), int addr);

#endif
