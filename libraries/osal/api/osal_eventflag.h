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
#ifndef _OSAL_EVENTFLAG_H_
#define _OSAL_EVENTFLAG_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_eventflag.h
    @brief      This header file implements public interface to the os abstraction layer: event flag
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_eventflag OS abstraction layer: event flag
    cecercer
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"
#include "osal_task.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


 
// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef uint32_t osal_eventflag_t
    @brief      osal_eventflag_t is the event flag type used by OSAL. It holds up to 32 different flags to each task.
 **/
typedef uint32_t                            osal_eventflag_t;          


/** @typedef    typedef enum osal_eventflag_waitmode_t 
    @brief      osal_eventflag_waitmode_t contains all possible modes with which an osal entity can wait for an eventflag
 **/  
typedef enum 
{
    osal_waitALLflags = 0,       /** < waits for every bit set in the eventflag */          
    osal_waitANYflag  = 1        /** < waits for only the first available in the bits set in the eventflag */ 
} osal_eventflag_waitmode_t;

   
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section
                 


// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern osal_result_t osal_eventflag_set(osal_eventflag_t flag, osal_task_t * totask, osal_caller_t caller) 
    @brief      Sends an event flag to a given task.
    @param      msk             The event mask.
    @param      totask          A valid handle to the destination task 
    @param      caller          The caller mode.  Use osal_callerTSK if the caller is a normal task and
                                osal_callerISR if the caller is an ISR, or osal_callerTMRMAN if it is a function executed by
                                the timer manager (hence by the scheduler).
    @return     The value of osal_res_OK upon success and osal_res_NOK_nullpointer if task handle is NULL.
**/
extern osal_result_t osal_eventflag_set(osal_eventflag_t flag, osal_task_t * totask, osal_caller_t caller); 


/** @fn         extern osal_result_t osal_eventflag_get(osal_eventflag_t waitmsk, osal_eventflag_waitmode_t waitmode, osal_eventflag_t *rxmsk, osal_reltime_t tout)
    @brief      Retrieves an event mask directed to the calling task.
    @param      waitmsk         The mask of eventflags to wait.
    @param      waitmode        Express the wait mode which calling task should wait upon the waitmsk: either ALL or ANY of 
                                the bits whcih are set in the waitmsk.
    @param      rxmsk           The received mask.  It contains a valid content only if result is not osal_res_NOK_timeout.
    @param      tout            The timeout of teh operation
    @return     The value of osal_res_OK upon success or a osal_res_NOK_timeout if timeout expired.
**/
extern osal_result_t osal_eventflag_get(osal_eventflag_t waitmsk, osal_eventflag_waitmode_t waitmode, osal_eventflag_t *rxmsk, osal_reltime_t tout);


/** @fn         extern osal_eventflag_t osal_eventflag_getany(osal_reltime_t tout)
    @brief      A quick and simplified way to retrieves an event mask directed to the calling task.
    @param      tout            The timeout of teh operation
    @return     The mask with the received events upon success or zero if timeout expired. 
**/
extern osal_eventflag_t osal_eventflag_getany(osal_reltime_t tout);


/* @}            
    end of group osal_eventflag  
 **/
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

