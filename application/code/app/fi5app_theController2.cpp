
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "fi5app_theController2.h"



// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

    
// - begin of placeholder 
//   for putting the interface towards the matlab generated code.
//   we need some .h files and a static object. maybe some macros to load one controller or another at compiletime

// fsm
#include <stddef.h>
#include "FSM0.h"                      // Model's header file
#include "rtwtypes.h"

// controller
#include "controller.h"                     
#include "rtwtypes.h"
#include "zero_crossing_types.h"




// - end of placeholder
    
struct fi5app::theController2::Impl
{   
    FSM0ModelClass FSM0_Obj;           
    controllerModelClass controller_Obj; 
    
    size_t _counter {0};

    Config _config {};
        
    theController2::Inp _inp {};     
    theController2::fsmOut _fsmout {};
    theController2::ctrlOut _ctrlout {};
        
    theController2::Out _out {};    

                
    Impl()
    { 
         
    }
    
    bool init(const Config &cfg)
    {
        _config = cfg;
             
        FSM0_Obj.initialize();
        controller_Obj.initialize();
        
        set(Mode::IDLE);
        
        return true;       
    }
    
    bool load(const board_sensor_data_t &sd)
    {
        _inp.load(sd);
        // i give the _inp values to the state machine and to the controller when i am inside the tick() method
        return true; 
    }
    
    bool set(Mode mode)
    {
        matlab_mode(vnt::core::tointegral(mode));
        return true;        
    }
    
    bool tick()
    {
        matlab_fsm_feed(_inp);
        FSM0_Obj.step();
        matlab_fsm_get(_fsmout); 
        
        matlab_controller_feed(_inp);
        matlab_controller_setparams();
        controller_Obj.step();        
        matlab_control_get(_ctrlout);  
        
        fill_out();   
        
        _counter++;
        return true;
    }  
    
    const Out& getout() const
    {
       return _out; 
    }   

    const ctrlOut& getctrlout() const
    {
       return _ctrlout; 
    }    
    
    const fsmOut& getfsmout() const
    {
       return _fsmout; 
    }    


    const Inp& getinp() const
    {
       return _inp; 
    } 
    

    void matlab_fsm_feed(const fi5app::theController2::Inp &inp)
    {
        FSM0ModelClass::ExtU_FSM0_T inputs;
        
        inputs.buttons[0] = (true == inp.BTNpressed[0]) ? StateButton::Pressed : StateButton::Released;       
        inputs.buttons[1] = (true == inp.BTNpressed[1]) ? StateButton::Pressed : StateButton::Released; 
        inputs.buttons[2] = (true == inp.BTNpressed[2]) ? StateButton::Pressed : StateButton::Released; 
        inputs.buttons[3] = (true == inp.BTNpressed[3]) ? StateButton::Pressed : StateButton::Released; 
        inputs.knobs_in[0] = inp.KNBvalue[0];           // (float in [0,1])
        inputs.knobs_in[1] = inp.KNBvalue[1];           // (float in [0,1])
        inputs.knobs_in[2] = inp.KNBvalue[2];           // (float in [0,1])
        inputs.knobs_in[3] = inp.KNBvalue[3];           // (float in [0,1])
        
        const controllerModelClass::ExtY_controller_T& ctrl_outputs = controller_Obj.getExternalOutputs();
        for(int i=0; i<10; i++)
        {
            inputs.Controller_Signals[i] = ctrl_outputs.signals[i];
        }        
        
        inputs.MultiSelector_Maps = 0;      // (uint32)
        inputs.MultiSelector_Go = StateButton::Released;    
        
        FSM0_Obj.setExternalInputs(&inputs);
 
    }
    
    void fill_out()
    {
        _out.CPvalvePerc = _ctrlout.CPvalvePerc;
        _out.CFBvalveON = _ctrlout.CFBvalveON;
        _out.estTidalVolume = _ctrlout.estTidalVolume;
        _out.filtS1pressure = _ctrlout.filtS1pressure;
        _out.S2flowrate = _ctrlout.S2flowrate;
        _out.S5flowrate = _ctrlout.S5flowrate;
        for(int i=0; i<4; i++) _out.leds[i] = _fsmout.LEDstates[i];
        _out.leds[4] = _fsmout.LEDalarm;
        _out.knobs = _fsmout.KNBgui;
        _out.buzzer = _fsmout.soundALARM;               
    }
    
