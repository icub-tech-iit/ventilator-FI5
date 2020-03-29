 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/



// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _STM32F746G_DISCOVERY_DEMO_H_
#define _STM32F746G_DISCOVERY_DEMO_H_

#ifdef __cplusplus
extern "C" {
#endif


/** @file       demo.h
    @brief      This header file gives the API of a basic demo on dev board STM32F746G-DISCOVERY
    @author     marco.accame@iit.it
    @date       2020/03/27
**/




// - external dependencies --------------------------------------------------------------------------------------------
// put in here only the #include that are needed by the API



// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types ------------------------------------------------------------------------- 
// empty-section

  
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------


extern void stm32f7_demo_init(void);

extern void stm32f7_demo_tick(void);


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



