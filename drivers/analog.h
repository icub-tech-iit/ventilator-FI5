#ifndef __ANALOG_H__
#define __ANALOG_H__

#include <stdint.h>

#include "hal.h"

typedef struct {
	/* 16 channels plus some room for stuff like temperature,
	 * internal vref, etc
	 */
	__IO uint16_t adc_buffer[24];
	ADC_HandleTypeDef *hadc;
	uint32_t ch_mask;
} analog_handle_t;

int analog_init(analog_handle_t *h, ADC_HandleTypeDef *hadc, int dma_irqn);
int analog_config(analog_handle_t *h, uint32_t ch_mask);
void analog_get_data(analog_handle_t *h, uint16_t *data);
#endif
