 
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


#include "controller.h"                     
#include "rtwtypes.h"
#include "zero_crossing_types.h"


// it is a bit of a mess becaue of the demo of 7 may 2020: end of ww2 in europe but enerrtheless we add code in here

struct knob_t 
{
    struct Cfg
    {
        uint32_t _min {0};
        uint32_t _max {1024};  
        constexpr Cfg(uint32_t mi, uint32_t ma) : _min(mi), _max(ma) {}        
    };
    
    static constexpr float _scale = 1.0f;
    float _bitresolution {1024};

    Cfg _cfg {0, 1024};
    
    constexpr knob_t(const Cfg &c) : _cfg(c) 
    {   // dont look at what happens if max is < min, both are zero etc. dont have time now BUT ............. hei i must do it
        _bitresolution = _scale / static_cast<float>(_cfg._max - _cfg._min);
    }
    
    float get(const uint32_t input) const
    {
        float v = std::max(std::min(input, _cfg._max), _cfg._min); // clipped in in [_min, _max]
        v -= _cfg._min; // removed _min: now we have range [0, _max-_min]
        return v*_scale; // now we resale in [0, 1]
    }
    
};

//struct the4knobs_t
//{
//    knob_t _freq {{0, 1}};
//    knob_t _pmax {{0, 1}};
//    knob_t _iera {{0, 1}};
//    knob_t _tida {{0, 1}};        
//    
//    constexpr the4knobs_t(const knob_t::Cfg &cfr, const knob_t::Cfg &cpm, const knob_t::Cfg &cie, const knob_t::Cfg &ctv) :
//        _freq(cfr), _pmax(cpm), _iera(cie), _tida(cie) {}

//    void get(
//    
//}

// min and max are assigned upon readings
constexpr knob_t knobFreq {{32, 512}}; // readings are 18->533 [ flicking ....]
constexpr knob_t knobPmax {{16, 450}}; // 6->497
constexpr knob_t knobIEratio {{16, 450}}; // 6->473
constexpr knob_t knobTidalVolum {{16, 500}}; // 4->506

// they are in order
// place where put the control mode



constexpr int IDLE = 1;
constexpr int CPAP = 2;
constexpr int VCV = 3;
constexpr int PRCV = 4;

volatile int CTRLmode = IDLE; 
volatile int currCTRLmode = IDLE;


volatile int FREQvalue = 10;    // [5, 40]
volatile int latestFREQvalue = 10;

volatile int PMAXvalue = 25;     // [5, 60]
volatile int latestPMAXvalue = 25;

volatile int IERvalue = 50;     // [25, 100]
volatile int latestIERvalue = 50;


volatile int TIDVOLvalue = 650;     // [200, 2000]
volatile int latestTIDVOLvalue = 650;


volatile int V1perc = 0;
volatile bool V2on = true;

volatile int v2val = 2500;


