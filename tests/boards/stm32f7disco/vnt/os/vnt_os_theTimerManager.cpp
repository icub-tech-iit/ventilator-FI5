
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

#include "vnt_os_theTimerManager.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::theTimerManager::Impl
{    
    Config config {};    
    vnt::os::MessageThread *task {nullptr};
    
    Impl() 
    {              
    }
    
    static void processtimer(Thread *t, os::Message m, void *o)
    {
        if(0 != m)
        {
            vnt::os::Timer *tmr = reinterpret_cast<vnt::os::Timer*>(m);
            tmr->execute();            
        }
            
    }
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::theTimerManager& vnt::os::theTimerManager::getInstance()
{
    static theTimerManager* p = new theTimerManager();
    return *p;
}

vnt::os::theTimerManager::theTimerManager()
{
    pImpl = std::make_unique<Impl>();
}  

    
vnt::os::theTimerManager::~theTimerManager() { }

void tTMRman(void *p)
{
    vnt::os::Thread * t = reinterpret_cast<vnt::os::Thread *>(p);
    t->run();
}

bool vnt::os::theTimerManager::start(const Config &config)
{       
    if(true == started())
    {
        return false;
    }
    
    pImpl->task = new vnt::os::MessageThread;
    
    vnt::os::MessageThread::Config cfg;
    cfg.priority = pImpl->config.priority;
    cfg.stacksize = pImpl->config.stacksize;
    cfg.messagequeuesize = pImpl->config.capacityofhandler;
    cfg.timeout = vnt::core::reltimeWaitForever;
    cfg.startup = nullptr;
    cfg.onmessage = pImpl->processtimer;
    cfg.param = this;
    
    pImpl->task->start(cfg, tTMRman);
    
    return true;
    
}

bool vnt::os::theTimerManager::started() const
{
    return (nullptr == pImpl->task) ? false : true;
}

bool vnt::os::theTimerManager::onexpiry(const Timer &timer)
{
    if(nullptr == pImpl->task)
    {
        return false;
    }
    
    pImpl->task->setMessage(reinterpret_cast<vnt::os::Message>(const_cast<Timer*>(&timer)), vnt::core::reltimeWaitNone);
    return true;
}
    

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

