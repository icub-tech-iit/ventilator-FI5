/**
  ******************************************************************************
  * @file    stm32f7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  *          This file template is located in the HAL folder and should be copied 
  *          to the user folder.
  *         
  @verbatim
 ===============================================================================
                     ##### How to use this driver #####
 ===============================================================================
    [..]


  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup STM32F7xx_HAL_Driver
  * @{
  */

/** @defgroup HAL_MSP
  * @brief HAL MSP module.
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
#if defined(EDL_DRIVER_CODE)
    // added edl's code
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
#endif    
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
  /* NOTE : This function is generated automatically by STM32CubeMX and eventually  
            modified by the user
   */
}

#if defined(EDL_DRIVER_CODE)


// adding in here the entire edl file
// i will comment out what i dont need

///* USER CODE BEGIN Header */
///**
//  ******************************************************************************
//  * File Name          : stm32f7xx_hal_msp.c
//  * Description        : This file provides code for the MSP Initialization
//  *                      and de-Initialization codes.
//  ******************************************************************************
//  * @attention
//  *
//  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
//  * All rights reserved.</center></h2>
//  *
//  * This software component is licensed by ST under BSD 3-Clause license,
//  * the "License"; You may not use this file except in compliance with the
//  * License. You may obtain a copy of the License at:
//  *                        opensource.org/licenses/BSD-3-Clause
//  *
//  ******************************************************************************
//  */
///* USER CODE END Header */

///* Includes ------------------------------------------------------------------*/
#include "main.h"
///* USER CODE BEGIN Includes */

///* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_adc3;

///* Private typedef -----------------------------------------------------------*/
///* USER CODE BEGIN TD */

///* USER CODE END TD */

///* Private define ------------------------------------------------------------*/
///* USER CODE BEGIN Define */

///* USER CODE END Define */

///* Private macro -------------------------------------------------------------*/
///* USER CODE BEGIN Macro */

///* USER CODE END Macro */

///* Private variables ---------------------------------------------------------*/
///* USER CODE BEGIN PV */

///* USER CODE END PV */

///* Private function prototypes -----------------------------------------------*/
///* USER CODE BEGIN PFP */

///* USER CODE END PFP */

///* External functions --------------------------------------------------------*/
///* USER CODE BEGIN ExternalFunctions */

///* USER CODE END ExternalFunctions */

///* USER CODE BEGIN 0 */

///* USER CODE END 0 */
                        
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                                            /**
//  * Initializes the Global MSP.
//  */
//void HAL_MspInit(void)
//{
//  /* USER CODE BEGIN MspInit 0 */

//  /* USER CODE END MspInit 0 */

//  __HAL_RCC_PWR_CLK_ENABLE();
//  __HAL_RCC_SYSCFG_CLK_ENABLE();

//  /* System interrupt init*/

//  /* USER CODE BEGIN MspInit 1 */

//  /* USER CODE END MspInit 1 */
//}

/**
* @brief ADC MSP Initialization
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hadc->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspInit 0 */

  /* USER CODE END ADC3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC3_CLK_ENABLE();
  
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**ADC3 GPIO Configuration    
    PF7     ------> ADC3_IN5
    PF6     ------> ADC3_IN4
    PF10     ------> ADC3_IN8
    PF9     ------> ADC3_IN7
    PF8     ------> ADC3_IN6
    PA0/WKUP     ------> ADC3_IN0 
    */
    GPIO_InitStruct.Pin = ARDUINO_A4_Pin|ARDUINO_A5_Pin|ARDUINO_A1_Pin|ARDUINO_A2_Pin 
                          |ARDUINO_A3_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC3 DMA Init */
    /* ADC3 Init */
    hdma_adc3.Instance = DMA2_Stream0;
    hdma_adc3.Init.Channel = DMA_CHANNEL_2;
    hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc3.Init.Mode = DMA_CIRCULAR;
    hdma_adc3.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc3.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc3) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc3);

  /* USER CODE BEGIN ADC3_MspInit 1 */

  /* USER CODE END ADC3_MspInit 1 */
  }

}

