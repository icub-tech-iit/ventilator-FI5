#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hsc.h"
#include "test_utils.h"


hsc_sensor_t datasheet_example = {
	.has_diagnostic = false,
	.pressure_max = 1,
	.pressure_min = -1,
	.output_max = 0x3999,
	.output_min = 0x666,
	.conv_fn = NULL,
};


#define TEST_RET_CB(fmt, ...) TEST_RET_CBX_COND(0, 1, fmt, ##__VA_ARGS__)
#define TEST_RET_CBN(fmt, ...) TEST_RET_CBX_COND(1, 1, fmt, ##__VA_ARGS__)
#define TEST_RET_CB_COND(cond, fmt, ...) TEST_RET_CBX_COND(0, cond, fmt, ##__VA_ARGS__)
#define TEST_RET_CBX_COND(cbx, cond, fmt, ...) 				\
	test((ret == 0) && ((!!cb_status) == (!!cbx)) && (cond),	\
	     fmt". (ret:%d cb_status:%d)" , ##__VA_ARGS__, ret, cb_status)

int main()
{
	hsc_handle_t hsc_handle;
	int mock_xfer_addr;
	int mock_xfer_status;
	int mock_xfer_cb_status = 0;
	float p, t;
	uint16_t raw_p, raw_t;
	int ret;
	uint8_t mock_xfer_data[4];
	int cb_status;
	bool cb_invoked = false;

	void read_cb(int status)
	{
		cb_invoked = true;
		cb_status = status;
	}

	int mock_read(hsc_handle_t *h, float *p, float *t,
		      uint16_t *raw_p, uint16_t *raw_t)
	{
		int ret;

		cb_invoked = false;
		ret = hsc_read(h, p, t, raw_p, raw_t, read_cb);
		if (ret)
			test_silent(!cb_invoked, "callback invoked when xfer fail");
		else
			test_silent(cb_invoked, "callback not invoked!");
		return ret;
	}

	int i2c_xfer_mock(i2c_xfer_list_t *xfers, int addr,
			  void (*i2c_xfer_cb)(int status, void *arg), void *arg)
	{
		if (addr != mock_xfer_addr)
			fail("i2c address mismatch %d %d",
			       addr, mock_xfer_addr);

		if (xfers->xfer_num != 1)
			fail("Unexpected xfer number %d", xfers->xfer_num);

		if (xfers->xfers == NULL)
			fail("Unexpected NULL xfer in list");

		if (xfers->xfers->direction != READ)
			fail("Unexpected xfer direction");

		if (xfers->xfers->buf == NULL)
			fail("Unexpected NULL xfer buffer");

		if (xfers->xfers->len != 4)
			fail("Unexpected xfer len %d", xfers->xfers->len);

	        if(mock_xfer_status)
			return mock_xfer_status;

		memcpy(xfers->xfers->buf, mock_xfer_data, 4);
		i2c_xfer_cb(mock_xfer_cb_status, arg);

		return 0;
	};

	mock_xfer_addr = 0x12;
	hsc_init(&hsc_handle, i2c_xfer_mock, mock_xfer_addr);
	hsc_config_device(&hsc_handle, &HSCMAND160MD2A5);

	/* test that a generic valid read passes */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x66;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CB("generic read");

	/* test that a xfer failure is propagated */
	mock_xfer_status = -1;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	test(ret != 0, "xfer failure check");

	/* test that a stale read code passes */
	mock_xfer_data[0] = 0x86;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CB("normal stale read");

	/* test that a stale read code /w wrong data fails */
	mock_xfer_data[2] = 1;

	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CBN("stale read /w wrong temp data");

	/* test that a generic valid read passes */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x66;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CB("normal read");

	/* test that a stale read code /w wrong data fails */
	mock_xfer_data[0] = 0x86;
	mock_xfer_data[1] = 0x67;

	cb_invoked = false;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CBN("stale read /w wrong pres data");

	/* test that a too low pressure code fails */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x65;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CBN("pressure lower bound");

	/* test that a too high pressure code fails */
	mock_xfer_data[0] = 0x04;
	mock_xfer_data[1] = 0x00;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, NULL, NULL);
	TEST_RET_CBN("pressure upper bound");

	/* test temperature decoding */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x66;
	mock_xfer_data[2] = 0xb6;
	mock_xfer_data[3] = 0x00;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, NULL, &raw_t);
	TEST_RET_CB_COND((t > 92.0) && (t < 92.5), "temperature decode %f", t);
	test(raw_t == (0xb6 << 3), "raw temperature read 0x%x", raw_t);

	/*
	 * test pressure reading; we only have an example from DS
	 * for a unknown sensor..
	 */
	hsc_config_device(&hsc_handle, &datasheet_example);
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x79;
	mock_xfer_data[2] = 0x00;
	mock_xfer_data[3] = 0x00;
	mock_xfer_status = 0;
	ret = mock_read(&hsc_handle, &p, &t, &raw_p, NULL);
	TEST_RET_CB_COND((p > -1.0) && (p < -0.995), "pressure decode %f", p);
	test(raw_p == 0x0679, "raw pressure read 0x%x", raw_p);

	printf("\nall test passed\n");
}
