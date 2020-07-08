
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_OS_TIMER_H_
#define _VNT_OS_TIMER_H_

#include "vnt_core.h"

#include "vnt_os.h"

#include "vnt_os_Action.h"

namespace vnt { namespace os {
    
    class Timer
    {
    public:
        
        enum class Status { idle = 0, counting = 1 }; 
        
        enum class Mode { oneshot = 0, someshots = 1, forever = 2 }; 
        
        struct Config
        {
            core::relTime   countdown {0};          // time to count before execute the action
            os::Action      onexpiry {};            // the action executed at expiry of the countdown
            Mode            mode {Mode::oneshot};   // the mode. we allow one shot, infinite shots or a limited number
            std::uint32_t   numofshots {0};         // in case of limited number this number tells how many. not zero....

            Config() = default;
            Config(core::relTime c, const os::Action &a, Mode m = Mode::oneshot, std::uint32_t n = 0) : countdown(c), onexpiry(a), mode(m), numofshots(n) {} 
            void clear() { countdown = 0;  onexpiry.clear(); mode = Mode::oneshot; numofshots = 0;}
            bool isvalid() const 
            { 
                if((0 == countdown) || (false == onexpiry.isvalid()) || ((Mode::someshots == mode) && (0 == numofshots))) 
                { 
                    return false; 
                }
                else 
                { 
                    return true; 
                }  
            }
        };
      
        Timer();
        ~Timer();
        bool start(const Config &config); 
        bool stop();  
        bool execute(); // to be called by the TimerManager only. if a brave user calls it ... it does nothing because ... black magic woman      
        
        Status getStatus() const;              
        const Config& getConfig() const;
                

    private:        
        struct Impl;
        Impl *pImpl;    
    };

    
}} // namespace vnt { namespace os {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
