 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/


#include "stm32hal.h"

#include "stm32f746g_discovery_demo.h"


static volatile uint64_t s_1mstickcount = 0; // it must be volatile
static const uint32_t s_rate1khz = 1000;

#ifdef __cplusplus
extern "C" {
#endif
    
    void SysTick_Handler(void)
    {
        s_1mstickcount++;
    }
    
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

static void stm32hal_tick1msecinit()
{
    HAL_SYSTICK_Config(SystemCoreClock/s_rate1khz);
}

static uint32_t stm32hal_tick1msecget()
{
    return s_1mstickcount;
}

// stm32hal requires two functions: one which inits and one that gets the 1 ms time.
static const stm32hal_config_t stm32halcfg = { stm32hal_tick1msecinit, stm32hal_tick1msecget };


int main(void)
{

    stm32hal_init(&stm32halcfg);

    stm32f7_demo_init();

    for(;;)
    {        
        stm32f7_demo_tick();        
    }
}



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


