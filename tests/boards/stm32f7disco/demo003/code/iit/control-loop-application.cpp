 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

#if 0
this demo simulates a skeleton of the control loop @ 100 hz with simulated use 
of some pressure sensors, call of the matlab-generated controller and valve actuation.
#endif


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
#include "vnt_bsp_sns_pressure.h"
#include "vnt_bsp_act_valve.h"


// from matlab
#include "controller.h"                // Model's header file
#include "rtwtypes.h"
#include "zero_crossing_types.h"


// the events sent to the tCTRL thread
constexpr vnt::os::Event evtCTRLtick = vnt::core::binary::mask::pos2mask<vnt::os::Event>(1);
constexpr vnt::os::Event evtSNSRacquire = vnt::core::binary::mask::pos2mask<vnt::os::Event>(2);
constexpr vnt::os::Event evtSNSRready01 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(3);
constexpr vnt::os::Event evtSNSRready02 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(4);
constexpr vnt::os::Event evtSNSRready03 = vnt::core::binary::mask::pos2mask<vnt::os::Event>(5);
// the timeout on waiting for the events
constexpr vnt::core::Time timeoutEVTwait = 10*vnt::core::time1millisec;

// the period of controller + anticipation of sensor reding
constexpr vnt::core::relTime periodOfCTRL = 10*vnt::core::time1millisec;
constexpr vnt::core::relTime anticipationOfSNSRreading = 4*vnt::core::time1millisec;

// the timers which trigger the control loop at precise times: start of acquisition + time of control 
vnt::os::Timer *tmrSNSR = nullptr;
vnt::os::Timer *tmrCTRL = nullptr;

#if 0
not needed in this demo if we use vnt::os::Action to alert and not callbacks
static void alertdataisready(void *p)
{
    vnt::os::Action *a = reinterpret_cast<vnt::os::Action*>(p);
    a->execute();                         
}
vnt::core::Callback cbkAlertreadySN01 {}; 
vnt::core::Callback cbkAlertreadySN02 {};
vnt::core::Callback cbkAlertreadySN03 {};  
#endif 


vnt::os::Action actAlertreadySNS01 {};
vnt::measure::TimedPressure tp01 {};
    
vnt::os::Action actAlertreadySNS02 {};
vnt::measure::TimedPressure tp02 {};    
    
vnt::os::Action actAlertreadySNS03 {};
vnt::measure::TimedPressure tp03 {};
    
    
struct AllSensors
{
    vnt::measure::TimedPressure s1 {};
    vnt::measure::TimedPressure s2 {};
    vnt::measure::TimedPressure s3 {};    
};

struct AllActuators
{
    vnt::measure::ValveCtrl v1 {};
    vnt::measure::ValveCtrl v2 {};  
};
 
  

static controllerModelClass controller_Obj;// Instance of model class


struct theController
{
    // it is just a fake controller!
    AllActuators ac {};
    float status {0};
    theController() = default;
    
    bool init() 
    {
        controller_Obj.initialize();
        //controller_Obj.controller_U.enable = true; // sic it is private. i have modified it inside teh code    
        return true;
    }
    
    const AllActuators& compute(const AllSensors &s)
    {

        volatile vnt::core::Time t0 = vnt::core::now();
        controller_Obj.step();
        volatile vnt::core::Time t1 = vnt::core::now();
        vnt::bsp::trace::puts("duration of controller_Obj.step() = " + vnt::core::TimeFormatter(t1-t0).to_string(vnt::core::TimeFormatter::Mode::milli));
     
        status = s.s1.pressure + s.s2.pressure  + s.s3.pressure;       
        ac.v1 = status + 1;
        ac.v2 = status + 2;
        return ac;
    }        

};   

AllSensors sensorcache {};
AllActuators actuators {};
theController controller {};  

