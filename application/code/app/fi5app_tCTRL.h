 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef APP_TCTRL_H_
#define APP_TCTRL_H_

#if 0
this file offers:

- interface for the use of thread tCTRL
  so that this important thread does not mix-up with other threads such as tHMI etc. 

  thread tCTRL  
  it implements the control loop which:
  - reads sensors, 
  - ticks the matlab-generated controller,
  - performs the actuations.
  this thread is event-triggered. it means that it is sleeping most of the time and executes
  upon request when an event is sent to it. its execution is designed to last the smallest 
  possible time. its execution time should / could / shall / be monitored by specially
  designed non-intrusive object which insures the real-timeness of the control.  

  the triggering events are:
  - EVT::tick  
    it is sent by a timer at a constant rate specified by tCTRL_tickrate (10 ms)
    it commands the start of sensors acquisition in non-blocking mode.
  - EVT::dataready  
    it is sent by a callback when all the sensors have acquired their values 
    it executes the control which produces the actuations, applies them.

  if macro TEST_tCTRL_control_use_stored_IO is defined, then the control uses data coming from ugo s
  matlab simulation
 
#endif


#include "vnt_core.h"
#include "vnt_core_binary.h"
#include "vnt_os.h"
#include "vnt_os_Thread.h"

// TODO: describe how to use it
#define TEST_tCTRL_control_use_stored_IO

namespace fi5app { namespace tCTRL { 

    // here some constants.
    // they stay here just for quicker visibility. 
        
    struct CoreSettings
    {
        uint16_t stacksize {8*1024};
        vnt::os::Priority priority {vnt::os::Priority::high204};
        vnt::core::relTime timeout {50*vnt::core::time1millisec};
        constexpr CoreSettings(uint16_t s, vnt::os::Priority p, vnt::core::relTime t) : stacksize(s), priority(p), timeout(t) {}  
        constexpr CoreSettings() = default;            
    };
    
    struct Settings : public CoreSettings
    {
        vnt::core::relTime periodOfTICK {10*vnt::core::time1millisec};
                
        constexpr Settings() : CoreSettings(8*1024, vnt::os::Priority::high204, 50*vnt::core::time1millisec) {} 
        //constexpr Settings() = default;         
    };
    
    // the events that this thread accept. externally use method trigger(EVT evt) to trigger the event
    // this enum class uses 32 bits. its tags are all zero but a single one in position specified by the magic number.
    // the numbers must be all different.
    enum class EVT : vnt::os::Event 
    { 
        tick = vnt::core::binary::mask::pos2mask<vnt::os::Event>(0),
        dataready = vnt::core::binary::mask::pos2mask<vnt::os::Event>(1),        
        
        SNSRready01 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(2),
        SNSRready02 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(3),
        SNSRready03 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(4)
    };
    
    
    // it is a constant evaluated at compile time. much much better than a macro.
    // it contains the settings of the thread tCTRL
    constexpr Settings settings {};
        
    // here are some access functions 
    bool start();
        
    vnt::os::Thread * thread();    
    bool trigger(const EVT ev);

}} // namespace fi5app { namespace tCTRL { 



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


