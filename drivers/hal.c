#include "hal.h"

#ifdef DEBUG_HAL

#include "board_sim.h" 

static void HAL_I2C_IRQHandler(int status);
static int get_i2c_bus_id(I2C_HandleTypeDef *hi2c);

extern void i2c_xfer_completed(int retcode);

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef HAL_Init(void)
{
    // Initialize the I2C 1 bus handler
    hi2c1.id = I2C_1;

    // Install the bus IRQ handler function
    board_sim_bus_irq_handler(1, HAL_I2C_IRQHandler);

    return 0;
}

void HAL_Delay(uint32_t delay)
{
    board_sim_bus_irq(1);
}

HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, 
                                          uint16_t DevAddress, 
                                          uint8_t *pData, 
                                          uint16_t Size, 
                                          uint32_t Timeout)
{
    int busId;
    int result;

    // Convert the bus identifier
    busId = get_i2c_bus_id(hi2c);

    // Call the board simulator write function
    result = board_sim_bus_write(busId, DevAddress, pData, Size);
    if(result)
        return result;

    // Wait for the IRQ to magically appear
    result = board_sim_bus_poll(busId);

    // Todo: return code conversion to HAL ret code values
    // ...

    return result;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, 
                                         uint16_t DevAddress, 
                                         uint8_t *pData, 
                                         uint16_t Size, 
                                         uint32_t Timeout)
{
    int busId;
    int result;

    // Convert the bus identifier
    busId = get_i2c_bus_id(hi2c);

    // Call the board simulator read function
    result = board_sim_bus_read(busId, DevAddress, pData, Size);
    if(result)
        return result;

    // Wait for the IRQ to magically appear
    result = board_sim_bus_poll(busId);

    // Todo: return code conversion to HAL ret code values
    // ...

    return result;
}


HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, 
                                             uint16_t DevAddress, 
                                             uint8_t *pData, 
                                             uint16_t Size)
{
    int busId;
    int result;

    // Convert the bus identifier
    busId = get_i2c_bus_id(hi2c);

    // Call the board simulator write function
    result = board_sim_bus_write(busId, DevAddress, pData, Size);

    return result;
}

HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
    int busId;
    int result;

    // Convert the bus identifier
    busId = get_i2c_bus_id(hi2c);

    // Call the board simulator read function
    result = board_sim_bus_read(busId, DevAddress, pData, Size);

    return result;
}

static void HAL_I2C_IRQHandler(int status)
{
    i2c_xfer_completed(status);
}

static int get_i2c_bus_id(I2C_HandleTypeDef *hi2c)
{
    int busId;

    switch(hi2c->id)
    {
        case I2C_1:
            busId = 1;
            break;

        case I2C_2:
            busId = 2;
            break;

        case I2C_3:
            busId = 3;
            break;

        case I2C_4:
            busId = 4;
            break;
        
        default:
            busId = 0;
    }

    return busId;
}

#endif