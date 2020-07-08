
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_OS_THECALLBACKMANAGER_H_
#define _VNT_OS_THECALLBACKMANAGER_H_

#include "vnt_core.h"
#include "vnt_os.h"
#include "vnt_os_Thread.h"
#include <memory>

namespace vnt { namespace os {
        
    class theCallbackManager
    {
    public:
        static theCallbackManager& getInstance();
        
    public:
        struct Config
        {
            vnt::os::Priority       priority {vnt::os::Priority::system230};
            std::uint16_t           stacksize {1024};
            std::uint16_t           capacityofhandler {8};          
            Config() = default;
            constexpr Config(std::uint16_t ss, std::uint16_t cap, vnt::os::Priority prio = vnt::os::Priority::system230)
                : stacksize(ss), capacityofhandler(cap), priority(prio) {}
            bool isvalid() const
            {
                return ((0 == stacksize) || (0 == capacityofhandler) || (false == vnt::os::isSystem(priority))) ? false : true;
            }
        }; 
        
        
        bool start(const Config &config);    
        bool started() const;    
        
        vnt::os::Thread * thread() const;

    private:
        theCallbackManager();  
        ~theCallbackManager(); // i dont want a fool can delete it
    
    private:    
        struct Impl;
        std::unique_ptr<Impl> pImpl;      
    };  

}} // namespace vnt { namespace os {


#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
