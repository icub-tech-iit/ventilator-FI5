#include "button.h"
#include "access_once.h"
#include "hal.h"

#define BUTTON_DEBOUNCE 100

int button_init(button_handle_t *h)
{
	h->systick = 0;
	h->prev_state = false;
	h->button_press = false;

	return 0;
}

void button_in(button_handle_t *h, bool state)
{
	uint32_t systick;
	systick = HAL_GetTick();

	/* we have been stable for a while, then button released */
	if (((systick - h->systick) > BUTTON_DEBOUNCE) &&
	    (h->prev_state && !state)) {
		ACCESS_ONCE(h->button_press) = 1;
	}

	if (h->prev_state != state)
		h->systick = HAL_GetTick();

	h->prev_state = state;
}

void button_out(button_handle_t *h, bool *state)
{
	*state = ACCESS_ONCE(h->button_press);
	ACCESS_ONCE(h->button_press) = 0;
}