/**
* @brief ADC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hadc: ADC handle pointer
* @retval None
*/
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC3)
  {
  /* USER CODE BEGIN ADC3_MspDeInit 0 */

  /* USER CODE END ADC3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC3_CLK_DISABLE();
  
    /**ADC3 GPIO Configuration    
    PF7     ------> ADC3_IN5
    PF6     ------> ADC3_IN4
    PF10     ------> ADC3_IN8
    PF9     ------> ADC3_IN7
    PF8     ------> ADC3_IN6
    PA0/WKUP     ------> ADC3_IN0 
    */
    HAL_GPIO_DeInit(GPIOF, ARDUINO_A4_Pin|ARDUINO_A5_Pin|ARDUINO_A1_Pin|ARDUINO_A2_Pin 
                          |ARDUINO_A3_Pin);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);

    /* ADC3 DMA DeInit */
    HAL_DMA_DeInit(hadc->DMA_Handle);
  /* USER CODE BEGIN ADC3_MspDeInit 1 */

  /* USER CODE END ADC3_MspDeInit 1 */
  }

}

/**
* @brief CRC MSP Initialization
* This function configures the hardware resources used in this example
* @param hcrc: CRC handle pointer
* @retval None
*/
void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
  if(hcrc->Instance==CRC)
  {
  /* USER CODE BEGIN CRC_MspInit 0 */

  /* USER CODE END CRC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CRC_CLK_ENABLE();
  /* USER CODE BEGIN CRC_MspInit 1 */

  /* USER CODE END CRC_MspInit 1 */
  }

}

/**
* @brief CRC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hcrc: CRC handle pointer
* @retval None
*/
void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc)
{
  if(hcrc->Instance==CRC)
  {
  /* USER CODE BEGIN CRC_MspDeInit 0 */

  /* USER CODE END CRC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CRC_CLK_DISABLE();
  /* USER CODE BEGIN CRC_MspDeInit 1 */

  /* USER CODE END CRC_MspDeInit 1 */
  }

}

///**
//* @brief DMA2D MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hdma2d: DMA2D handle pointer
//* @retval None
//*/
//void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef* hdma2d)
//{
//  if(hdma2d->Instance==DMA2D)
//  {
//  /* USER CODE BEGIN DMA2D_MspInit 0 */

//  /* USER CODE END DMA2D_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_DMA2D_CLK_ENABLE();
//    /* DMA2D interrupt Init */
//    HAL_NVIC_SetPriority(DMA2D_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(DMA2D_IRQn);
//  /* USER CODE BEGIN DMA2D_MspInit 1 */

//  /* USER CODE END DMA2D_MspInit 1 */
//  }

//}

///**
//* @brief DMA2D MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hdma2d: DMA2D handle pointer
//* @retval None
//*/
//void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef* hdma2d)
//{
//  if(hdma2d->Instance==DMA2D)
//  {
//  /* USER CODE BEGIN DMA2D_MspDeInit 0 */

//  /* USER CODE END DMA2D_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_DMA2D_CLK_DISABLE();

//    /* DMA2D interrupt DeInit */
//    HAL_NVIC_DisableIRQ(DMA2D_IRQn);
//  /* USER CODE BEGIN DMA2D_MspDeInit 1 */

//  /* USER CODE END DMA2D_MspDeInit 1 */
//  }

//}


// marco.accame: managed in a different mode: see next the used HAL_I2C_MspInit()
///**
//* @brief I2C MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hi2c: I2C handle pointer
//* @retval None
//*/
//void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hi2c->Instance==I2C1)
//  {
//  /* USER CODE BEGIN I2C1_MspInit 0 */
//  /* USER CODE END I2C1_MspInit 0 */
//  
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    /**I2C1 GPIO Configuration    
//    PB8     ------> I2C1_SCL
//    PB9     ------> I2C1_SDA 
//    */
//    GPIO_InitStruct.Pin = ARDUINO_SCL_D15_Pin|ARDUINO_SDA_D14_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    /* Peripheral clock enable */
//    __HAL_RCC_I2C1_CLK_ENABLE();
//    /* I2C1 interrupt Init */
//    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
//    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
//  /* USER CODE BEGIN I2C1_MspInit 1 */

