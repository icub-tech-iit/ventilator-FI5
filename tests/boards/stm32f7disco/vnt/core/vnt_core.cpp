
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware-shared for use
 *          in the ventilator project
*/



// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_core.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------

namespace vnt { namespace core {
       
    static vnt::core::Time s_faketimenow()
    {
        static volatile uint64_t tt = 0;
        return tt++;
    }
    
    static bool s_initted = false;    
    static vnt::core::fpGetU64 s_timenow = s_faketimenow;
           
    bool initialised() 
    {
        return s_initted;
    }
    
    bool init(const Config &config)
    {
        if(false == config.isvalid())
        {
            return false;
        }
        
        if(nullptr != config.timeconfig.init)
        {
            config.timeconfig.init();
        }
        
        if(nullptr != config.timeconfig.get)
        {
            s_timenow = config.timeconfig.get;
        }
        
        s_initted = true;
        
        return true;
    }
    
    // it returns time in microseconds as configured by Config::timeconfig:init()
    vnt::core::Time now()
    {
        return s_timenow();
    }

    void delay(vnt::core::Time t)
    {
        // not accurate in case of os initialization ... much better to init that with vnt::hw::wait() 
        volatile vnt::core::Time expiry = vnt::core::now() + t; 
        while(expiry > vnt::core::now());
    }    
    
}} // namespace vnt { namespace core {





// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

