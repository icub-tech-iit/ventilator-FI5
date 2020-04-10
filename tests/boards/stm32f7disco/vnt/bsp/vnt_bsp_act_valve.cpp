
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

//#define TEST_VNT_BSP_ACT_VALVE

// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_bsp_act_valve.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_core_binary.h"

// it will uses the stm32hal ...
#include "stm32hal.h"

using namespace vnt::bsp;

namespace vnt { namespace bsp { namespace act { namespace valve {
    
    
    constexpr uint32_t supportedmask = vnt::core::binary::mask::pos2mask<uint32_t>(act::VLV::V1) |
                                       vnt::core::binary::mask::pos2mask<uint32_t>(act::VLV::V2);

    static std::uint32_t initialisedmask = 0;
    
    
    bool supported(vnt::bsp::act::VLV a)
    {
        return vnt::core::binary::bit::check(supportedmask, vnt::core::tointegral(a));
    }
  
    bool initialised(vnt::bsp::act::VLV a)
    {   // tells if the particular a is already initialised or not.
        return vnt::core::binary::bit::check(initialisedmask, vnt::core::tointegral(a));        
    }
    
    
    result_t init(vnt::bsp::act::VLV a, const Config &config)
    {
        if(!supported(a))
        {
            return resNOK;
        }  
        
        if(initialised(a))
        {   // dont need to re-init
            return resOK;
        }
        
        
        if(!vnt::bsp::initialised())
        {   // requires vnt::hw::bsp::init()
            return resNOK;
        }
        
        // TODO_CODE: start
        // in here add the code for initialization of the specific chip / peripheral
        
        // TODO_CODE: end
        
        vnt::core::binary::bit::set(initialisedmask, vnt::core::tointegral(a));
        
        return resOK;     
    }
    
    

    result_t apply(vnt::bsp::act::VLV a, const vnt::measure::ValveCtrl &value)
    {
        if(false == initialised(a))
        {
            return resNOK;
        }   
      
        
        // TODO_CODE: start
        // in here add the code for writing value to the peripheral / chip. return resNOK or resOK
        
        
        
        // TODO_CODE: end
        
        return resOK;       
    }
 
    
    result_t apply(vnt::bsp::act::VLV a, const vnt::measure::ValveCtrl &value, const vnt::core::Callback &oncompletion)
    {
        if(false == initialised(a))
        {
            return resNOK;
        }    
        
        
        // TODO_CODE: start
        // in here add the code for writing value to the peripheral / chip. return resNOK or resOK
        // we also exec the callback when the actuation is surely done / programmed
        
        
        
        // TODO_CODE: end
        
        return resOK;                
    }
    
 
}}}} // namespace vnt { namespace bsp { namespace sns { namespace pressure {


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

