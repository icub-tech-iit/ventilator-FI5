
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_SNS_PRESSURE_H_
#define _VNT_BSP_SNS_PRESSURE_H_


#include "vnt_core.h"
#include "vnt_bsp.h"
#include "vnt_measure.h"
#include "vnt_os_Timer.h"

namespace vnt { namespace bsp { namespace sns { namespace pressure {
        
    struct Config
    {
        std::uint32_t whateverisneeded {0};
        bool isvalid() const 
        { 
            return true;
        }
    }; 
    
    
    bool initialised(vnt::bsp::sns::PRS s);    
    result_t init(vnt::bsp::sns::PRS s, const Config &config); 
    
    
    // blocking mode: the return value will be in value and timedvalue if return value is resOK
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::Pressure &value, const vnt::core::relTime timeout);  
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::TimedPressure &timedvalue, const vnt::core::relTime timeout);   

    // non-blocking mode: if the read is successfully started it returns resOK.  
    // when the read is available, the IRQhandler (or whatever reads in background) copy the measure in memory pointed by value
    // and executes the callback oncompletion. in this way, the caller can retrieve what it needs.  
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::Pressure *pvalue, const vnt::core::Callback &oncompletion);
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::TimedPressure *ptimedvalue, const vnt::core::Callback &oncompletion);
    
    // i try also the os version. the alert is quicker in simulation because we dont need to go through theCallbakManager
    // just a test
    result_t read(vnt::bsp::sns::PRS s, vnt::measure::TimedPressure *ptimedvalue, const vnt::os::Action &alert);
    
}}}} // namespace vnt { namespace bsp { namespace sns { namespace pressure {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


