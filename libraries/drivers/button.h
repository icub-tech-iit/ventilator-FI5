#ifndef __BUTTON_H__
#define __BUTTON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint32_t systick;
	bool prev_state;
	volatile bool button_press;
	int(*button_sample)(void);
} button_handle_t;

void button_notify(button_handle_t *h);
void button_out(button_handle_t *h, bool *state);
int button_init(button_handle_t *h, int(*button_sample)(void));

#ifdef __cplusplus
}
#endif

#endif
