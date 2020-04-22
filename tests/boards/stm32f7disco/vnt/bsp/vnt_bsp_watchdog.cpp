
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

//#define TEST_VNT_BSP_WATCHDOG

// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_bsp_watchdog.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#if defined(TEST_VNT_BSP_WATCHDOG)
#include "vnt_bsp_trace.h"
#include "vnt_os_theScheduler.h"
#endif

// it wiil uses the stm32hal ...
#include "stm32hal.h"




namespace vnt { namespace bsp { namespace watchdog {
    
    // TODO: when and if supported... set it to true and add code
    // for now keep it false
    constexpr bool supported = false;
    
    Config theconfig {};
    bool initted {false};   
    vnt::core::Time lastrefresh {0};
  
    bool init(const Config &config)
    {
        if(false == supported)
        {
            return false;
        }
        
        if(initted)
        {
            return false;
        }
        
#if defined(TEST_VNT_BSP_WATCHDOG)
        vnt::os::Thread * t = vnt::os::theScheduler::getInstance().scheduledtask(); 
        vnt::bsp::trace::puts(std::string(t->getName()) + " has started the WDT @ " + vnt::core::TimeFormatter(config.timeout).to_string(vnt::core::TimeFormatter::Mode::milli));
#endif
        
        // TODO_CODE: PUT ACTUAL CODE IN HERE
               
        
        lastrefresh = vnt::core::now();
        
        initted = true;
        return true; 
    }
    
    
    bool refresh()
    {
        if(false == initted)
        {
            return false;
        }
        
        // TODO_CODE: put actual code in here
        
        vnt::core::Time prev = lastrefresh;
        lastrefresh = vnt::core::now();
        
#if defined(TEST_VNT_BSP_WATCHDOG)
        vnt::os::Thread * t = vnt::os::theScheduler::getInstance().scheduledtask(); 
        vnt::bsp::trace::puts(std::string(t->getName()) + " has refreshed the WDT @ " + vnt::core::TimeFormatter(lastrefresh).to_string() + 
                              " after " + vnt::core::TimeFormatter(lastrefresh-prev).to_string(vnt::core::TimeFormatter::Mode::milli));
#endif

        return true;
    }        
 
    
}}} // namespace vnt { namespace bsp { namespace watchdog {


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

