#include <stdbool.h>

#if defined(USE_STM32HAL )
#include "board_driver.h"
#include "stm32hal.h"
#else
#include "drivers/board_driver.h"
#include "stm32f7xx_hal.h"
#endif

extern void encoder_changed(bool a, bool b, bool button);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	int a;
	int b;
	int button;

	if ((GPIO_Pin == GPIO_PIN_15) ||
	    (GPIO_Pin == GPIO_PIN_2) ||
	    (GPIO_Pin == GPIO_PIN_1)) {
		a = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15));
		b = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2));
		button = (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_1));
		encoder_changed(a, b, button);
	}
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	i2c_xfer_completed(RC_OK);
}


void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	i2c_xfer_completed(RC_OK);
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
	i2c_xfer_completed(RC_BUS_ERROR);
}
