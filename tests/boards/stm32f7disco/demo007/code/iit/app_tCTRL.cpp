 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

#include "app_tCTRL.h"


// needed to show the name of the thread in uvision: keep it global out of any namespace
void t_CTRL(void *p)
{
    vnt::os::Thread *t = reinterpret_cast<vnt::os::Thread*>(p);
    t->run();
}

// the very basic needed to start a thread. it may become a code template. for now we duplicate it.

namespace app { namespace tCTRL { 

    void startup(vnt::os::Thread *t, void *param);
    void onevent(vnt::os::Thread *t, vnt::os::EventMask eventmask, void *param);

    constexpr vnt::os::EventThread::Config tcfg { 
        settings.stacksize, 
        settings.priority, 
        startup,
        nullptr,
        settings.timeout,
        onevent,
        "t_CTRL"
    };

    vnt::os::EventThread* thr = nullptr;

    bool start()
    {
        if(nullptr != thr)
        {   // i dont want to start twice
            return false;
        }

        thr = new vnt::os::EventThread;

        if(nullptr == thr)
        {
            return false;
        }

        return thr->start(tcfg, t_CTRL);
    }
    
    vnt::os::Thread * thread()
    {
        return thr;
    }
    
    
    bool trigger(const EVT ev)
    {
        if(nullptr == thr)
        {
            return false;
        }
        return thr->setEvent(vnt::core::tointegral(ev));         
    }
    
       
}} // namespace app { namespace tCTRL { 


// - the code which specialises the thread CTRL
//   i can put includes in here. it is not canonical but it more clear to show the actual dependencies

#include "vnt_bsp.h"
#include "vnt_bsp_trace.h"
#include "vnt_bsp_watchdog.h"
#include "vnt_os_Timer.h"


#include "vnt_measure.h"
#include "vnt_bsp_sns_pressure.h"
#include "vnt_bsp_act_valve.h"

#include "board_driver.h"

#include "app_theController.h"

#if defined(TEST_tCTRL_control_use_stored_IO)
#warning CAVEAT: TEST_tCTRL_control_use_stored_IO is defined 
// marco.accame: the formatted puts lasts a loooot of time: ~200 usec w/ -O0 and 70 usec w/ -Os balanced. better removing it if we dont need it
#define TEST_tCTRL_control_use_stored_IO__TRACE_FORMATTED   
#if defined(TEST_tCTRL_control_use_stored_IO__TRACE_FORMATTED)
#warning CAVEAT: TEST_tCTRL_control_use_stored_IO__TRACE_FORMATTED is defined. just this activity lasts about 200 usec 
#endif
#endif

#if defined(TEST_tCTRL_control_use_stored_IO)
#include "app_theControllerValidator.h"
#endif

