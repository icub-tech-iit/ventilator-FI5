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
#ifndef _OSAL_H_
#define _OSAL_H_

#ifdef __cplusplus
extern "C" {
#endif

// - doxy begin -------------------------------------------------------------------------------------------------------

/** @file       osal.h
    @brief      This header file implements public interface to the osal modules.
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/** @defgroup osal OSAL

    The OSAL is an abstraction layer towards a real-time operative system.
 
    @todo acemor-facenda: review documentation
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"
#include "osal_eventflag.h"
#include "osal_info.h"
#include "osal_mutex.h"
#include "osal_messagequeue.h"
#include "osal_semaphore.h"
#include "osal_system.h"
#include "osal_task.h"
#include "osal_timer.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 
// empty-section

 
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------
// empty-section


/** @}            
    end of group hal  
 **/
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



