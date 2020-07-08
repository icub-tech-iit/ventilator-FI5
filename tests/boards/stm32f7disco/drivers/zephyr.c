/* driver for honeywell zephyr sensors */

#include <errno.h>
#include <stdio.h>
#include "delay.h"
#include "zephyr.h"

#if defined(__ARMCOMPILER_VERSION)

    #if !defined(EIO)
    #define EIO 5
    #endif

#endif

#define ZEPHYR_RESPONSE_ACK 0xCCA5
#define ZEPHYR_SN_CMD 0x1
#define ZEPHYR_RESET_CMD 0x2
#define ZEPHYR_CSUM_CMD 0x3

static int zephyr_read_word(zephyr_handle_t *h, uint16_t *word);
static int zephyr_write_cmd(zephyr_handle_t *h, uint8_t cmd);


static int zephyr_calc_mslpm(int val)
{
	const int full_range = 50; /* SLPM */
	int tmp = (float)full_range * ((float)val / 16384.0f - 0.1f) / 0.8f * 1000.0f;

	if (tmp < 0)
		tmp = 0;

	if (tmp > (full_range * 1000))
		tmp = full_range * 1000;
	return tmp;
}


static uint16_t zephyr_unpack_word(uint8_t zephyr_word[2])
{
	return ((uint16_t)zephyr_word[0] << 8) |
		(uint16_t)zephyr_word[1];
}


/* provide the device SN as read and verified in the initialization procedure */
int zephyr_get_sn(zephyr_handle_t *h, uint32_t *sn)
{
	if (!h->init_ok)
		return -EIO;

	*sn = h->sn;
	return 0;
}


/* internal read callback */
static void zephyr_read_cb(int ret, void *arg)

{
	uint16_t tmp;
	zephyr_handle_t *h = arg;

	if (ret)
		goto exit;

	tmp = zephyr_unpack_word(h->async_zephyr_word);

	/* we expect regular sensor data; 2 MSBs has to be zero */
	if (tmp & 0xC000)
		ret = -EIO;
	else {
		if (h->async_raw_flow_ptr)
			*h->async_raw_flow_ptr = tmp;
		if (h->async_flow_ptr)
			*h->async_flow_ptr = zephyr_calc_mslpm(tmp);
	}

exit:
	h->async_read_cb(ret);
}


/* read and provide the flow measured by the device */
int zephyr_read(zephyr_handle_t *h, uint16_t *flow, uint16_t *raw_flow,
		void (*read_cb)(int status))
{
	if (!h->init_ok)
		return -EIO;

	i2c_xfer_list_t xfers = {
		.xfer_num = 1,
		.xfers = &h->async_read_xfer
	};

	h->async_read_cb = read_cb;
	h->async_flow_ptr = flow;
	h->async_raw_flow_ptr = raw_flow;
	return h->i2c_xfer(&xfers, h->addr, zephyr_read_cb, h);
}


/* trigger a SW reset and wait for completion */
static int zephyr_reset(zephyr_handle_t *h)
{
	int ret;

	ret = zephyr_write_cmd(h, ZEPHYR_RESET_CMD);
	if (ret)
		return ret;
	/*
	 * Zephyr datasheet declare 20mS as max response time for reset command.
	 * We add some extra margin..
	 */
	mdelay(25);
	return 0;
}


/* read a word from the I2C */
static int zephyr_read_word(zephyr_handle_t *h, uint16_t *word)
{
	int ret;
	uint8_t zephyr_word[2];

	i2c_xfer_t xfer = {
		.direction = READ,
		.len = 2,
		.buf = zephyr_word,
	};

	i2c_xfer_list_t xfers = {
		.xfer_num = 1,
		.xfers = &xfer
	};

	ret = h->i2c_xfer(&xfers, h->addr, NULL, NULL);
	if (ret)
		return ret;

	*word = zephyr_unpack_word(zephyr_word);

	return 0;
}


/*
 * Reads the SN from the HW. NOTE: this function does not trigger the
 * device for making it sending out the SN
 */
static int zephyr_read_sn(zephyr_handle_t *h, uint32_t *sn)
{
	int ret;
	uint16_t tmp;
	uint32_t _sn;

	ret = zephyr_read_word(h, &tmp);
	if (ret)
		return ret;

	_sn = (uint32_t)tmp << 16;

	/*
	 * Datasheet recommends to wait for 10mS before performing second read
	 * when we read the SN that the device sends after reset. This does not
	 * seem a max time that the command might take to execute, rather it
	 * seems like an hard requirement, so we just do that.
	 * Doing that unconditionally (i.e. when explictly reading SN) shouldn't
	 * hurt and We also add some extra margin.
	 */
	mdelay(15);
	ret = zephyr_read_word(h, &tmp);
	if (ret)
		return ret;

	_sn |= tmp;
	*sn = _sn;

	return 0;
}


static int zephyr_write_cmd(zephyr_handle_t *h, uint8_t cmd)
{
	i2c_xfer_t xfer = {
		.direction = WRITE,
		.len = 1,
		.buf = &cmd,
	};

	i2c_xfer_list_t xfers = {
		.xfer_num = 1,
		.xfers = &xfer
	};

	return h->i2c_xfer(&xfers, h->addr, NULL, NULL);
}


static int zephyr_verify_csum(zephyr_handle_t *h)
{
	int ret;
	uint16_t tmp;

	ret = zephyr_write_cmd(h, ZEPHYR_CSUM_CMD);
	if (ret)
		return ret;

	/* Checking the CSUM can take up to a second. We add some extra margin */
	mdelay(1010);
	ret = zephyr_read_word(h, &tmp);
	if (ret)
		return ret;

	if (tmp != ZEPHYR_RESPONSE_ACK)
		return -EIO;

	return 0;
}


/*
 * Explicitly ask for the sn to the device, verify whether it matches with
 * the SN we got from the priming procedure.
 */
static int zephyr_verify_sn(zephyr_handle_t *h)
{
	uint32_t sn;
	int ret;

	ret = zephyr_write_cmd(h, ZEPHYR_SN_CMD);
	if (ret)
		return ret;

	/*
	 * datasheet says that reading SN could take up to 10mS. We add
	 * some extra margin here
	 */
	mdelay(15);
	ret = zephyr_read_sn(h, &sn);
	if (ret)
		return ret;

	if (sn != h->sn)
		return -EIO;

	return 0;
}


int zephyr_init(zephyr_handle_t *h,
		int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr,
			       i2c_xfer_cb_t i2c_xfer_cb, void *arg), int addr)
{
	uint32_t sn;
	int ret;

	h->init_ok = false;
	h->i2c_xfer = i2c_xfer;
	h->addr = addr;

	/* initialize read xfer descriptor: we can recycle this one forever */
	h->async_read_xfer.direction = READ;
	h->async_read_xfer.len = 2;
	h->async_read_xfer.buf = h->async_zephyr_word;

	/*
	 * Ensure initial startup time is elapsed. No assumption about elapsed
	 * time since POR.
	 * The datasheet specify 35mS warm time
	 */
	mdelay(40);

	/* reset sensor to ensure it's in a known state */
	ret = zephyr_reset(h);
	if (ret)
		return ret;

	/* after reset, sensor sends SN, then it's ready */
	ret = zephyr_read_sn(h, &sn);
	if (ret)
		return ret;

	h->sn = sn;
	ret = zephyr_verify_sn(h);
	if (ret)
		return ret;

	ret = zephyr_verify_csum(h);
	if (ret)
		return ret;

	h->init_ok = true;

	return 0;
}
