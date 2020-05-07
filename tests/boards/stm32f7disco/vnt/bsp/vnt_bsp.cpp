
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

#include "vnt_bsp.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include <cstring>
#include <vector>

#include "vnt_core.h"

#include "vnt_bsp_lowlevel.h"

// it uses the stm32hal ...
#include "stm32hal.h"

using namespace std;



namespace vnt { namespace bsp {
    
    static bool initted = false; 
    
    bool initialised()
    {
        return initted;
    }   
        

    bool init(const Config &config)
    {
        if(true == vnt::bsp::initialised())
        {
            return true;
        }
                
        // put whatwever is required for ...        
        stm32hal_config_t cfg = {0};
        cfg.tick1ms_init = config.initclock;
        cfg.tick1ms_get = config.get1millitime;
        
        stm32hal_init(&cfg);
        
        vnt::core::init({{nullptr, config.get1microtime}});
        
        initted = true;
        return true;
    }
       
}} // namespace vnt { namespace bsp {


namespace vnt { namespace bsp {
  
    std::uint32_t clock(vnt::bsp::CLOCK clk)
    {
        std::uint32_t value = 0;
        switch(clk)
        {
            case vnt::bsp::CLOCK::pclk1:
            {
                value = HAL_RCC_GetPCLK1Freq();
            } break;
            
            case vnt::bsp::CLOCK::pclk2:
            {
                value = HAL_RCC_GetPCLK2Freq();
            } break;
            
            default:
            case vnt::bsp::CLOCK::syscore:
            {
                value = HAL_RCC_GetHCLKFreq();
            } break;            
        }   
        return value;
    }
    
    void reset()
    {        
        NVIC_SystemReset();
    }

 

    void delay(vnt::core::relTime t)
    {   
        static uint64_t s_hl_sys_numofops1sec = 0;
        static uint32_t s_hl_sys_used_systemcoreclock = 0;
        if(s_hl_sys_used_systemcoreclock != SystemCoreClock)
        {

            // to occupy a millisec i execute an operation for a number of times which depends on: 
            // SystemCoreClock, cortex gain(1.25 dmips/mhz), flash access, etc.
            // to overcome all this i just consider SystemCoreClock w/out 1.25 gain and i measures
            // extra gain with on a simple assembly function which should take 4 cycles per iteration (?).      
            //s_hl_sys_numofops1sec = (5*(SystemCoreClock)) / 4; 
            s_hl_sys_used_systemcoreclock = SystemCoreClock;
            s_hl_sys_numofops1sec = SystemCoreClock;

            #if defined(STM32H7)
            // empirically removed ...
            #elif defined(STM32L4) 
            s_hl_sys_numofops1sec /= 3;
            #elif defined(STM32F7)
            
            #else
            #error specify tuning             
            #endif

            // at this point i normalise the variable to keep not the nymber of operations for 1 sec,
            // but for 1024*1024 microsec. by doing so, later on i shift by 20 instead of using a division. 
            s_hl_sys_numofops1sec <<= 20;
            s_hl_sys_numofops1sec /= 1000000;
        }
        

        if(t > vnt::core::time1second)
        {
            
        }
        
        volatile uint64_t num = s_hl_sys_numofops1sec * t;
        num >>= 20; 
        //num -= offset; //we may remove some cycles to compensates for previous instructions, but ... we dont do it. it depends on c compiler optimisation 
        if(0 == num)
        {
            return;
        }
        
        // if num is higher than ... max uint32_t we must do a for loop
        if(num < (static_cast<uint64_t>(1) << 31))
        {
            vnt::bsp::lowlevel::asmEXECcycles(static_cast<uint32_t>(num));
        }
        else
        {
            uint64_t n = num >> 31;
            uint64_t r = num - n;
            for(uint64_t i=0; i<n; i++)
            {
                vnt::bsp::lowlevel::asmEXECcycles(static_cast<uint32_t>(1) << 31);
            }
            vnt::bsp::lowlevel::asmEXECcycles(static_cast<uint32_t>(r));           
        }
    }

    
    
}}


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

