 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

#include "fi5app_tCTRL.h"


// needed to show the name of the thread in uvision: keep it global out of any namespace
void t_CTRL(void *p)
{
    vnt::os::Thread *t = reinterpret_cast<vnt::os::Thread*>(p);
    t->run();
}

// the very basic needed to start a thread. it may become a code template. for now we duplicate it.

namespace fi5app { namespace tCTRL { 

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
    
       
}} // namespace fi5app { namespace tCTRL { 


// - the code which specialises the thread CTRL
//   i can put includes in here. it is not canonical but it more clear to show the actual dependencies

#include "fi5application.h"

#include "vnt_bsp.h"
#include "vnt_bsp_trace.h"
#include "vnt_bsp_watchdog.h"
#include "vnt_os_Timer.h"

#include "stm32hal.h"
extern UART_HandleTypeDef UART_Handle;
namespace vnt { namespace bsp { namespace serial {
    
    int puts(const std::string &str)
    {
        const char *cs = str.c_str();
        size_t ss = strlen(cs);
        constexpr uint32_t mstimeout {50}; 
        HAL_UART_Transmit( &UART_Handle, (uint8_t*)cs, ss, mstimeout);
        return 1; 
    }
}}}



#include "board_driver.h"

#include "stm32hal.h"

#include "fi5app_theController2.h"

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
    void print_knobs()
    {
        vnt::bsp::trace::puts(std::string("knobs:") +

                        "knobs[] = (" +
                        std::to_string(data.analog_input[0]) + ", " +
                        std::to_string(data.analog_input[1]) + ", " +
                        std::to_string(data.analog_input[2]) + ", " +
                        std::to_string(data.analog_input[3]) +
                        ")"
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


namespace fi5app { namespace tCTRL {
    
    
// these are the inputs and the outputs of the driver    
driverInput driverinput {};
driverOutput driveroutput {};


// the parameters of hmi

struct knobParams_t
{
    float FREQvalue {10};           // [5, 40]
    float PMAXvalue {25};           // [5, 60]
    float IERvalue {50};            // [25, 100]
    float TIDVOLvalue {650};        // [200, 2000]  
    
    knobParams_t() = default;
    knobParams_t(const float &fr, const float &pm, const float &ie, const float &ti) : FREQvalue(fr), PMAXvalue(pm), IERvalue(ie), TIDVOLvalue(ti) {}
   
    void load(const std::array<float, 4> &ar)
    {
        FREQvalue = ar[0];
        PMAXvalue = ar[1];
        IERvalue = ar[2];
        TIDVOLvalue = ar[3]; 
    }
    
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
    
    void load(const fi5app::theController2::fsmOut &fsmout, const fi5app::theController2::ctrlOut &ctrlout)
    {
        lock();
        

        params.load(fsmout.KNBgui);
     
        estTidalVolume = ctrlout.estTidalVolume;
        filtS1pressure = ctrlout.filtS1pressure;
        S2flowrate = ctrlout.S2flowrate;
        S5flowrate = ctrlout.S5flowrate;
        
        unlock();            
    }
    
    void load(const fi5app::theController2::Out &out)
    {
        lock();
        
        params.load(out.knobs);     
        estTidalVolume = out.estTidalVolume;
        filtS1pressure = out.filtS1pressure;
        S2flowrate = out.S2flowrate;
        S5flowrate = out.S5flowrate;
        
        unlock();          
    }
    
};

//struct btnMode_t
//{
//    enum class Mode {IDLE = 1, CPAP = 2, VCV = 3, PRCV = 4 };
//    
//    Mode mode {Mode::IDLE}; 
//    
//    btnMode_t() = default;    
//};


// the data shared w/ the hmi
sharedHMIdata_t sharedHMIdata {};
    
//knobParams_t knobparams {};
    
    
//btnMode_t btnmode {};     
    
//fi5app::theController2::Mode ctrlmode {fi5app::theController2::Mode::IDLE};

// the timer which ticks the control
vnt::os::Timer *timerTICK = nullptr;
    
    
void startup(vnt::os::Thread *t, void *param)
{
    vnt::core::TimeFormatter tf1(vnt::core::now());
    vnt::bsp::trace::puts("theScheduler @ " + tf1.to_string() + " has called startup() of thread " + std::string(t->getName()) + " w/ prio = " + std::to_string(vnt::core::tointegral(t->getPriority())));  
                

    sharedHMIdata.init();
//    sharedHMIdata.params = knobparams;    
//    ctrlmode = fi5app::theController2::Mode::IDLE;
    
    timerTICK = new vnt::os::Timer;
    
    timerTICK->start({settings.periodOfTICK, {vnt::os::EventToThread(vnt::core::tointegral(EVT::tick), t)}, vnt::os::Timer::Mode::forever, 0});
    
        
    fi5app::theController2::getInstance().init({});    
    fi5app::theController2::getInstance().set(fi5app::theController2::Mode::IDLE);  
                                  
}


static volatile int acquisitionresult = 0;
void alertme(int v)
{
    acquisitionresult = v;
    fi5app::tCTRL::trigger(fi5app::tCTRL::EVT::dataready);
}


// keep it lean.
// you may add a histogram builder upon duration of events, maybe ...



static void processINPUT(const board_sensor_data_t &ai);
static void setSTATE(const board_sensor_data_t &ai);

void apply(driverOutput &drvout, sharedHMIdata_t &shmi, const fi5app::theController2::fsmOut &ofsm, const fi5app::theController2::ctrlOut &octr);
void apply(driverOutput &drvout, sharedHMIdata_t &shmi, const fi5app::theController2::Out &out);

void print_log(const fi5app::theController2::Inp &inp, const fi5app::theController2::Out &out, const fi5app::theController2::fsmOut &fsmout, const fi5app::theController2::ctrlOut &ctrlout);


//volatile vnt::core::Time times[8] = {0};


#if defined(DRIVER_HAS_INTERNALDEBOUNCE)
    #error DRIVER_HAS_INTERNALDEBOUNCE is not compatible with use of the FSM inside app_theController2
#endif

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
        driverinput.data.read_mask = BOARD_ALL_SENSORS;
        
//        times[0] = vnt::core::now();
        board_read_sensors_async(&driverinput.data, alertme);
//        times[1] = vnt::core::now();

    }


    if(true == vnt::core::binary::mask::check(eventmask, vnt::core::tointegral(EVT::dataready)))
    {
        static uint32_t _tick = 0;
//        times[2] = vnt::core::now();
        
        fi5app::theController2::getInstance().load(driverinput.data);
        fi5app::theController2::getInstance().tick();
        // we fill driveroutput and sharedHMIdata
        apply(driveroutput, sharedHMIdata, fi5app::theController2::getInstance().getOut());        
        // we write to drivers        
        board_apply_actuation(&driveroutput.out);
        
        print_log(fi5app::theController2::getInstance().getInp(), fi5app::theController2::getInstance().getOut(), fi5app::theController2::getInstance().get_fsmOut(), fi5app::theController2::getInstance().get_ctrlOut());
        
        _tick++;        
    }  

}


#define PRINT_LOG
#define PRINT_LOG_SERIAL
constexpr uint32_t decimationfactor = 1;

void print_log(const fi5app::theController2::Inp &inp, const fi5app::theController2::Out &out, const fi5app::theController2::fsmOut &fsmout, const fi5app::theController2::ctrlOut &ctrlout)
{
#if !defined(PRINT_LOG)   
#else
    static uint32_t iter {0};
    static std::string msg;
    static std::array<float, 14> v {0};    
    static vnt::core::Time prevduration {0};
        
    // reserve memory to avoid resizing the string during filling it w/ values.
    if(0 == iter)
    {
        msg.reserve(512);
    }

    // increase now the iteration number
    iter++;
    
    if(0 != (iter % decimationfactor))
    {
        return;
    }        
    
    volatile vnt::core::Time start = vnt::core::now();
           
    // assign the values
    v[0] = ctrlout.CPvalvePerc;
    v[1] = ctrlout.CFBvalveON;
    v[2] = ctrlout.targetFlowRate;
    v[3] = ctrlout.targetPressure;
    v[4] = ctrlout.filtS1pressure;
    v[5] = ctrlout.S2flowrate;
    v[6] = ctrlout.filtS3pressure;
    v[7] = ctrlout.S3temperature;
    v[8] = ctrlout.filtS4pressure;
    v[9] = ctrlout.S5flowrate;
    v[10] = ctrlout.filtS7pressure;
    v[11] = ctrlout.estTidalVolume;
    v[12] = ctrlout.maxTidalVolume2Cycles;
    v[13] = ctrlout.assistedVentilTrigger;

    // clear the string
    msg.clear();
    
    // fill it:
    // iter, ctrlmode, knobsout[4], ctrlout[]
    msg += std::to_string(iter); 
    
//    // prev duration
//    msg += ", ";
//    msg += std::to_string(prevduration); 
    
    // control mode
    msg += ", ";
    msg += std::to_string(vnt::core::tointegral(fsmout.mode));
    
    // knobs out
    for(int i=0; i<fsmout.KNBgui.size(); i++)
    {
        msg += ", ";
        msg += std::to_string(fsmout.KNBgui[i]);        
    }
          
    // control out
    for(int i=0; i<v.size(); i++)
    {
        msg += ", ";
        msg += std::to_string(v[i]);
    }
 
    
    // and now print it
    
    #if defined(PRINT_LOG_SERIAL)
        msg += '\r';
        msg += '\n';
        vnt::bsp::serial::puts(msg);
    #else
        vnt::bsp::trace::puts(msg);
    #endif
        
    
    // finally, update the duration
    prevduration = vnt::core::now() - start;
    
#endif    
}

void apply(driverOutput &drvout, sharedHMIdata_t &shmi, const fi5app::theController2::Out &out)
{   
//    driverinput.print_knobs();
    
//    const fi5app::theController2::Inp &in = fi5app::theController2::getInstance().getInp();
//    vnt::bsp::trace::puts(std::string("knobs[] = [") +
//        std::to_string(driverinput.data.analog_input[0]) + ", " + std::to_string(in.KNBvalue[0]) + ", " +
//        std::to_string(driverinput.data.analog_input[1]) + ", " + std::to_string(in.KNBvalue[1]) + ", " +
//        std::to_string(driverinput.data.analog_input[2]) + ", " + std::to_string(in.KNBvalue[2]) + ", " +
//        std::to_string(driverinput.data.analog_input[3]) + ", " + std::to_string(in.KNBvalue[3]) + "]"
//    );
    
//    static uint32_t ii = 0;
//    if((++ii%100) == 0)
//    {
//        fi5app::theController2::getInstance().getInp().printknobs();
//        out.print();
////        vnt::bsp::trace::puts(  std::to_string(fi5app::theController2::getInstance().getInp().KNBvalue[0]) + "; " +
////                                std::to_string(out.knobs[0])
////        );
//        
//    }
    
    // apply the output of the two valves
    drvout.out.valve1 = static_cast<uint32_t>(5000.0*out.CPvalvePerc/100.0);
    drvout.out.valve2 = (true == out.CFBvalveON) ? (5000) : (1000);
    
    // apply leds + buzzer.
    drvout.out.gpio = 0;
    
    for(int i=0; i<4; i++)
    {
        if(true == out.leds[i])
        {
            vnt::core::binary::bit::set(drvout.out.gpio, i); 
        }
    }
    
    if(true == out.leds[4])
    {
        drvout.out.gpio |= BOARD_LED_ALARM;
    }

    drvout.out.buzzer = (true == out.buzzer) ? 1 : 0;  
    
    // copy data required by the HMI: 
    shmi.load(out);  
}

void setState(driverOutput &drvout, const fi5app::theController2::fsmOut &ofsm);

void apply(driverOutput &drvout, sharedHMIdata_t &shmi, const fi5app::theController2::fsmOut &ofsm, const fi5app::theController2::ctrlOut &octr)
{
    // apply the output of the two valves
    drvout.out.valve1 = static_cast<uint32_t>(5000.0*octr.CPvalvePerc/100.0);
    drvout.out.valve2 = (true == octr.CFBvalveON) ? (5000) : (1000);
    
    // apply leds etc.
    setState(drvout, ofsm);
    
    // copy data required by the HMI: 
    shmi.load(ofsm, octr);  
}


void setState(driverOutput &drvout, const fi5app::theController2::fsmOut &ofsm)
{
    drvout.out.gpio = 0;
    
    if(ofsm.LEDstates[0])
    {
        vnt::core::binary::bit::set(drvout.out.gpio, 0);        
    }  
    
    if(ofsm.LEDstates[1])
    {
        vnt::core::binary::bit::set(drvout.out.gpio, 1);
    }
    
    if(ofsm.LEDstates[2])
    {
        vnt::core::binary::bit::set(drvout.out.gpio, 2);
    }  
    
    if(ofsm.LEDstates[3])
    {
        vnt::core::binary::bit::set(drvout.out.gpio, 3);
    } 
    
    if(ofsm.LEDalarm)
    {
        drvout.out.gpio |= BOARD_LED_ALARM;
    } 

    drvout.out.buzzer = (true == ofsm.soundALARM) ? 1 : 0;  
}


}} // namespace fi5app { namespace tCTRL {