//  /* USER CODE END I2C1_MspInit 1 */
//  }
//  else if(hi2c->Instance==I2C3)
//  {
//  /* USER CODE BEGIN I2C3_MspInit 0 */

//  /* USER CODE END I2C3_MspInit 0 */
//  
//    __HAL_RCC_GPIOH_CLK_ENABLE();
//    /**I2C3 GPIO Configuration    
//    PH7     ------> I2C3_SCL
//    PH8     ------> I2C3_SDA 
//    */
//    GPIO_InitStruct.Pin = LCD_SCL_Pin|LCD_SDA_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
//    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

//    /* Peripheral clock enable */
//    __HAL_RCC_I2C3_CLK_ENABLE();
//  /* USER CODE BEGIN I2C3_MspInit 1 */

//  /* USER CODE END I2C3_MspInit 1 */
//  }

//}

///**
//* @brief I2C MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hi2c: I2C handle pointer
//* @retval None
//*/
//void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
//{
//  if(hi2c->Instance==I2C1)
//  {
//  /* USER CODE BEGIN I2C1_MspDeInit 0 */

//  /* USER CODE END I2C1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_I2C1_CLK_DISABLE();
//  
//    /**I2C1 GPIO Configuration    
//    PB8     ------> I2C1_SCL
//    PB9     ------> I2C1_SDA 
//    */
//    HAL_GPIO_DeInit(GPIOB, ARDUINO_SCL_D15_Pin|ARDUINO_SDA_D14_Pin);

//    /* I2C1 interrupt DeInit */
//    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
//    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
//  /* USER CODE BEGIN I2C1_MspDeInit 1 */

//  /* USER CODE END I2C1_MspDeInit 1 */
//  }
//  else if(hi2c->Instance==I2C3)
//  {
//  /* USER CODE BEGIN I2C3_MspDeInit 0 */

//  /* USER CODE END I2C3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_I2C3_CLK_DISABLE();
//  
//    /**I2C3 GPIO Configuration    
//    PH7     ------> I2C3_SCL
//    PH8     ------> I2C3_SDA 
//    */
//    HAL_GPIO_DeInit(GPIOH, LCD_SCL_Pin|LCD_SDA_Pin);

//  /* USER CODE BEGIN I2C3_MspDeInit 1 */

//  /* USER CODE END I2C3_MspDeInit 1 */
//  }

//}

///**
//* @brief LTDC MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hltdc: LTDC handle pointer
//* @retval None
//*/
//void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hltdc->Instance==LTDC)
//  {
//  /* USER CODE BEGIN LTDC_MspInit 0 */

//  /* USER CODE END LTDC_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_LTDC_CLK_ENABLE();
//  
//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    __HAL_RCC_GPIOJ_CLK_ENABLE();
//    __HAL_RCC_GPIOK_CLK_ENABLE();
//    __HAL_RCC_GPIOG_CLK_ENABLE();
//    __HAL_RCC_GPIOI_CLK_ENABLE();
//    /**LTDC GPIO Configuration    
//    PE4     ------> LTDC_B0
//    PJ13     ------> LTDC_B1
//    PK7     ------> LTDC_DE
//    PK6     ------> LTDC_B7
//    PK5     ------> LTDC_B6
//    PG12     ------> LTDC_B4
//    PJ14     ------> LTDC_B2
//    PI10     ------> LTDC_HSYNC
//    PK4     ------> LTDC_B5
//    PJ15     ------> LTDC_B3
//    PI9     ------> LTDC_VSYNC
//    PK1     ------> LTDC_G6
//    PK2     ------> LTDC_G7
//    PI15     ------> LTDC_R0
//    PJ11     ------> LTDC_G4
//    PK0     ------> LTDC_G5
//    PI14     ------> LTDC_CLK
//    PJ8     ------> LTDC_G1
//    PJ10     ------> LTDC_G3
//    PJ7     ------> LTDC_G0
//    PJ9     ------> LTDC_G2
//    PJ6     ------> LTDC_R7
//    PJ4     ------> LTDC_R5
//    PJ5     ------> LTDC_R6
//    PJ3     ------> LTDC_R4
//    PJ2     ------> LTDC_R3
//    PJ0     ------> LTDC_R1
//    PJ1     ------> LTDC_R2 
//    */
//    GPIO_InitStruct.Pin = LCD_B0_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//    HAL_GPIO_Init(LCD_B0_GPIO_Port, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = LCD_B1_Pin|LCD_B2_Pin|LCD_B3_Pin|LCD_G4_Pin 
//                          |LCD_G1_Pin|LCD_G3_Pin|LCD_G0_Pin|LCD_G2_Pin 
//                          |LCD_R7_Pin|LCD_R5_Pin|LCD_R6_Pin|LCD_R4_Pin 
//                          |LCD_R3_Pin|LCD_R1_Pin|LCD_R2_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//    HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = LCD_DE_Pin|LCD_B7_Pin|LCD_B6_Pin|LCD_B5_Pin 
//                          |LCD_G6_Pin|LCD_G7_Pin|LCD_G5_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//    HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = LCD_B4_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
//    HAL_GPIO_Init(LCD_B4_GPIO_Port, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = LCD_HSYNC_Pin|LCD_VSYNC_Pin|LCD_R0_Pin|LCD_CLK_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
//    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

