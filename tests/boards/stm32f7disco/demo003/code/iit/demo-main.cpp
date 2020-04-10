 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/


#include "stm32hal.h"

#include "stm32f746g_discovery_demo.h"

#include "vnt_core.h"
#include "vnt_core_binary.h"
#include "vnt_bsp.h"
#include "vnt_bsp_trace.h"
#include "vnt_bsp_watchdog.h"

#include "vnt_os_theScheduler.h"
#include "vnt_os_theTimerManager.h"
#include "vnt_os_theCallbackManager.h"
#include "vnt_app_theLEDmanager.h"

#include "vnt_measure.h"


constexpr vnt::os::Event evtTick = 0x01;
constexpr vnt::core::Time evtTimeout = 50*vnt::core::time1millisec;


constexpr vnt::core::relTime tickperiod = 250*vnt::core::time1millisec;

void eventbasedthread_startup(vnt::os::Thread *t, void *param)
{
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has called startup() of thread " + std::string(t->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(t->getPriority())));  

  
    vnt::os::Timer *tmr = new vnt::os::Timer;
    
    vnt::os::Action act(vnt::os::EventToThread(evtTick, t));
    vnt::os::Timer::Config cfg{tickperiod, act, vnt::os::Timer::Mode::forever, 0};
    tmr->start(cfg);
    
    vnt::bsp::trace::puts(std::string(t->getName()) + ".startup() has started timer which sends evtTick to itself every " + std::to_string(tickperiod) + " usec");

    stm32f7_demo_init();
    
    
    vnt::bsp::watchdog::Config wdt {};
    vnt::bsp::watchdog::init(wdt);
    
}


void eventbasedthread_onevent(vnt::os::Thread *t, vnt::os::EventMask eventmask, void *param)
{
    vnt::bsp::watchdog::refresh();  
    
    if(0 == eventmask)
    {   // timeout ...         
        return;
    }

    if(true == vnt::core::binary::mask::check(eventmask, evtTick))
    {
              
        vnt::core::TimeFormatter tf(vnt::core::now()); 
        vnt::bsp::trace::puts(std::string(t->getName()) + ".onevent(): evtTick received @ " + tf.to_string());

        stm32f7_demo_tick();        
    }
                 
}


void onIdle(vnt::os::Thread *t, void* idleparam)
{
    static uint32_t i = 0;
    i++;
}


void initSystem(vnt::os::Thread *t, void* initparam)
{
    vnt::os::Thread * thr0 = vnt::os::theScheduler::getInstance().scheduledtask();  
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has started thread " + std::string(thr0->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(thr0->getPriority())));  

    
    vnt::bsp::trace::puts("thread " + std::string(thr0->getName()) + " @ " + tf1.to_string() + " is starting: theTimerManager, theCallbackManager"); 
    
    vnt::os::theTimerManager::getInstance().start({});     
    vnt::os::theCallbackManager::getInstance().start({});  
    
//    static const std::initializer_list<vnt::bsp::LED> allleds = {vnt::bsp::LED::one};  
//    vnt::app::theLEDmanager &theleds = vnt::app::theLEDmanager::getInstance();     
//    theleds.init(allleds);    
//    theleds.get(vnt::bsp::LED::one).pulse(vnt::core::time1second); 
       
    
    vnt::os::EventThread::Config configEV { 
        6*1024, 
        vnt::os::Priority::high200, 
        eventbasedthread_startup,
        nullptr,
        evtTimeout,
        eventbasedthread_onevent,
        "mainEVTthread"
    };
       
    tf1.load(vnt::core::now());    
    vnt::bsp::trace::puts("thread " + std::string(thr0->getName()) + " @ " + tf1.to_string() + " is starting: the " + std::string(configEV.name));     
    
    // create the main thread 
    vnt::os::EventThread* thr = new vnt::os::EventThread;          
    // and start it
    thr->start(configEV);
    
    
    
#if 0    
    // test of delay()
    vnt::core::Time t0 = vnt::core::now();
//    vnt::core::delay(60*1000*vnt::core::time1millisec);
    vnt::core::Time t1 = vnt::core::now();
    
    vnt::core::TimeFormatter tf(t1-t0); 
    
    vnt::bsp::trace::puts(std::string(t->getName()) + " has tested delay()");

    vnt::bsp::trace::puts(std::string(t->getName()) + " w/ vnt::bsp::wait(1000000). measured delta = " + tf.to_string());
    
    t0 = vnt::core::now();
 
    for(int i=0; i<120000; i++)
    {
        vnt::bsp::delay(vnt::core::time1millisec);
    }
//    vnt::bsp::delay(120*vnt::core::time1second);
    t1 = vnt::core::now();
    
    tf.load(t1-t0);
    vnt::bsp::trace::puts(std::string(t->getName()) + " measured delta = " + tf.to_string());
#endif

    tf1.load(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " is terminating thread " + std::string(thr0->getName()));    

//    vnt::measure::TimedPressure pp0;
//    vnt::measure::TimedPressure pp {};
//    pp.pressure = 7;
//    pp.timestamp = 2;
//        
//    vnt::measure::TimedPressure pp1 {};
//        
//    pp1 = pp;
//    vnt::measure::TimedPressure pp2 {pp};    

}

void onOSerror(void *p)
{
    vnt::os::Thread * thr = vnt::os::theScheduler::getInstance().scheduledtask();  
    int errorcode = -1;
    const char *errorstring = vnt::os::theScheduler::getInstance().getOSerror(errorcode);
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("OS ERROR @ " + tf1.to_string() + " in thread " + std::string(thr->getName()) + " -> code = " + std::to_string(errorcode) + ", msg = " + errorstring); 
    vnt::bsp::trace::puts("theScheduler will keep execution but the OS will stop the thread which caused the error. please check.");  
}


int main(void)
{

    constexpr vnt::os::InitThread::Config initcfg = { 4*1024, initSystem, nullptr };
    constexpr vnt::os::IdleThread::Config idlecfg = { 512, nullptr, nullptr, onIdle };

    constexpr vnt::os::Config osconfig {vnt::core::time1millisec, initcfg, idlecfg, vnt::core::Callback{onOSerror, nullptr}};
    
    // vnt::os::init() internally calls vnt::bsp::init() which also calls vnt::core::init()
    vnt::os::init(osconfig);
    
    // now i start the os    
    vnt::os::start();
}



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


