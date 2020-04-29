#ifndef __HAL_H__
#define __HAL_H__

#ifdef DEBUG_HAL
    #include <stdint.h>

    #define __IO volatile

    typedef enum
    {
        HAL_OK       = 0x00U,
        HAL_ERROR    = 0x01U,
        HAL_BUSY     = 0x02U,
        HAL_TIMEOUT  = 0x03U
    } HAL_StatusTypeDef;

    typedef enum
    {
        DMA2_Stream0_IRQn
    } IRQn_Type;

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

    enum
    {
        I2C_FIRST_AND_LAST_FRAME,
        I2C_FIRST_FRAME,
        I2C_LAST_FRAME,
        I2C_NEXT_FRAME,
    };

    enum
    {
        HAL_I2C_STATE_BUSY,
        HAL_I2C_STATE_READY,
    };

    typedef struct
    {
        int id;
    } ADC_HandleTypeDef;

    // Replace with CubeMx like initialization or whatever
    HAL_StatusTypeDef HAL_Init(void);

    uint32_t HAL_GetTick(void);
    void HAL_Delay(uint32_t delay);

    static inline HAL_StatusTypeDef HAL_TIM_PWM_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel) { return 0; }
    static inline HAL_StatusTypeDef HAL_TIM_PWM_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel) { return 0; }
    static inline HAL_StatusTypeDef HAL_TIMEx_PWMN_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel) { return 0; }
    static inline HAL_StatusTypeDef HAL_TIMEx_PWMN_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel) { return 0; }

    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);

    HAL_StatusTypeDef HAL_I2C_Master_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);
    HAL_StatusTypeDef HAL_I2C_Master_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size);

    HAL_StatusTypeDef HAL_I2C_Master_Sequential_Transmit_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);
    HAL_StatusTypeDef HAL_I2C_Master_Sequential_Receive_IT(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t XferOptions);

    HAL_StatusTypeDef HAL_I2C_GetState(I2C_HandleTypeDef *hi2c);
    uint32_t HAL_I2C_GetError(I2C_HandleTypeDef *hi2c);

    static inline HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length) { return 0; }

    static inline void HAL_NVIC_DisableIRQ(IRQn_Type IRQn) {}

#else
#if defined(USE_STM32HAL)
#include "stm32hal.h"
#else
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_i2c.h"
#endif

#endif

#endif // __HAL_H__
