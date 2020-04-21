/* driver for mcp23017 IO expander */

#include <errno.h>
#include <stdio.h>
#include "delay.h"
#include "mcp23017.h"

#define MCP23017_DIRECTION_REG 	0x00
#define MCP23017_PUP_REG 	0x0C
#define MCP23017_PORT_REG 	0x12

static uint16_t mcp23017_unpack_data(uint8_t val[2])
{
	uint16_t tmp;

	tmp = val[0];
	tmp |= (uint16_t)val[1] << 8;
	return tmp;
}


/* internal read callback */
static void mcp23017_read_cb(int ret, void *arg)
{
	mcp23017_handle_t *h = arg;

	if (!ret)
		*h->async_ptr = mcp23017_unpack_data(h->xfer_r_buf);

	h->async_cb(ret);
}


/* internal write callback */
static void mcp23017_write_cb(int ret, void *arg)
{
	mcp23017_handle_t *h = arg;

	h->async_cb(ret);
}


/* read a reg on the I2C */
int mcp23017_read_reg(mcp23017_handle_t *h, uint8_t reg, uint16_t *val,
		void (*read_cb)(int status))
{
	int ret;
	uint16_t tmp;
	i2c_xfer_list_t xfers = {
		.xfer_num = 2,
		.xfers = h->xfer
	};

	h->xfer[0].len = 1;
	h->xfer_w_buf[0] = reg;

	if (read_cb) {
		h->async_cb = read_cb;
		h->async_ptr = val;
		ret = h->i2c_xfer(&xfers, h->addr, mcp23017_read_cb, h);
	} else {
		ret = h->i2c_xfer(&xfers, h->addr, NULL, NULL);
		if (!ret)
			*val = mcp23017_unpack_data(h->xfer_r_buf);
	}
	return ret;
}


/* write a reg on the I2C */
static int mcp23017_write_reg(mcp23017_handle_t *h, uint8_t reg, uint16_t val,
			      void (*write_cb)(int status))
{
	int ret;
	i2c_xfer_list_t xfers = {
		.xfer_num = 1,
		.xfers = h->xfer
	};

	h->xfer[0].len = 3;
	h->xfer_w_buf[0] = reg;
	h->xfer_w_buf[1] = val & 0xff;
	h->xfer_w_buf[2] = (val >> 8) & 0xff;

	if (write_cb) {
		h->async_cb = write_cb;
		ret = h->i2c_xfer(&xfers, h->addr, mcp23017_write_cb, h);
	} else {
		ret = h->i2c_xfer(&xfers, h->addr, NULL, NULL);
	}
	return ret;
}


int mcp23017_write(mcp23017_handle_t *h,
		  uint16_t port,
		  void (*write_cb)(int status))
{
	return mcp23017_write_reg(h, MCP23017_PORT_REG, port, write_cb);
}


int mcp23017_read(mcp23017_handle_t *h,
		  uint16_t *port,
		  void (*read_cb)(int status))
{
	return mcp23017_read_reg(h, MCP23017_PORT_REG, port, read_cb);
}


/* Configure the chip. Currently only direction */
int mcp23017_config_device(mcp23017_handle_t *h, mcp23017_cfg_t *cfg)
{
	int ret;
	int i;

	/* PUPs can be acrivated only on input pins */
	if ((cfg->pup & cfg->direction) != cfg->pup)
		return -EINVAL;

	ret = mcp23017_write_reg(h, MCP23017_DIRECTION_REG,
				 cfg->direction, NULL);
	if (ret)
		return ret;

	return mcp23017_write_reg(h, MCP23017_PUP_REG, cfg->pup, NULL);
}


int mcp23017_init(mcp23017_handle_t *h,
		int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
			       i2c_xfer_cb_t i2c_xfer_cb, void *arg), int addr)
{
	uint32_t sn;
	int ret;

	h->i2c_xfer = i2c_xfer;
	h->addr = addr;

	/*
	 * Initialize xfer descriptors: we can recycle those forever.
	 * First xfer might be a 3-byte write or a 1-byte write.
	 * In the first case we are ok, otherwise there is another 2-byte
	 * read xfer
	 */
	h->xfer[0].direction = WRITE;
	h->xfer[0].buf = h->xfer_w_buf;

	h->xfer[1].direction = READ;
	h->xfer[1].buf = h->xfer_r_buf;
	h->xfer[1].len = 2;

	return 0;
}