//    /* LTDC interrupt Init */
//    HAL_NVIC_SetPriority(LTDC_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(LTDC_IRQn);
//  /* USER CODE BEGIN LTDC_MspInit 1 */

//  /* USER CODE END LTDC_MspInit 1 */
//  }

//}

///**
//* @brief LTDC MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hltdc: LTDC handle pointer
//* @retval None
//*/
//void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc)
//{
//  if(hltdc->Instance==LTDC)
//  {
//  /* USER CODE BEGIN LTDC_MspDeInit 0 */

//  /* USER CODE END LTDC_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_LTDC_CLK_DISABLE();
//  
//    /**LTDC GPIO Configuration    
//    PE4     ------> LTDC_B0
//    PJ13     ------> LTDC_B1
//    PK7     ------> LTDC_DE
//    PK6     ------> LTDC_B7
//    PK5     ------> LTDC_B6
//    PG12     ------> LTDC_B4
//    PJ14     ------> LTDC_B2
//    PI10     ------> LTDC_HSYNC
//    PK4     ------> LTDC_B5
//    PJ15     ------> LTDC_B3
//    PI9     ------> LTDC_VSYNC
//    PK1     ------> LTDC_G6
//    PK2     ------> LTDC_G7
//    PI15     ------> LTDC_R0
//    PJ11     ------> LTDC_G4
//    PK0     ------> LTDC_G5
//    PI14     ------> LTDC_CLK
//    PJ8     ------> LTDC_G1
//    PJ10     ------> LTDC_G3
//    PJ7     ------> LTDC_G0
//    PJ9     ------> LTDC_G2
//    PJ6     ------> LTDC_R7
//    PJ4     ------> LTDC_R5
//    PJ5     ------> LTDC_R6
//    PJ3     ------> LTDC_R4
//    PJ2     ------> LTDC_R3
//    PJ0     ------> LTDC_R1
//    PJ1     ------> LTDC_R2 
//    */
//    HAL_GPIO_DeInit(LCD_B0_GPIO_Port, LCD_B0_Pin);

//    HAL_GPIO_DeInit(GPIOJ, LCD_B1_Pin|LCD_B2_Pin|LCD_B3_Pin|LCD_G4_Pin 
//                          |LCD_G1_Pin|LCD_G3_Pin|LCD_G0_Pin|LCD_G2_Pin 
//                          |LCD_R7_Pin|LCD_R5_Pin|LCD_R6_Pin|LCD_R4_Pin 
//                          |LCD_R3_Pin|LCD_R1_Pin|LCD_R2_Pin);

//    HAL_GPIO_DeInit(GPIOK, LCD_DE_Pin|LCD_B7_Pin|LCD_B6_Pin|LCD_B5_Pin 
//                          |LCD_G6_Pin|LCD_G7_Pin|LCD_G5_Pin);

//    HAL_GPIO_DeInit(LCD_B4_GPIO_Port, LCD_B4_Pin);

