/*
 * Copyright (C) 2012 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

// - include guard ----------------------------------------------------------------------------------------------------
#ifndef _OOSIIT_HID_H_
#define _OOSIIT_HID_H_

#ifdef __cplusplus
extern "C" {
#endif


/* @file       oosiit_hid.h
    @brief      This header file implements hidden interface to a module.
    @author     marco.accame@iit.it
    @date       07/20/2012
 **/


// - external dependencies --------------------------------------------------------------------------------------------


#include "stdint.h"


// - declaration of extern public interface ---------------------------------------------------------------------------
 
#include "oosiit.h"
    
    
typedef struct
{    
    uint8_t     priority;
    uint16_t    stacksize;
    void*       extdata;
} osiit_hid_tsk_create_other_args_t;


// - declaration of externally defined variables ----------------------------------------------------------------------

// the configuration in use
extern const oosiit_cfg_t *oosiit_cfg_in_use;
// used for ram of objects specified in oosiit_cfg_in_use if static mode is used
extern       uint32_t *oosiit_params_ram32data;
// used for ram of objects specified in oosiit_cfg_in_use if static mode is used
extern       uint64_t *oosiit_params_ram64data;
// used for stdlib32
extern       uint32_t *oosiit_params_stdlib32data;

// the time of the system expressed in ticks
extern volatile uint64_t oosiit_time;
// the duration in picosec of each unity of the systick register. we use picosec to avoid precision problems.
extern uint64_t oosiit_picosec_per_unit_of_systick;
// the max number of unity of the systick register
extern uint32_t oosiit_num_units_of_systick;

// - definition of hidden constants -----------------------------------------------------------------------------------



// - definition of hidden types ---------------------------------------------------------------------------------------



// - declaration of extern hidden functions ---------------------------------------------------------------------------


//void oosiit_hid_initialise(void);

extern uint16_t oosiit_hid_params_get_stdlib32(const oosiit_cfg_t *cfg, uint16_t *stdlib32size);;
extern uint16_t oosiit_hid_params_get_ram32(const oosiit_cfg_t *cfg, uint16_t *ram32size);
extern uint16_t oosiit_hid_params_get_ram64(const oosiit_cfg_t *cfg, uint16_t *ram64size);


#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 


#endif  // include guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



