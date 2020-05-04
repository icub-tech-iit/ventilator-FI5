/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code is a custumization for the ventilator project 
 *          of code originally published in https://github.com/robotology/icub-firmware
 *
*/


// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _STM32HAL_BOARD_H_
#define _STM32HAL_BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif


// - external dependencies --------------------------------------------------------------------------------------------  
    
#include "stm32hal_define.h"
#include "stm32hal_driver.h"
    
// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
  

// - declaration of public user-defined types -------------------------------------------------------------------------    
// empty-section
    
    
// - declaration of extern public functions ---------------------------------------------------------------------------
 
// called by stm32hal_init() if stm32hal_config_t::initbsp is true
    
extern void stm32hal_board_init(void);     
    
// - public interface: begin  -----------------------------------------------------------------------------------------  
// it contains whatever cube-mx generates.


#if     defined(STM32HAL_BOARD_F7DISCO) 

    #if (STM32HAL_DRIVER_VERSION == 0x01020800)

        #include "../src/config/stm32hal_driver_cfg_of_f7disco_v1280.h"

        //#include "../src/board/f7disco/v1280/inc/dma.h"
        //#include "../src/board/f7disco/v1280/inc/gpio.h"
        //#include "../src/board/f7disco/v1280/inc/i2c.h"
        #include "../src/board/f7disco/v1280/inc/main.h"
        //#include "../src/board/f7disco/v1280/inc/usb_otg.h"
        //#include "../src/board/f7disco/v1280/inc/usart.h"

        //#include "../src/board/nucleoh7/v1280/inc/stm32h7xx_it.h"

    #else
        #error unsupported driver version for nucleoh7
    #endif
    
#else
    #error STM32HAL: you must define a STM32HAL_BOARD_${BRD}
#endif


// - public interface: end --------------------------------------------------------------------------------------------


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
