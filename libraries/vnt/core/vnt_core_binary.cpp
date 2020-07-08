
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware-shared for use
 *          in the ventilator project
*/


// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_core_binary.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - pimpl: private implementation (see scott meyers: item 22 of effective modern c++, item 31 of effective c++
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------



namespace vnt { namespace core { namespace binary { namespace bit {
    

    // see http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
    constexpr std::uint8_t s_eocommon_oneBitsInU8[256] = 
    {
    //  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F (<- n)
    //  =====================================================
        0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, // 0n
        
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 1n (+1)
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 2n (+1)
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 3n (+2)    
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 4n (+1)
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 3n (+2)
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 6n (+2)
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // 7n (+3)
        1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 8n (+1)
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 9n (+2)
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // An (+2)
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // Bn (+3)
        2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // Cn (+2)
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // Dn (+3)
        3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // En (+3)  
        4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8  // Fn (+4)
    };

    constexpr std::uint8_t countU08(const std::uint8_t value)
    {
        return(s_eocommon_oneBitsInU8[value & 0xff]);
    }
    
    constexpr std::uint8_t countU16(const std::uint16_t value)
    {
        return(countU08(value&0xff)+countU08((value>>8)&0xff));
    }
    
    constexpr std::uint8_t countU32(const std::uint32_t value)
    {
        return(countU16(value&0xffff) + countU16((value>>16)&0xffff));
    }
    
    constexpr std::uint8_t countU64(const std::uint64_t value)
    {
        return(countU32(value&0xffffffff) + countU32((value>>32)&0xffffffff));
    }

    std::uint8_t countUsize(const std::uint64_t value, const std::uint8_t size)
    {

        if(1 == size)
        {
            return countU08(static_cast<std::uint8_t>(value));
        }
        else if(2 == size)
        {
            return countU16(static_cast<std::uint16_t>(value));
        }
        else if(4 == size)
        {
            return countU32(static_cast<std::uint32_t>(value));
        }
        else if(8 == size)
        {
            return countU64(value);
        }
        
        return 0;
    }
    
   
} } } } // namespace vnt { namespace core { namespace binary { namespace bit


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

