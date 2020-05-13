 
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


// they are in order
// place where put the control mode



// - the code which specialises the thread CTRL
//   i can put includes in here. it is not canonical but it more clear to show the actual dependencies


#include "vnt_bsp.h"
#include "vnt_bsp_trace.h"
#include "vnt_bsp_watchdog.h"
#include "vnt_os_Timer.h"



#include "board_driver.h"

#include "stm32hal.h"

#include "app_theController.h"

#include "osal_mutex.h"


struct driverInput
{
    board_sensor_data_t data {0};
    
    driverInput() = default;

    void print()
    {
        char gpio[16] = {0};
        snprintf(gpio, sizeof(gpio), "0x%x", data.gpio);
        char btns[16] = {0};
        snprintf(btns, sizeof(btns), "0x%x", data.buttons);
        char rmsk[16] = {0};
        snprintf(rmsk, sizeof(rmsk), "0x%x", data.read_mask);
        vnt::bsp::trace::puts(std::string("SNSdata:") +
                        " .pressure1 = " + std::to_string(data.pressure1) +
                        " .pressure1_raw = " + std::to_string(data.pressure1_raw) +
                        " .pressure2 = " + std::to_string(data.pressure2) +
                        " .pressure2_raw = " + std::to_string(data.pressure2_raw) +
                        " .pressure3 = " + std::to_string(data.pressure3) +
                        " .pressure3_raw = " + std::to_string(data.pressure3_raw) +
                        " .pressure4 = " + std::to_string(data.pressure4) +
                        " .pressure4_raw = " + std::to_string(data.pressure4_raw) +
                        " .flow1 = " + std::to_string(data.flow1) +
                        " .flow1_raw = " + std::to_string(data.flow1_raw) +
                        " .flow2 = " + std::to_string(data.flow2) +
                        " .flow2_raw = " + std::to_string(data.flow2_raw) +    
                        " .o2 = " + std::to_string(data.o2) +
                        " .temperature1 = " + std::to_string(data.temperature1) +
                        " .temperature1_raw = " + std::to_string(data.temperature1_raw) +
                        " .temperature2 = " + std::to_string(data.temperature2) +
                        " .temperature2_raw = " + std::to_string(data.temperature2_raw) +
                        " .temperature3 = " + std::to_string(data.temperature3) +
                        " .temperature3_raw = " + std::to_string(data.temperature3_raw) +    
                        " .temperature4 = " + std::to_string(data.temperature4) +
                        " .temperature4_raw = " + std::to_string(data.temperature4_raw) +    
                        " .gpio = " + gpio +
                        " .encoder = " + std::to_string(data.encoder) +
                        " .buttons = " + btns +
                        " .analog_input[0] = " + std::to_string(data.analog_input[0]) +
                        " .analog_input[1] = " + std::to_string(data.analog_input[1]) +
                        " .analog_input[2] = " + std::to_string(data.analog_input[2]) +
                        " .analog_input[3] = " + std::to_string(data.analog_input[3]) +
                        " .read_mask = " + rmsk
        );  
    }

    void print_IOs()
    {
        char gpio[16] = {0};
        snprintf(gpio, sizeof(gpio), "0x%x", data.gpio);
        char btns[16] = {0};
        snprintf(btns, sizeof(btns), "0x%x", data.buttons);
        char rmsk[16] = {0};
        snprintf(rmsk, sizeof(rmsk), "0x%x", data.read_mask);
        vnt::bsp::trace::puts(std::string("SNSdata:") +

                        " .gpio = " + gpio +
                        " .enc = " + std::to_string(data.encoder) +
                        " .btns = " + btns +
                        " .ai[0] = " + std::to_string(data.analog_input[0]) +
                        " .ai[1] = " + std::to_string(data.analog_input[1]) +
                        " .ai[2] = " + std::to_string(data.analog_input[2]) +
                        " .ai[3] = " + std::to_string(data.analog_input[3]) +
                        " .read_mask = " + rmsk
        ); 
    }
    
};

struct driverOutput
{
    board_actuation_data_t out {0};
    
    driverOutput() = default;

    void print()
    {
        
    }
};