void tCTRL_startup(vnt::os::Thread *t, void *param)
{
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has called startup() of thread " + std::string(t->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(t->getPriority())));  
    
    static const std::initializer_list<vnt::bsp::LED> allleds = {vnt::bsp::LED::testGREEN};  
    vnt::app::theLEDmanager &theleds = vnt::app::theLEDmanager::getInstance();     
    theleds.init(allleds);    
    theleds.get(vnt::bsp::LED::one).pulse(vnt::core::time1second);
  
    tmrSNSR = new vnt::os::Timer;
    tmrCTRL = new vnt::os::Timer;
    

    tmrSNSR->start({periodOfCTRL-anticipationOfSNSRreading, vnt::os::EventToThread(evtSNSRacquire, t), vnt::os::Timer::Mode::oneshot, 0});
    tmrCTRL->start({periodOfCTRL, {vnt::os::EventToThread(evtCTRLtick, t)}, vnt::os::Timer::Mode::forever, 0});
    
//    vnt::bsp::trace::puts(std::string(t->getName()) + ".startup() has started timer which sends evtTick to itself every " + std::to_string(tickperiod) + " usec");

    //#warning TODO: much easier if we use an osAction as argument of vnt::bsp::sns::pressure::read()
    vnt::bsp::sns::pressure::init(vnt::bsp::sns::PRS::S1, {});
    actAlertreadySNS01 = {vnt::os::EventToThread(evtSNSRready01, t)}; 
    //cbkAlertreadySN01 = {alertdataisready, &actAlertreadySNS01}; 
    
    vnt::bsp::sns::pressure::init(vnt::bsp::sns::PRS::S2, {});
    actAlertreadySNS02 = {vnt::os::EventToThread(evtSNSRready02, t)}; 
    //cbkAlertreadySN02 = {alertdataisready, &actAlertreadySNS02}; 
    
    vnt::bsp::sns::pressure::init(vnt::bsp::sns::PRS::S3, {});
    actAlertreadySNS03 = {vnt::os::EventToThread(evtSNSRready03, t)}; 
    //cbkAlertreadySN03 = {alertdataisready, &actAlertreadySNS03};
        
    vnt::bsp::act::valve::init(vnt::bsp::act::VLV::V1, {});
    vnt::bsp::act::valve::init(vnt::bsp::act::VLV::V2, {});
        
    controller.init();    
    
    vnt::bsp::watchdog::Config wdt {};
    vnt::bsp::watchdog::init(wdt);
    
}


 
    
void tCTRL_onevent(vnt::os::Thread *t, vnt::os::EventMask eventmask, void *param)
{
    vnt::bsp::watchdog::refresh();  
    
    if(0 == eventmask)
    {   // timeout ...         
        return;
    }

    if(true == vnt::core::binary::mask::check(eventmask, evtSNSRacquire))
    {
        if(vnt::os::Timer::Status::idle == tmrSNSR->getStatus())
        {   // starting it in forever mode to send event evtCTRLtick every periodOfCTRL usec. 
            tmrSNSR->start({periodOfCTRL, vnt::os::EventToThread(evtSNSRacquire, t), vnt::os::Timer::Mode::forever, 0});
        }
        
        vnt::bsp::trace::puts("");
        vnt::bsp::trace::puts("->");
        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL commands to HW to start acquisition of 3 sensors");      

#if 0
        // this is the mode with the callback. it is redundant because we go through the callback manager
        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S1, &tp01, cbkAlertreadySN01);  
        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S2, &tp02, cbkAlertreadySN02);   
        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S3, &tp03, cbkAlertreadySN03);
#else
        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S1, &tp01, actAlertreadySNS01);  
        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S2, &tp02, actAlertreadySNS02);   
        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S3, &tp03, actAlertreadySNS03);
