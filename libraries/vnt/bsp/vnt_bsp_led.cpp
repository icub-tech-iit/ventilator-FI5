
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

#include "vnt_bsp_led.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stm32hal.h"
#include <cstring>
#include <vector>

#include "vnt_core_binary.h"

// marco.accame: we can manage the LED or LEDs in many ways. for now we use the BSP of stm32746g_discovery
#include "stm32746g_discovery.h"

#define VNT_ENABLE_hw_led

// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------

// for now ... 


// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------

using namespace vnt::bsp;

#if !defined(VNT_ENABLE_hw_led)

namespace vnt { namespace bsp { namespace led {
    
    #error fill it
}}}

#else

namespace vnt { namespace bsp { namespace led {
    
    // we must make sure that the value of LED::one is equal to LED_GREEN
    static_assert(vnt::core::tointegral(LED::one) == LED_GREEN, "LED::one != LED_GREEN from stm32746g_discovery");
    constexpr uint32_t supportedmask = vnt::core::binary::mask::pos2mask<uint32_t>(LED::one);
    
    // initialised mask
    static std::uint32_t initialisedmask = 0;
            
  
    
    bool supported(LED led)
    {
        //return true;
        //return vnt::hw::bsp::led::getBSP().supported(led);
        return vnt::core::binary::bit::check(supportedmask, vnt::core::tointegral(led));
    }
    
    bool initialised(LED led)
    {
        return vnt::core::binary::bit::check(initialisedmask, vnt::core::tointegral(led));
    }
            
    result_t init(LED led)
    {
        if(!supported(led))
        {
            return resNOK;
        }  
        
        if(initialised(led))
        {   // dont need to re-init
            return resOK;
        }
        
        
        if(!vnt::bsp::initialised())
        {   // requires vnt::hw::bsp::init()
            return resNOK;
        }
        
        
        // typically every led initalisation is done in the bsp, but ...
        // init peripheral
        // embot::hw::bsp::led::getBSP().init(led);
        
        
        BSP_LED_Init(static_cast<Led_TypeDef>(vnt::core::tointegral(led)));
                        

       
        vnt::core::binary::bit::set(initialisedmask, vnt::core::tointegral(led));
        
        // we just switch it off        
        vnt::bsp::led::off(led);
        
        return resOK;        

    }

          
    result_t on(LED led)
    {
        if(!initialised(led))
        {
            return resNOK;
        }  
        //const embot::hw::GPIO *gpio = &embot::hw::bsp::led::getBSP().getPROP(led)->gpio;      
        ////HAL_GPIO_WritePin(static_cast<GPIO_TypeDef*>(gpio->port), static_cast<uint16_t>(gpio->pin), convert(embot::hw::bsp::led::getBSP().getPROP(led)->on));
        //embot::hw::gpio::set(*gpio, embot::hw::bsp::led::getBSP().getPROP(led)->on);  
        
        BSP_LED_On(static_cast<Led_TypeDef>(vnt::core::tointegral(led)));
        return resOK;        
    }
    
    result_t off(LED led)
    {
        if(!initialised(led))
        {
            return resNOK;
        }  
        //const embot::hw::GPIO *gpio = &embot::hw::bsp::led::getBSP().getPROP(led)->gpio;       
        ////HAL_GPIO_WritePin(static_cast<GPIO_TypeDef*>(gpio->port), static_cast<uint16_t>(gpio->pin), convert(embot::hw::bsp::led::getBSP().getPROP(led)->off));
        //embot::hw::gpio::set(*gpio, embot::hw::bsp::led::getBSP().getPROP(led)->off);  
        BSP_LED_Off(static_cast<Led_TypeDef>(vnt::core::tointegral(led)));
        return resOK;          
    }
    
    result_t toggle(LED led)
    {
        if(!initialised(led))
        {
            return resNOK;
        }  
        //const embot::hw::GPIO *gpio = &embot::hw::bsp::led::getBSP().getPROP(led)->gpio;       
        ////HAL_GPIO_TogglePin(static_cast<GPIO_TypeDef*>(gpio->port), static_cast<uint16_t>(gpio->pin));
        //embot::hw::gpio::toggle(*gpio);  
        BSP_LED_Toggle(static_cast<Led_TypeDef>(vnt::core::tointegral(led)));
        return resOK;          
    }
    
    
}}} // namespace vnt { namespace bsp { namespace led 


#endif //#if defined(VNT_ENABLE_hw_led)



// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

