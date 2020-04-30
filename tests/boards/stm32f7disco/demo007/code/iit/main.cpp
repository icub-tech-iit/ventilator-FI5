 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

#if 0
this demo implements:

- tCTRL
  it is a skeleton of the control loop with simulated use of some pressure sensors, 
  call of the matlab-generated controller and valve actuation.
  the control loop is executed inside the high priority thread called tCTRL which
  is triggered by a single timer @ 100 hz which sends an event which starts acquisition. 
  when the sensors are ready, they finally trigger a second event which executes the control tick.
  the control tick is c++ code generated by matlab.
  if TEST_tCTRL_control_use_fake_IO is defined, then the control just uses ...
  if TEST_tCTRL_control_use_stored_IO is defined, then the control uses data coming from ugo's
  matlab simulation
  
- tHMI
  it is a second thread tHMI at lower priority which manages the hmi. the thread is event based
  and is regularly ticked every timeHMItick Hz (e.g., 50-100 milli). at the tick event,
  the tHMI thread will sample the inputs (phisical buttons + touch screen) and refresh
  the LCD and possibly propagates commands. the thread can also be triggered a-synchronously
  by specific events coming from physical buttons or sliders which may be labeled as 
  high priority.

#endif


#if defined(APP_HMI_disable)
    #warning CAVEAT EMPTOR: the HMI is disabled
#else
    #warning CAVEAT EMPTOR: the HMI is enabled
#endif

#include "vnt_core.h"
#include "vnt_bsp.h"
#include "vnt_bsp_trace.h"
#include "vnt_bsp_watchdog.h"

#include "vnt_os_theScheduler.h"
#include "vnt_os_theTimerManager.h"
#include "vnt_os_theCallbackManager.h"

#include "app_tCTRL.h"
#include "app_tHMI.h"

#include "board_driver.h"

void onIdle(vnt::os::Thread *t, void* idleparam);
void onOSerror(void *p);
void initSystem(vnt::os::Thread *t, void* initparam);

constexpr uint16_t stackOfInitThread = 4*1024;
constexpr uint16_t stackOfIdleThread = 1*1024;
constexpr vnt::os::InitThread::Config initcfg = { stackOfInitThread, initSystem, nullptr };
constexpr vnt::os::IdleThread::Config idlecfg = { stackOfIdleThread, nullptr, nullptr, onIdle };
constexpr vnt::os::Config osconfig {vnt::core::time1millisec, initcfg, idlecfg, vnt::core::Callback{onOSerror, nullptr}};


int main(void)
{
    // initialise the os
    vnt::os::init(osconfig);
    
    vnt::bsp::trace::puts("CAVEAT: trace is enabled. to see the scope disable it!"); 
    
    // and start it    
    vnt::os::start();
}

// - here is what the os does

void onIdle(vnt::os::Thread *t, void* idleparam)
{
    static uint32_t i = 0;
    i++;
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

#if defined(APP_HMI_disable)
#else
#include "ewmain.h"
#include "ewrte.h"
#include "ew_bsp_system.h"
#include "ew_bsp_console.h"
#include "ew_bsp_clock.h"
#endif

//static  board_config_t board_config =
//{
//    .pressure_sensor1 =
//    {
//        .type = PRESSURE_SENSOR_TYPE_HSCDANN150PG2A5,
//        .address = 0x28
//    },
//    .pressure_sensor2 =
//    {
//        .type = PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
//        .address = 0x28
//    },
//    .pressure_sensor3 =
//    {
//        .type = PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
//        .address = 0x28
//    },
//    .pressure_sensor4 =
//    {
//        .type = PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
//        .address = 0x28
//    },
//    .flow_sensor1 =
//    {
//        .type = FLOW_SENSOR_TYPE_ZEPHYR,
//        .address = 0x49
//    },
//    .flow_sensor2 =
//    {
//        .type = FLOW_SENSOR_TYPE_ZEPHYR,
//        .address = 0x49
//    },
//    .o2_sensor =
//    {
//        .type = O2_SENSOR_TYPE_NONE,
//        .address = 0x00
//    },
//    .gpio_expander =
//    {
//        .type = GPIO_EXPANDER_TYPE_MCP23017,
//        .address = 0x20
//    }
//};

void initSystem(vnt::os::Thread *t, void* initparam)
{
    vnt::os::Thread * thr0 = vnt::os::theScheduler::getInstance().scheduledtask();  
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has started thread " + std::string(thr0->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(thr0->getPriority())));  

    
    vnt::bsp::trace::puts("thread " + std::string(thr0->getName()) + " @ " + tf1.to_string() + " is starting: theTimerManager, theCallbackManager"); 
    
    vnt::os::theTimerManager::getInstance().start({1024, 16, vnt::os::Priority::system240});     
    vnt::os::theCallbackManager::getInstance().start({1024, 16, vnt::os::Priority::high200});  
    
    
    // start the tCTRL thread [at exit of tINIT thread]
    app::tCTRL::start();
    
    // start the tHMI thread [at exit of tINIT thread]
    app::tHMI::start();

    board_init(p_board_config_default);  

#if defined(APP_HMI_disable)
#else
    // for now: embedded wizard initialization must stay in here
    // it cannot stay in the tHMI.startup() ... one problem at a time     
    EwBspSystemInit();
    /* initialize console interface for debug messages */
    EwBspConsoleInit();
    /* initialize Embedded Wizard application */
    EwInit();
    EwPrintSystemInfo(); 
#endif  

    volatile std::uint32_t clk = clock(vnt::bsp::CLOCK::syscore);
    clk = clk;
    
    // start wdt
    vnt::bsp::watchdog::Config wdt {};
    vnt::bsp::watchdog::init(wdt);  
    // in multi-thread all threads must refresh, hence we need to add a register thread method. or an id,
    // TODO: think of it. 


    tf1.load(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " is terminating thread " + std::string(thr0->getName()));       
}

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


