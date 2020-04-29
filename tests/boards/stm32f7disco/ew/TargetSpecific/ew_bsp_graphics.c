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
*   This file contains an interface to the DMA2D hardware of the STM32 target
*   in order to provide graphics acceleration for the Embedded Wizard generated
*   UI applications.
*   This interface is intended to be used only by the Graphics Engine of
*   Embedded Wizard.
*
*******************************************************************************/

#include "ewconfig.h"

#if defined STM32F746xx || defined STM32F769xx
  #include "stm32hal.h"
#endif

#if defined STM32F429xx || defined STM32F439xx || defined STM32F469xx
  #include "stm32f4xx_hal.h"
#endif

#if defined STM32L496xx || defined STM32L4R9xx
  #include "stm32l4xx_hal.h"
#endif

#if defined STM32H743xx || defined STM32H747xx || defined STM32H7B3xxQ
  #include "stm32h7xx_hal.h"
#endif

#include <string.h>

#include "ew_bsp_graphics.h"
#include "ew_bsp_clock.h"

#if EW_USE_FREE_RTOS == 1
  #include "cmsis_os.h"
  osSemaphoreId AcceleratorSemaphoreId;
#endif


static volatile char       TransferInProgress  = 0;
static char                ConcurrentOperation = 1;
static DMA2D_HandleTypeDef Accelerator;


/**
  * @brief DMA2D MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  __HAL_RCC_DMA2D_CLK_ENABLE();

  /*##-2- NVIC configuration  ################################################*/
  /* NVIC configuration for DMA2D transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA2D_IRQn, 8, 0);
  HAL_NVIC_EnableIRQ(DMA2D_IRQn);
}

/**
  * @brief DMA2D MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hdma2d: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{
  /*##-1- Reset peripherals ##################################################*/
   HAL_NVIC_DisableIRQ(DMA2D_IRQn);
  /* Enable DMA2D reset state */
  __HAL_RCC_DMA2D_FORCE_RESET();

  /* Release DMA2D from reset state */
  __HAL_RCC_DMA2D_RELEASE_RESET();
}

