
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

static void matlab_feed(const app::Input &inp)
{
    controllerModelClass::ExtU_controller_T inputs;
    inputs.enable = true;
    inputs.IV_Vdot_fbk = 0.F;
    inputs.IV_P_fbk = (float)inp[0]; // assign the IV_P_fbk as input of the controller 
    inputs.params[0] = 0.F;
    inputs.params[1] = 160.F;
    controller_Obj.setExternalInputs(&inputs);       
}

static void matlab_tick()
{
    controller_Obj.step();
}

static void matlab_get(app::Output &out)
{
    const controllerModelClass::ExtY_controller_T& outputs = controller_Obj.getExternalOutputs();
    out[0] = outputs.IV_dc;
    out[1] = outputs.IV_P_ref;
    out[2] = outputs.signals[1];
    out[3] = outputs.signals[2]; // P_hsc_filt(i)        
}


// - end of placeholder
    
struct app::theController::Impl
{      
    size_t _counter {0};

    Config _config {};
    Input _input {0};
    Output _output {0};
    
    Impl()
    { 
         
    }
    
    bool init(const Config &cfg)
    {
        _config = cfg;
             
        matlab_init();
        
        return true;       
    }

    bool set(const Input &inp)
    {
        _input = inp;
        matlab_feed(_input);
        
        return true;
    }
    
    bool tick()
    {
        matlab_tick();
        matlab_get(_output);
        
        //       copy results in _output
        
//        _output[0] = _input[0] + _counter;
//        _output[1] = _input[1] + _counter;
        
        _counter++;
        return true;
    }

    const Output& get() const
    {
       return _output; 
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
    
    bool theController::set(const Input &inp)
    {
        return pImpl->set(inp);
    }
    
    bool theController::tick()
    {
        return pImpl->tick();
    }

    const Output& theController::get() const
    {
        return pImpl->get();
    }
    

} // namespace app {

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

