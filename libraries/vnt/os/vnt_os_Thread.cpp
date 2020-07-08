
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

#include "vnt_os_Thread.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "osal.h"


// - class InitThread

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::InitThread::Impl
{    
    osal_task_t *osaltask {nullptr};
    //Config config {};       
    Impl() = default;
    
    ~Impl() { }       
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::InitThread& vnt::os::InitThread::getInstance()
{
    static InitThread* p = new InitThread();
    return *p;
}

vnt::os::InitThread::InitThread()
{   
    pImpl = std::make_unique<Impl>();    
}



vnt::os::InitThread::~InitThread() { }


vnt::os::Thread::Type vnt::os::InitThread::getType() const 
{
    return Type::Init;
}


vnt::os::Priority vnt::os::InitThread::getPriority() const
{   
    return vnt::os::Priority::schedInit;
}


bool vnt::os::InitThread::setPriority(vnt::os::Priority priority)
{
    return false;
}

const char * vnt::os::InitThread::getName() const
{
   return "InitThread"; 
}

  
bool vnt::os::InitThread::setEvent(vnt::os::Event event)
{
    return false;
}  


bool vnt::os::InitThread::setMessage(vnt::os::Message message, core::relTime timeout)
{
    return false;
}

bool vnt::os::InitThread::setCallback(const core::Callback &callback, core::relTime timeout)
{
    return false;
}

void vnt::os::InitThread::synch()
{ 
    pImpl->osaltask = osal_task_get(osal_callerAUTOdetect);
    osal_task_extdata_set(pImpl->osaltask, this);   
}

void vnt::os::InitThread::run() {}


// - class IdleThread

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::IdleThread::Impl
{    
    bool started {false};
    osal_task_t *osaltask {nullptr};
    //Config config {};
        
    Impl() = default; 
    
    ~Impl() { }    
   
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::IdleThread& vnt::os::IdleThread::getInstance()
{
    static IdleThread* p = new IdleThread();
    return *p;
}

vnt::os::IdleThread::IdleThread()
{   
    pImpl = std::make_unique<Impl>();    
}


vnt::os::IdleThread::~IdleThread() { }


vnt::os::Thread::Type vnt::os::IdleThread::getType() const 
{
    return Type::Idle;
}


vnt::os::Priority vnt::os::IdleThread::getPriority() const
{   
    return vnt::os::Priority::schedIdle;
}


bool vnt::os::IdleThread::setPriority(vnt::os::Priority priority)
{
    return false;
}

const char * vnt::os::IdleThread::getName() const
{
   return "IdleThread"; 
}
  
bool vnt::os::IdleThread::setEvent(vnt::os::Event event)
{
    if(!pImpl->started)
    {
        return false;
    }
    osal_result_t r = osal_eventflag_set(static_cast<osal_eventflag_t>(event), pImpl->osaltask, osal_callerAUTOdetect);
    return (osal_res_OK == r) ? (true) : (false);
}  


bool vnt::os::IdleThread::setMessage(vnt::os::Message message, core::relTime timeout)
{
    return false;
}

bool vnt::os::IdleThread::setCallback(const core::Callback &callback, core::relTime timeout)
{
    return false;
}

void vnt::os::IdleThread::synch()
{ 
    pImpl->osaltask = osal_task_get(osal_callerAUTOdetect);
    osal_task_extdata_set(pImpl->osaltask, this);   
    pImpl->started = true;
}

void vnt::os::IdleThread::run() {}

// - class EventThread

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::EventThread::Impl
{    
    EventThread * taskParent;
    osal_task_t *osaltask;

    Config config;
    
    osal_task_properties_t osaltaskproperties;
    
    Impl(EventThread *parent) 
    {
        taskParent = parent;
        osaltask = nullptr;
        
        config.startup = nullptr;
        config.onevent = dummyOnEvent; // i prefer assigning a dummy value in here. just in case. because to speed things up i dont check pImpl->onevent inside osal_eventdriven_loop()
        config.param = nullptr;
        config.stacksize = 64;
        config.priority = Priority::minimum;
        config.timeout = vnt::core::reltimeWaitForever;
        
        // to be used in osal_task_new1(). only priority and stacksize will change
        osaltaskproperties.function = osal_eventdriven_loop;
        osaltaskproperties.param = taskParent;
        osaltaskproperties.priority = vnt::core::tointegral(config.priority);
        osaltaskproperties.stacksize = config.stacksize;
  
    }
    
    ~Impl()
    {
        if(nullptr != osaltask)
        {
            osal_task_delete(osaltask);
            osaltask = nullptr;
        }
    }
    
    static void dummyOnEvent(Thread *t, os::EventMask m, void *p) {}
    
    static void osal_eventdriven_loop(void *p) 
    {
        EventThread *t = reinterpret_cast<EventThread*>(p);
        const osal_reltime_t tout = static_cast<osal_reltime_t>(t->pImpl->config.timeout); // they are the same uint32_t type expressed in usec
        const Thread::fpStartup startup = t->pImpl->config.startup;
        const Thread::fpOnEvent onevent = t->pImpl->config.onevent;
        void * param = t->pImpl->config.param;

        // exec the startup
        if(nullptr != startup)
        {
            startup(t, param);
        }

        
        // start the forever loop
        for(;;)
        {
            onevent(t, osal_eventflag_getany(tout), param);
        }        
    }
    
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::EventThread::EventThread()
: pImpl(new Impl(this))
{   
    
}


vnt::os::EventThread::~EventThread()
{   
    delete pImpl;
}


vnt::os::Thread::Type vnt::os::EventThread::getType() const 
{
    return Type::eventTrigger;
}


vnt::os::Priority vnt::os::EventThread::getPriority() const
{   
    return pImpl->config.priority;
}


bool vnt::os::EventThread::setPriority(vnt::os::Priority priority)
{
    pImpl->config.priority = priority;
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(priority);

    osal_result_t r = osal_task_priority_set(pImpl->osaltask, pImpl->osaltaskproperties.priority);
    return (osal_res_OK == r) ? (true) : (false);
}

const char * vnt::os::EventThread::getName() const
{
    return (pImpl->config.name != nullptr) ? pImpl->config.name : "EventThread"; 
}
  
bool vnt::os::EventThread::setEvent(vnt::os::Event event)
{
    osal_result_t r = osal_eventflag_set(static_cast<osal_eventflag_t>(event), pImpl->osaltask, osal_callerAUTOdetect);
    return (osal_res_OK == r) ? (true) : (false);
}  


bool vnt::os::EventThread::setMessage(vnt::os::Message message, core::relTime timeout)
{
    return false;
}

bool vnt::os::EventThread::setCallback(const core::Callback &callback, core::relTime timeout)
{
    return false;
}


bool vnt::os::EventThread::start(const Config &cfg)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    pImpl->osaltaskproperties.function = pImpl->osal_eventdriven_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;    
        
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

bool vnt::os::EventThread::start(const Config &cfg, vnt::core::fpCaller eviewername)
{
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    pImpl->osaltaskproperties.function = (nullptr != eviewername) ? eviewername : pImpl->osal_eventdriven_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;    
        
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);    
    
    return true; 
}

void vnt::os::EventThread::run()
{
    pImpl->osal_eventdriven_loop(this);
}


// - class MessageThread

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::MessageThread::Impl
{    
    MessageThread * taskParent;
    osal_task_t *osaltask;    
    osal_messagequeue_t *osalmessagequeue;
    
    Config config {};
    
    osal_task_properties_t osaltaskproperties;
    
    Impl(MessageThread *parent) 
    {
        taskParent = parent;
        osaltask = nullptr;
        osalmessagequeue = nullptr;

        config.startup = nullptr;
        config.onmessage = dummyOnMessage; // i prefer assigning a dummy value in here. just in case. because to speed things up i dont check pImpl->onevent inside osal_messagedriven_loop()
        config.param = nullptr;
        config.stacksize = 64;
        config.priority = Priority::minimum;
        config.timeout = vnt::core::reltimeWaitForever;
        config.messagequeuesize = 2;
        
        // to be used in osal_task_new1(). only priority and stacksize will change
        osaltaskproperties.function = osal_messagedriven_loop;
        osaltaskproperties.param = taskParent;
        osaltaskproperties.priority = vnt::core::tointegral(config.priority);
        osaltaskproperties.stacksize = config.stacksize;
  
    }
    
    ~Impl()
    {
        if(nullptr != osaltask)
        {
            osal_task_delete(osaltask);
            osaltask = nullptr;
        }
        
        if(nullptr != osalmessagequeue)
        {
            osal_messagequeue_delete(osalmessagequeue);
            osalmessagequeue = nullptr;
        }
    }
    
    static void dummyOnMessage(Thread *t, os::Message m, void *p) {}
    
    static void osal_messagedriven_loop(void *p) 
    {
        MessageThread *t = reinterpret_cast<MessageThread*>(p);
        const osal_reltime_t tout = static_cast<osal_reltime_t>(t->pImpl->config.timeout); // they are the same uint32_t type expressed in usec
        const Thread::fpStartup startup = t->pImpl->config.startup;
        const Thread::fpOnMessage onmessage = t->pImpl->config.onmessage;
        void * param = t->pImpl->config.param;
        osal_messagequeue_t *mq = t->pImpl->osalmessagequeue;
       

        // exec the startup
        if(nullptr != startup)
        {
            startup(t, param);
        }

        
        // start the forever loop
        for(;;)
        {
            // osal_messagequeue_getquick() returns a osal_message_t which is a uint32_t* which may hold a simple integer of a pointer to larger data
            // the use depends on situations ...
            os::Message msg = reinterpret_cast<os::Message>(osal_messagequeue_getquick(mq, tout, osal_callerAUTOdetect));
            onmessage(t, msg, param);
        }        
    }
    
};



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------

vnt::os::MessageThread::MessageThread()
: pImpl(new Impl(this))
{   
    
}


vnt::os::MessageThread::~MessageThread()
{   
    delete pImpl;
}


vnt::os::Thread::Type vnt::os::MessageThread::getType() const
{
    return Type::messageTrigger;
}


vnt::os::Priority vnt::os::MessageThread::getPriority() const
{   
    return pImpl->config.priority;
}


bool vnt::os::MessageThread::setPriority(vnt::os::Priority priority)
{   
    pImpl->config.priority = priority;
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(priority);

    osal_result_t r = osal_task_priority_set(pImpl->osaltask, pImpl->osaltaskproperties.priority);
    return (osal_res_OK == r) ? (true) : (false);
}

const char * vnt::os::MessageThread::getName() const
{
    return (pImpl->config.name != nullptr) ? pImpl->config.name : "MessageThread"; 
}
  
bool vnt::os::MessageThread::setEvent(vnt::os::Event event)
{
    return false;
}  


bool vnt::os::MessageThread::setMessage(vnt::os::Message message, core::relTime timeout)
{
    osal_result_t r = osal_messagequeue_put(pImpl->osalmessagequeue, reinterpret_cast<osal_message_t>(message), static_cast<osal_reltime_t>(timeout), osal_callerAUTOdetect);
    return (osal_res_OK == r) ? (true) : (false);
}

bool vnt::os::MessageThread::setCallback(const core::Callback &callback, core::relTime timeout)
{
    return false;
}


bool vnt::os::MessageThread::start(const Config &cfg)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    pImpl->osalmessagequeue = osal_messagequeue_new(pImpl->config.messagequeuesize); 
    
    pImpl->osaltaskproperties.function = pImpl->osal_messagedriven_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;  
       
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

bool vnt::os::MessageThread::start(const Config &cfg, vnt::core::fpCaller eviewername)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    pImpl->osalmessagequeue = osal_messagequeue_new(pImpl->config.messagequeuesize); 
    
    pImpl->osaltaskproperties.function = (nullptr != eviewername) ? eviewername : pImpl->osal_messagedriven_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;  
       
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

void vnt::os::MessageThread::run()
{
    pImpl->osal_messagedriven_loop(this);
}

// - class CallbackThread

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::CallbackThread::Impl
{    
    CallbackThread * taskParent;
    osal_task_t *osaltask;    
    osal_messagequeue_t *osalfunctionqueue;
    osal_messagequeue_t *osalargumentqueue;

    Config config;
    
    osal_task_properties_t osaltaskproperties;
    
    Impl(CallbackThread *parent) 
    {
        taskParent = parent;
        osaltask = nullptr;
        osalfunctionqueue = nullptr;
        osalargumentqueue = nullptr;
        
        config.startup = nullptr;
        config.aftercallback = dummyAfter; // i prefer assigning a dummy value in here. just in case. because to speed things up i dont check pImpl->onevent inside osal_messagedriven_loop()
        config.param = nullptr;
        config.stacksize = 64;
        config.priority = Priority::minimum;
        config.timeout = vnt::core::reltimeWaitForever;
        config.queuesize = 2;
        
        // to be used in osal_task_new1(). only priority and stacksize will change
        osaltaskproperties.function = osal_callbackdriven_loop;
        osaltaskproperties.param = taskParent;
        osaltaskproperties.priority = vnt::core::tointegral(config.priority);
        osaltaskproperties.stacksize = config.stacksize;
  
    }
    
    ~Impl()
    {
        if(nullptr != osaltask)
        {
            osal_task_delete(osaltask);
            osaltask = nullptr;
        }
        
        if(nullptr != osalfunctionqueue)
        {
            osal_messagequeue_delete(osalfunctionqueue);
            osalfunctionqueue = nullptr;
        }
        
        if(nullptr != osalargumentqueue)
        {
            osal_messagequeue_delete(osalargumentqueue);
            osalargumentqueue = nullptr;
        }
    }
    
    static void dummyAfter(Thread *t, core::Callback &m, void *p) {}
    
    static void osal_callbackdriven_loop(void *p) 
    {
        CallbackThread *t = reinterpret_cast<CallbackThread*>(p);
        const osal_reltime_t tout = static_cast<osal_reltime_t>(t->pImpl->config.timeout); // they are the same uint32_t type expressed in usec
        const Thread::fpStartup startup = t->pImpl->config.startup;
        const Thread::fpAfterCallback after = (nullptr != t->pImpl->config.aftercallback) ? (t->pImpl->config.aftercallback) : (dummyAfter);
        void * param = t->pImpl->config.param;
        osal_messagequeue_t *fQ = t->pImpl->osalfunctionqueue;
        osal_messagequeue_t *aQ = t->pImpl->osalargumentqueue;
        
        vnt::core::Callback cbk;;
       

        // exec the startup
        if(nullptr != startup)
        {
            startup(t, param);
        }
        
        // start the forever loop
        for(;;)
        {
            // osal_messagequeue_getquick() returns a osal_message_t which is a uint32_t* which may hold a simple integer of a pointer to larger data
            // the order is importnt in here.... dont exchange the following two lines .....
            cbk.arg = reinterpret_cast<void*>(osal_messagequeue_getquick(aQ, tout, osal_callerAUTOdetect)); 
            cbk.call = reinterpret_cast<vnt::core::fpCaller>(osal_messagequeue_getquick(fQ, tout, osal_callerAUTOdetect));
            // it executes only if it is valid 
            cbk.execute();
            after(t, cbk, param);
        }   

    }
    
};



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::CallbackThread::CallbackThread()
: pImpl(new Impl(this))
{   
    
}


vnt::os::CallbackThread::~CallbackThread()
{   
    delete pImpl;
}


vnt::os::Thread::Type vnt::os::CallbackThread::getType() const
{
    return Type::callbackTrigger;
}


vnt::os::Priority vnt::os::CallbackThread::getPriority() const
{   
    return pImpl->config.priority;
}


bool vnt::os::CallbackThread::setPriority(vnt::os::Priority priority)
{
    pImpl->config.priority = priority;
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(priority);

    osal_result_t r = osal_task_priority_set(pImpl->osaltask, pImpl->osaltaskproperties.priority);
    return (osal_res_OK == r) ? (true) : (false);
}

const char * vnt::os::CallbackThread::getName() const
{
    return (pImpl->config.name != nullptr) ? pImpl->config.name : "CallbackThread"; 
}
  
bool vnt::os::CallbackThread::setEvent(vnt::os::Event event)
{
    return false;
}  


bool vnt::os::CallbackThread::setMessage(vnt::os::Message message, core::relTime timeout)
{
    return false;
}

bool vnt::os::CallbackThread::setCallback(const vnt::core::Callback &callback, core::relTime timeout)
{
    if(false == callback.isvalid())
    {
        return false;
    }
    
    osal_result_t res = osal_res_NOK_generic;

    res = osal_messagequeue_put(pImpl->osalargumentqueue, reinterpret_cast<osal_message_t>(callback.arg), timeout, osal_callerAUTOdetect);
    if(osal_res_OK != res)
    {
        return false;
    }
    
    res = osal_messagequeue_put(pImpl->osalfunctionqueue, reinterpret_cast<osal_message_t>(callback.call), timeout, osal_callerAUTOdetect);
    if(osal_res_OK != res)
    {
        return false;
    }
 
    return true;
}



bool vnt::os::CallbackThread::start(const Config &cfg)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;    
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    pImpl->osalargumentqueue = osal_messagequeue_new(pImpl->config.queuesize); 
    pImpl->osalfunctionqueue = osal_messagequeue_new(pImpl->config.queuesize); 
    
    pImpl->osaltaskproperties.function = pImpl->osal_callbackdriven_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;  
       
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

bool vnt::os::CallbackThread::start(const Config &cfg, vnt::core::fpCaller eviewername)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;    
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    pImpl->osalargumentqueue = osal_messagequeue_new(pImpl->config.queuesize); 
    pImpl->osalfunctionqueue = osal_messagequeue_new(pImpl->config.queuesize); 
    
    pImpl->osaltaskproperties.function = (nullptr != eviewername) ? eviewername : pImpl->osal_callbackdriven_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;  
       
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

void vnt::os::CallbackThread::run()
{
    pImpl->osal_callbackdriven_loop(this);
}


// - class PeriodicThread

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

struct vnt::os::PeriodicThread::Impl
{    
    PeriodicThread * taskParent;
    osal_task_t *osaltask;    

    Config config;
    
    osal_task_properties_t osaltaskproperties;
    
    Impl(PeriodicThread *parent) 
    {
        taskParent = parent;
        osaltask = nullptr;

        config.startup = nullptr;
        config.onperiod = dummyOnPeriod; // i prefer assigning a dummy value in here. just in case. because to speed things up i dont check pImpl->onevent inside osal_messagedriven_loop()
        config.param = nullptr;
        config.stacksize = 64;
        config.priority = Priority::minimum;
        config.period = vnt::core::time1second;
        
        // to be used in osal_task_new1(). only priority and stacksize will change
        osaltaskproperties.function = osal_periodic_loop;
        osaltaskproperties.param = taskParent;
        osaltaskproperties.priority = vnt::core::tointegral(config.priority);
        osaltaskproperties.stacksize = config.stacksize;
  
    }
    
    ~Impl()
    {
        if(nullptr != osaltask)
        {
            osal_task_delete(osaltask);
            osaltask = nullptr;
        }        
    }
    
    static void dummyOnPeriod(Thread *t, void *p) {}
    
    static void osal_periodic_loop(void *p) 
    {
        PeriodicThread *t = reinterpret_cast<PeriodicThread*>(p);
        const osal_reltime_t period = static_cast<osal_reltime_t>(t->pImpl->config.period); // they are the same uint32_t type expressed in usec
        const Thread::fpStartup startup = t->pImpl->config.startup;
        const Thread::fpOnPeriod onperiod = t->pImpl->config.onperiod;
        void * param = t->pImpl->config.param;
       

        // exec the startup
        if(nullptr != startup)
        {
            startup(t, param);
        }

        osal_task_period_set(period);
        
        // start the forever loop
        for(;;)
        {
            osal_task_period_wait();
            onperiod(t, param);
        }        
    }
    
};


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


vnt::os::PeriodicThread::PeriodicThread()
: pImpl(new Impl(this))
{   
    
}


vnt::os::PeriodicThread::~PeriodicThread()
{   
    delete pImpl;
}


vnt::os::Thread::Type vnt::os::PeriodicThread::getType() const
{
    return Type::periodicTrigger;
}


vnt::os::Priority vnt::os::PeriodicThread::getPriority() const
{   
    return pImpl->config.priority;
}


bool vnt::os::PeriodicThread::setPriority(vnt::os::Priority priority)
{
    pImpl->config.priority = priority;
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(priority);

    osal_result_t r = osal_task_priority_set(pImpl->osaltask, pImpl->osaltaskproperties.priority);
    return (osal_res_OK == r) ? (true) : (false);
}


const char * vnt::os::PeriodicThread::getName() const
{
    return (pImpl->config.name != nullptr) ? pImpl->config.name : "PeriodicThread"; 
}
  
bool vnt::os::PeriodicThread::setEvent(vnt::os::Event event)
{
    return false;
}  


bool vnt::os::PeriodicThread::setMessage(vnt::os::Message message, core::relTime timeout)
{
    return false;
}

bool vnt::os::PeriodicThread::setCallback(const core::Callback &callback, core::relTime timeout)
{
    return false;
}


bool vnt::os::PeriodicThread::start(const Config &cfg)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    
    pImpl->osaltaskproperties.function = pImpl->osal_periodic_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;  
       
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

bool vnt::os::PeriodicThread::start(const Config &cfg, vnt::core::fpCaller eviewername)
{    
    if(false == cfg.isvalid())
    {
        return false;
    }
    
    pImpl->config = cfg;
    
    pImpl->config.stacksize = (pImpl->config.stacksize+7)/8;
    pImpl->config.stacksize *= 8;
    
    
    pImpl->osaltaskproperties.function = (nullptr == eviewername) ? eviewername : pImpl->osal_periodic_loop;
    pImpl->osaltaskproperties.param = this;        
    pImpl->osaltaskproperties.priority = vnt::core::tointegral(pImpl->config.priority);
    pImpl->osaltaskproperties.stacksize = pImpl->config.stacksize;  
       
    pImpl->osaltask = osal_task_new1(&pImpl->osaltaskproperties);
    // by the following i add a reference of this task to the osal object. it is used in vnt::os::taskRunning() 
    osal_task_extdata_set(pImpl->osaltask, this);
    
    return true;    
}

void vnt::os::PeriodicThread::run()
{
    pImpl->osal_periodic_loop(this);
}

    
// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

