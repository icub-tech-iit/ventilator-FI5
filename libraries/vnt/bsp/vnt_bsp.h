

/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_H_
#define _VNT_BSP_H_

#include "vnt_core.h"


namespace vnt { namespace bsp {
    
    // maybe move onto namespace vnt::bsp::hmi
    
    // the name of LEDs can stay from one to eight. one is used for testing. {four, five, sixe} for the front-panel  
    enum class LED : std::uint8_t { one = 0, two = 1, three = 2, four = 3, five = 4, six = 5, seven = 6, eight = 7, none = 31, maxnumberof = 8,
                                    testGREEN = one,        // on the stm32 board and used so far for testing        
                                    RED = four,             // the red one on front-panel
                                    GREEN = five,           // the green one on front-panel
                                    YELLOW = six            // the yellow one on front-panel
    };  
        
    // evaluate to add an emergency button to push in case of fault     
    enum class BTN : std::uint8_t { one = 0, two = 1, three = 2, four = 3, five = 4, six = 5, seven = 6, eight = 7, none = 31, maxnumberof = 8,
                                    testBLUE = one,         // on the stm32board and used so far for testing
                                    hmiOK = four           // the OK for HMI 
    };
     
}} // namespace vnt { namespace bsp {



namespace vnt { namespace bsp {
    
    enum class result_t : std::int8_t { OK = 0, NOK = -1, NOKtimeout = -2 }; 
    
    const result_t resOK            = result_t::OK;
    const result_t resNOK           = result_t::NOK;
    const result_t resNOKtimeout    = result_t::NOKtimeout;
        
    enum class CLOCK : std::uint8_t { syscore = 0, pclk1 = 1, pclk2 = 2, none = 31, maxnumberof = 3 };      
    
    struct Config
    {         
        vnt::core::fpWorker initclock {nullptr};  
        vnt::core::fpGetU64 get1microtime {nullptr}; 
        vnt::core::fpGetU32 get1millitime {nullptr};        
        
        constexpr Config() = default;
        constexpr Config(vnt::core::fpWorker _init, vnt::core::fpGetU64 _tmicro, vnt::core::fpGetU32 _tmilli) : initclock(_init), get1microtime(_tmicro), get1millitime(_tmilli) {}
        bool isvalid() const { if((nullptr != get1microtime) || (nullptr != get1millitime)) { return true; } else { return false; } }
    }; 
            
    bool initialised();
    
    // it calls the proper initialisations for stm32hal etc.
    bool init(const Config &config);       

    // retrieves the specified clock speed    
    std::uint32_t clock(vnt::bsp::CLOCK clk);
    
    // forces a reset
    void reset();
        
    // forces the cpu to exec a given number of assembly instructions to reach the specified delay
    // if os is running, prefer vnt::core::delay() 
    void delay(vnt::core::relTime t);  
    
}} // namespace vnt { namespace bsp {



#if defined(VNTcfg_FI5)
    // nothing so far
#else
    
    // we dont need them anymore. it was just a proposal for how to manage sensors before using another approach
namespace vnt { namespace bsp { namespace sns {
    

//    #warning TODO: adjust the names of the sensors according to BOM rev 9
    
    enum class PRS : std::uint8_t { S1 = 0, S2 = 1, S3 = 2, none = 31, maxnumberof = 3 };
    
    enum class FLW : std::uint8_t { F1 = 0, F2 = 1, F3 = 2, none = 31, maxnumberof = 3 };
    
    // put others and review

}}} // namespace vnt { namespace bsp { namespace sns {

namespace vnt { namespace bsp { namespace act {
    
    // valve
    enum class VLV : std::uint8_t { V1 = 0, V2 = 1, none = 31, maxnumberof = 2 };
    
    enum class XXX : std::uint8_t { x1 = 0, X2 = 1, X3 = 2, none = 31, maxnumberof = 3 };
    
    // put others and review

}}} // namespace vnt { namespace bsp { namespace sns {

#endif

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


