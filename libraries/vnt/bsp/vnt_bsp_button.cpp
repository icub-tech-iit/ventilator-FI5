
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_bsp_button.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stm32hal.h"
#include "vnt_bsp.h"

#include <cstring>
#include <array>

#include "vnt_core_binary.h"

// marco.accame: we can manage the BTN or BTNs in many ways. for now we use the BSP of stm32746g_discovery
#include "stm32746g_discovery.h"

#define VNT_ENABLE_hw_button


// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------


namespace vnt { namespace bsp { namespace button {
          
    // we must make sure that the value of BTN::three is equal to BUTTON_KEY
    static_assert(vnt::core::tointegral(BTN::three) == BUTTON_KEY, "BTN::three != BUTTON_KEY from stm32746g_discovery");
    constexpr uint32_t supportedmask = vnt::core::binary::mask::pos2mask<uint32_t>(BTN::three);
    
    // initialised mask
    std::uint32_t initialisedmask = 0;
    
    std::array<vnt::core::Callback, vnt::core::tointegral(BTN::maxnumberof)> s_callbacks;
    
    
    bool supported(BTN btn)
    {
        return vnt::core::binary::bit::check(supportedmask, vnt::core::tointegral(btn));
    }
    
    bool initialised(BTN btn)
    {
        return vnt::core::binary::bit::check(initialisedmask, vnt::core::tointegral(btn));
    }
        
    result_t init(BTN btn)
    {
        if(!supported(btn))
        {
            return resNOK;
        }  
        
        if(initialised(btn))
        {   // dont need to re-init
            return resOK;
        }
        
        // every button initalisation is done in the bsp, but ... 
        //vnt::bsp::button::getBSP().init(btn);        
       
        if(!vnt::bsp::initialised())
        {   // requires vnt::bsp::init()
            return resNOK;
        }
        

        BSP_PB_Init(static_cast<Button_TypeDef>(vnt::core::tointegral(btn)), BUTTON_MODE_GPIO);
        
        
        vnt::core::binary::bit::set(initialisedmask, vnt::core::tointegral(btn));
                
        return resOK;        
    }

    
    bool pressed(BTN btn)
    {
        if(!initialised(btn))
        {
            return false;
        }  
        
        // if GPIO_PIN_SET -> pressed
        return GPIO_PIN_SET == BSP_PB_GetState(static_cast<Button_TypeDef>(vnt::core::tointegral(btn)));      
    }
    
    result_t onpressed(BTN btn, const vnt::core::Callback &callback)
    {
        if(!initialised(btn))
        {
            return resNOK;
        }  

        s_callbacks[vnt::core::tointegral(btn)] = callback;
        
        // now we must make sure that when the button is pressed the IRQHandler just triggers this callback.

        // TODO_CODE: start
        // in here add the code which make the IRQhandler trigger and execute teh callback when button is pressed

        
        
        // TODO_CODE: end
        
        return resOK;        
    }
    
    
}}} // namespace vnt { namespace hw { namespace button 






    



// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

