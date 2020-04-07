
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code is a custumization for the ventilator project 
 *          of code originally published in https://github.com/robotology/icub-firmware
 *
*/

// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _STM32HAL_DRIVER_CFG_H_
#define _STM32HAL_DRIVER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

// path in this file is referred to stm32hal/api

#include "stm32hal_define.h" 
#include "stm32hal_driver.h" 

#if defined(STM32HAL_BOARD_F7DISCO)
    
    #if (STM32HAL_DRIVER_VERSION == 0x01020800)
        #define USE_FULL_LL_DRIVER
        #include "../src/config/stm32hal_driver_cfg_of_f7disco_v1280.h"   
        // we also need some ll includes contained in the driver section
        #include "../src/driver/stm32f7-v1280/inc/stm32f7xx_ll_gpio.h"
    #else
        #error unknown driver version
    #endif

#else

        #error pls define a STM32HAL_BOARD_${BRD}
    
#endif


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard

