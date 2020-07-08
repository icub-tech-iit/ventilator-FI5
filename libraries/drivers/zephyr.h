#ifndef __ZEPHYR_H__
#define __ZEPHYR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "i2c_api.h"

typedef struct {
	int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
		       i2c_xfer_cb_t i2c_xfer_cb, void *arg);
	int addr;
	bool init_ok;
	uint32_t sn;

	/* buffers for async I2C xfers */
	uint16_t *async_flow_ptr;
	uint16_t *async_raw_flow_ptr;
	void (*async_read_cb)(int status);
	uint8_t async_zephyr_word[2];
	i2c_xfer_t async_read_xfer;
} zephyr_handle_t;

int zephyr_get_sn(zephyr_handle_t *h, uint32_t *sn);
int zephyr_read(zephyr_handle_t *h, uint16_t *flow, uint16_t *raw_flow,
		void (*read_cb)(int status));
int zephyr_init(zephyr_handle_t *h,
		int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
			       i2c_xfer_cb_t i2c_xfer_cb, void *arg), int addr);
                   
#ifdef __cplusplus
}
#endif                   

#endif
