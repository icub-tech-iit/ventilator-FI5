#include <stdbool.h>

#if defined(USE_STM32HAL )
#include "board_driver.h"
#include "stm32hal.h"
#else
#include "drivers/board_driver.h"
#include "stm32f7xx_hal.h"
#endif

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
	case GPIO_PIN_15:
	case GPIO_PIN_2:
		encoder_changed();
		break;

	case GPIO_PIN_0:
		button_changed(1);
		break;

	case GPIO_PIN_7:
		button_changed(2);
		break;

	case GPIO_PIN_4:
		button_changed(3);
		break;

	case GPIO_PIN_6:
		button_changed(4);
		break;

	case GPIO_PIN_3:
		button_changed(5);
		break;

	case GPIO_PIN_1:
		button_changed(6);
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
