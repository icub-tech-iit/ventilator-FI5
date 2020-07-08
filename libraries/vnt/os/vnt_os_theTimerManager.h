
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_OS_THETIMERMANAGER_H_
#define _VNT_OS_THETIMERMANAGER_H_

#include "vnt_core.h"
#include "vnt_os.h"
#include "vnt_os_Timer.h"
#include <memory>

namespace vnt { namespace os {
            
    class theTimerManager
    {
    public:
        static theTimerManager& getInstance();
        
    public:
        struct Config
        {
            vnt::os::Priority       priority {vnt::os::Priority::system240};
            std::uint16_t           stacksize {1024};
            std::uint16_t           capacityofhandler {8};          
            Config() = default;
            constexpr Config(std::uint16_t ss, std::uint16_t cap, vnt::os::Priority prio = vnt::os::Priority::system240)
                : stacksize(ss), capacityofhandler(cap), priority(prio) {}
            bool isvalid() const
            {
                return ((0 == stacksize) || (0 == capacityofhandler) || (false == vnt::os::isSystem(priority))) ? false : true;
            }
        }; 
        
        bool start(const Config &config);   

        // this funtion is called by the timer. if false the expiration of the timer is processed internally and not w/ onexpiry()
        bool started() const;        
        
        // this funtion is called only by the timer. if called by someone else it returns false
        // because the action of the timer is enabled only by internals of the timer itself.
        bool onexpiry(const Timer &timer);

    private:
        theTimerManager();  
        ~theTimerManager(); // i dont want a fool can delete it

    private:    
        struct Impl;
        std::unique_ptr<Impl> pImpl;       
    };       


}} // namespace vnt { namespace os {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
