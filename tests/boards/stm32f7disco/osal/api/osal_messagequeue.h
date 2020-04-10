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
#ifndef _OSAL_MESSAGEQUEUE_H_
#define _OSAL_MESSAGEQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_messagequeue.h
    @brief      This header file implements public interface to the os abstraction layer: message queue
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_base OS abstraction layer: message queue
     cre vre ver verv cew
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section

 
// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef uint32_t*  osal_message_t
    @brief      osal_message_t is the message type used by OSAL. It holds 32 bit data or a pointer to larger data.
 **/
typedef uint32_t *                          osal_message_t;            


/** @typedef    typedef struct osal_messagequeue_opaque_t   osal_messagequeue_t
    @brief      osal_messagequeue_t is the opaque definition of a message queue. It can hold a variable number
                of messages of type osal_message_t (32 bit data). 
 **/
typedef struct osal_messagequeue_opaque_t   osal_messagequeue_t;    

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section
                 

// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern osal_messagequeue_t * osal_messagequeue_new(uint16_t maxmsg)
    @brief      Creates a messagequeue  with a given capacity.
    @param      maxmsg          The maxim number of message that the messagequeue can contains.
    @return     A not NULL handle to the message queue or NULL upon failure.
**/
extern osal_messagequeue_t * osal_messagequeue_new(uint16_t maxmsg);


/** @fn         extern osal_result_t osal_messagequeue_get(osal_messagequeue_t *mq, osal_message_t *pmsg, 
                                                           osal_caller_t caller, osal_reltime_t tout)
    @brief      Gets a message @e msg from a messagequeue.
    @param      mq              A valid handler to a messagequeue.
    @param      pmsg            The pointer to the message to be retrieved or NULL upon failure.  
                                If pmymsg is a mytype_t *, then you can retrieve it by using (osal_message_t*)&pmymsg as 
                                function parameter.
    @param      tout            The timeout on operation in microseconds (if the caller is an ISR the used timeout shall 
                                be zero irrespectively of the value of @e tout)
    @param      caller          The caller mode.  Use osal_callerTSK if the caller is a normal task and
                                osal_callerISR is the caller is an ISR, or osal_callerTMRMAN if it is a function executed by
                                the timer manager (hence by the scheduler).
    @return     If success, a osal_res_OK. A osal_res_NOK_nullpointer value upon NULL pointer, a osal_res_NOK_timeout 
                if timeout expired without a valid retrieval (even if called by an ISR). 
**/
extern osal_result_t osal_messagequeue_get(osal_messagequeue_t *mq, osal_message_t *pmsg, osal_reltime_t tout, osal_caller_t caller);


/** @fn         extern osal_message_t osal_messagequeue_getquick(osal_messagequeue_t *mq, osal_caller_t caller, osal_reltime_t tout) 
    @brief      Gets a message from a messagequeue in a quicker way, but without internal controls.
    @param      mq              A valid handler to a messagequeue. It must not be NULL.
    @param      caller          The caller mode.  Use osal_callerTSK if the caller is a normal task and
                                osal_callerISR is the caller is an ISR, or osal_callerTMRMAN if it is a function executed by
                                the timer manager (hence by the scheduler).
    @param      tout            The timeout on operation in microseconds.
    @return     A pointer to the message or NULL upon failure.
**/
extern osal_message_t osal_messagequeue_getquick(osal_messagequeue_t *mq, osal_reltime_t tout, osal_caller_t caller);


/** @fn         extern uint16_t osal_messagequeue_available(osal_messagequeue_t *mq, osal_caller_t caller) 
    @brief      Gets the number of messages which can still be placed inside a messagequeue.
    @param      mq              A valid handler to a messagequeue. It must not be NULL.
    @param      caller          The caller mode.  Use osal_callerTSK if teh caller is a normal task and
                                osal_callerISR is the caller is an ISR, or osal_callerTMRMAN if it is a function executed by
                                the timer manager (hence by the scheduler).
    @return     The number of messages which can still be put inside the messagequeue. It returns zero if mq is NULL.
**/
extern uint16_t osal_messagequeue_available(osal_messagequeue_t *mq, osal_caller_t caller);


/** @fn         extern uint16_t osal_messagequeue_size(osal_messagequeue_t *mq, osal_caller_t caller) 
    @brief      Gets the number of messages which already are inside a messagequeue.
    @param      mq              A valid handler to a messagequeue. 
    @param      caller          The caller mode.  Use osal_callerTSK if teh caller is a normal task and
                                osal_callerISR is the caller is an ISR, or osal_callerTMRMAN if it is a function executed by
                                the timer manager (hence by the scheduler).
    @return     The number of messages which already are inside the messagequeue. It returns zero if mq is NULL. 
**/
extern uint16_t osal_messagequeue_size(osal_messagequeue_t *mq, osal_caller_t caller);


/** @fn         extern osal_result_t osal_messagequeue_put(osal_messagequeue_t *mq, osal_message_t msg, osal_caller_t caller, osal_reltime_t tout) 
    @brief      Put a message inside a messagequeue.
    @param      mq              A valid handler to a messagequeue.
    @param      msg             The message to be put inside 
    @param      tout            The timeout of the operation
    @param      caller          The caller mode.  Use osal_callerTSK if the caller is a normal task and
                                osal_callerISR is the caller is an ISR, or osal_callerTMRMAN if it is a function executed by
                                the timer manager (hence by the scheduler).
    @return     If success, a osal_res_OK. A osal_res_NOK_nullpointer value upon NULL pointer, a osal_res_NOK_timeout if timeout expired
                without having access to the messagequeue (even if called by an ISR). 
**/
extern osal_result_t osal_messagequeue_put(osal_messagequeue_t *mq, osal_message_t msg, osal_reltime_t tout, osal_caller_t caller);


/** @fn         extern osal_result_t osal_messagequeue_delete(osal_messagequeue_t *mq)
    @brief      The messagequeue is deleted. 
    @return     A value of osal_res_OK upon successful release, osal_res_NOK_generic upon generic failure, 
                osal_res_NOK_nullpointer if mutex is NULL, or osal_res_NOK_generic if called from an ISR.
**/
extern osal_result_t osal_messagequeue_delete(osal_messagequeue_t *mq);

/* @}            
    end of group osal_messagequeue  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

