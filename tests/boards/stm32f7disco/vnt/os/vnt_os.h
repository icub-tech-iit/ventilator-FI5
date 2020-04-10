
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_OS_H_
#define _VNT_OS_H_

#include "vnt_core.h"
#include "vnt_os_Thread.h"

namespace vnt { namespace os {
    
    struct Config
    {         
        vnt::core::Time tick {vnt::core::time1millisec};                 
       
        vnt::os::InitThread::Config initconfig {};
        vnt::os::IdleThread::Config idleconfig {};
        vnt::core::Callback onOSerror {};
            
        constexpr Config() = default;
            
        constexpr Config(vnt::core::Time _t, const vnt::os::InitThread::Config &ini, const vnt::os::IdleThread::Config &idl, const vnt::core::Callback &err) 
           : tick(_t), initconfig(ini), idleconfig(idl), onOSerror(err) {}            
        
        bool isvalid() const { if((0 == tick) || (false == initconfig.isvalid()) || (false == idleconfig.isvalid())) { return false; } else { return true; } }        
    }; 
    
    bool init(const Config &config);
    
    bool initialised();
                
    // it starts the scheduler
    [[noreturn]] void start();
    
    // tells if the scheduler has started
    bool started();
                   
}} // namespace vnt { namespace os {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

