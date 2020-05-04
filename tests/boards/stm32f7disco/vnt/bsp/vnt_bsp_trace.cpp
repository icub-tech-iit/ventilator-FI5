
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

#include "vnt_bsp_trace.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------


#include "vnt_core.h"

// it uses the stm32hal ...
#include "stm32hal.h"




namespace vnt { namespace bsp { namespace trace {
    
    bool _enabled {false};
    
    bool isenabled()
    {
        return _enabled;
    }
    
    void enable(const bool yes)
    {
       _enabled = yes;       
    }
  
    
    int puts(const char* str) 
    {
        if(!_enabled)
        {
            return 0;
        }
        
        if(nullptr == str)
        {
            return(0);
        }
    
        std::uint32_t ch;
        int num = 0;
        while('\0' != (ch = *str))
        {
            ITM_SendChar(ch);
            str++;
            num++;
        }
         
        ITM_SendChar('\n');
        return(++num);    
    }
    
    
    int puts(const std::string &str) 
    {
        return puts(str.c_str());
    }    
 
    
}}} // namespace vnt { namespace bsp { namespace trace {


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

