 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/



// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _MATLABCODE_H_
#define _MATLABCODE_H_

#ifdef __cplusplus
extern "C" {
#endif



// - external dependencies --------------------------------------------------------------------------------------------
// put in here only the #include that are needed by the API


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


// - declaration of public user-defined types ------------------------------------------------------------------------- 
// empty-section

  
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------


extern void matlabcode_init();

extern void matlabcode_tick();



#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



