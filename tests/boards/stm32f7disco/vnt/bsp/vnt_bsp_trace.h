
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_TRACE_H_
#define _VNT_BSP_TRACE_H_


#include "vnt_core.h"

namespace vnt { namespace bsp { namespace trace {
    
    // enabled by default
    bool isenabled();
    void enable(const bool yes);
    
    // prints on the ITM trace port
    int puts(const char* str);
    int puts(const std::string &str);     

    // in future put in here also serial
    
}}} // namespace vnt { namespace bsp { namespace trace {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


