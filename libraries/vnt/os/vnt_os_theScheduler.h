
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_OS_THESCHEDULER_H_
#define _VNT_OS_THESCHEDULER_H_

#include "vnt_core.h"
#include "vnt_os.h"
#include "vnt_os_Thread.h"
#include <memory>

namespace vnt { namespace os {
    
    // the method theScheduler::start(cfg) does the following:
    // if already started or if the cfg is not valid: it stays in a forever loop otherwise:
    // 1. starts the OS with cfg.timing.ticktime, sets the idle task as specified by cfg.behaviour.idleconfig
    // 2. executes at maximum priority the init task as specified by cfg.behaviour.initconfig
    // after the function cfg.behaviour.initconfig.startup(cfg.behaviour.initconfig.param) returns, the init task
    // is deleted and the OS schedules any task started inside startup() or the idle task, depending on their
    // priority and ready/blocked status. the OS execute the ready task w/ highest priority.    
    // the method theScheduler::start(cfg) does does not return

    class theScheduler
    {
    public:
        static theScheduler& getInstance();
        
    public:
                            
        struct Timing
        {
            std::uint32_t clockfrequency {10000000}; // it must be equal to the cpu speed expressed in hz: use vnt::hw::sys::clock(vnt::hw::sys::CLOCK::syscore)
            vnt::core::relTime ticktime {vnt::core::time1millisec}; // the resolution of the scheduler
            
            Timing() = default;
            Timing(std::uint32_t c, core::relTime t = vnt::core::time1millisec) : clockfrequency(c), ticktime(t) {}
            void clear() { clockfrequency = 10000000; ticktime = vnt::core::time1millisec; }
            bool isvalid() const { if((0 == clockfrequency) || (0 == ticktime)) { return false; } else { return true; } }
        };
        
        struct Behaviour
        {
            vnt::os::InitThread::Config initconfig {};
            vnt::os::IdleThread::Config idleconfig {};
            vnt::core::Callback onOSerror {};
            
            Behaviour() = default;
            Behaviour(const vnt::os::InitThread::Config &ini, const vnt::os::IdleThread::Config &idl, const vnt::core::Callback &err) : initconfig(ini), idleconfig(idl), onOSerror(err) {}            
            bool isvalid() const { if((false == initconfig.isvalid()) || (false == idleconfig.isvalid())) { return false; } else { return true; } }
        };
        
        struct Config
        {
            Timing timing {};
            Behaviour behaviour {};  
            
            Config() = default;
            Config(const Timing &tim, const Behaviour &beh) : timing(tim), behaviour(beh) {}
            bool isvalid() const { if((false == timing.isvalid()) || (false == behaviour.isvalid())) { return false; } else { return true; } }
        }; 
        
        
        [[noreturn]] void start(const Config &config);  
        
        bool started() const;
        
        vnt::core::relTime ticktime() const;
        
        Thread * scheduledtask() const;    

        const char * getOSerror(int &errorcode) const;
        
    private:
        theScheduler();  
        ~theScheduler(); 

    private:    
        struct Impl;
        std::unique_ptr<Impl> pImpl;   
    };   

}} // namespace vnt { namespace os {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
