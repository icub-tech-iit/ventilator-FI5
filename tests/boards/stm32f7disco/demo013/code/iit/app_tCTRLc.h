 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef APP_TCTRLC_H_
#define APP_TCTRLC_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
    
typedef struct
{
    float min;
    float max;
    float val;    
} Plot_t;

typedef struct
{
    Plot_t plot1;
    Plot_t plot2;
    Plot_t plot3;  
    
    float val1;
    float val2;
    float val3;  
    float val4;    
      
} GraphicValues_t;

extern uint8_t getvalues(GraphicValues_t *gv);

#ifdef __cplusplus
}
#endif

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