controllerModelClass::P_controller_T _param;  

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
        
    app::theController::getInstance().setFreq(static_cast<float>(FREQvalue));
    app::theController::getInstance().setPMax(static_cast<float>(PMAXvalue));
    app::theController::getInstance().setIEratio(static_cast<float>(IERvalue)/100.0f);  
    app::theController::getInstance().setTidalVolume(static_cast<float>(TIDVOLvalue)/1000.0f);        
                          
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

                
        // ok, in here we use allinput
        
        bool bIDLE = vnt::core::binary::bit::check(allinput.buttons, 3);
        bool bCPAP = vnt::core::binary::bit::check(allinput.buttons, 2);
        bool bVCV = vnt::core::binary::bit::check(allinput.buttons, 1);
        bool bPRCV = vnt::core::binary::bit::check(allinput.buttons, 0);  

        if(bIDLE)
        {
            currCTRLmode = IDLE;
            app::theController::getInstance().set(app::theController::Mode::IDLE);
        }
        else if(IDLE == currCTRLmode)
        {
            // change mode
            if(bCPAP) 
            { 
                currCTRLmode = CPAP; 
                app::theController::getInstance().set(app::theController::Mode::CPAP);
            }
            else if(bVCV)
            {
                currCTRLmode = VCV; 
                app::theController::getInstance().set(app::theController::Mode::VCV); 
            }
            else if(bPRCV)
            {
                currCTRLmode = PRCV; 
                app::theController::getInstance().set(app::theController::Mode::PRCV); 
            }
        }
        
        if(currCTRLmode == IDLE)
        {
            // i can change the config using the debugger 
            outdata.gpio = 0;
            vnt::core::binary::bit::set(outdata.gpio, 3);
          
            if(FREQvalue != latestFREQvalue)
            {
                if(FREQvalue > 40) FREQvalue = 40;
                if(FREQvalue < 5)  FREQvalue = 5;
                
                latestFREQvalue = FREQvalue;
                app::theController::getInstance().setFreq(static_cast<float>(FREQvalue));                  
            }   

            if(PMAXvalue != latestPMAXvalue)
            {
                if(PMAXvalue > 60) PMAXvalue = 60;
                if(PMAXvalue < 5)  PMAXvalue = 5;
                
                latestPMAXvalue = PMAXvalue;
                app::theController::getInstance().setPMax(static_cast<float>(PMAXvalue));                  
            }  
            
            if(IERvalue != latestIERvalue)
            {
                if(IERvalue > 100) IERvalue = 100;
                if(IERvalue < 25)  IERvalue = 25;
                
                latestIERvalue = IERvalue;
                app::theController::getInstance().setIEratio(static_cast<float>(IERvalue)/100.0f);                  
            }   


            if(TIDVOLvalue != latestTIDVOLvalue)
            {
                if(TIDVOLvalue > 2000) TIDVOLvalue = 2000;
                if(TIDVOLvalue < 200)  TIDVOLvalue = 200;
                
                latestTIDVOLvalue = TIDVOLvalue;
                app::theController::getInstance().setTidalVolume(static_cast<float>(TIDVOLvalue)/1000.0f);                  
            }              
        }
        else if(currCTRLmode == CPAP)
        {
            outdata.gpio = 0;
            vnt::core::binary::bit::set(outdata.gpio, 2);
        }
        else if(currCTRLmode == VCV)
        {
            outdata.gpio = 0;
            vnt::core::binary::bit::set(outdata.gpio, 1);
        }  
        else if(currCTRLmode == PRCV)
        {
            outdata.gpio = 0;
            vnt::core::binary::bit::set(outdata.gpio, 0);
        }        
        


        const controllerModelClass::P_controller_T& pp = app::theController::getInstance().getPARs();
        _param = pp;        
        
        app::theController::getInstance().set(allinput);
        app::theController::getInstance().tick();
        const app::theController::Inp &inp = app::theController::getInstance().getInp();
        const app::theController::Out &out = app::theController::getInstance().getOut();
        
        
        
//        float kFREQ = knobFreq.get(allinput.analog_input[0]);
//        float kPMAX = knobPmax.get(allinput.analog_input[1]);
//        float kIERATIO = knobIEratio.get(allinput.analog_input[2]);
//        float kTIDALVOL = knobTidalVolum.get(allinput.analog_input[3]);       


//        
//        if(bIDLE)
//        {
//            vnt::bsp::trace::puts("IDLE pressed");
//            vnt::bsp::trace::puts
//                (
//                std::string(" kFREQ = ") + std::to_string(kFREQ) + " ( " + std::to_string(allinput.analog_input[0]) + ")" +
//                std::string(" kPMAX = ") + std::to_string(kPMAX) +  " ( " + std::to_string(allinput.analog_input[1]) + ")" +
//                std::string(" kIERATIO = ") + std::to_string(kIERATIO) +  " ( " + std::to_string(allinput.analog_input[2]) + ")" +
//                std::string(" kTIDALVOL = ") + std::to_string(kTIDALVOL) +  " ( " + std::to_string(allinput.analog_input[3]) + ")"
//                );
//        }
 
//        if(bCPAP)
//        {
//            vnt::bsp::trace::puts("bCPAP pressed");
//            
//            print_inputs(&allinput);
//        }  
//        if(bVCV)
//        {
//            vnt::bsp::trace::puts("bVCV pressed");
//        }  
//        
//        if(bPRCV)
//        {
//            vnt::bsp::trace::puts("bPRCV pressed");
//        }       
//        
        

        
        // apply the output.
        // must transform out into fileds of outdata;


        // 1000 chiusa 5000 aperta (5000 no pwm) 

//        outdata.buzzer = 5000; // magic number found inside boar_driver.c which is max value.
//        outdata.valve1 = static_cast<uint32_t>(5000.0*out.CPvalvePerc/100.0);
//        outdata.valve2 = v2val;

        outdata.buzzer = 5000; // magic number found inside boar_driver.c which is max value.
        outdata.valve1 = static_cast<uint32_t>(5000.0*out.CPvalvePerc/100.0);
        outdata.valve2 = (true == out.CFBvalveON) ? (5000) : (1000); 
        

        int r = board_apply_actuation(&outdata);


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
    gv->plot2.val = app::tCTRL::latestCTRLout.S2flowrate + app::tCTRL::latestCTRLout.S5flowrate;
    gv->plot3.val = app::tCTRL::latestCTRLout.estTidalVolume;

//    float v = app::tCTRL::latestCTRLout.S2flowrate;
    
    gv->val1 = static_cast<float>(FREQvalue);
    gv->val2 = static_cast<float>(PMAXvalue);
    gv->val3 = static_cast<float>(IERvalue)/100.0f;
    gv->val4 = static_cast<float>(TIDVOLvalue);

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