#include "fi5app_tCTRLc.h"

uint8_t getvalues(GraphicValues_t *gv)
{
    if(nullptr == gv)
    {
        return 0;
    }
    
    fi5app::tCTRL::sharedHMIdata.lock();   
               
    gv->plot1.val = fi5app::tCTRL::sharedHMIdata.filtS1pressure;
    gv->plot2.val = fi5app::tCTRL::sharedHMIdata.S2flowrate + fi5app::tCTRL::sharedHMIdata.S5flowrate;
    gv->plot3.val = fi5app::tCTRL::sharedHMIdata.estTidalVolume;

    gv->val1 = static_cast<float>(fi5app::tCTRL::sharedHMIdata.params.FREQvalue);
    gv->val2 = static_cast<float>(fi5app::tCTRL::sharedHMIdata.params.PMAXvalue);
    gv->val3 = static_cast<float>(fi5app::tCTRL::sharedHMIdata.params.IERvalue);
    gv->val4 = static_cast<float>(fi5app::tCTRL::sharedHMIdata.params.TIDVOLvalue);
    
    gv->appl_major = fi5version.major;
    gv->appl_minor = fi5version.minor;
    gv->errormessage[0] = 0;

    fi5app::tCTRL::sharedHMIdata.unlock();    
    
    return 1;
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


