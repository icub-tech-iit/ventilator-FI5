 
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


// place where put the control mode



constexpr int IDLE = 1;
constexpr int CPAP = 2;
constexpr int VCV = 3;
constexpr int PRVC = 4;

volatile int CTRLmode = IDLE; // vcv
volatile int IERvalue = 50;

volatile int V1perc = 0;
volatile bool V2on = true;

volatile int v2val = 2500;


// - the code which specialises the thread CTRL
//   i can put includes in here. it is not canonical but it more clear to show the actual dependencies

struct printOptions
{
    bool enabled {false};
    uint32_t printDIVIDER {10};
    bool enableBRDdriver {true};
    bool enableCTRLinput {true};
    bool enableCTRLoutput {true};    
    constexpr printOptions(bool en, uint32_t dv, bool eBd, bool eCi, bool eCo) : enabled(en), printDIVIDER(dv), enableBRDdriver(eBd), enableCTRLinput(eCi), enableCTRLoutput(eCo) {}
    
    printOptions() = default;    
};

constexpr printOptions _printOPT {true, 100, true, false, false};


//#define TEST_CP_WAVEFORM

#if defined(APP_DRV_onlyBTNSGPIO)
#define TEST_BTNSGPIO
#endif

#include "vnt_bsp.h"
#include "vnt_bsp_trace.h"
#include "vnt_bsp_watchdog.h"
#include "vnt_os_Timer.h"


#include "vnt_measure.h"
#include "vnt_bsp_sns_pressure.h"
#include "vnt_bsp_act_valve.h"

#include "board_driver.h"

#include "stm32hal.h"

#include "app_theController.h"

#include "osal_mutex.h"

namespace app { namespace tCTRL {

struct PercWaveform
{
    uint32_t _t {0};
    float _step {1.0};
    
    float tick()
    {
        float v = ++_t*_step;
        if(v > 100.0)
        {
            _t = 0;
            v = 0;
        }
        return v;
    }
    