namespace app { namespace tCTRL {
    
    
// these are the inputs and the outputs of the driver    
driverInput driverinput {};
driverOutput driveroutput {};


// the parameters: to be moved into theController

struct knobParams_t
{
    float FREQvalue {10};           // [5, 40]
    float PMAXvalue {25};           // [5, 60]
    float IERvalue {50};            // [25, 100]
    float TIDVOLvalue {650};        // [200, 2000]  
    
    knobParams_t() = default;
    knobParams_t(const float &fr, const float &pm, const float &ie, const float &ti) : FREQvalue(fr), PMAXvalue(pm), IERvalue(ie), TIDVOLvalue(ti) {}
        
    void setdefault() 
    {
        FREQvalue = 10;
        PMAXvalue = 25;
        IERvalue = 50;
        TIDVOLvalue = 650;        
    }
};
       

struct sharedHMIdata_t
{
    osal_mutex_t *mtx {nullptr};
       
    knobParams_t params {};  
        
    float filtS1pressure {0};
    float S2flowrate {0};
    float S5flowrate {0};
    float estTidalVolume {0};

    sharedHMIdata_t() = default;
    
    void init() 
    { 
        if(nullptr == mtx) { mtx = osal_mutex_new();} 
        params.setdefault();
    }
    void lock() { osal_mutex_take(mtx, OSAL_reltimeINFINITE); }
    void unlock() { osal_mutex_release(mtx); }   
    
    void load(const knobParams_t &p, const app::theController::Out &out)
    {
        lock();
        
        params = p;
        
        estTidalVolume = out.estTidalVolume;
        filtS1pressure = out.filtS1pressure;
        S2flowrate = out.S2flowrate;
        S5flowrate = out.S5flowrate;
        
        unlock();            
    }
    
};

struct btnMode_t
{
    enum class Mode {IDLE = 1, CPAP = 2, VCV = 3, PRCV = 4 };
    
    Mode mode {Mode::IDLE}; 
    
    btnMode_t() = default;    
};


// the data shared w/ the hmi
sharedHMIdata_t sharedHMIdata {};
    
knobParams_t knobparams {};
    
    
btnMode_t btnmode {};     
    
app::theController::Mode ctrlmode {app::theController::Mode::IDLE};

// the timer which ticks the control
vnt::os::Timer *timerTICK = nullptr;
    
    
void startup(vnt::os::Thread *t, void *param)
{
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has called startup() of thread " + std::string(t->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(t->getPriority())));  
                

    sharedHMIdata.init();
    sharedHMIdata.params = knobparams;
    
    ctrlmode = app::theController::Mode::IDLE;
    
    timerTICK = new vnt::os::Timer;
    
    timerTICK->start({settings.periodOfTICK, {vnt::os::EventToThread(vnt::core::tointegral(EVT::tick), t)}, vnt::os::Timer::Mode::forever, 0});
    
        
    app::theController::getInstance().init({});    
    app::theController::getInstance().set(app::theController::Mode::IDLE);  
        
    app::theController::getInstance().setFreq(static_cast<float>(knobparams.FREQvalue));
    app::theController::getInstance().setPMax(static_cast<float>(knobparams.PMAXvalue));
    app::theController::getInstance().setIEratio(static_cast<float>(knobparams.IERvalue)/100.0f);  
    app::theController::getInstance().setTidalVolume(static_cast<float>(knobparams.TIDVOLvalue)/1000.0f);        
                          
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


static void processINPUT(const board_sensor_data_t &ai);
static void setSTATE(const board_sensor_data_t &ai);

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
      
        driverinput.data.read_mask = BOARD_ALL_SENSORS;
        
//        times[0] = vnt::core::now();
        board_read_sensors_async(&driverinput.data, alertme);
//        times[1] = vnt::core::now();

    }


    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::dataready)))
    {
        static uint32_t _tick = 0;

        // the state machine for now is not used. we use the following
        processINPUT(driverinput.data);
                     
        app::theController::getInstance().set(driverinput.data);
        app::theController::getInstance().tick();
        const app::theController::Out &out = app::theController::getInstance().getOut();
                                
        // apply the output of the two valves
        driveroutput.out.valve1 = static_cast<uint32_t>(5000.0*out.CPvalvePerc/100.0);
        driveroutput.out.valve2 = (true == out.CFBvalveON) ? (5000) : (1000); 
        
        // apply the leds etc. i keep it after the controller so that if we have an alarm ...
        setSTATE(driverinput.data);
        
        // we write to drivers        
        int r = board_apply_actuation(&driveroutput.out);
                
        // copy data required by the HMI: 
        // knobparams is modified by processKNOBs() or by the state machine  
        // out is modified by the controller
        sharedHMIdata.load(knobparams, out);
              
        _tick++;        
    }  


}