/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsInit
*
* DESCRIPTION:
*   Initialize the DMA2D graphics accelerator.
*
* ARGUMENTS:
*   aDstColorMode - Colorformat of the destination bitmap
*
* RETURN VALUE:
*   If successful, returns != 0.
*
*******************************************************************************/
int EwBspGraphicsInit( uint32_t aDstColorMode )
{
  /* clear entire accelerator struct including layer configurations */
  memset( &Accelerator, 0, sizeof( DMA2D_HandleTypeDef ));

  #if EW_USE_FREE_RTOS == 1

    /* Create the accelerator semaphore */
    osSemaphoreDef(AcceleratorSemaphore);
    AcceleratorSemaphoreId = osSemaphoreCreate(osSemaphore(AcceleratorSemaphore), 1);

    /* initially take the accelerator token for the first DMA2D transfer */
    osSemaphoreWait( AcceleratorSemaphoreId, 1000 );

  #endif

  /* prepare configuration of the DMA2D graphics accelerator */
  Accelerator.Instance       = DMA2D;
  Accelerator.Init.Mode      = DMA2D_M2M;
  Accelerator.Init.ColorMode = aDstColorMode;
  Accelerator.Lock           = HAL_UNLOCKED;
  Accelerator.State          = HAL_DMA2D_STATE_RESET;

  TransferInProgress  = 0;

  /* initialize the DMA2D graphics accelerator */
  if ( HAL_DMA2D_Init( &Accelerator ) != HAL_OK )
    return 0;

  return 1;
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsDone
*
* DESCRIPTION:
*   Deinitialize the DMA2D graphics accelerator.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsDone( void )
{
  /* deinitialize the DMA2D graphics accelerator */
  HAL_DMA2D_DeInit( &Accelerator );
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsWaitForCompletion
*
* DESCRIPTION:
*   The function EwBspGraphicsWaitForCompletion returns as soon as the DMA2D has
*   completed a pending graphics instruction. If the DMA2D is in idle mode,
*   the function returns immediately.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsWaitForCompletion()
{
  /* return immediately if no DMA2D transfer is ongoing */
  if ( TransferInProgress == 0 )
    return;

  CPU_LOAD_SET_IDLE();

#ifdef EW_USE_DMA2D_INTERRUPT_MODE

  #if EW_USE_FREE_RTOS == 1

    osSemaphoreWait( AcceleratorSemaphoreId, 1000 );
    TransferInProgress = 0;

  #else

    /* wait until DMA2D transfer is done */
    while( TransferInProgress )
      ;

  #endif

#else

  /* wait until DMA2D transfer is done */
  HAL_DMA2D_PollForTransfer( &Accelerator, 1000 );
  TransferInProgress = 0;

#endif

  CPU_LOAD_SET_ACTIVE();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsConcurrentOperation
*
* DESCRIPTION:
*   The function EwBspGraphicsConcurrentOperation configures the operation mode
*   of DMA2D and CPU. If concurrent operation is enabled, the CPU will work in
*   parallel while the DMA2D is transferring data. If concurrent operation is
*   disabled, the CPU will wait everytime the DMA2D is active.
*   This feature is intended to limit the memory bandwidth, e.g. during display
*   update or other bandwidth consuming activities.
*
* ARGUMENTS:
*   aEnable - flag to switch on/off concurrent operation mode.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsConcurrentOperation( int aEnable )
{
  ConcurrentOperation = (char)aEnable;
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsFill
*
* DESCRIPTION:
*   The function EwBspGraphicsFill is used by the Graphics Engine, when a
*   rectangular area should be filled with a constant color by using the DMA2D
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aDstOffset    - Offset in pixel to next row within the destination.
*   aWidth,
*   aHeight       - Size of the area to fill.
*   aDstColorMode - Colorformat of the destination.
*   aSrcColor     - Color to fill the rectangular area.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsFill( uint32_t aDstAddr, uint32_t aDstOffset, uint32_t aWidth,
  uint32_t aHeight, uint32_t aDstColorMode, uint32_t aSrcColor )
{
  /* ensure that previous DMA2D instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* prepare destination output configuration */
  Accelerator.Init.Mode                  = DMA2D_R2M;
  Accelerator.Init.ColorMode             = aDstColorMode;
  Accelerator.Init.OutputOffset          = aDstOffset;

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );

  /* set the flag for DMA2D transfer */
  TransferInProgress = 1;

#ifdef EW_USE_DMA2D_INTERRUPT_MODE

  /* start the transfer */
  HAL_DMA2D_Start_IT( &Accelerator, aSrcColor, aDstAddr, aWidth, aHeight );

#else

  /* start the transfer */
  HAL_DMA2D_Start( &Accelerator, aSrcColor, aDstAddr, aWidth, aHeight );

#endif

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsFillBlend
*
* DESCRIPTION:
*   The function EwBspGraphicsFillBlend is used by the Graphics Engine, when a
*   rectangular area should be blended with a constant color by using the DMA2D
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aDstOffset    - Offset in pixel to next row within the destination.
*   aWidth,
*   aHeight       - Size of the area to blend.
*   aDstColorMode - Colorformat of the destination.
*   aSrcColor     - Color to blend the rectangular area.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsFillBlend( uint32_t aDstAddr, uint32_t aDstOffset, uint32_t aWidth,
  uint32_t aHeight, uint32_t aDstColorMode, uint32_t aSrcColor )
{
  /* ensure that previous DMA2D instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* prepare destination output configuration */
  Accelerator.Init.Mode                  = DMA2D_M2M_BLEND;
  Accelerator.Init.ColorMode             = aDstColorMode;
  Accelerator.Init.OutputOffset          = aDstOffset;

  /* prepare destination input configuration */
  Accelerator.LayerCfg[0].InputOffset    = aDstOffset;
  Accelerator.LayerCfg[0].InputColorMode = aDstColorMode;
  Accelerator.LayerCfg[0].AlphaMode      = DMA2D_COMBINE_ALPHA;
  Accelerator.LayerCfg[0].InputAlpha     = 255;

  /* replace alpha channel if destination does not provide one */
  if (( aDstColorMode == DMA2D_OUTPUT_RGB888 ) || ( aDstColorMode == DMA2D_OUTPUT_RGB565 ))
    Accelerator.LayerCfg[0].AlphaMode    = DMA2D_REPLACE_ALPHA;

  /* foreground layer configuration is used as source of transfer */
  Accelerator.LayerCfg[1].InputOffset    = 0x00; /* no offset in input */
  Accelerator.LayerCfg[1].InputColorMode = DMA2D_INPUT_A8;
  Accelerator.LayerCfg[1].AlphaMode      = DMA2D_REPLACE_ALPHA;
  Accelerator.LayerCfg[1].InputAlpha     = aSrcColor;

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );

  /* set the layer configurtation (foreground layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 1 );

  /* set the layer configurtation (background layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 0 );

  /* set the flag for DMA2D transfer */
  TransferInProgress = 1;

#ifdef EW_USE_DMA2D_INTERRUPT_MODE

  /* start the transfer */
  HAL_DMA2D_BlendingStart_IT( &Accelerator, aDstAddr, aDstAddr, aDstAddr, aWidth, aHeight );

#else

  /* start the transfer */
  HAL_DMA2D_BlendingStart( &Accelerator, aDstAddr, aDstAddr, aDstAddr, aWidth, aHeight );

#endif

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsCopy
*
* DESCRIPTION:
*   The function EwBspGraphicsCopy is used by the Graphics Engine, when a source
*   bitmap should be copied into a destination bitmap by using the DMA2D
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aSrcAddr      - Source address of the transfer.
*   aDstOffset    - Offset in pixel to next row within the destination bitmap.
*   aSrcOffset    - Offset in pixel to next row within the source bitmap.
*   aWidth,
*   aHeight       - Size of the area to transfer the bitmap.
*   aDstColorMode - Colorformat of the destination bitmap
*   aSrcColorMode - Colorformat of the source bitmap.
*   aSrcColor     - Color/alpha value to be used for the transfer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsCopy( uint32_t aDstAddr, uint32_t aSrcAddr, uint32_t aDstOffset,
  uint32_t aSrcOffset, uint32_t aWidth, uint32_t aHeight, uint32_t aDstColorMode,
  uint32_t aSrcColorMode, uint32_t aSrcColor )
{
  unsigned char alpha = (unsigned char)( aSrcColor >> 24 );

  /* ensure that previous DMA2D instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* prepare destination output configuration */
  Accelerator.Init.Mode                  = ((aDstColorMode == aSrcColorMode) && (alpha == 0xFF)) ? DMA2D_M2M : DMA2D_M2M_PFC;
  Accelerator.Init.ColorMode             = aDstColorMode;
  Accelerator.Init.OutputOffset          = aDstOffset;

  /* prepare source input configuration */
  Accelerator.LayerCfg[1].InputOffset    = aSrcOffset;
  Accelerator.LayerCfg[1].InputColorMode = aSrcColorMode;

  if ( aSrcColorMode == DMA2D_INPUT_A8 )
  {
    Accelerator.LayerCfg[1].AlphaMode    = DMA2D_COMBINE_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha   = aSrcColor;
  }
  else
  {
    Accelerator.LayerCfg[1].AlphaMode    = (alpha == 0xFF) ? DMA2D_NO_MODIF_ALPHA : DMA2D_COMBINE_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha   = alpha;
  }

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );

  /* set the layer configurtation (foreground layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 1 );

  /* set the flag for DMA2D transfer */
  TransferInProgress = 1;

#ifdef EW_USE_DMA2D_INTERRUPT_MODE

  /* start the transfer */
  HAL_DMA2D_Start_IT( &Accelerator, aSrcAddr, aDstAddr, aWidth, aHeight );

#else

  /* start the transfer */
  HAL_DMA2D_Start( &Accelerator, aSrcAddr, aDstAddr, aWidth, aHeight );

#endif

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsCopyBlend
*
* DESCRIPTION:
*   The function EwBspGraphicsCopyBlend is used by the Graphics Engine, when a
*   source bitmap should be blended over a destination bitmap by using the DMA2D
*   functionality.
*
* ARGUMENTS:
*   aDstAddr      - Destination address of the transfer.
*   aSrcAddr      - Source address of the transfer.
*   aDstOffset    - Offset in pixel to next row within the destination bitmap.
*   aSrcOffset    - Offset in pixel to next row within the source bitmap.
*   aWidth,
*   aHeight       - Size of the area to transfer the bitmap.
*   aDstColorMode - Colorformat of the destination bitmap
*   aSrcColorMode - Colorformat of the source bitmap.
*   aSrcColor     - Color/alpha value to be used for the transfer.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsCopyBlend( uint32_t aDstAddr, uint32_t aSrcAddr, uint32_t aDstOffset,
  uint32_t aSrcOffset, uint32_t aWidth, uint32_t aHeight, uint32_t aDstColorMode,
  uint32_t aSrcColorMode, uint32_t aSrcColor )
{
  unsigned char alpha = (unsigned char)( aSrcColor >> 24 );

  /* ensure that previous DMA2D instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* prepare destination output configuration */
  Accelerator.Init.Mode                  = DMA2D_M2M_BLEND;
  Accelerator.Init.ColorMode             = aDstColorMode;
  Accelerator.Init.OutputOffset          = aDstOffset;

  /* prepare destination input configuration */
  Accelerator.LayerCfg[0].InputOffset    = aDstOffset;
  Accelerator.LayerCfg[0].InputColorMode = aDstColorMode;
  Accelerator.LayerCfg[0].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
  Accelerator.LayerCfg[0].InputAlpha     = 0;

  /* prepare source input configuration */
  Accelerator.LayerCfg[1].InputOffset    = aSrcOffset;
  Accelerator.LayerCfg[1].InputColorMode = aSrcColorMode;

  if ( aSrcColorMode == DMA2D_INPUT_A8 )
  {
    Accelerator.LayerCfg[1].AlphaMode    = DMA2D_COMBINE_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha   = aSrcColor;
  }
  else
  {
    Accelerator.LayerCfg[1].AlphaMode    = (alpha == 0xFF) ? DMA2D_NO_MODIF_ALPHA : DMA2D_COMBINE_ALPHA;
    Accelerator.LayerCfg[1].InputAlpha   = alpha;
  }

  /* initialize the DMA2D graphics accelerator */
  HAL_DMA2D_Init( &Accelerator );

  /* set the layer configurtation (foreground layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 1 );

  /* set the layer configurtation (background layer) */
  HAL_DMA2D_ConfigLayer( &Accelerator, 0 );

  /* set the flag for DMA2D transfer */
  TransferInProgress = 1;

#ifdef EW_USE_DMA2D_INTERRUPT_MODE

  /* start the transfer */
  HAL_DMA2D_BlendingStart_IT( &Accelerator, aSrcAddr, aDstAddr, aDstAddr, aWidth, aHeight );

#else

  /* start the transfer */
  HAL_DMA2D_BlendingStart( &Accelerator, aSrcAddr, aDstAddr, aDstAddr, aWidth, aHeight );

#endif

  /* check for immediate completion of drawing operation */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


/*******************************************************************************
* FUNCTION:
*   EwBspGraphicsLoadClut
*
* DESCRIPTION:
*   The function EwBspGraphicsLoadClut is used by the Graphics Engine, when
*   a Index8 source bitmap should be copied into a destination bitmap by using
*   the DMA2D functionality. In this case, the CLUT of the DMA2D has to be loaded
*   with the given CLUT.
*
* ARGUMENTS:
*   aClutAddr     - Address of the first color entry of the CLUT.
*   aClutSize     - Number of color entries within the CLUT.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspGraphicsLoadClut( uint32_t aClutAddr, uint32_t aClutSize )
{
  DMA2D_CLUTCfgTypeDef clutCfg;

  /* ensure that previous DMA2D instruction is finished */
  EwBspGraphicsWaitForCompletion();

  /* prepare CLUT configuration */
  clutCfg.pCLUT         = (uint32_t*)aClutAddr;
  clutCfg.CLUTColorMode = DMA2D_CCM_ARGB8888;
  clutCfg.Size          = aClutSize - 1; /* size is expected as size - 1 ... */

  /* set the flag for CLUT transfer */
  TransferInProgress = 1;

#ifdef EW_USE_DMA2D_INTERRUPT_MODE

  /* start the transfer */
  HAL_DMA2D_CLUTLoad_IT( &Accelerator, clutCfg, 1 );

#else

  /* start the transfer */
  HAL_DMA2D_CLUTLoad( &Accelerator, clutCfg, 1 );

#endif

  /* check for immediate completion of CLUT loading */
  if ( !ConcurrentOperation )
    EwBspGraphicsWaitForCompletion();
}


#ifdef EW_USE_DMA2D_INTERRUPT_MODE

/*******************************************************************************
* FUNCTION:
*   DMA2D_IRQHandler
*
* DESCRIPTION:
*   DMA2D Interrupt Handler.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void DMA2D_IRQHandler( void )
{
  HAL_DMA2D_IRQHandler( &Accelerator );

  if ( Accelerator.State == HAL_DMA2D_STATE_READY )
  {
    #if EW_USE_FREE_RTOS == 1

      osSemaphoreRelease( AcceleratorSemaphoreId );

    #else

      TransferInProgress = 0;

    #endif
  }
}

#endif

/* msy */
