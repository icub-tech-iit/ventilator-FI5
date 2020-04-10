
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _VNT_BSP_BUTTON_H_
#define _VNT_BSP_BUTTON_H_

#include "vnt_core.h"
#include "vnt_bsp.h"



namespace vnt { namespace bsp { namespace button {
    

    bool supported(BTN btn);
    
    bool initialised(BTN btn);
        
    result_t init(BTN btn);
        
    bool pressed(BTN btn);
    
    result_t onpressed(BTN btn, const vnt::core::Callback &callback);
       
}}} // namespace vnt { namespace bsp { namespace button 



#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


