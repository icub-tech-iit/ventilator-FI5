

/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_LED_H_
#define _VNT_BSP_LED_H_


#include "vnt_core.h"
#include "vnt_bsp.h"


namespace vnt { namespace bsp { namespace led {
        
    bool supported(vnt::bsp::LED led);    
    bool initialised(vnt::bsp::LED led);
        
    result_t init(vnt::bsp::LED led);
    result_t on(vnt::bsp::LED led);
    result_t off(vnt::bsp::LED led);
    result_t toggle(vnt::bsp::LED led);        
       
}}} // namespace vnt { namespace bsp { namespace led 



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


