
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

#include "vnt_os_theScheduler.h"



// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include <cstring>


#include "osal.h"


// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::theScheduler::Impl
{ 
    Config config {};    
    osal_cfg_t osalConfig; 
    bool started {false};
    int latesterrorcode {0};
    const char * latesterrorstring {nullptr};

    void set_osal_default_config(osal_cfg_t &cfg)
    {
        std::memset(&cfg, 0, sizeof(osal_cfg_t));
        
        cfg.rtostype = osal_rtostype_oosiit;
        cfg.memorymodel = osal_memmode_dynamic;
        cfg.prio = 15;
        cfg.cpufam = osal_cpufam_armcm4;  
        cfg.cpufreq = 168000000;
        cfg.tick = 1000;
        cfg.launcherstacksize = 2048;
        cfg.idlestacksize = 512;
        cfg.roundrobin = osal_false;
        cfg.roundrobintick = 0;
        cfg.tasknum = 0;
        // etc.
        cfg.extfn.usr_on_fatal_error = osalOnError;
        cfg.extfn.usr_on_idle = osalIdleThread;
    }
            
    static void osalIdleThread(void)
    {
        vnt::os::theScheduler &thesched = vnt::os::theScheduler::getInstance(); 
        IdleThread &theidlethread = vnt::os::IdleThread::getInstance();
        const Thread::fpStartup startup = thesched.pImpl->config.behaviour.idleconfig.startup;
        const Thread::fpOnIdle onidle = thesched.pImpl->config.behaviour.idleconfig.onidle;
        void * param = thesched.pImpl->config.behaviour.idleconfig.param;
       
        // make sure the idletask is linked to the rtos
        theidlethread.synch();

        // exec the startup
        if(nullptr != startup)
        {
            startup(&theidlethread, param);
        }
        
        // start the forever loop
        for(;;)
        {
            if(nullptr != onidle)
            {
                onidle(&theidlethread, param);
            }
        }        
    }
        
    
    static void osalOnError(void* task, osal_fatalerror_t errorcode, const char * errormsg)
    {
        vnt::os::theScheduler &thesched = vnt::os::theScheduler::getInstance();   
        thesched.pImpl->latesterrorcode = static_cast<int>(errorcode);
        thesched.pImpl->latesterrorstring = errormsg;        
        thesched.pImpl->config.behaviour.onOSerror.execute();            
        // we dont stop in here. we keep going. but the osal will internally stop
        // for(;;);  
    }

                     
    static void osalLauncher(void) 
    {
        vnt::os::InitThread &inittask = vnt::os::InitThread::getInstance();
        inittask.synch();
        
        vnt::os::theScheduler &thesched = vnt::os::theScheduler::getInstance();                
        if(nullptr != thesched.pImpl->config.behaviour.initconfig.startup)
        {
            thesched.pImpl->config.behaviour.initconfig.startup(&inittask, thesched.pImpl->config.behaviour.initconfig.param);
        }        
    }
    

    [[noreturn]] void start(const Config &cfg)
    {
        if(false == cfg.isvalid())
        {
            for(;;);
        }
        
        // init part
        
        config = cfg;
        
        osalConfig.cpufreq = config.timing.clockfrequency;
        osalConfig.tick = config.timing.ticktime;            
        osalConfig.launcherstacksize = config.behaviour.initconfig.stacksize;        
        osalConfig.idlestacksize = config.behaviour.idleconfig.stacksize;
                        
        // start part        
        osalInit();
            
        // 1. init rtos in standard way:
    
        started = true;
        osal_system_start(osalLauncher);  
        
        for(;;);
    }
    
    void osalInit(void)
    {
        std::uint32_t ram08size = 0;
        uint64_t *ram08data = NULL;
        
        osal_base_memory_getsize(&osalConfig, &ram08size);
        
        if(0 != ram08size)
        {
            ram08data = (uint64_t*)(uint64_t*)osal_base_memory_new(ram08size);
        }
        osal_base_initialise(&osalConfig, ram08data);                 
    }
    
    Impl() 
    {        
        set_osal_default_config(osalConfig);  
        started = false;  
    }
};

// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------



vnt::os::theScheduler& vnt::os::theScheduler::getInstance()
{
    static theScheduler* p = new theScheduler();
    return *p;
}

vnt::os::theScheduler::theScheduler()
{
    pImpl = std::make_unique<Impl>();
}  

    
vnt::os::theScheduler::~theScheduler() { }


[[noreturn]] void vnt::os::theScheduler::start(const Config &config)
{  
    if(true == started())
    {
        for(;;);
    }
    pImpl->start(config);
}

bool vnt::os::theScheduler::started() const
{   
    return pImpl->started;
}

vnt::core::relTime  vnt::os::theScheduler::ticktime() const
{
    return pImpl->config.timing.ticktime;
}

vnt::os::Thread * vnt::os::theScheduler::scheduledtask() const
{
    if(false == pImpl->started)
    {
        return nullptr;
    }
    
    osal_task_t *p = osal_task_get(osal_callerAUTOdetect);

    if(nullptr == p)
    {
        return(nullptr);
    }

    return reinterpret_cast<Thread*>(osal_task_extdata_get(p));         
}

const char * vnt::os::theScheduler::getOSerror(int &errorcode) const
{ 
    errorcode = pImpl->latesterrorcode;
    return pImpl->latesterrorstring; 
}


//vnt::core::relTime vnt::os::theScheduler::getTick()
//{
//    return pImpl->osalConfig.tick;
//}


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

