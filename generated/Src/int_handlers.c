#include "drivers/board_driver.h"
#include "stm32f7xx_hal.h"

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