#endif        
    }
    
    if(true == vnt::core::binary::mask::check(eventmask, evtSNSRready01))
    {
        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has retrieved sensor 1: " + 
                              std::to_string(tp01.pressure) + "@" + vnt::core::TimeFormatter(tp01.timestamp).to_string()); 
        // copy tp01 into status
        sensorcache.s1 = tp01;              
    }  

    if(true == vnt::core::binary::mask::check(eventmask, evtSNSRready02))
    {
        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has retrieved sensor 2: " + 
                              std::to_string(tp02.pressure) + "@" + vnt::core::TimeFormatter(tp02.timestamp).to_string());
        // copy tp02 into status
        sensorcache.s2 = tp02;               
    }    
 
    if(true == vnt::core::binary::mask::check(eventmask, evtSNSRready03))
    {
        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has retrieved sensor 3: " + 
                              std::to_string(tp03.pressure) + "@" + vnt::core::TimeFormatter(tp03.timestamp).to_string());        
        // copy tp03 into status
        sensorcache.s3 = tp03;                
    }    

    if(true == vnt::core::binary::mask::check(eventmask, evtCTRLtick))
    {
        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL executes the control");  

        const AllActuators &act = controller.compute(sensorcache);
        vnt::bsp::act::valve::apply(vnt::bsp::act::VLV::V1, act.v1);  
        vnt::bsp::act::valve::apply(vnt::bsp::act::VLV::V2, act.v2);    

        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has applied values " +  
                              std::to_string(act.v1) + " " + std::to_string(act.v2));   


        vnt::bsp::trace::puts("<-");      
        vnt::bsp::trace::puts("");        
    }        
    
}


void onIdle(vnt::os::Thread *t, void* idleparam)
{
    static uint32_t i = 0;
    i++;
}

void tCTRL(void *p)
{
    vnt::os::Thread *t = reinterpret_cast<vnt::os::Thread*>(p);
    t->run();
}

void initSystem(vnt::os::Thread *t, void* initparam)
{
    vnt::os::Thread * thr0 = vnt::os::theScheduler::getInstance().scheduledtask();  
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has started thread " + std::string(thr0->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(thr0->getPriority())));  

    
    vnt::bsp::trace::puts("thread " + std::string(thr0->getName()) + " @ " + tf1.to_string() + " is starting: theTimerManager, theCallbackManager"); 
    
    vnt::os::theTimerManager::getInstance().start({1024, 16, vnt::os::Priority::system240});     
    vnt::os::theCallbackManager::getInstance().start({1024, 16, vnt::os::Priority::high200});  
    
    static const std::initializer_list<vnt::bsp::LED> allleds = {vnt::bsp::LED::one};  
    vnt::app::theLEDmanager &theleds = vnt::app::theLEDmanager::getInstance();     
    theleds.init(allleds);    
    theleds.get(vnt::bsp::LED::one).pulse(vnt::core::time1second); 
       
    
    vnt::os::EventThread::Config configCTRLthread { 
        8*1024, 
        vnt::os::Priority::high204, 
        tCTRL_startup,
        nullptr,
        timeoutEVTwait,
        tCTRL_onevent,
        "threadCTRL"
    };
       
    tf1.load(vnt::core::now());    
    vnt::bsp::trace::puts("thread " + std::string(thr0->getName()) + " @ " + tf1.to_string() + " is starting: the " + std::string(configCTRLthread.name));     
    
    // create the main control thread 
    vnt::os::EventThread* thr = new vnt::os::EventThread;          
    // and start it
    //thr->start(configCTRLthread);
    thr->start(configCTRLthread, tCTRL);
    
    
    
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

    // testing w/ some different systick periods
    // conclusion: so far 1 milli is ok. we can go down to 500 usec but no more otherwise the uvision stops working
//    constexpr vnt::os::Config osconfig {100*vnt::core::time1microsec, initcfg, idlecfg, vnt::core::Callback{onOSerror, nullptr}};
//    constexpr vnt::os::Config osconfig {500*vnt::core::time1microsec, initcfg, idlecfg, vnt::core::Callback{onOSerror, nullptr}};
    constexpr vnt::os::Config osconfig {vnt::core::time1millisec, initcfg, idlecfg, vnt::core::Callback{onOSerror, nullptr}};
    vnt::os::init(osconfig);
    
    // now i start the os    
    vnt::os::start();
}



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


