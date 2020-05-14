
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_os.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "vnt_bsp.h"
#include "vnt_os_theScheduler.h"
#include <new>

// the used rtos is osal, a fully preemptive rtos derived from keil-rtx.
// to use another rtos, maybe certified for security such as SafeRTOS, change the calls with osal_
#include "osal.h"

#include "osal_system.h"

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------

namespace vnt { namespace os {
    
    static core::Time _now()
    { 
        static volatile core::Time tt = 0; // if osal is not started i offer a very simple (and inaccurate) implementation.
        return (osal_info_status_running != osal_info_get_status()) ? (tt++) : osal_system_abstime_get();
    }
    
    static uint32_t _milli()
    {         
        return osal_system_millitime_get();
//        return _now() / 1000;
    }    
    
    static bool _initted = false;
    static Config _config {};
    
    bool initialised() { return _initted; }
    
    bool init(const Config &config)
    {
        if(false == config.isvalid())
        {
            return false;
        }
        
        _config = config;
        // must init the hw bsp with now()        
        vnt::bsp::init({nullptr, _now, _milli});
        _initted = true;
        return true;        
    }
    
    bool started()
    {        
        return osal_info_status_running == osal_info_get_status();        
    }
    
    
    [[noreturn]] void start()
    {
        if((false == _initted) || (false == _config.isvalid()))
        {
            for(;;);
        }
        
        vnt::os::theScheduler::Config cfg { vnt::os::theScheduler::Timing(vnt::bsp::clock(vnt::bsp::CLOCK::syscore),  _config.tick), {_config.initconfig, _config.idleconfig, _config.onOSerror} };    
        vnt::os::theScheduler &thescheduler = vnt::os::theScheduler::getInstance();
        thescheduler.start(cfg);    
        
        for(;;); 
    }
      

}} // namespace vnt { namespace os {


// --------------------------------------------------------------------------------------------------------------------
// - c code required by osal in order to offer thread-safe heap management  
// --------------------------------------------------------------------------------------------------------------------


// c code required by osal to make thread-safe its osal_base_memory_new() and osal_base_memory_del()
// they become thread-safe because are executed by an svc call.

extern "C" void* osal_ext_calloc(uint32_t s, uint32_t n)
{
    void* ret = calloc(s, n);
    return(ret);
}

extern "C" void* osal_ext_realloc(void* m, uint32_t s)
{
    void* ret = realloc(m, s);
    return(ret);
}

extern "C" void osal_ext_free(void* m)
{
    free(m);
}

// thread-safe redefinition of c++ allocator / deallocators required by armclang and -std=c++17 / c++14
void* operator new(std::size_t size) noexcept(false)
{
    void* ptr = osal_base_memory_new(size);
    return ptr;
}

void* operator new (std::size_t size, const std::nothrow_t& nothrow_value) noexcept
{
    void* ptr = osal_base_memory_new(size);
    return ptr;    
}

void operator delete (void* ptr) noexcept
{
    osal_base_memory_del(ptr);
}

    

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

