
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code is a custumization for the ventilator project 
 *          of code originally published in https://github.com/robotology/icub-firmware
 *
*/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "string.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "stm32hal.h"

// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static void s_stm32hal_bps_dummy_tick1msinit(void);
static uint32_t s_stm32hal_bps_dummy_tick1msget(void);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------



static stm32hal_config_t s_stm32hal_bsp_config =
{
    s_stm32hal_bps_dummy_tick1msinit,
    s_stm32hal_bps_dummy_tick1msget
};




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------

// - defined in the bsp_xxxxx.c file. i am lazy and i dont want to create a .h file
extern void stm32hal_bsp_init(void);

extern stm32hal_res_t stm32hal_init(const stm32hal_config_t *cfg)
{    
    static const stm32hal_config_t s_config = 
    {
        s_stm32hal_bps_dummy_tick1msinit,
        s_stm32hal_bps_dummy_tick1msget
    };    
    
    if(NULL == cfg)
    {
        cfg = &s_config;
    }
    
    memmove(&s_stm32hal_bsp_config, cfg, sizeof(s_stm32hal_bsp_config));
    
    if(NULL == s_stm32hal_bsp_config.tick1ms_init)
    {
        s_stm32hal_bsp_config.tick1ms_init = s_stm32hal_bps_dummy_tick1msinit;
    }
    
    if(NULL == s_stm32hal_bsp_config.tick1ms_get)
    {
        s_stm32hal_bsp_config.tick1ms_get = s_stm32hal_bps_dummy_tick1msget;
    }
        
    // defined in the specific board-xxxx section
    stm32hal_board_init();    
    
    return stm32hal_res_OK;
}



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static void s_stm32hal_bps_dummy_tick1msinit(void)
{
}

static uint32_t s_stm32hal_bps_dummy_tick1msget(void)
{
    static volatile uint64_t cnt = 0;
    return cnt++ / 1000;
}



// --------------------------------------------------------------------------------------------------------------------
// - redefinition of functions required by stm32 and weakly defined
// --------------------------------------------------------------------------------------------------------------------



// redefinition of the HAL_* function which we want to behave differently.

// this section keeps, so far, only tick & delay functions.
// cube-mx redefines those funtions to start the systick timer at 1 ms which increments a counter.
// however, we dont want the systick behaving like that and we have decide to give the tick functionality
// as the user wants w/ externally defined funtions.

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
//  /*Configure the SysTick to have interrupt in 1ms time basis*/
//  HAL_SYSTICK_Config(SystemCoreClock/1000);
//
//  /*Configure the SysTick IRQ priority */
//  HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority ,0);
//
//  /* Return function status */
    
    // it is never NULL
    s_stm32hal_bsp_config.tick1ms_init();
    
    return HAL_OK;
}

void HAL_IncTick(void)
{
//  uwTick++;
}

uint32_t HAL_GetTick(void)
{
//  return uwTick;
    
    // tick1ms_get() is never NULL, hence i save the check vs NULL
    return s_stm32hal_bsp_config.tick1ms_get();
}


void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = 0;
  tickstart = HAL_GetTick();
  while((HAL_GetTick() - tickstart) < Delay)
  {
  }
}


void HAL_SuspendTick(void)
{
 // /* Disable SysTick Interrupt */
//  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}


void HAL_ResumeTick(void)
{
//  /* Enable SysTick Interrupt */
//  SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}




// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------