    PercWaveform() = default;
};
    

PercWaveform percwave {};
    
board_sensor_data_t allinput {0};
board_actuation_data_t outdata {};
    
board_sensor_data_t latestSNSdata {0};

app::theController::Out latestCTRLout {0};
    
vnt::os::Timer *timerTICK = nullptr;
    
osal_mutex_t *mtx =  nullptr;


void print(const board_sensor_data_t &sns)
{
    
    char gpio[16] = {0};
    snprintf(gpio, sizeof(gpio), "0x%x", sns.gpio);
    char btns[16] = {0};
    snprintf(btns, sizeof(btns), "0x%x", sns.buttons);
    char rmsk[16] = {0};
    snprintf(rmsk, sizeof(rmsk), "0x%x", sns.read_mask);
    vnt::bsp::trace::puts(std::string("SNSdata:") +
                    " .pressure1 = " + std::to_string(sns.pressure1) +
                    " .pressure1_raw = " + std::to_string(sns.pressure1_raw) +
                    " .pressure2 = " + std::to_string(sns.pressure2) +
                    " .pressure2_raw = " + std::to_string(sns.pressure2_raw) +
                    " .pressure3 = " + std::to_string(sns.pressure3) +
                    " .pressure3_raw = " + std::to_string(sns.pressure3_raw) +
                    " .pressure4 = " + std::to_string(sns.pressure4) +
                    " .pressure4_raw = " + std::to_string(sns.pressure4_raw) +
                    " .flow1 = " + std::to_string(sns.flow1) +
                    " .flow1_raw = " + std::to_string(sns.flow1_raw) +
                    " .flow2 = " + std::to_string(sns.flow2) +
                    " .flow2_raw = " + std::to_string(sns.flow2_raw) +    
                    " .o2 = " + std::to_string(sns.o2) +
                    " .temperature1 = " + std::to_string(sns.temperature1) +
                    " .temperature1_raw = " + std::to_string(sns.temperature1_raw) +
                    " .temperature2 = " + std::to_string(sns.temperature2) +
                    " .temperature2_raw = " + std::to_string(sns.temperature2_raw) +
                    " .temperature3 = " + std::to_string(sns.temperature3) +
                    " .temperature3_raw = " + std::to_string(sns.temperature3_raw) +    
                    " .temperature4 = " + std::to_string(sns.temperature4) +
                    " .temperature4_raw = " + std::to_string(sns.temperature4_raw) +    
                    " .gpio = " + gpio +
                    " .encoder = " + std::to_string(sns.encoder) +
                    " .buttons = " + btns +
                    " .analog_input[0] = " + std::to_string(sns.analog_input[0]) +
                    " .analog_input[1] = " + std::to_string(sns.analog_input[1]) +
                    " .analog_input[2] = " + std::to_string(sns.analog_input[2]) +
                    " .analog_input[3] = " + std::to_string(sns.analog_input[3]) +
                    " .read_mask = " + rmsk
    );  
    
}

void print_inputs(const board_sensor_data_t *sns)
{
    
    if(0 != sns->buttons)
    {
        for(;;);
        
    }
    
    char gpio[16] = {0};
    snprintf(gpio, sizeof(gpio), "0x%x", sns->gpio);
    char btns[16] = {0};
    snprintf(btns, sizeof(btns), "0x%x", sns->buttons);
    char rmsk[16] = {0};
    snprintf(rmsk, sizeof(rmsk), "0x%x", sns->read_mask);
    vnt::bsp::trace::puts(std::string("SNSdata:") +

                    " .gpio = " + gpio +
                    " .enc = " + std::to_string(sns->encoder) +
                    " .btns = " + btns +
                    " .ai[0] = " + std::to_string(sns->analog_input[0]) +
                    " .ai[1] = " + std::to_string(sns->analog_input[1]) +
                    " .ai[2] = " + std::to_string(sns->analog_input[2]) +
                    " .ai[3] = " + std::to_string(sns->analog_input[3]) +
                    " .read_mask = " + rmsk
    );  
    
}

void startup(vnt::os::Thread *t, void *param)
{
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has called startup() of thread " + std::string(t->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(t->getPriority())));  
        
    
    mtx = osal_mutex_new();

    timerTICK = new vnt::os::Timer;
    
    timerTICK->start({settings.periodOfTICK, {vnt::os::EventToThread(vnt::core::tointegral(EVT::tick), t)}, vnt::os::Timer::Mode::forever, 0});
    
//    vnt::bsp::trace::puts(std::string(t->getName()) + ".startup() has started timer which sends evtTick to itself every " + std::to_string(tickperiod) + " usec");

//    vnt::bsp::act::valve::init(vnt::bsp::act::VLV::V1, {});
//    vnt::bsp::act::valve::init(vnt::bsp::act::VLV::V2, {});
        
        
    app::theController::getInstance().init({});    
    app::theController::getInstance().set(app::theController::Mode::IDLE);  
    app::theController::getInstance().setIEratio(0.5f);         
                          
}


static volatile int acquisitionresult = 0;
void alertme(int v)
{
    acquisitionresult = v;
    app::tCTRL::trigger(app::tCTRL::EVT::dataready);
}


// keep it lean.
// you may add a histogram builder upon duration of events, maybe ...

volatile vnt::core::Time times[8] = {0};

app::theController::Inp iii {};
app::theController::Out ooo {};

volatile float tincode = 0;
volatile float toutval  =0;

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
        
//        vnt::bsp::trace::puts("");
//        vnt::bsp::trace::puts("->");
//        vnt::bsp::trace::puts("@ " + vnt::core::TimeFormatter(vnt::core::now()).to_string() + " tCTRL commands to HW to start acquisition of sensors");      

#if defined(APP_DRV_onlyBTNSGPIO) || defined(TESTRUNTIMEDBG)
        allinput.read_mask  = BOARD_GPIO | BOARD_ENCODER | BOARD_ANALOG; 
#else        
        allinput.read_mask  = BOARD_ALL_SENSORS;
#endif
        
