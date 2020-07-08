#include "analog.h"
#include <errno.h>

int analog_init(analog_handle_t *h, ADC_HandleTypeDef *hadc, int dma_irqn)
{
	h->ch_mask = 0;
	h->hadc = hadc;
	HAL_NVIC_DisableIRQ(dma_irqn);

	return 0;
}


int analog_config(analog_handle_t *h, uint32_t ch_mask)
{
	int i, num = 0;

	if (ch_mask & 0xff000000)
		return -EINVAL;

	for (i = 0; i < 24; i++)
		if (ch_mask & (1 << i))
			num++;

	h->ch_mask = ch_mask;

	return HAL_ADC_Start_DMA(h->hadc,
				 (uint32_t*)h->adc_buffer, num) != HAL_OK;
}


void analog_get_data(analog_handle_t *h, uint16_t *data)
{
	int i, j;

	for (i = 0, j = 0; i < 24; i++) {
		if (h->ch_mask & (1 << i))
			data[j++] = h->adc_buffer[i];
	}
}

#if 0
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	printf("Unexpected ADC callback .. You shouldn't see me\n");
}
#endif
