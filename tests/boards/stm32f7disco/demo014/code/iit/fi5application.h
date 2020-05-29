 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef FI5APPLICATION_H_
#define FI5APPLICATION_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "stdint.h"
    
struct fi5Version
{
    uint8_t major {1};
    uint8_t minor {0};
    uint8_t year {20}; // since 2000
    uint8_t month {5}; // jan = 1 ..
    uint8_t day {29}; // 1-31
    
    fi5Version() = default;
    constexpr fi5Version(uint8_t ma, uint8_t mi, uint8_t y, uint8_t m, uint8_t d) : major(mi), minor(mi), year(y), month(m), day(d) {}
    uint32_t tou32() const
    {
        return (static_cast<uint32_t>(major) << 24) | (static_cast<uint32_t>(minor) << 16) | 
               (static_cast<uint32_t>(year&0x0f) << 12) | (static_cast<uint32_t>(month&0x0f) << 8) | (static_cast<uint32_t>(day));
    }
};

constexpr fi5Version fi5version {1, 0, 20, 5, 29};

#ifdef __cplusplus
}
#endif

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


