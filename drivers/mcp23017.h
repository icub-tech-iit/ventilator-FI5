#ifndef __MCP23017_H__
#define __MCP23017_H__

#include <stdint.h>
#include "i2c_api.h"

typedef struct {
	uint16_t direction;
	uint16_t pup;
} mcp23017_cfg_t;

typedef struct {
	int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
		       i2c_xfer_cb_t i2c_xfer_cb, void *arg);
	int addr;

	/* buffers for (a)sync I2C xfers */
	uint16_t *async_ptr;
	void (*async_cb)(int status);
	i2c_xfer_t xfer[2];
	uint8_t xfer_w_buf[3];
	uint8_t xfer_r_buf[2];
} mcp23017_handle_t;

int mcp23017_read(mcp23017_handle_t *h, uint16_t *port, void (*read_cb)(int status));

int mcp23017_write(mcp23017_handle_t *h, uint16_t port, void (*write_cb)(int status));

int mcp23017_config_device(mcp23017_handle_t *h, mcp23017_cfg_t *cfg);
int mcp23017_init(mcp23017_handle_t *h,
	     int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
			     i2c_xfer_cb_t i2c_xfer_cb, void *arg), int addr);
#endif
