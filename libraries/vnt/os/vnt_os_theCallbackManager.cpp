
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

#include "vnt_os_theCallbackManager.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::theCallbackManager::Impl
{    
    Config config {};    
    vnt::os::CallbackThread *thrd {nullptr};
    
    Impl() = default;
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::theCallbackManager& vnt::os::theCallbackManager::getInstance()
{
    static theCallbackManager* p = new theCallbackManager();
    return *p;
}

vnt::os::theCallbackManager::theCallbackManager()
{
    pImpl = std::make_unique<Impl>();
}  

    
vnt::os::theCallbackManager::~theCallbackManager() { }

void tCBKman(void *p)
{
    vnt::os::Thread * t = reinterpret_cast<vnt::os::Thread *>(p);
    t->run();
}

bool vnt::os::theCallbackManager::start(const Config &config)
{       
    if(true == started())
    {
        return false;
    }
    
    pImpl->config = config;
        
    pImpl->thrd = new vnt::os::CallbackThread;
    
    vnt::os::CallbackThread::Config cfg {};
    cfg.priority = pImpl->config.priority;
    cfg.stacksize = pImpl->config.stacksize;
    cfg.queuesize = pImpl->config.capacityofhandler;
    cfg.timeout = vnt::core::reltimeWaitForever;
    cfg.startup = nullptr;
    
    pImpl->thrd->start(cfg, tCBKman);
    
    return true;    
}

bool vnt::os::theCallbackManager::started() const
{
    return (nullptr == pImpl->thrd) ? false : true;
}

vnt::os::Thread * vnt::os::theCallbackManager::thread() const
{
    return pImpl->thrd;
}
    

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

