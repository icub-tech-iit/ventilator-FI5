#include <string.h>
#include <stdio.h>
#include "zephyr.h"
#include "test_utils.h"
#include "string.h"
#include "delay_mock.h"

/* emulate bad csum when true */
bool mock_csum_bad = false;
/* data returned for airflow reading */
uint8_t mock_flow_data[2];
/* serial number returned when explicitly asked via command */
int mock_sn_when_requested;
/* serial number returned automatically after reset */
int mock_sn_after_reset;

/*
 * Xfer status returned by the I2C mock. There are several possible
 * values depending by the FSM state
 */
#define XFER_BRANCH_NUM 9
int xfer_status[XFER_BRANCH_NUM] = {[0 ... (XFER_BRANCH_NUM - 1)] = 0};

/* Coverage for FSM state, (reset /w zephyr_fsm_clear_xfer_done())  */
int fsm_xfer_done[XFER_BRANCH_NUM];

/* internal FSM use only */
int mock_xfer_addr;
int mock_xfer_direction;
int mock_sn;

/* async API aux vars */
int cb_status;
bool cb_invoked = false;

enum {
	STATE_UNK,
	STATE_READY,
	STATE_PENDING_SN,
	STATE_PENDING_SN_2,
	STATE_PENDING_RESET,
	STATE_PENDING_CSUM,
} zephyr_fsm_state;
long long zephyr_fsm_time;

#define CMD_SN 0x1
#define CMD_RESET 0x2
#define CMD_CSUM 0x3

#define ARRAY_ZERO(x, y) memset(x, 0, (y) * sizeof((x)[0]))


/* cause FSM transition */
void zephyr_fsm_switch(int state)
{
	delay_zero();
	zephyr_fsm_state = state;
}


/* clear FSM state coverage buffer */
void zephyr_fsm_clear_xfer_done()
{
	ARRAY_ZERO(fsm_xfer_done, XFER_BRANCH_NUM);
}


/* (re)initialize FSM */
void zephyr_fsm_init()
{
	zephyr_fsm_switch(STATE_UNK);
	zephyr_fsm_clear_xfer_done();
}


/* check for DUT timing requirements */
bool zephyr_fsm_time_ok(int *time_delta)
{
	int delta = delay_get_last();
	*time_delta = delta;
	switch (zephyr_fsm_state) {
	case STATE_READY:
		return true;
		break;
	case STATE_PENDING_SN_2:
		/*
		 * DS recommend to wait for 10mS when initially
		 * reading SN, before reading 2nd word
		 */
		return (delta > 10);
		break;
	case STATE_UNK:
		/* We know nothing. Wait for POR warmup time */
		return (delta > 35);
		break;
	case STATE_PENDING_SN:
		/* command exec time for reading SN is max 10mS */
		return (delta > 10);
		break;
	case STATE_PENDING_RESET:
		/* command exec time for reset is 20mS */
		return (delta > 20);
		break;
	case STATE_PENDING_CSUM:
		/* calculating csum can took up to a second */
		return (delta > 1000);
		break;
	default:
		fail("the bug is in the test this time :<");
	}

	return true;
}