namespace app { namespace tCTRL {


// here are the callbacks which alert the thread that teh sensors are ready
// they must be changed when hw driver is ready
// the idea is that the callback sends an event to the thread.

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

// something like that for each sensor. it can be called from an IRQhandler. 
static void alertdata01isready(void *p)
{
    app::tCTRL::thread()->setEvent(vnt::core::tointegral(EVT::SNSRready01));
}
vnt::core::Callback cbkAlertreadySN01 {alertdata01isready, nullptr};


// so far, for simulation of the sensors we just use an action ... 
    
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
 
  

AllSensors sensorcache {};
AllActuators actuators {};
    

    
board_sensor_data_t allinput {0};
board_actuation_data_t outdata {};
    
vnt::os::Timer *timerTICK = nullptr;

void startup(vnt::os::Thread *t, void *param)
{
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has called startup() of thread " + std::string(t->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(t->getPriority())));  
    
  
    timerTICK = new vnt::os::Timer;
    
    timerTICK->start({settings.periodOfTICK, {vnt::os::EventToThread(vnt::core::tointegral(EVT::tick), t)}, vnt::os::Timer::Mode::forever, 0});
    
//    vnt::bsp::trace::puts(std::string(t->getName()) + ".startup() has started timer which sends evtTick to itself every " + std::to_string(tickperiod) + " usec");

    //#warning TODO: much easier if we use an osAction as argument of vnt::bsp::sns::pressure::read()
    vnt::bsp::sns::pressure::init(vnt::bsp::sns::PRS::S1, {});
    actAlertreadySNS01 = {vnt::os::EventToThread(vnt::core::tointegral(EVT::SNSRready01), t)}; 
    //cbkAlertreadySN01 = {alertdataisready, &actAlertreadySNS01}; 
    
    vnt::bsp::sns::pressure::init(vnt::bsp::sns::PRS::S2, {});
    actAlertreadySNS02 = {vnt::os::EventToThread(vnt::core::tointegral(EVT::SNSRready02), t)}; 
    //cbkAlertreadySN02 = {alertdataisready, &actAlertreadySNS02}; 
    
    vnt::bsp::sns::pressure::init(vnt::bsp::sns::PRS::S3, {});
    actAlertreadySNS03 = {vnt::os::EventToThread(vnt::core::tointegral(EVT::SNSRready03), t)}; 
    //cbkAlertreadySN03 = {alertdataisready, &actAlertreadySNS03};
        
    vnt::bsp::act::valve::init(vnt::bsp::act::VLV::V1, {});
    vnt::bsp::act::valve::init(vnt::bsp::act::VLV::V2, {});
        
 
        
    app::theController::getInstance().init({});    
        
#if defined(TEST_tCTRL_control_use_stored_IO)        
    app::theControllerValidator::getInstance().init({});     
#endif  

       
            
}

void alertme(int v)
{
    app::tCTRL::trigger(app::tCTRL::EVT::dataready);
}

constexpr vnt::os::EventMask evtmask_allsensors = vnt::core::tointegral(EVT::SNSRready01) | vnt::core::tointegral(EVT::SNSRready02) | vnt::core::tointegral(EVT::SNSRready03);
volatile vnt::os::EventMask evtmask_rxsensors = 0;


// keep it lean.
// you may add a histogram builder upon duration of events, maybe ...

//volatile vnt::core::Time times[8] = {0};
void onevent(vnt::os::Thread *t, vnt::os::EventMask eventmask, void *param)
{
//    times[7] = vnt::core::now();
    vnt::bsp::watchdog::refresh();  
    
    if(0 == eventmask)
    {   // timeout ...         
        return;
    }

    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::tick)))
    {
//        times[0] = vnt::core::now();
        evtmask_rxsensors = 0;
        
//        vnt::bsp::trace::puts("");
//        vnt::bsp::trace::puts("->");
//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL commands to HW to start acquisition of 3 sensors");      

//#if 0
//        // this is the mode with the callback. it is redundant because we go through the callback manager
//        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S1, &tp01, cbkAlertreadySN01);  
//        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S2, &tp02, cbkAlertreadySN02);   
//        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S3, &tp03, cbkAlertreadySN03);
//#else
//        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S1, &tp01, actAlertreadySNS01);  
//        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S2, &tp02, actAlertreadySNS02);   
//        vnt::bsp::sns::pressure::read(vnt::bsp::sns::PRS::S3, &tp03, actAlertreadySNS03);
//#endif    
        
        board_read_sensors_async(&allinput, alertme);

    }

