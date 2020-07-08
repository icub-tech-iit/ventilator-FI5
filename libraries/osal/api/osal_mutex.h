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
#ifndef _OSAL_MUTEX_H_
#define _OSAL_MUTEX_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_mutex.h
    @brief      This header file implements public interface to the os abstraction layer: mutex
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_base OS abstraction layer: mutex
    crecvrecverdec    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
 
 
// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef struct osal_mutex_opaque_t          osal_mutex_t
    @brief      osal_mutex_t is the opaque definition of a mutex. It offers priority inversion. 
 **/
typedef struct osal_mutex_opaque_t          osal_mutex_t; 

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section
                 


// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern osal_mutex_t * osal_mutex_new(void)
    @brief      Creates a mutex.
    @return     A not NULL handle to the mutex or NULL upon failure.
**/
extern osal_mutex_t * osal_mutex_new(void);


/** @fn         extern osal_result_t osal_mutex_take(osal_mutex_t *mutex, osal_reltime_t tout)
    @brief      The caller tasks takes the mutex if some other task does not already holds it. If the same
                task takes mulytiple times the same mutex, then the function returns immediately after it has
                incremented an internal counter. If some other task holds the mutex, the caller task waits
                up to @e tout microseconds. The mutex offers priority-inversion, so that a calling task 
                whcih is blocked by a lower-priority task which is owner of the mutex passes its priority
                to the owner.
    @param      tout            The timeout of the operation
    @return     A value of osal_res_OK on successful take, osal_res_NOK_timeout upon timeout failure,  
                osal_res_NOK_nullpointer if mutex is NULL, or osal_res_NOK_generic if called from an ISR.
**/
extern osal_result_t osal_mutex_take(osal_mutex_t *mutex, osal_reltime_t tout);


/** @fn         extern osal_result_t osal_mutex_release(osal_mutex_t *mutex)
    @brief      The caller tasks releases the mutex. If it called osal_mutex_take() multiple times then
                the mutex is released only when it has called osal_mutex_release() for the same number of
                times (aka, when the internal counter is zero again).
    @return     A value of osal_res_OK upon successful release, osal_res_NOK_generic upon generic failure, 
                osal_res_NOK_nullpointer if mutex is NULL, or osal_res_NOK_generic if called from an ISR.
**/
extern osal_result_t osal_mutex_release(osal_mutex_t *mutex);


/** @fn         extern osal_result_t osal_mutex_delete(osal_mutex_t *mutex)
    @brief      The mutex is deleted. 
    @return     A value of osal_res_OK upon successful release, osal_res_NOK_generic upon generic failure, 
                osal_res_NOK_nullpointer if mutex is NULL, or osal_res_NOK_generic if called from an ISR.
**/
extern osal_result_t osal_mutex_delete(osal_mutex_t *mutex);

/* @}            
    end of group osal_mutex  
 **/
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

