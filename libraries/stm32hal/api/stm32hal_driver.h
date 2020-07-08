
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code is a custumization for the ventilator project 
 *          of code originally published in https://github.com/robotology/icub-firmware
 *
*/


// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _STM32HAL_DRIVER_H_
#define _STM32HAL_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

    
    
// - external dependencies --------------------------------------------------------------------------------------------  
    
#include "stm32hal_define.h"



// - public interface  ------------------------------------------------------------------------------------------------
  
     
#if defined(STM32HAL_BOARD_F7DISCO)

    // one case only: STM32Cube_FW_F7_V1.16.0 which has __STM32F7xx_HAL_VERSION = 0x01020800, 
    // nicked by stm as `HAL Driver version number V1.2.8` but i use 1280...
    #if !defined(STM32HAL_DRIVER_V1280)
        #define STM32HAL_DRIVER_V1280
    #endif
    #define STM32HAL_DRIVER_VERSION 0x01020800

#else
    #error STM32HAL: the STM32HAL_BOARD_${BRD} is undefined
#endif


// now extra code-shaping macros which depend on the family and the driver version 
#if defined(STM32HAL_STM32F7)
    #if (STM32HAL_DRIVER_VERSION == 0x01020800)
    // nothing so far for any driver on F7 
    #endif
#elif defined(STM32HAL_STM32L4)
//    i leave the following as an example in case you need to specify code-shaping macros for the driver
//    #if (STM32HAL_DRIVER_VERSION >= 0x183)
//       // there is a new api for can
//        #if !defined(USE_HAL_CAN_REGISTER_CALLBACKS)
//        #define USE_HAL_CAN_REGISTER_CALLBACKS 1
//        #endif  
//    #endif
#endif



// and only now ... include the .h which must see the code-shaping macros

#if defined(STM32HAL_STM32F7)

    #if defined(STM32HAL_DRIVER_V1280)    
        #include "../src/driver/stm32f7-v1280/inc/stm32f7xx_hal.h"       
    #else
        #error STM32HAL: the STM32HAL_DRIVER_${VER} is not managed
    #endif

#else
    #error STM32HAL: the STM32HAL_DRIVER_${VER} is not managed
#endif   

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