static void processBTNs(const board_sensor_data_t &ai);
static void processKNOBs(const board_sensor_data_t &ai);
static void processENC(const board_sensor_data_t &ai);
static void setLEDs(const board_sensor_data_t &ai);

static void processINPUT(const board_sensor_data_t &ai)
{
    processBTNs(ai);                        
    processKNOBs(ai); 
    processENC(ai); 
}

static void setSTATE(const board_sensor_data_t &ai)
{
    setLEDs(ai); 
}


static void processBTNs(const board_sensor_data_t &ai)
{
    
    bool bIDLE = vnt::core::binary::bit::check(ai.buttons, 3);
    bool bCPAP = vnt::core::binary::bit::check(ai.buttons, 2);
    bool bVCV = vnt::core::binary::bit::check(ai.buttons, 1);
    bool bPRCV = vnt::core::binary::bit::check(ai.buttons, 0);  

    if(bIDLE)
    {
        ctrlmode = app::theController::Mode::IDLE;
        app::theController::getInstance().set(app::theController::Mode::IDLE);
    }
    else if(app::theController::Mode::IDLE == ctrlmode)
    {
        // change mode
        if(bCPAP) 
        { 
            ctrlmode = app::theController::Mode::CPAP; 
            app::theController::getInstance().set(app::theController::Mode::CPAP);
        }
        else if(bVCV)
        {
            ctrlmode = app::theController::Mode::VCV; 
            app::theController::getInstance().set(app::theController::Mode::VCV);              
        }
        else if(bPRCV)
        {
            ctrlmode = app::theController::Mode::PRCV; 
            app::theController::getInstance().set(app::theController::Mode::PRCV); 
        }
    }
}

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
        return v*_bitresolution; // now we rescale in [0, 1]
    }
    
};


// min and max are assigned upon readings
constexpr knob_t knobFreq {{77, 400}}; 
constexpr knob_t knobPmax {{58, 400}}; 
constexpr knob_t knobIEratio {{46, 400}}; 
constexpr knob_t knobTidalVolum {{56, 400}}; 

void processKNOBs(const board_sensor_data_t &ai)
{
    static volatile bool firsttime = true;
    
    if(app::theController::Mode::IDLE != ctrlmode)
    {                       
        return;            
    }
    
//    if(ai.encoder != 0)
//    {
//        vnt::bsp::trace::puts(std::to_string(ai.encoder));        
//    }
    
   
    // reduce noise a bit
    volatile uint32_t aa[4] = {0};
    for(int i=0; i<4; i++)
    {
        aa[i] = ai.analog_input[i] / 4;
        aa[i] *= 4;
    }
            
         
    static float kFREQpre = 0;
    float kFREQ = knobFreq.get(aa[0]);
    float tmp1 = 5.0f + kFREQ*(40.0f-5.0f);
    if(firsttime || (abs(kFREQpre-kFREQ) > 0.10) || (kFREQ<0.01f) || (kFREQ> 0.99f))
    { 
        kFREQpre = kFREQ;                
        knobparams.FREQvalue = tmp1;
        app::theController::getInstance().setFreq(static_cast<float>(knobparams.FREQvalue));                  
    }  
    
    static float kPMAXpre = 0;
    float kPMAX = knobPmax.get(aa[1]);
    float tmp2 = 5.0f + kPMAX*(60.0f-5.0f);
    if(firsttime || (abs(kPMAXpre-kPMAX) > 0.10) || (kPMAX<0.01f) || (kPMAX> 0.99f))
    { 
        kPMAXpre = kPMAX;                
        knobparams.PMAXvalue = tmp2;
        app::theController::getInstance().setPMax(static_cast<float>(knobparams.PMAXvalue));                  
    } 
    

    static float kIERATIOpre = 0;
    float kIERATIO = knobIEratio.get(aa[2]);
    float tmp3 = 100.0f - kIERATIO*(100.0f-25.0f);
    if(firsttime || (abs(kIERATIOpre-kIERATIO) > 0.10) || (kIERATIO<0.01f) || (kIERATIO> 0.99f))
    {  
        kIERATIOpre = kIERATIO;                
        knobparams.IERvalue = tmp3;
        app::theController::getInstance().setIEratio(static_cast<float>(knobparams.IERvalue)/100.0f);                  
    }    
 

    static float kTIDALVOLpre = 0;
    float kTIDALVOL = knobTidalVolum.get(aa[3]); 
    float tmp4 = 200.0f + kTIDALVOL*(2000.0f-200.0f);
    if(firsttime || (abs(kTIDALVOLpre-kTIDALVOL) > 0.10) || (kTIDALVOL<0.01f) || (kTIDALVOL> 0.99f))
    {
        kTIDALVOLpre = kTIDALVOL;                
        knobparams.TIDVOLvalue = tmp4;
        app::theController::getInstance().setTidalVolume(static_cast<float>(knobparams.TIDVOLvalue)/1000.0f);                  
    } 
        
    
    firsttime = false;
}

