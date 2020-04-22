#include <stdio.h>
#include "mcp23017.h"
#include "test_utils.h"

/* internal FSM use only */
int mock_xfer_addr;
int mock_xfer_direction;

/* async API aux vars */
int cb_status;
bool read_cb_invoked = false, write_cb_invoked = false;

uint16_t mock_pup_reg = 0xdead, mock_dir_reg = 0xdead;
uint16_t mock_port = 0xbeef;


/* main mock FSM routine */
int mcp23017_mock(i2c_xfer_t *xfers, int num)
{
	int i;
	uint16_t *sel_reg = NULL;
	int ret = 0;

	for (i = 0; i < num; i++) {
		if (xfers[i].direction == WRITE) {

			switch (xfers[i].buf[0]) {
			case 0x0C:
				sel_reg = &mock_pup_reg;
				break;
			case 0x0:
				sel_reg = &mock_dir_reg;
				break;
			case 0x12:
				sel_reg = &mock_port;
				break;
			default:
				fail("attempted write on unsupported mock reg %d", xfers[i].buf[0]);
				break;
			}

			if (xfers[i].len == 3) {
				*sel_reg = xfers[i].buf[1] |
					((uint16_t) xfers[i].buf[2] << 8);
				sel_reg = NULL;
			}
		} else {
			if (!sel_reg)
				fail("maybe a bug in the test?");
			xfers[i].buf[0] = *sel_reg & 0xff;
			xfers[i].buf[1] = *sel_reg >> 8;
			sel_reg = NULL;
			break;
		}
	}
	/* xfer status */
	return ret;
}


/* mock for the I2C xfer API */
int i2c_xfer_mock(i2c_xfer_list_t *xfers, int addr,
		  void (*i2c_xfer_cb)(int status, void *arg), void *arg)
{
	int ret;
	int i;

	if (addr != mock_xfer_addr)
		fail("i2c address mismatch %d %d",
		       addr, mock_xfer_addr);

	if (xfers->xfers == NULL)
		fail("Unexpected NULL xfer in list");

	switch (xfers->xfer_num) {
	case 1:
		if (xfers->xfers[0].len != 3)
			fail("Unexpected xfer len %d %d",
			     xfers->xfer_num, xfers->xfers[0].len);

		if (xfers->xfers[0].direction != WRITE)
			fail("Unexpected xfer direction %d",
			     xfers->xfers[0].direction);
		break;
	case 2:
		if ((xfers->xfers[0].len != 1) &&
		    (xfers->xfers[1].len != 2))
			fail("Unexpected xfer len %d %d %d",
			     xfers->xfer_num,
			     xfers->xfers[0].len,
			     xfers->xfers[1].len);

		if (xfers->xfers[0].direction != WRITE)
			fail("Unexpected xfer 0 direction %d",
			     xfers->xfers[0].direction);

		if (xfers->xfers[1].direction != READ)
			fail("Unexpected xfer 1 direction %d",
			     xfers->xfers[1].direction);
		break;
	default:
		fail("Unexpected xfer number %d", xfers->xfer_num);
		break;
	}


	for (i = 0; i < xfers->xfer_num; i++)
		if (xfers->xfers[i].buf == NULL)
			fail("Unexpected NULL xfer %d buffer", i);

	ret = mcp23017_mock(xfers->xfers, xfers->xfer_num);

	if (!i2c_xfer_cb)
		return ret;

	i2c_xfer_cb(ret, arg);

#warning TODO
	return 0;
};