#if 0    
    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::SNSRready01)))
    {
//        times[1] = vnt::core::now();
//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has retrieved sensor 1: " + 
//                              std::to_string(tp01.pressure) + "@" + vnt::core::TimeFormatter(tp01.timestamp).to_string()); 
        // copy tp01 into status
        sensorcache.s1 = tp01;    
        evtmask_rxsensors |= vnt::core::tointegral(EVT::SNSRready01);       
    }  

    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::SNSRready02)))
    {
//        times[2] = vnt::core::now();
//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has retrieved sensor 2: " + 
//                              std::to_string(tp02.pressure) + "@" + vnt::core::TimeFormatter(tp02.timestamp).to_string());
        // copy tp02 into status
        sensorcache.s2 = tp02;
        evtmask_rxsensors |= vnt::core::tointegral(EVT::SNSRready02);        
    }    
 
    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::SNSRready03)))
    {
//        times[3] = vnt::core::now();
//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has retrieved sensor 3: " + 
//                              std::to_string(tp03.pressure) + "@" + vnt::core::TimeFormatter(tp03.timestamp).to_string());        
        // copy tp03 into status
        sensorcache.s3 = tp03; 
        evtmask_rxsensors |= vnt::core::tointegral(EVT::SNSRready03);        
    }    
#endif

    // keep it last ...
    if((evtmask_allsensors == evtmask_rxsensors) || (true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::dataready))))
    {
        
        static volatile uint16_t x = 0;
        x = allinput.flow1;
        
        
//        times[4] = vnt::core::now();
        evtmask_rxsensors = 0;
//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL executes the control");  
        
#if !defined(TEST_tCTRL_control_use_stored_IO)
        app::Input inp {0};        
        app::theController::getInstance().set(inp);
        app::theController::getInstance().tick();
        const app::Output &out = app::theController::getInstance().get();
        #warning TODO: apply output
        
//        const AllActuators &act = controller.compute(sensorcache);
//        vnt::bsp::act::valve::apply(vnt::bsp::act::VLV::V1, act.v1);  
//        vnt::bsp::act::valve::apply(vnt::bsp::act::VLV::V2, act.v2);   
        
#else  
        //volatile vnt::core::Time t0 = vnt::core::now();        
        app::Input inp {0};        
        app::Output target {0};
        app::theControllerValidator::getInstance().get(inp, target);
        static size_t step = 0;
        app::theController::getInstance().set(inp);
        volatile vnt::core::Time t1 = vnt::core::now();
        app::theController::getInstance().tick();
        volatile vnt::core::Time t2 = vnt::core::now();
        const app::Output &out = app::theController::getInstance().get();
        float ae = 0;
        bool ok = app::theControllerValidator::getInstance().check(out, target, ae);
        // marco.accame: the formatted puts lasts a loooot of time: ~200 usec. better removing it if we dont need it
#if defined(TEST_tCTRL_control_use_stored_IO__TRACE_FORMATTED)        
        vnt::bsp::trace::puts(std::to_string(step) + 
            std::string(ok? ", 1, " : ", 0, ") +
            vnt::core::TimeFormatter(t2-t1).to_string(vnt::core::TimeFormatter::Mode::milli) + ", " +
            std::to_string(ae)              + ", " + 
            std::to_string(inp[0])          + ", " +
            std::to_string(target[0])       + ", " + 
            std::to_string(target[1])       + ", " +
            std::to_string(target[2])       + ", " +
            std::to_string(target[3])       + ", " +
            std::to_string(out[0])          + ", " +
            std::to_string(out[1])          + ", " +
            std::to_string(out[2])          + ", " +
            std::to_string(out[3])
        );
#else
    // marco.accame: this puts lasts much less 
    char numofusec[16] = {0}; std::snprintf(numofusec, sizeof(numofusec), "m%du%d", static_cast<unsigned int>((t2-t1)/1000), static_cast<unsigned int>((t2-t1)%1000));
    vnt::bsp::trace::puts(numofusec);
#endif
        step++;
        
        ok = ok;
        ae = ae;
//        times[5] = vnt::core::now();
#endif        
 

//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL has applied values " +  
//                              std::to_string(act.v1) + " " + std::to_string(act.v2));   


//        vnt::bsp::trace::puts("<-");      
//        vnt::bsp::trace::puts("");    

        board_apply_actuation(&outdata);
    }  

//    if(evtmask_allsensors == evtmask_rxsensors)
//    {
//        t->setEvent(vnt::core::tointegral(EVT::dataready));
//    }        
    
}


}} // namespace app { namespace tCTRL {


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