//    HAL_GPIO_DeInit(GPIOI, LCD_HSYNC_Pin|LCD_VSYNC_Pin|LCD_R0_Pin|LCD_CLK_Pin);

//    /* LTDC interrupt DeInit */
//    HAL_NVIC_DisableIRQ(LTDC_IRQn);
//  /* USER CODE BEGIN LTDC_MspDeInit 1 */

//  /* USER CODE END LTDC_MspDeInit 1 */
//  }

//}

///**
//* @brief QSPI MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hqspi: QSPI handle pointer
//* @retval None
//*/
//void HAL_QSPI_MspInit(QSPI_HandleTypeDef* hqspi)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(hqspi->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspInit 0 */

//  /* USER CODE END QUADSPI_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_QSPI_CLK_ENABLE();
//  
//    __HAL_RCC_GPIOE_CLK_ENABLE();
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_GPIOD_CLK_ENABLE();
//    /**QUADSPI GPIO Configuration    
//    PE2     ------> QUADSPI_BK1_IO2
//    PB6     ------> QUADSPI_BK1_NCS
//    PB2     ------> QUADSPI_CLK
//    PD12     ------> QUADSPI_BK1_IO1
//    PD13     ------> QUADSPI_BK1_IO3
//    PD11     ------> QUADSPI_BK1_IO0 
//    */
//    GPIO_InitStruct.Pin = QSPI_D2_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(QSPI_D2_GPIO_Port, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = QSPI_NCS_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
//    HAL_GPIO_Init(QSPI_NCS_GPIO_Port, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = QSPI_D1_Pin|QSPI_D3_Pin|QSPI_D0_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
//    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//  /* USER CODE BEGIN QUADSPI_MspInit 1 */

//  /* USER CODE END QUADSPI_MspInit 1 */
//  }

//}

///**
//* @brief QSPI MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hqspi: QSPI handle pointer
//* @retval None
//*/
//void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* hqspi)
//{
//  if(hqspi->Instance==QUADSPI)
//  {
//  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

//  /* USER CODE END QUADSPI_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_QSPI_CLK_DISABLE();
//  
//    /**QUADSPI GPIO Configuration    
//    PE2     ------> QUADSPI_BK1_IO2
//    PB6     ------> QUADSPI_BK1_NCS
//    PB2     ------> QUADSPI_CLK
//    PD12     ------> QUADSPI_BK1_IO1
//    PD13     ------> QUADSPI_BK1_IO3
//    PD11     ------> QUADSPI_BK1_IO0 
//    */
//    HAL_GPIO_DeInit(QSPI_D2_GPIO_Port, QSPI_D2_Pin);

//    HAL_GPIO_DeInit(GPIOB, QSPI_NCS_Pin|GPIO_PIN_2);

//    HAL_GPIO_DeInit(GPIOD, QSPI_D1_Pin|QSPI_D3_Pin|QSPI_D0_Pin);

//  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

//  /* USER CODE END QUADSPI_MspDeInit 1 */
//  }

//}

///**
//* @brief RTC MSP Initialization
//* This function configures the hardware resources used in this example
//* @param hrtc: RTC handle pointer
//* @retval None
//*/
//void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
//{
//  if(hrtc->Instance==RTC)
//  {
//  /* USER CODE BEGIN RTC_MspInit 0 */

//  /* USER CODE END RTC_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_RTC_ENABLE();
//  /* USER CODE BEGIN RTC_MspInit 1 */

//  /* USER CODE END RTC_MspInit 1 */
//  }

//}

///**
//* @brief RTC MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param hrtc: RTC handle pointer
//* @retval None
//*/
//void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
//{
//  if(hrtc->Instance==RTC)
//  {
//  /* USER CODE BEGIN RTC_MspDeInit 0 */

//  /* USER CODE END RTC_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_RTC_DISABLE();
//  /* USER CODE BEGIN RTC_MspDeInit 1 */

//  /* USER CODE END RTC_MspDeInit 1 */
//  }

//}