    void matlab_fsm_get(fi5app::theController2::fsmOut &out)
    {
        const FSM0ModelClass::ExtY_FSM0_T & outputs = FSM0_Obj.getExternalOutputs();
        
        for(int i=0; i<4; i++)
        {
            out.LEDstates[i] = (StateLed::Off == outputs.ledsSTATES[i]) ? false : true; 
        }  
        for(int i=0; i<4; i++)
        {
            out.KNBgui[i] = outputs.knobs_out[i];  
        }
        out.knobs_transfer = outputs.knobs_transfer;
        
        out.testCPvalvePerc = outputs.test_IV_dc;             
        out.testCFBvalveON = outputs.test_OV_cmd;            
        out.soundALARM = outputs.soundALARM;           
        out.idALARM = outputs.idALARM;    // is a AlarmIndex
        out.LEDalarm = (StateLed::Off == outputs.ledALARM) ? false : true; 
        out.mode = static_cast<Mode>(outputs.Controller_Mode + 1);         
        volatile int stophere = 0;
        stophere++;
    }    
        
    void matlab_controller_feed(const fi5app::theController2::Inp &inp)
    {
        controllerModelClass::ExtU_controller_T inputs;
        inputs.enable = true;
        inputs.S1_Pi = inp.S1pressure;         // S1 Pressure [code] (float)
        inputs.S2_Qi = inp.S2flowrate;         // S2 Flow Rate [code] (float)
        inputs.S3_i[0] = inp.S3pressure;       // S3 Pressure [code] (float)
        inputs.S3_i[1] = inp.S3temperature;       // S3 Temperature [code] (float)
        inputs.S4_Pi = inp.S4pressure;         // S4 Pressure [code] (float)
        inputs.S5_Qi = inp.S5flowrate;         // S5 Flow Rate [code] (float)
        inputs.S7_Pi = inp.S7pressure;         // S7 Pressure [code] (float)
        inputs.params[0] = 50.F;    // Zephyr FS [SLPM] (float)
        inputs.params[1] = 160.F;   // HSC LP FS [mbar] (float)
        inputs.params[2] = 150.F; // HSC HP FS [mbar] (float)
        controller_Obj.setExternalInputs(&inputs);   
    }
    
    void matlab_controller_setparams()
    {
        // use _fsmout or .. just get directly fsm's outputs
        const FSM0ModelClass::ExtY_FSM0_T & outputs = FSM0_Obj.getExternalOutputs();
        
        // Interface FSM/Controller
        // ahhhhhhhhhhhhhhhhhhh wiki errata! devo prendere un handle oppure chiamare il set
        controllerModelClass::P_controller_T parameters = controller_Obj.getBlockParameters();
        
        // controller’s state handling
        
        parameters.Controller_control_mode = outputs.Controller_Mode + 1;
        
        // knobs and maps handling
        switch(outputs.knobs_transfer) 
        {
            
            case MAP_Transfer_Standard:
            {
                parameters.respiratory_rate_Value = outputs.knobs_out[0];
                parameters.PIP_Gain = outputs.knobs_out[1];
                parameters.IE_ratio_Value = outputs.knobs_out[2];
                parameters.tidal_volume_Value = outputs.knobs_out[3] / 1000.F;
            } break;

            case MAP_Transfer_VCV:
            {
                parameters.VC_P_Value = outputs.knobs_out[0];
                parameters.VC_I_Value = outputs.knobs_out[1];
                parameters.VC_P_rec_Value = outputs.knobs_out[2];
                parameters.VC_I_rec_Value = outputs.knobs_out[3];
            } break;

            case MAP_Transfer_PRVC_1:
            {
                parameters.PC_P_Value = outputs.knobs_out[0];  // ugo: is it maybe PC_P_Value instead of Controller_PC_P?
                parameters.PC_I_Value = outputs.knobs_out[1];    // ugo: is it PC_I_Value instead of Controller_PC_I?
                parameters.PC_P_rec_Value = outputs.knobs_out[2]; // ugo: PC_P_rec_Value instead of Controller_PC_P_rec
                parameters.PC_I_rec_Value = outputs.knobs_out[3]; // ugo: is it PC_I_rec_Value instead of Controller_PC_I_rec?
            } break;

            case MAP_Transfer_PRVC_2:
            {
                parameters.PC_FFW_Value = outputs.knobs_out[0]; // PC_FFW_Value instead of Controller_FFW?
                parameters.PC_FFW_rec_Value = outputs.knobs_out[1]; // PC_FFW_rec_Value instead of Controller_FFW_rec?
                parameters.PIP_step_perc_Gain = outputs.knobs_out[2];
                parameters.PIP_rise_perc_Value = outputs.knobs_out[3];
            } break;

            case MAP_Transfer_Advanced:
            {
                parameters.enable_trigger_Value = (outputs.knobs_out[0] > 0.5);
                parameters.pressure_thres_const = outputs.knobs_out[1];
                parameters.flowby_target_Value = outputs.knobs_out[2];
                parameters.TV_correction_gain_Value = outputs.knobs_out[3];
            } break;
            
            case MAP_Transfer_None:
            default:
            {
                
            } break;
            
        }
        
        controller_Obj.setBlockParameters(&parameters);
    }
    
