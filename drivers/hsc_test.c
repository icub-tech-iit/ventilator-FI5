#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hsc.h"


void fail(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    printf(" ----- FAILED ------\n");
    vprintf(fmt, args);
    va_end(args);
    printf("\n -------------------\n");
    exit(-1);
}


void test(bool cond, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    if (cond) {
	    printf("PASS: ");
	    vprintf(fmt, args);
	    va_end(args);
	    printf("\n");
    } else {
	    printf(" ----- FAILED ------\n");
	    vprintf(fmt, args);
	    va_end(args);
	    printf("\n -------------------\n");
	    exit(-1);
    }
}

hsc_sensor_t datasheet_example = {
	.has_diagnostic = false,
	.pressure_max = 1,
	.pressure_min = -1,
	.output_max = 0x3999,
	.output_min = 0x666,
	.conv_fn = NULL,
};

int main()
{
	hsc_handle_t hsc_handle;
	int mock_xfer_addr;
	int mock_xfer_status;
	float p, t;
	int ret;
	uint8_t mock_xfer_data[4];

	int i2c_xfer_mock(i2c_xfer_list_t *xfers, int addr)
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
		memcpy(xfers->xfers->buf, mock_xfer_data, 4);

		return mock_xfer_status;
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
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret == 0, "generic read");


	/* test that a xfer failure is propagated */
	mock_xfer_status = -1;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret != 0, "xfer failure check");

	/* test that a stale read code passes */
	mock_xfer_data[0] = 0x86;
	mock_xfer_status = 0;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret == 0, "normal stale read");

	/* test that a stale read code /w wrong data fails */
	mock_xfer_data[2] = 1;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret != 0, "stale read /w wrong temp data");

	/* test that a generic valid read passes */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x66;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret == 0, "normal read");

	/* test that a stale read code /w wrong data fails */
	mock_xfer_data[0] = 0x86;
	mock_xfer_data[1] = 0x67;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret != 0, "stale read /w wrong pres data");

	/* test that a too low pressure code fails */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x65;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret != 0, "pressure lower bound");

	/* test that a too high pressure code fails */
	mock_xfer_data[0] = 0x04;
	mock_xfer_data[1] = 0x00;
	mock_xfer_data[2] = 0;
	mock_xfer_data[3] = 0;
	mock_xfer_status = 0;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret != 0, "pressure higher bound");

	/* test temperature decoding */
	mock_xfer_data[0] = 0x06;
	mock_xfer_data[1] = 0x66;
	mock_xfer_data[2] = 0xb6;
	mock_xfer_data[3] = 0x00;
	mock_xfer_status = 0;
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret == 0, "unexpected failure");
	ret = (t > 92.0) && (t < 92.5);
	test(ret != 0, "temperature decode %f", t);


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
	ret = hsc_read(&hsc_handle, &p, &t);
	test(ret == 0, "unexpected failure");
	ret = (p > -1.0) && (p < -0.995);
	test(ret != 0, "pressure decode %f", p);



	printf("\nall test passed\n");
}
