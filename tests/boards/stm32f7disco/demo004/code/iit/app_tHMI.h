 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef APP_THMI_H_
#define APP_THMI_H_

#if 0
this file offers:

TBD

- interface for the use of thread tHMI
  so that this thread does not mix-up with other threads such as tHMI etc. 

  thread tHMI  
  wevwev
 
#endif


#include "vnt_core.h"
#include "vnt_core_binary.h"
#include "vnt_os.h"
#include "vnt_os_Thread.h"

// TODO: describe how to use it
//#define TEST_tHMI_control_use_stored_IO

namespace app { namespace tHMI { 

    // here some constants.
    // they stay here just for quicker visibility. 
        
    struct CoreSettings
    {
        uint16_t stacksize {4*1024};
        vnt::os::Priority priority {vnt::os::Priority::medium102};
        vnt::core::relTime timeout {50*vnt::core::time1millisec};
        constexpr CoreSettings(uint16_t s, vnt::os::Priority p, vnt::core::relTime t) : stacksize(s), priority(p), timeout(t) {}  
        constexpr CoreSettings() = default;            
    };
    
    struct Settings : public CoreSettings
    {
        vnt::core::relTime periodOfTICK {33*vnt::core::time1millisec};
                
        constexpr Settings() : CoreSettings(8*1024, vnt::os::Priority::medium102, 33*vnt::core::time1millisec) {} 
        //constexpr Settings() = default;         
    };
    
    // the events that this thread accept. externally use method trigger(EVT evt) to trigger the event
    // this enum class uses 32 bits. its tags are all zero but a single one in position specified by the magic number.
    // the numbers must be all different.
    enum class EVT : vnt::os::Event 
    { 
        tick = vnt::core::binary::mask::pos2mask<vnt::os::Event>(0),
        userBTN01 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(1),
        userBTN02 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(2)
    };
    
    
    // it is a constant evaluated at compile time. much much better than a macro.
    // it contains the settings of the thread tHMI
    constexpr Settings settings {};
        
    // here are some access functions 
    bool start();
        
    vnt::os::Thread * thread();    
    bool trigger(const EVT ev);

}} // namespace app { namespace tHMI { 



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