        times[0] = vnt::core::now();
        board_read_sensors_async(&allinput, alertme);
        times[1] = vnt::core::now();

    }


    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::dataready)))
    {
        static uint32_t _tick = 0;
        static volatile int latestCTRLmode = IDLE;
        static volatile int latestIERvalue = 50;
        
        times[2] = vnt::core::now();
                
        // ok, in here we use allinput
               
        static volatile uint16_t x = 0;
        x = allinput.flow1;
        
        if(allinput.buttons != 0)
        {
            vnt::bsp::trace::puts("button pressed");
        }
        
        if(CTRLmode != latestCTRLmode)
        {
            if((CTRLmode == VCV) || (CTRLmode == IDLE) || (CTRLmode == CPAP) || (CTRLmode == PRVC))
            {
                latestCTRLmode = CTRLmode;
                app::theController::Mode m = static_cast<app::theController::Mode>(CTRLmode);
                app::theController::getInstance().set(m);                  
            }  
        }

        if(IERvalue != latestIERvalue)
        {
            latestIERvalue = IERvalue;
            app::theController::getInstance().setIEratio(static_cast<float>(IERvalue)/100.0f);                  
        }

        
       
        
        app::theController::getInstance().set(allinput);
        app::theController::getInstance().tick();
        const app::theController::Inp &inp = app::theController::getInstance().getInp();
        const app::theController::Out &out = app::theController::getInstance().getOut();
        
        iii = inp;
        ooo = out;
        
        tincode = iii.S3temperature;
        toutval = ooo.S3temperature;
        
        
//        const app::theController::Inp &inp = app::theController::getInstance().getInp();
//        if(0 == (_tick % 100))
//        {            
//            inp.print();
//            out.print();
//        }
        
#if defined(TESTRUNTIMEDBG) 
#else        

        
        // apply the output.
        // must transform out into fileds of outdata;

#if defined(TEST_CP_WAVEFORM)   
        
        // note: valvola v2 it is ok to use 2500 aperta (proviamo a chiuderla a 500).
        static bool v2on = true;         
        float cp = percwave.tick();
        bool cfbON = true;
        outdata.buzzer = 5000; // magic number found inside boar_driver.c which is max value.
        outdata.valve1 = static_cast<uint32_t>(5000.0*cp/100.0);
//        outdata.valve2 = (true == cfbON) ? (5000) : (0);        
//        outdata.valve2 = (true == cfbON) ? (2000) : (5000);

//        if(cp == 100.0)
//        {
//            v2on = !v2on;
//        }
        // 2500 is open
        // 0 is closed
        outdata.valve2 = (true == v2on) ? 2500 : 0;

        outdata.gpio = 0;   
//        vnt::bsp::trace::puts(std::to_string(cp) + " v1 = " + std::to_string(outdata.valve1) + " v2 = " + std::to_string(outdata.valve2));

        if((_printOPT.enabled) && (0 == (_tick % _printOPT.printDIVIDER)))
        {
            if(_printOPT.enableBRDdriver)
            {
                //print(allinput);
                print_inputs(allinput);
            }
            if(_printOPT.enableCTRLinput)
            {
                const app::theController::Inp &inp = app::theController::getInstance().getInp();
                inp.print();
            }
            if(_printOPT.enableCTRLoutput)
            {
                out.print();
            }           
        }

#elif defined(TEST_BTNSGPIO)


        outdata.buzzer = 5000; // magic number found inside boar_driver.c which is max value.
        outdata.gpio = 0;

        if(allinput.buttons != 0)
        {
            vnt::bsp::trace::puts("EVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVIVA");
        }

        if((_printOPT.enabled) && (0 == (_tick % _printOPT.printDIVIDER)))
        {
            if(_printOPT.enableBRDdriver)
            {
                //print(allinput);
                print_inputs(&allinput);
            }
//            if(_printOPT.enableCTRLinput)
//            {
//                const app::theController::Inp &inp = app::theController::getInstance().getInp();
//                inp.print();
//            }
//            if(_printOPT.enableCTRLoutput)
//            {
//                out.print();
//            }           
        }
#else

// 1000 chiusa 5000 aperta (5000 no pwm) 

//        outdata.buzzer = 5000; // magic number found inside boar_driver.c which is max value.
//        outdata.valve1 = static_cast<uint32_t>(5000.0*out.CPvalvePerc/100.0);
//        outdata.valve2 = v2val;

        outdata.buzzer = 5000; // magic number found inside boar_driver.c which is max value.
        outdata.valve1 = static_cast<uint32_t>(5000.0*out.CPvalvePerc/100.0);
        outdata.valve2 = (true == out.CFBvalveON) ? (5000) : (1000); 

#endif   

       
        int r = board_apply_actuation(&outdata);

#endif

        // i copy the inputs and outputs to the data structure used by the HMI
        osal_mutex_take(mtx, OSAL_reltimeINFINITE);
        std::memmove(&latestSNSdata, &allinput, sizeof(latestSNSdata));
        std::memmove(&latestCTRLout, &out, sizeof(latestCTRLout));
        osal_mutex_release(mtx);
      
        _tick++;        
    }  


}


}} // namespace app { namespace tCTRL {


#include "app_tCTRLc.h"

uint8_t getvalues(GraphicValues_t *gv)
{
    if(nullptr == gv)
    {
        return 0;
    }
    
    osal_mutex_take(app::tCTRL::mtx, OSAL_reltimeINFINITE);
    
    // use app::tCTRL::latestCTRLout and app::tCTRL::latestSNSdata
    
    #if 0
    plot1: press
    plot2: flow
    plot3: tidal volume
    
    val1: resp rate
    val2: pip
    val3: insp/exp
    val4: tidal volume
    #endif
    
    constexpr float fac = 10;
    
//    gv->plot1.val = app::tCTRL::latestCTRLout.filtS7pressure;
    gv->plot1.val = app::tCTRL::latestCTRLout.filtS1pressure;
    gv->plot2.val = app::tCTRL::latestCTRLout.S2flowrate;
    gv->plot3.val = app::tCTRL::latestCTRLout.estTidalVolume;

//    float v = app::tCTRL::latestCTRLout.S2flowrate;
    
    gv->val1 = 0;
    gv->val2 = 0;
    gv->val3 = static_cast<float>(IERvalue)/100.0f;
    gv->val4 = app::tCTRL::latestCTRLout.estTidalVolume;

    osal_mutex_release(app::tCTRL::mtx);
    
//    static float max = -100000;
//    static float min = +100000;
//    if(v>max) max = v;
//    if(v<min) min = v;
//    gv->plot3.val = (v / 500) - 0.5;
    
    return 1;
}

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


