#ifndef __HAL_H__
#define __HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

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


    typedef int GPIO_TypeDef;

    #define GPIOA   ((GPIO_TypeDef*)0x0)
    #define GPIOB   ((GPIO_TypeDef*)0x0)
    #define GPIOC   ((GPIO_TypeDef*)0x0)
    #define GPIOD   ((GPIO_TypeDef*)0x0)
    #define GPIOE   ((GPIO_TypeDef*)0x0)
    #define GPIOF   ((GPIO_TypeDef*)0x0)
    #define GPIOG   ((GPIO_TypeDef*)0x0)
    #define GPIOH   ((GPIO_TypeDef*)0x0)
    #define GPIOI   ((GPIO_TypeDef*)0x0)
    
    typedef enum
    {
        GPIO_PIN_RESET = 0,
        GPIO_PIN_SET
    }GPIO_PinState;
    
    #define GPIO_PIN_0                 ((uint16_t)0x0001U)  /* Pin 0 selected    */
    #define GPIO_PIN_1                 ((uint16_t)0x0002U)  /* Pin 1 selected    */
    #define GPIO_PIN_2                 ((uint16_t)0x0004U)  /* Pin 2 selected    */
    #define GPIO_PIN_3                 ((uint16_t)0x0008U)  /* Pin 3 selected    */
    #define GPIO_PIN_4                 ((uint16_t)0x0010U)  /* Pin 4 selected    */
    #define GPIO_PIN_5                 ((uint16_t)0x0020U)  /* Pin 5 selected    */
    #define GPIO_PIN_6                 ((uint16_t)0x0040U)  /* Pin 6 selected    */
    #define GPIO_PIN_7                 ((uint16_t)0x0080U)  /* Pin 7 selected    */
    #define GPIO_PIN_8                 ((uint16_t)0x0100U)  /* Pin 8 selected    */
    #define GPIO_PIN_9                 ((uint16_t)0x0200U)  /* Pin 9 selected    */
    #define GPIO_PIN_10                ((uint16_t)0x0400U)  /* Pin 10 selected   */
    #define GPIO_PIN_11                ((uint16_t)0x0800U)  /* Pin 11 selected   */
    #define GPIO_PIN_12                ((uint16_t)0x1000U)  /* Pin 12 selected   */
    #define GPIO_PIN_13                ((uint16_t)0x2000U)  /* Pin 13 selected   */
    #define GPIO_PIN_14                ((uint16_t)0x4000U)  /* Pin 14 selected   */
    #define GPIO_PIN_15                ((uint16_t)0x8000U)  /* Pin 15 selected   */
    #define GPIO_PIN_All               ((uint16_t)0xFFFFU)  /* All pins selected */

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

    static inline GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) { return GPIO_PIN_RESET; }
    static inline void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) { return; }

    static inline void HAL_NVIC_DisableIRQ(IRQn_Type IRQn) {}

#else
#if defined(USE_STM32HAL)
#include "stm32hal.h"
#else
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_i2c.h"
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif // __HAL_H__
