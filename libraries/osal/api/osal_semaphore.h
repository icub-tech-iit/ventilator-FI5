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
#ifndef _OSAL_SEMAPHORE_H_
#define _OSAL_SEMAPHORE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_sempahore.h
    @brief      This header file implements public interface to the os abstraction layer: semaphore
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_semaphore OS abstraction layer: semaphore
    cxece
    
    @{  
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


 
// - declaration of public user-defined types ------------------------------------------------------------------------- 

/** @typedef    typedef struct osal_semaphore_opaque_t      osal_semaphore_t
    @brief      osal_semaphore_t is the opaque definition of a sempahore.  
 **/         
typedef struct osal_semaphore_opaque_t      osal_semaphore_t; 

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section                 


// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern osal_semaphore_t * osal_semaphore_new(uint8_t maxtokens, uint8_t tokens)
    @brief      Creates a sempahore.
    @param      maxtokens       The maximum number of tokens that the semaphore can accept
    @param      tokens          The initial number of tokens in the semaphore.
    @return     A not NULL handle to the sempahore or NULL upon failure.
**/
extern osal_semaphore_t * osal_semaphore_new(uint8_t maxtokens, uint8_t tokens);


/** @fn         extern osal_result_t osal_semaphore_set(osal_semaphore_t *sem, uint8_t tokens)
    @brief      Sets the number of tokens of a semaphore.
    @param      sem             The semaphore
    @param      tokens          The number of token in the semaphore.
    @return     A value of osal_res_OK on success, osal_res_NOK_generic upon failure, or  
                osal_res_NOK_nullpointer if sem is NULL.
**/
extern osal_result_t osal_semaphore_set(osal_semaphore_t *sem, uint8_t tokens);


/** @fn         extern osal_result_t osal_semaphore_decrement(osal_semaphore_t *sem, osal_reltime_t tout)
    @brief      The function returns succesfully only if the number of internal tokens is more than zero. 
                In this case, the number of tokens is decremented at each call. If the number of tokens is
                zero, the function blocks up to @e tout microseconds until some other task calls a
                osal_semaphore_increment(). 
                The osal_semaphore_decrement() function can be used by a task to block itself until some
                other task calls osal_semaphore_increment(). 
    @param      sem             The semaphore
    @param      tout            The timeout of the operation
    @return     A value of osal_res_OK on success, osal_res_NOK_timeout upon timeout failure or a 
                osal_res_NOK_nullpointer if sem is NULL.
**/
extern osal_result_t osal_semaphore_decrement(osal_semaphore_t *sem, osal_reltime_t tout);


/** @fn         extern osal_result_t osal_semaphore_increment(osal_semaphore_t *sem)
    @brief      The function increments the number of internal tokens until its maximum number which was specified
                at creation time with @e osal_semaphore_new().
                The osal_semaphore_increment() function can be used by a task to un-block another task
                which is waiting to decrement the same semaphore.
    @param      sem             The semaphore
    @param      caller          The caller of the function
    @return     A value of osal_res_OK upon successful increment, of osal_res_NOK_generic if internal tokens have already
                reached the maximum number, of osal_res_NOK_nullpointer upon calls with a NULL semaphore.
**/
extern osal_result_t osal_semaphore_increment(osal_semaphore_t *sem, osal_caller_t caller);


/** @fn         extern osal_result_t osal_semaphore_delete(osal_semaphore_t *sem)
    @brief      The semaphore is deleted. 
    @return     A value of osal_res_OK upon successful release, osal_res_NOK_generic upon generic failure, 
                osal_res_NOK_nullpointer if mutex is NULL, or osal_res_NOK_generic if called from an ISR.
**/
extern osal_result_t osal_semaphore_delete(osal_semaphore_t *sem);

/* @}            
    end of group osal_semaphore  
 **/
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

