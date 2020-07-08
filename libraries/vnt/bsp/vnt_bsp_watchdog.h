
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_WATCHDOG_H_
#define _VNT_BSP_WATCHDOG_H_


#include "vnt_core.h"

namespace vnt { namespace bsp { namespace watchdog {
    
    // caveat: this approach protects from teh crash of a single thread.
    // to be more protective we should:
    // 1. register every thread we want to protect by passing its Thread* in Comfig or with a specific register() function.
    // 2. let each thread call refresh(), which internally set a presence flag
    // 3. refresh() will call the actual WDT refresh funtion only when every registered thread has called the function.
    //    at this stage will also reset all the presence flags.
    // the beforereset() callback could ... know which threads were not seen and print info on permanent memory ...
    // but that in the future, not at first stage.
    
    struct Config
    {
        vnt::core::relTime  timeout {100*vnt::core::time1millisec};
        vnt::core::Callback beforereset {}; // stm32 will enable the early wakeup interrupt if beforereset.isvalid() == true  
        std::uint32_t       whateverisneeded {0};
        bool isvalid() const 
        { 
            return timeout > vnt::core::time1millisec;
        }
    }; 
    
    bool init(const Config &config); // it also starts
    
    bool refresh();     
    
}}} // namespace vnt { namespace bsp { namespace trace {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


