/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   Please note: The implementation of this module is partially based on
*   examples that are provided within the STM32 cube firmware. In case you want
*   to adapt this module to your custom specific hardware, please adapt the
*   hardware initialization code according your needs or integrate the generated
*   initialization code created by using the tool CubeMX.
*   This template is responsible to establish a console connection in order
*   to send debug messages to a PC terminal tool, or to receive key events
*   for the UI application.
*
*******************************************************************************/

#include "ewconfig.h"
#include "stm32hal.h"
#include "stm32746g_discovery.h"

#include "ew_bsp_console.h"

#define UART_PORTID                     USART1

#define USARTx_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()            __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()          __HAL_RCC_USART1_RELEASE_RESET()

#define USARTx_TX_PIN                   GPIO_PIN_9
#define USARTx_TX_GPIO_PORT             GPIOA
#define USARTx_TX_AF                    GPIO_AF7_USART1
#define USARTx_RX_PIN                   GPIO_PIN_7
#define USARTx_RX_GPIO_PORT             GPIOB
#define USARTx_RX_AF                    GPIO_AF7_USART1


UART_HandleTypeDef                      UART_Handle;


//void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//{
//  GPIO_InitTypeDef  GPIO_InitStruct;

//  /*##-1- Enable peripherals and GPIO Clocks #################################*/
//  /* Enable GPIO TX/RX clock */
//  USARTx_TX_GPIO_CLK_ENABLE();
//  USARTx_RX_GPIO_CLK_ENABLE();

//  /* Enable USART1 clock */
//  USARTx_CLK_ENABLE();

//  /*##-2- Configure peripheral GPIO ##########################################*/
//  /* UART TX GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
//  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull      = GPIO_PULLUP;
//  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
//  GPIO_InitStruct.Alternate = USARTx_TX_AF;

//  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

//  /* UART RX GPIO pin configuration  */
//  GPIO_InitStruct.Pin       = USARTx_RX_PIN;
//  GPIO_InitStruct.Alternate = USARTx_RX_AF;

//  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
//}


//void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
//{
//  /*##-1- Reset peripherals ##################################################*/
//  USARTx_FORCE_RESET();
//  USARTx_RELEASE_RESET();

//  /*##-2- Disable peripherals and GPIO Clocks #################################*/
//  /* Configure UART Tx as alternate function  */
//  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
//  /* Configure UART Rx as alternate function  */
//  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
//}


/*******************************************************************************
* FUNCTION:
*   EwBspConsoleInit
*
* DESCRIPTION:
*   The function EwBspConsoleInit initializes a console connection via UART/USART
*   interface used to print error and trace messages from an Embedded Wizard
*   GUI application.
*   This implementation uses the following configuration:
*   - BaudRate = 115200 baud
*   - Word Length = 8 Bits
*   - One Stop Bit
*   - No parity
*   - Hardware flow control disabled (RTS and CTS signals)
*   - Receive and transmit enabled
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConsoleInit( void )
{
  UART_Handle.Instance            = UART_PORTID;
  UART_Handle.Init.BaudRate       = 2000000; //2000000; //921600; //4000000; // 921600; // 20*112500;// 112500;
  UART_Handle.Init.WordLength     = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits       = UART_STOPBITS_1;
  UART_Handle.Init.Parity         = UART_PARITY_NONE;
  UART_Handle.Init.Mode           = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl      = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling   = UART_OVERSAMPLING_16;

  HAL_UART_Init( &UART_Handle );
}


/*******************************************************************************
* FUNCTION:
*   EwBspConsoleDone
*
* DESCRIPTION:
*   The function EwBspConsoleDone terminates a console connection.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConsoleDone( void )
{
}


/*******************************************************************************
* FUNCTION:
*   EwBspConsolePutCharacter
*
* DESCRIPTION:
*   The function EwBspConsolePutCharacter sends the given character to the
*   console interface.
*
* ARGUMENTS:
*   aCharacter - The character to be send via the console interface.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspConsolePutCharacter( unsigned char aCharacter )
{
  HAL_UART_Transmit( &UART_Handle, (uint8_t*)&aCharacter, 1, 10 );
}


/*******************************************************************************
* FUNCTION:
*   EwBspConsoleGetCharacter
*
* DESCRIPTION:
*   The function EwBspConsoleGetCharacter returns the current character from the 
*   console interface. If no character is available within the input buffer, 0 
*   is returned.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   Current character from console input buffer or 0.
*
*******************************************************************************/
unsigned char EwBspConsoleGetCharacter( void )
{
  uint8_t ret;
  HAL_UART_Receive( &UART_Handle, &ret, 1, 10 );
  return ret;
}

/* msy */