void processENC(const board_sensor_data_t &ai)
{
    // marco.accame: so far it is just for testing the encoder
//    if(0 != ai.encoder)
//    {
//        vnt::bsp::trace::puts(std::string("encoder = ") + std::to_string(ai.encoder));
//    }

//    if(0 != ai.buttons)  
//    {
//        char btns[32] = {0};
//        snprintf(btns, sizeof(btns), "0x%x", ai.buttons);
//        vnt::bsp::trace::puts(std::string("buttons = ") + btns);
//    }  
//    else
//    {
//        vnt::bsp::trace::puts(std::string("buttons = 0"));
//    }
}

void setLEDs(const board_sensor_data_t &ai)
{        
    driveroutput.out.gpio = 0;
    
    if(app::theController::Mode::IDLE == ctrlmode)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 0);        
    }        
    else if(app::theController::Mode::CPAP == ctrlmode)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 1);
    }
    else if(app::theController::Mode::VCV == ctrlmode)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 2);
    }  
    else if(app::theController::Mode::PRCV == ctrlmode)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 3);
    } 
    
    bool alarm = false;
    if(alarm)
    {
        driveroutput.out.gpio |= BOARD_LED_ALARM;
    }
    
    bool bIDLE = vnt::core::binary::bit::check(ai.buttons, 3);
    bool bCPAP = vnt::core::binary::bit::check(ai.buttons, 2);
    bool bVCV = vnt::core::binary::bit::check(ai.buttons, 1);
    bool bPRCV = vnt::core::binary::bit::check(ai.buttons, 0);
    if(bIDLE)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 0);
    }
    if(bCPAP)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 1);
    }    
    if(bVCV)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 2);
    }
    if(bPRCV)
    {
        vnt::core::binary::bit::set(driveroutput.out.gpio, 3);
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
    
    app::tCTRL::sharedHMIdata.lock();   
               
    gv->plot1.val = app::tCTRL::sharedHMIdata.filtS1pressure;
    gv->plot2.val = app::tCTRL::sharedHMIdata.S2flowrate + app::tCTRL::sharedHMIdata.S5flowrate;
    gv->plot3.val = app::tCTRL::sharedHMIdata.estTidalVolume;

    gv->val1 = static_cast<float>(app::tCTRL::sharedHMIdata.params.FREQvalue);
    gv->val2 = static_cast<float>(app::tCTRL::sharedHMIdata.params.PMAXvalue);
    gv->val3 = static_cast<float>(app::tCTRL::sharedHMIdata.params.IERvalue)/100.0f;
    gv->val4 = static_cast<float>(app::tCTRL::sharedHMIdata.params.TIDVOLvalue);

    app::tCTRL::sharedHMIdata.unlock();    
    
    return 1;
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