#define TEST_RET_CB(fmt, ...) TEST_RET_CBX_COND(0, 1, fmt, ##__VA_ARGS__)
#define TEST_RET_CBN(fmt, ...) TEST_RET_CBX_COND(1, 1, fmt, ##__VA_ARGS__)
#define TEST_RET_CB_COND(cond, fmt, ...) TEST_RET_CBX_COND(0, cond, fmt, ##__VA_ARGS__)
#define TEST_RET_CBX_COND(cbx, cond, fmt, ...) 				\
	test((ret == 0) && ((!!cb_status) == (!!cbx)) && (cond),	\
	     fmt". (ret:%d cb_status:%d)" , ##__VA_ARGS__, ret, cb_status)

void read_cb(int status)
{
	read_cb_invoked = true;
	cb_status = status;
}

void write_cb(int status)
{
	write_cb_invoked = true;
	cb_status = status;
}

int mock_read(mcp23017_handle_t *h, uint16_t *data)
{
	int ret;

	read_cb_invoked = false;
	write_cb_invoked = false;

	ret = mcp23017_read(h, data, read_cb);
	if (ret)
		test_silent(!read_cb_invoked, "unexpected R callback invokation!");
	else
		test_silent(read_cb_invoked, "callback R not invoked!");

	if (write_cb_invoked)
		test_silent(!write_cb_invoked, "unexpected W callback invokation!");
	return ret;
}

int mock_write(mcp23017_handle_t *h, uint16_t data)
{
	int ret;

	read_cb_invoked = false;
	write_cb_invoked = false;

	ret = mcp23017_write(h, data, write_cb);
	if (ret)
		test_silent(!write_cb_invoked, "unexpected W callback invokation!");
	else
		test_silent(write_cb_invoked, "callback not invoked!");
	if (read_cb_invoked)
		test_silent(!write_cb_invoked, "unexpected W callback invokation!");

	return ret;
}

int main()
{
	int i;
	int ret;
	uint16_t val;
	uint16_t prev_pup, prev_dir;
	mcp23017_handle_t mcp23017_handle;
	mcp23017_cfg_t cfg = {
		.direction = 0x55AA,
		.pup = 0x0
	};

	/* test initialization */
	ret = mcp23017_init(&mcp23017_handle, i2c_xfer_mock, mock_xfer_addr);
	test(ret == 0, "mcp init");

	/* test configuration for no PUPs and DIRs */
	ret = mcp23017_config_device(&mcp23017_handle, &cfg);
	test((ret == 0) &&
	     (mock_pup_reg == cfg.pup) &&
	     (mock_dir_reg == cfg.direction),
	     "mcp config 0x%x 0x%x", mock_pup_reg, mock_dir_reg);

	/* test confituration for some PUPs and DIRs */
	cfg.direction = 0xffff;
	cfg.pup = 0xcafe;
	ret = mcp23017_config_device(&mcp23017_handle, &cfg);
	test((ret == 0) &&
	     (mock_pup_reg == cfg.pup) &&
	     (mock_dir_reg == cfg.direction),
	     "mcp config 0x%x 0x%x", mock_pup_reg, mock_dir_reg);

	/* test for wrong PUP config */
	cfg.direction = 0;
	cfg.pup = 0x1;
	prev_pup = mock_pup_reg;
	prev_dir = mock_dir_reg;
	ret = mcp23017_config_device(&mcp23017_handle, &cfg);
	test((ret != 0) &&
	     (mock_pup_reg == prev_pup) &&
	     (mock_dir_reg == prev_dir),
	     "mcp wrong config 0x%x 0x%x", mock_pup_reg, mock_dir_reg);


	/* test some emulated gpio read */
	for (i = 0; i < 5; i++) {
		mock_port = 0xbeef << i;
		ret = mock_read(&mcp23017_handle, &val);
		TEST_RET_CB_COND(val == mock_port, "port read 0x%x", val);
	}

	/* test some emulated gpio write */
	for (i = 0; i < 5; i++) {
		val = 0xa1b2 << i;
		ret = mock_write(&mcp23017_handle, val);
		TEST_RET_CB_COND(val == mock_port, "port write 0x%x", val);
	}

	/* test rw */
	ret = mock_write(&mcp23017_handle, 0x5678);
	TEST_RET_CB("port write for read");
	ret = mock_read(&mcp23017_handle, &val);
	TEST_RET_CB_COND(val == 0x5678, "port rw 0x%x", val);

	printf("\nAll test passed!\n\n");
	return 0;
}
