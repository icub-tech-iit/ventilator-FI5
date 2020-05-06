#ifndef __ENCODER_H_
#define __ENCODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "button.h"

typedef struct {
	int encoder_pos;
	uint8_t encoder_prev;
	int encoder_tmp;
	volatile int encoder_count;
	enum {
		ENCODER_IDLE,
		ENCODER_RELEASE
	} encoder_fsm;
	button_handle_t button_h;

} encoder_handle_t;

int encoder_init(encoder_handle_t *h);
void encoder_decode(encoder_handle_t *h, bool a, bool b, bool button);
void encoder_get(encoder_handle_t *h, int *tick, bool *button);

#ifdef __cplusplus
}
#endif

#endif
