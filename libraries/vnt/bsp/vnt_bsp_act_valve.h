
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_ACT_VALVE_H_
#define _VNT_BSP_ACT_VALVE_H_

#if defined(VNTcfg_FI5)
    // nothing so far
#else

    // we dont use it anymore. it was used just for test


#include "vnt_core.h"
#include "vnt_bsp.h"
#include "vnt_measure.h"


namespace vnt { namespace bsp { namespace act { namespace valve {
    
    
    struct Config
    {
        std::uint32_t whateverisneeded {0};
        bool isvalid() const 
        { 
            return true;
        }
    }; 
    
    
    bool initialised(vnt::bsp::act::VLV a);    
    result_t init(vnt::bsp::act::VLV a, const Config &config); 
        
    // non-blocking mode: hopefully the application of the value must be immediate
    result_t apply(vnt::bsp::act::VLV a, const vnt::measure::ValveCtrl &value);
    
    // but we surely can start application of the actuation and have a callback which signal the end
    result_t apply(vnt::bsp::act::VLV a, const vnt::measure::ValveCtrl &value, const vnt::core::Callback &oncompletion);
    
}}}} // namespace vnt { namespace bsp { namespace act { namespace valve {

#endif

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


