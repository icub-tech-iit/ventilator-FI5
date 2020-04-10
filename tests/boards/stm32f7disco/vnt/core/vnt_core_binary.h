
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware-shared for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_CORE_BINARY_H_
#define _VNT_CORE_BINARY_H_

#include <cstdint>
#include <cstring>
#include <initializer_list>

#include "vnt_core.h"



namespace vnt { namespace core { namespace binary { namespace bit {
       
    // it forces 1 into pos-th bit of value
    template<typename T>
    constexpr void set(T &value, std::uint8_t pos)
    {
        value |= (static_cast<T>(1)<<pos);
    }
    
    // it forces 0 into pos-th bit of value
    template<typename T>
    void constexpr clear(T &value, std::uint8_t pos)
    {
        value &= (~(static_cast<T>(1)<<pos));
    }
    
    // it toggles the pos-th bit of value
    template<typename T>
    void constexpr toggle(T &value, std::uint8_t pos)
    {
        value ^= (static_cast<T>(1)<<pos);
    }
    
    // it returns true if the pos-th bit of value is 1
    template<typename T>
    constexpr bool check(const T value, std::uint8_t pos)
    {
        if(value & (static_cast<T>(1)<<pos))
        {
            return true;
        }
        return false;
    } 


    std::uint8_t countUsize(const std::uint64_t value, const std::uint8_t size);

    // it tells how many 1 bits there are value
    template<typename T>
    constexpr std::uint8_t count(const T value)
    {
        return countUsize(static_cast<std::uint64_t>(value), sizeof(value));
    }  


    template<typename T>
    constexpr std::int8_t posofmostsignificant(const T value)
    {
        int8_t p = 8*sizeof(value)-1;
        for(; p>0; p--)
        {
            if(check(value, p))
            {
                break;
            }
        }
        return p;
    }      
       
} } } } // namespace vnt { namespace core { namespace binary { namespace bit



namespace vnt { namespace core { namespace binary { namespace mask {
    
    // it creates a mask which as `1` value in positions specified by posofonbits
    template<typename M, typename T>
    constexpr M generate(const std::initializer_list<T> &posofonbits)
    {
        M mask = 0;
        for(auto a : posofonbits) vnt::core::binary::bit::set(mask, a);
        return mask;
    } 
         
    // it forces to 1 the bits in value which are selected by msk (where msk has bits of value 1). the other bits of value stay unchanged.
    template<typename T>
    constexpr void set(T &value, const T msk, const uint8_t shift = 0)
    {
        value |= (msk << shift);
    }
    
    // it forces to 0 the bits in value which are selected by msk (where msk has bits of value 1). the other bits of value stay unchanged.
    template<typename T>
    constexpr void clear(T &value, const T msk, const uint8_t shift = 0)
    {
        value &= (~(msk << shift));
    }
    
    // it toggles the bits in value which are selected by msk (where msk has bits of value 1). the other bits of value stay unchanged.
    template<typename T>
    constexpr void toggle(T &value, const T msk, const uint8_t shift = 0)
    {
        value ^= (msk << shift);
    }
    
    // it returns true if the bits in value which are selected by msk are all 1.
    template<typename T>
    constexpr bool check(const T value, const T msk, const uint8_t shift = 0)
    {
        if(msk == (value & (msk << shift)))
        {
            return true;
        }
        return false;
    }   
     
    template<typename M, typename T>
    constexpr M pos2mask(const T t)
    {
        return (1 << static_cast<M>(t));
    } 
      
    
} } } } // namespace vnt { namespace core { namespace binary { namespace mask



namespace vnt { namespace core { namespace binary { namespace nibble {
    
    using NIBBLE = std::uint8_t;
    
    // it convert any integer into a nibble by getting only its lowest significant 4 bits.
    template<typename T>
    constexpr NIBBLE convert(const T &value)
    {
        return static_cast<NIBBLE>(value) & 0x0f;
    }
        
    // it assign nib into the pos-th nibble of value.
    template<typename T>
    void assign(T &value, const NIBBLE nib, std::uint8_t pos)
    {
        vnt::core::binary::mask::clear(value, static_cast<T>(0xf)<<(4*pos));
        vnt::core::binary::mask::set(value, static_cast<T>(nib & 0xf)<<(4*pos));
    }
    
    template<typename T>
    constexpr NIBBLE get(const T &value, std::uint8_t pos)
    {
        return convert(value >> pos);
    }

    // it puts 0x0 into the pos-th nibble of value
    template<typename T>
    constexpr void clear(T &value, std::uint8_t pos)
    {
        vnt::core::binary::mask::clear(value, static_cast<T>(0xf)<<(4*pos));
    }
    
    // it returns true if nib is equal to the pos-th nibble of value
    template<typename T>
    constexpr bool check(const T value, const NIBBLE nib, std::uint8_t pos)
    {
        return vnt::core::binary::mask::check(value, static_cast<T>(nib & 0xf)<<(4*pos));
    } 
       
} } } } // namespace vnt { namespace core { namespace binary { namespace nibble


namespace vnt { namespace core { namespace binary { namespace pair {
    
    using PAIR = std::uint8_t;
    
    // it convert any integer into a pair by getting only its lowest significant 2 bits.
    template<typename T>
    constexpr PAIR convert(const T &value)
    {
        return static_cast<PAIR>(value) & 0x03;
    }
        
    // it assign pa into the pos-th pair of value.
    template<typename T>
    constexpr void assign(T &value, const PAIR pa, std::uint8_t pos)
    {
        vnt::core::binary::mask::clear(value, static_cast<T>(0x3)<<(2*pos));
        vnt::core::binary::mask::set(value, static_cast<T>(pa & 0x3)<<(2*pos));
    }
    
    template<typename T>
    PAIR get(const T &value, std::uint8_t pos)
    {
        return convert(value >> pos);
    }
    
    // it puts 0x0 into the pos-th pair of value
    template<typename T>
    constexpr void clear(T &value, std::uint8_t pos)
    {
        vnt::core::binary::mask::clear(value, static_cast<T>(0x3)<<(2*pos));
    }
    
    // it returns true if pa is equal to the pos-th pair of value
    template<typename T>
    constexpr bool check(const T value, const PAIR pa, std::uint8_t pos)
    {
        return vnt::core::binary::mask::check(value, static_cast<T>(pa & 0x3)<<(2*pos));
    } 
       
} } } } // namespace vnt { namespace core { namespace binary { namespace pair

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
