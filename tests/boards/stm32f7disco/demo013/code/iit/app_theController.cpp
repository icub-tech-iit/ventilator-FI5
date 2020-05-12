
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

#include "app_theController.h"



// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

    
// - begin of placeholder 
//   for putting the interface towards the matlab generated code.
//   we need some .h files and a static object. maybe some macros to load one controller or another at compiletime

#include "controller.h"                     
#include "rtwtypes.h"
#include "zero_crossing_types.h"

static controllerModelClass controller_Obj; 



static void matlab_init()
{
    controller_Obj.initialize();
}

static void matlab_mode(int m)
{
    const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
    controllerModelClass::P_controller_T p = parameters;
    p.Controller_control_mode = m; 
    controller_Obj.setBlockParameters(&p); 
}


static void matlab_feed(const app::theController::Inp &inp)
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

static void matlab_tick()
{
    controller_Obj.step();
}


static void matlab_get(app::theController::Out &out)
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
    out.assistedVentilTrigger = outputs.signals[8];         // Assisted Ventilation Trigger [-] (float)   
}

// - end of placeholder
    
struct app::theController::Impl
{      
    size_t _counter {0};

    Config _config {};
   
    theController::Out _out {};
    theController::Inp _inp {};  
        
        
    Impl()
    { 
         
    }
    
    bool init(const Config &cfg)
    {
        _config = cfg;
             
        matlab_init();
        
        set(Mode::IDLE);
        
        return true;       
    }
    
    bool set(const board_sensor_data_t &sd)
    {
       _inp.load(sd);
       matlab_feed(_inp);
       return true; 
    }
    
    bool set(Mode mode)
    {
        matlab_mode(vnt::core::tointegral(mode));
        return true;        
    }
    
    bool tick()
    {
        matlab_tick();
        matlab_get(_out);
                
        _counter++;
        return true;
    }  

    const Out& getout() const
    {
       return _out; 
    }    

    const Inp& getinp() const
    {
       return _inp; 
    } 
    
    bool setIEratio(float v)
    {
        const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
        controllerModelClass::P_controller_T p = parameters;
        p.IE_ratio_Value = v;      
        controller_Obj.setBlockParameters(&p); 
        return true;
    }     

    bool setTidalVolume(float v)
    {
        const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
        controllerModelClass::P_controller_T p = parameters;
        p.TidalVolume_Gain = v;      
        controller_Obj.setBlockParameters(&p); 
        return true;
    }
        
    bool setPMax(float v)
    {
        const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
        controllerModelClass::P_controller_T p = parameters;
        p.PIP_Gain = v;   
        controller_Obj.setBlockParameters(&p); 
        return true;
    }
        
    bool setFreq(float v)  
    {
        const controllerModelClass::P_controller_T& parameters = controller_Obj.getBlockParameters();
        controllerModelClass::P_controller_T p = parameters;
        p.respiratory_rate_Value = v;        
        controller_Obj.setBlockParameters(&p);        
        return true;
    }    

    const controllerModelClass::P_controller_T& getPARs()    
    {
        const controllerModelClass::P_controller_T& p = controller_Obj.getBlockParameters();
        return p;        
    }
};



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


namespace app {

    theController& theController::getInstance()
    {
        static theController* p = new theController();
        return *p;
    }
    
    theController::theController()
    {
        pImpl = std::make_unique<Impl>();
    }  
    
    
    theController::~theController() { }
   

    bool theController::init(const Config &cfg)
    {
        return pImpl->init(cfg);
    } 
        
    bool theController::set(const board_sensor_data_t &sd)
    {
        return pImpl->set(sd);
    }

    
    bool theController::set(Mode mode)
    {
        return pImpl->set(mode);
    }
    
    bool theController::tick()
    {
        return pImpl->tick();
    }
   
    const theController::Out& theController::getOut() const
    {
        return pImpl->getout();
    }

    const theController::Inp& theController::getInp() const
    {
        return pImpl->getinp();
    }  

    bool theController::setIEratio(float ieratio)
    {
        return pImpl->setIEratio(ieratio);
    }    

    bool theController::setTidalVolume(float v)
    {
        return pImpl->setTidalVolume(v);
    }
        
    bool theController::setPMax(float v)
    {
        return pImpl->setPMax(v);
    }
        
    bool theController::setFreq(float v)
    {
        return pImpl->setFreq(v);
    }    

    const controllerModelClass::P_controller_T& theController::getPARs()
    {
        return pImpl->getPARs();
    }        

} // namespace app {

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