/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  else if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(htim_base->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
  else if(htim_base->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
  else if(htim_base->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspInit 0 */

  /* USER CODE END TIM8_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM8_CLK_ENABLE();
  /* USER CODE BEGIN TIM8_MspInit 1 */

  /* USER CODE END TIM8_MspInit 1 */
  }
  else if(htim_base->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspInit 0 */

  /* USER CODE END TIM12_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM12_CLK_ENABLE();
  /* USER CODE BEGIN TIM12_MspInit 1 */

  /* USER CODE END TIM12_MspInit 1 */
  }

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspPostInit 0 */

  /* USER CODE END TIM1_MspPostInit 0 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration    
    PA8     ------> TIM1_CH1 
    */
    GPIO_InitStruct.Pin = ARDUINO_PWM_D10_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(ARDUINO_PWM_D10_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM1_MspPostInit 1 */

  /* USER CODE END TIM1_MspPostInit 1 */
  }
  else if(htim->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspPostInit 0 */

  /* USER CODE END TIM8_MspPostInit 0 */
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM8 GPIO Configuration    
    PC7     ------> TIM8_CH2
    PB14     ------> TIM8_CH2N 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM8_MspPostInit 1 */

  /* USER CODE END TIM8_MspPostInit 1 */
  }
  else if(htim->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspPostInit 0 */

  /* USER CODE END TIM12_MspPostInit 0 */
  
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM12 GPIO Configuration    
    PH6     ------> TIM12_CH1
    PB15     ------> TIM12_CH2 
    */
    GPIO_InitStruct.Pin = ARDUINO_PWM_D6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(ARDUINO_PWM_D6_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_TIM12;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM12_MspPostInit 1 */

  /* USER CODE END TIM12_MspPostInit 1 */
  }

}
/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();
  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspDeInit 0 */

  /* USER CODE END TIM8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM8_CLK_DISABLE();
  /* USER CODE BEGIN TIM8_MspDeInit 1 */

  /* USER CODE END TIM8_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM12)
  {
  /* USER CODE BEGIN TIM12_MspDeInit 0 */

  /* USER CODE END TIM12_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM12_CLK_DISABLE();
  /* USER CODE BEGIN TIM12_MspDeInit 1 */

  /* USER CODE END TIM12_MspDeInit 1 */
  }

}

///**
//* @brief UART MSP Initialization
//* This function configures the hardware resources used in this example
//* @param huart: UART handle pointer
//* @retval None
//*/
//void HAL_UART_MspInit(UART_HandleTypeDef* huart)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  if(huart->Instance==USART1)
//  {
//  /* USER CODE BEGIN USART1_MspInit 0 */

//  /* USER CODE END USART1_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_USART1_CLK_ENABLE();
//  
//    __HAL_RCC_GPIOB_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**USART1 GPIO Configuration    
//    PB7     ------> USART1_RX
//    PA9     ------> USART1_TX 
//    */
//    GPIO_InitStruct.Pin = VCP_RX_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//    HAL_GPIO_Init(VCP_RX_GPIO_Port, &GPIO_InitStruct);

//    GPIO_InitStruct.Pin = VCP_TX_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//    HAL_GPIO_Init(VCP_TX_GPIO_Port, &GPIO_InitStruct);

//  /* USER CODE BEGIN USART1_MspInit 1 */

//  /* USER CODE END USART1_MspInit 1 */
//  }

//}

///**
//* @brief UART MSP De-Initialization
//* This function freeze the hardware resources used in this example
//* @param huart: UART handle pointer
//* @retval None
//*/
//void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
//{
//  if(huart->Instance==USART1)
//  {
//  /* USER CODE BEGIN USART1_MspDeInit 0 */

//  /* USER CODE END USART1_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_USART1_CLK_DISABLE();
//  
//    /**USART1 GPIO Configuration    
//    PB7     ------> USART1_RX
//    PA9     ------> USART1_TX 
//    */
//    HAL_GPIO_DeInit(VCP_RX_GPIO_Port, VCP_RX_Pin);

//    HAL_GPIO_DeInit(VCP_TX_GPIO_Port, VCP_TX_Pin);

//  /* USER CODE BEGIN USART1_MspDeInit 1 */

//  /* USER CODE END USART1_MspDeInit 1 */
//  }

//}

///* USER CODE BEGIN 1 */

