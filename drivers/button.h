#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint32_t systick;
	bool prev_state;
	volatile bool button_press;
} button_handle_t;

void button_in(button_handle_t *h, bool state);
void button_out(button_handle_t *h, bool *state);
int button_init(button_handle_t *h);

#endif