/* main mock FSM routine */
int zephyr_mock(uint8_t *buf, int direction)
{
	int delta;
	int ret = -1;

	if (!zephyr_fsm_time_ok(&delta))
		fail("transaction too early (%d mS)! state %d. %s (%d)",
		     delta, zephyr_fsm_state,
		     (direction == READ) ? "READ" : "WRITE",
		     buf[0]);

	if (direction == WRITE) {
		//printf("write in state %d cmd %d\n", zephyr_fsm_state, buf[0]);
		switch (zephyr_fsm_state) {
		case STATE_UNK:
			/* always allow for reset */
			if (buf[0] == CMD_RESET) {
				mock_sn = mock_sn_after_reset;
				zephyr_fsm_switch(STATE_PENDING_RESET);
				ret = xfer_status[0];
				fsm_xfer_done[0] = 1;
			} else
				fail("tried to write sensor in unknown state");
			break;

		case STATE_READY:
			switch (buf[0]) {
			case CMD_RESET:
				mock_sn = mock_sn_after_reset;
				zephyr_fsm_switch(STATE_PENDING_RESET);
				ret = xfer_status[1];
				fsm_xfer_done[1] = 1;
				break;
			case CMD_CSUM:
				zephyr_fsm_switch(STATE_PENDING_CSUM);
				ret = xfer_status[2];
				fsm_xfer_done[2] = 1;
				break;
			case CMD_SN:
				mock_sn = mock_sn_when_requested;
				zephyr_fsm_switch(STATE_PENDING_SN);
				ret = xfer_status[3];
				fsm_xfer_done[3] = 1;
				break;
			}
			break;
		default:
			fail("command written while in state %d",
			     zephyr_fsm_state);
			break;
		}
	} else {
		//printf("read in state %d\n", zephyr_fsm_state);
		/* READ COMMAND */
		switch (zephyr_fsm_state) {
		case STATE_UNK:
			fail("tried to read sensor in unknown state");
			break;

		case STATE_READY:
			memcpy(buf, mock_flow_data, 2);
			ret = xfer_status[4];
			fsm_xfer_done[4] = 1;
			break;

		case STATE_PENDING_RESET:
		case STATE_PENDING_SN:
			buf[0] = mock_sn >> 24;
			buf[1] = (mock_sn >> 16) & 0xff;
			zephyr_fsm_switch(STATE_PENDING_SN_2);
			ret = xfer_status[6];
			fsm_xfer_done[6] = 1;
			break;

		case STATE_PENDING_SN_2:
			buf[0] = (mock_sn >> 8) & 0xff;
			buf[1] = mock_sn & 0xff;
			zephyr_fsm_switch(STATE_READY);
			ret = xfer_status[7];
			fsm_xfer_done[7] = 1;
			break;

		case STATE_PENDING_CSUM:
			buf[0] = 0xCC;
			if (mock_csum_bad)
				buf[1] = 0x90;
			else
				buf[1] = 0xA5;
			zephyr_fsm_switch(STATE_READY);
			ret = xfer_status[8];
			fsm_xfer_done[8] = 1;
			break;

		default:
			fail("this time the bug is in the test :(");
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
	int expected_len;

	if (addr != mock_xfer_addr)
		fail("i2c address mismatch %d %d",
		       addr, mock_xfer_addr);

	if (xfers->xfer_num != 1)
		fail("Unexpected xfer number %d", xfers->xfer_num);

	if (xfers->xfers == NULL)
		fail("Unexpected NULL xfer in list");

	if (xfers->xfers->buf == NULL)
		fail("Unexpected NULL xfer buffer");

	if (xfers->xfers->direction == READ)
		expected_len = 2;
	else
		expected_len = 1;

	if (xfers->xfers->len != expected_len)
		fail("Unexpected xfer len %d", xfers->xfers->len);

	ret = zephyr_mock(xfers->xfers->buf, xfers->xfers->direction);

	if (!i2c_xfer_cb)
		return ret;

	i2c_xfer_cb(ret, arg);

#warning TODO
	return 0;
};

void read_cb(int status)
{
	cb_invoked = true;
	cb_status = status;
}

int mock_read(zephyr_handle_t *h, uint16_t *data)
{
	int ret;

	cb_invoked = false;
	ret = zephyr_read(h, data, read_cb);
	if (ret)
		test_silent(!cb_invoked, "unexpected callback invokation!");
	else
		test_silent(cb_invoked, "callback not invoked!");
	return ret;
}


#define TEST_RET_CB(fmt, ...) TEST_RET_CBX_COND(0, 1, fmt, ##__VA_ARGS__)
#define TEST_RET_CBN(fmt, ...) TEST_RET_CBX_COND(1, 1, fmt, ##__VA_ARGS__)
#define TEST_RET_CB_COND(cond, fmt, ...) TEST_RET_CBX_COND(0, cond, fmt, ##__VA_ARGS__)
#define TEST_RET_CBX_COND(cbx, cond, fmt, ...) 				\
	test((ret == 0) && ((!!cb_status) == (!!cbx)) && (cond),	\
	     fmt". (ret:%d cb_status:%d)" , ##__VA_ARGS__, ret, cb_status)

int main()
{
	zephyr_handle_t zephyr_handle;
	int ret;
	uint32_t sn;
	uint16_t data;
	int i;

	zephyr_fsm_init();
	mock_sn_when_requested = 0xf0cacc1a;
	mock_sn_after_reset = 0xf0cacc1a;

	mock_xfer_addr = 0x12;

	/* test init() success/failure */
	ret = zephyr_init(&zephyr_handle, i2c_xfer_mock, mock_xfer_addr);
	test(ret == 0, "zephyr init");

	/* check that zephyr_get_sn() API is working */
	ret = zephyr_get_sn(&zephyr_handle, &sn);
	test(ret == 0, "get SN operation status");
	test(sn == mock_sn, "read SN 0x%x", sn);

	/* check that for zephyr_read() API is working */
	mock_flow_data[0] = 0x03;
	mock_flow_data[1] = 0x21;
	ret = mock_read(&zephyr_handle, &data);
	TEST_RET_CB_COND(data == 0x321, "read data 0x%x", data);

	/*
	 * check whether zephyr_read() recognizes invalid data
	 * (valid data always has 0 in first two MSBs
	 */
	mock_flow_data[0] = 0x83;
	mock_flow_data[1] = 0x21;
	ret = mock_read(&zephyr_handle, &data);
	TEST_RET_CBN("recognize invalid read data");
	mock_flow_data[0] = 0x03;

	/* see if bad csum is recognized */
	mock_csum_bad = true;
	zephyr_fsm_init();
	ret = zephyr_init(&zephyr_handle, i2c_xfer_mock, mock_xfer_addr);
	test(ret != 0, "csum check");
	mock_csum_bad = false;

	/* see if SN verification recognizes bad SN */
	zephyr_fsm_init();
	mock_sn_when_requested = 0xcafebabe;
	ret = zephyr_init(&zephyr_handle, i2c_xfer_mock, mock_xfer_addr);
	test(ret != 0, "SN check");
	mock_sn_when_requested = mock_sn_after_reset;

	/* check if calling zephyr_init() twice works. */
	zephyr_fsm_init();
	zephyr_init(&zephyr_handle, i2c_xfer_mock, mock_xfer_addr);
	ret = zephyr_init(&zephyr_handle, i2c_xfer_mock, mock_xfer_addr);
	test(ret == 0, "Init twice");

	/*
	 * Check if driver recognizes xfer failures.
	 * We inject a failure in every possible FSM branch, then
	 * we run Zephyr operations and we check that erros in
	 * all FSM branches we passed through gets detected
	 */

	/* check for xfer failure detections in zephyr_init() API */
	for (i = 0; i < XFER_BRANCH_NUM; i++) {
		/* enable injecting the error on i-th FSM branch */
		ARRAY_ZERO(xfer_status,XFER_BRANCH_NUM);
		xfer_status[i] = -1;

		/* restart FSM, clear coverage, run DUT */
		zephyr_fsm_init();
		ret = zephyr_init(&zephyr_handle,
				  i2c_xfer_mock, mock_xfer_addr);

		/*
		 * Check the coverage buffer to see if the related xfer
		 * has been attepted, if not, then we cannot pretend
		 * its failure gets detected.
		 */
		if (!fsm_xfer_done[i])
			continue;

		test(ret != 0, "xfer failure check when init (%d)", i);
	}

	/*
	 * Check for xfer failure detections in zephyr_read() API
	 *
	 * Re-initialize FSM and DUT..
	 */
	zephyr_fsm_init();
	ARRAY_ZERO(xfer_status,XFER_BRANCH_NUM);
	ret = zephyr_init(&zephyr_handle,
			  i2c_xfer_mock, mock_xfer_addr);
	test_silent(ret == 0, "unexpected failure", i);

	for (i = 0; i < XFER_BRANCH_NUM; i++) {

		/* enable injecting the error on i-th FSM branch */
		ARRAY_ZERO(xfer_status, XFER_BRANCH_NUM);
		xfer_status[i] = -1;
		/* clear coverage buffer */
		zephyr_fsm_clear_xfer_done();

		ret = mock_read(&zephyr_handle, &data);
		if (fsm_xfer_done[i])
			TEST_RET_CBN("xfer failure check when reading %d", i);
		else
			TEST_RET_CB("xfer NOT failure check when reading %d", i);
	}

	printf("\nAll test passed!\n\n");
	return 0;
}
