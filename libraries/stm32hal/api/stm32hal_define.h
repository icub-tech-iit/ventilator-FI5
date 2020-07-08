
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code is a custumization for the ventilator project 
 *          of code originally published in https://github.com/robotology/icub-firmware
 *
*/

// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _STM32HAL_DEFINE_H_
#define _STM32HAL_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

// - doxy begin -------------------------------------------------------------------------------------------------------

/** @file       stm32hal_define.h
    @brief      This header file transform the macro STM32HAL_BOARD_xxx int what is required.
    @author     marco.accame@iit.it
    @date       03/21/2017
**/

    
/** @defgroup stm32hal_define STM32HALdefine STM32HALdefine

    The STM32HAL is ..... 
    
    @{        
 **/
    
    
#if !defined(USE_STM32HAL)
#define USE_STM32HAL
#endif   

#if !defined(USE_HAL_DRIVER)
#define USE_HAL_DRIVER
#endif   


// c++-11 makes deprecated keyword volatile. c++-17 makes it not valid anymore
// i have two options: (a) use  -Wno-deprecated-register (b) remove it to avoid armclang complains.
// i choose the -Wno-deprecated-register solution in projects to avoid interference w/ stm32 lib. but with c++17 i must remove it

#if (__cplusplus >= 201703L)
    //#warning c++-17 features enabled: disabling keyword register in stm32hal
    #define register 
#endif
    

#if defined(STM32HAL_BOARD_F7DISCO)

    // it has a STM32F746xx mpu
    #if !defined(STM32F746xx)
    #define STM32F746xx
    #endif  

    // of family STM32F7
    #if !defined(STM32F7)
    #define STM32F7
    #endif  

    // of family STM32F7 but in STM32HAL_ format  
    #if !defined(STM32HAL_STM32F7)
    #define STM32HAL_STM32F7
    #endif  

#else
        #error STM32HAL: you must define a STM32HAL_BOARD_${BRD}
#endif

                
/** @}            
    end of group stm32hal_define  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