    void matlab_mode(int m)
    {
        const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
        controllerModelClass::P_controller_T p = parameters;
        p.Controller_control_mode = m; 
        controller_Obj.setBlockParameters(&p); 
    }
    
    void matlab_control_get(fi5app::theController2::ctrlOut &out)
    {
        const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
        out.CPvalvePerc = outputs.IV_dc;              // CP valve DC command [%] (float)
        out.CFBvalveON = outputs.OV_cmd;             // CFB valve ON/OFF command [-] (boolean)
        out.targetFlowRate = outputs.Q_ref;              // Target Flow Rate [L/min] (float)
        out.targetPressure = outputs.P_ref;              // Target Pressure [cmH2O] (float)
        out.filtS1pressure = outputs.signals[0];         // S1 Filtered Pressure [cmH2O] (float)
        out.S2flowrate = outputs.signals[1];         // S2 Flow Rate [L/min] (float)
        out.filtS3pressure = outputs.signals[2];         // S3 Filtered Pressure [cmH2O] (float)
        out.S3temperature = outputs.signals[3];         // S3 Temperature [°C] (float)
        out.filtS4pressure = outputs.signals[4];         // S4 Filtered Pressure [cmH2O] (float)
        out.S5flowrate = outputs.signals[5];         // S5 Flow Rate [L/min] (float)
        out.filtS7pressure = outputs.signals[6];         // S7 Filtered Pressure [cmH2O] (float)
        out.estTidalVolume = outputs.signals[7];         // Tidal Volume estimates [L] (float)
        out.maxTidalVolume2Cycles = outputs.signals[8];  // Maximum Tidal Volume per 2 cycles [L] (float)
        out.assistedVentilTrigger = outputs.signals[9];     // Assisted Ventilation Trigger [-] (float)   
    }

};



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


namespace fi5app {

    theController2& theController2::getInstance()
    {
        static theController2* p = new theController2();
        return *p;
    }
    
    theController2::theController2()
    {
        pImpl = std::make_unique<Impl>();
    }  
    
    
    theController2::~theController2() { }
   

    bool theController2::init(const Config &cfg)
    {
        return pImpl->init(cfg);
    } 
        
    bool theController2::load(const board_sensor_data_t &sd)
    {
        return pImpl->load(sd);
    }
   
    bool theController2::set(Mode mode)
    {
        return pImpl->set(mode);
    }
    
    bool theController2::tick()
    {
        return pImpl->tick();
    }
    
    const theController2::Out& theController2::getOut() const
    {
        return pImpl->getout();
    }
   
    const theController2::ctrlOut& theController2::get_ctrlOut() const
    {
        return pImpl->getctrlout();
    }
    
    const theController2::fsmOut& theController2::get_fsmOut() const
    {
        return pImpl->getfsmout();
    }

    const theController2::Inp& theController2::getInp() const
    {
        return pImpl->getinp();
    }  


} // namespace app {

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

