#ifndef __HAL_H__
#define __HAL_H__

#ifndef DEBUG_HAL
    #define DEBUG_HAL 
#endif

#ifdef DEBUG_HAL
    #include <stdint.h>

    typedef int HAL_StatusTypeDef;

    enum
    {
        TIM_CHANNEL_1,
        TIM_CHANNEL_2,
        TIM_CHANNEL_3,
        TIM_CHANNEL_4,
        TIM_CHANNEL_5,
        TIM_CHANNEL_6
    };

    enum
    {
        I2C_1,
        I2C_2,
        I2C_3,
        I2C_4
    };

    typedef struct
    {
        uint32_t CCR1;
        uint32_t CCR2;
        uint32_t CCR3;
        uint32_t CCR4;
        uint32_t CCR5;
        uint32_t CCR6;
    } TIM_TypeDef;

    typedef struct
    {
        TIM_TypeDef* Instance;
    } TIM_HandleTypeDef;

    typedef struct
    {
        int id; // Todo: fill
    } I2C_HandleTypeDef;

    // Replace with CubeMx like initialization or whatever
    HAL_StatusTypeDef HAL_Init(void);

    void HAL_Delay(uint32_t delay);

    static inline HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel) { return 0; }
    static inline HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel) { return 0; }

    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

    HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
    HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

#else

    #include <robadellahal.h>

#endif

#endif // __HAL_H__