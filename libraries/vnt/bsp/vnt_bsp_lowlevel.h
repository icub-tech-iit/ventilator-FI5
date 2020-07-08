

/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_LOWLEVEL_H_
#define _VNT_BSP_LOWLEVEL_H_

#include "vnt_core.h"


namespace vnt { namespace bsp { namespace lowlevel {
    
    void asmEXECcycles(std::uint32_t numcycles);
    
}}} // namespace vnt { namespace bsp { namespace lowlevel {




#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


