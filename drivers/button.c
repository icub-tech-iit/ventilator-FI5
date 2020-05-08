#include "button.h"
#include "access_once.h"
#include "hal.h"

/*
 * Time in mS for debouncing filter. If zero then debouncing
 * disabled (i.e. GPIO read on-the-fly)
 */

#define BUTTON_DEBOUNCE 0

int button_init(button_handle_t *h, int(*button_sample)(void))
{
	h->systick = 0;
	h->prev_state = false;
	h->button_press = false;
	h->button_sample = button_sample;

	return 0;
}

static void button_in(button_handle_t *h, bool state)
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

void button_notify(button_handle_t *h)
{
	bool val = h->button_sample();
	button_in(h, val);
}

void button_out(button_handle_t *h, bool *state)
{
#if (BUTTON_DEBOUNCE > 0)
	*state = ACCESS_ONCE(h->button_press);
	ACCESS_ONCE(h->button_press) = 0;
#else
	*state = h->button_sample();
#endif
}