///* USER CODE END 1 */

///************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


// asfidanken

#warning about I2Cx_MspInit() read the following
// in stm32746g_discovery.c there is I2Cx_MspInit() which for I2C1 calls directly gpio init plus 
// others but it is much better to use edl's ... so we are sure it is ok


#if 0

    /* External, camera and Arduino connector I2C MSP init */

    /*** Configure the GPIOs ***/
    /* Enable GPIO clock */
    DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_CLK_ENABLE();

    /* Configure I2C Tx as alternate function */
    gpio_init_structure.Pin = DISCOVERY_EXT_I2Cx_SCL_PIN;
    gpio_init_structure.Mode = GPIO_MODE_AF_OD;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
    gpio_init_structure.Alternate = DISCOVERY_EXT_I2Cx_SCL_SDA_AF;
    HAL_GPIO_Init(DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

    /* Configure I2C Rx as alternate function */
    gpio_init_structure.Pin = DISCOVERY_EXT_I2Cx_SDA_PIN;
    HAL_GPIO_Init(DISCOVERY_EXT_I2Cx_SCL_SDA_GPIO_PORT, &gpio_init_structure);

    /*** Configure the I2C peripheral ***/
    /* Enable I2C clock */
    DISCOVERY_EXT_I2Cx_CLK_ENABLE();

    /* Force the I2C peripheral clock reset */
    DISCOVERY_EXT_I2Cx_FORCE_RESET();

    /* Release the I2C peripheral clock reset */
    DISCOVERY_EXT_I2Cx_RELEASE_RESET();

    /* Enable and set I2Cx Interrupt to a lower priority */
    HAL_NVIC_SetPriority(DISCOVERY_EXT_I2Cx_EV_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(DISCOVERY_EXT_I2Cx_EV_IRQn);

    /* Enable and set I2Cx Interrupt to a lower priority */
    HAL_NVIC_SetPriority(DISCOVERY_EXT_I2Cx_ER_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(DISCOVERY_EXT_I2Cx_ER_IRQn);
    


#endif

#warning NOTE: the stm32bsp in file stm32746g_discovery.c uses sligthly different settings for i2c1. see below
    
#if 0
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FAST;
#endif

/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */
  /* USER CODE END I2C1_MspInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
  else if(hi2c->Instance==I2C3)
  {
      
      #warning CAVEAT: i have removed I2C3 from code by edl because that is managed by I2Cx_MspInit() in stm32746g_discovery.c
//  /* USER CODE BEGIN I2C3_MspInit 0 */

//  /* USER CODE END I2C3_MspInit 0 */
//  
//    __HAL_RCC_GPIOH_CLK_ENABLE();
//    /**I2C3 GPIO Configuration    
//    PH7     ------> I2C3_SCL
//    PH8     ------> I2C3_SDA 
//    */
//    GPIO_InitStruct.Pin = LCD_SCL_Pin|LCD_SDA_Pin;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
//    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

//    /* Peripheral clock enable */
//    __HAL_RCC_I2C3_CLK_ENABLE();
//  /* USER CODE BEGIN I2C3_MspInit 1 */

//  /* USER CODE END I2C3_MspInit 1 */
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* I2C1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
  else if(hi2c->Instance==I2C3)
  {
//      #warning CAVEAT: i have removed I2C3 from code by edl because that is managed by I2Cx_MspInit() in stm32746g_discovery.c
//  /* USER CODE BEGIN I2C3_MspDeInit 0 */

//  /* USER CODE END I2C3_MspDeInit 0 */
//    /* Peripheral clock disable */
//    __HAL_RCC_I2C3_CLK_DISABLE();
//  
//    /**I2C3 GPIO Configuration    
//    PH7     ------> I2C3_SCL
//    PH8     ------> I2C3_SDA 
//    */
//    HAL_GPIO_DeInit(GPIOH, LCD_SCL_Pin|LCD_SDA_Pin);

//  /* USER CODE BEGIN I2C3_MspDeInit 1 */

//  /* USER CODE END I2C3_MspDeInit 1 */
  }

}

#endif // #if defined(EDL_DRIVER_CODE)

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
