#include <stdint.h>
#include "access_once.h"
#include "encoder.h"

int encoder_init(encoder_handle_t *h,
		 void(*encoder_sample)(bool *a, bool *b))
{
	h->encoder_pos = 0;
	h->encoder_prev = 3;
	h->encoder_tmp = 0;
	h->encoder_count = 0;
	h->encoder_fsm = ENCODER_IDLE;
	h->encoder_sample = encoder_sample;

	return 0;
}


void encoder_get(encoder_handle_t *h, int *tick)
{
	*tick = ACCESS_ONCE(h->encoder_count);
	ACCESS_ONCE(h->encoder_count) = 0;
}


void encoder_decode(encoder_handle_t *h)
{
	uint8_t tmp;
	bool a, b;
	int encoder_tick = 0;

	h->encoder_sample(&a, &b);
	tmp = a ? 1 : 0;
	tmp |= b ? 2 : 0;

	switch (h->encoder_fsm) {
	case ENCODER_IDLE:
		if ((h->encoder_prev == 3) && (tmp == 2)) {
			h->encoder_tmp = 1;
		} else if ((h->encoder_prev == 3) && (tmp == 1)) {
			h->encoder_tmp = -1;
		}

		if (tmp == 0) {
			h->encoder_fsm = ENCODER_RELEASE;
			encoder_tick = h->encoder_tmp;
			h->encoder_tmp = 0;
		}
		break;
	case ENCODER_RELEASE:
		if (tmp == 3)
			h->encoder_fsm = ENCODER_IDLE;
		break;
	}

	h->encoder_prev = tmp;

	ACCESS_ONCE(h->encoder_count) += encoder_tick;
}
