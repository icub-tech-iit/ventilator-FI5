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
#ifndef _OSAL_TASK_H_
#define _OSAL_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_task.h
    @brief      This header file implements public interface to the os abstraction layer: task
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_task OS abstraction layer: task
    xecxwexcwe.
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


 
// - declaration of public user-defined types ------------------------------------------------------------------------- 
                  
/** @typedef    typedef struct osal_task_opaque_t osal_task_t
    @brief      osal_task_t is the opaque definition of a task. It is executed in fully preemptive manner, w/ 254 
                different priorities and also round robin. 
 **/
typedef struct osal_task_opaque_t           osal_task_t;   


/** @typedef    typedef uint8_t osal_task_id_t
    @brief      osal_task_id_t contains the unique ID number of a task. 
 **/
typedef uint8_t osal_task_id_t;  


/** @typedef    typedef struct osal_task_properties_t
    @brief      Contains the properties used to launch a task.
 **/
typedef struct
{
    void(*function)(void* param);       /**< the function executed by the task. typically it is a forever loop. */
    void*               param;          /**< the param of the function executed by the task.  */
    uint8_t             priority;       /**< the priority of the task. Valid values are between osal_prio_usrtsk_min (1) and osal_prio_usrtsk_max (253) */
    uint16_t            stacksize;      /**< the stack size */
} osal_task_properties_t;

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section
                 


// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern osal_task_t * osal_task_new(void (*run_fn)(void*), void *run_arg, uint8_t prio, uint16_t stksize)
    @brief      Starts a task with a given execution function and argument running at a given priority and with a stack
                made available by the osal.
    @param      run_fn          The running function. It should contains a forever loop in its inside with some sort
                                of blocking on wait events or messages or timeout expiry. It must not be NULL unless 
                                the creation of teh task fails.
    @param      arg_fn          The argument passed to the running function. It can be used to communicate a data 
                                structure to the task.
    @param      prio            The running priority. Valid values are between osal_prio_usrtsk_min (1) and 
                                osal_prio_usrtsk_max (253). Values outside those will make the function fail.
    @param      stksize         The size of the requested stack. If zero, the creation of the task will fail.
    @return     A not NULL handle to the task or NULL upon failure.
**/
extern osal_task_t * osal_task_new(void (*run_fn)(void*), void *run_arg, uint8_t prio, uint16_t stksize);


/** @fn         extern osal_task_t * osal_task_new1(osal_task_properties_t *tskprop)
    @brief      Starts a task with a given execution function and argument running at a given priority and with a stack
                made available by the osal.
    @param      tskprop         A pointer to a struct containing the necessary params.
    @return     A not NULL handle to the task or NULL upon failure.
**/
extern osal_task_t * osal_task_new1(osal_task_properties_t *tskprop);


/** @fn         extern osal_task_t * osal_task_get(osal_caller_t caller)
    @brief      Gets the handle of the running task (or interrupted by the ISR or timer manager)
    @param      caller      the caller.
    @return     A not NULL handle to the task.
**/
extern osal_task_t * osal_task_get(osal_caller_t caller);


/** @fn         extern void * osal_task_stack_get(uint16_t *size)
    @brief      Gets the stack of the running task
    @param      size        pointer to stack size
    @return     pointer to the stack.
**/
extern void * osal_task_stack_get(uint16_t *size);


/** @fn         extern void * osal_task_stack_get1(osal_task_t *tsk, uint16_t *size)
    @brief      Gets the stack of the task
    @param      tsk             The handle to the task.
    @param      size        pointer to stack size
    @return     pointer to the stack.
**/
extern void * osal_task_stack_get1(osal_task_t *tsk, uint16_t *size);

/** @fn         extern osal_result_t osal_task_priority_get(osal_task_t *tsk, uint8_t *prio)
    @brief      Retrieves the priority of a task.
    @param      tsk             The handle to the task.
    @param      prio            The returned priority. 
    @return     On success: osal_res_OK, otherwise: osal_res_NOK_nullpointer.
 **/
extern osal_result_t osal_task_priority_get(osal_task_t *tsk, uint8_t *prio);


/** @fn         extern osal_result_t osal_task_priority_set(osal_task_t *tsk, uint8_t *prio)
    @brief      Sets the priority of a task.
    @param      tsk             The handle to the task.
    @param      prio            The target priority. It must be within osal_prio_usrtsk_min and osal_prio_usrtsk_max. 
    @return     On success: osal_res_OK, otherwise: osal_res_NOK_nullpointer or osal_res_NOK_generic 
               (if @e prio is not valid).
 **/
extern osal_result_t osal_task_priority_set(osal_task_t *tsk, uint8_t prio);


/** @fn         extern osal_result_t osal_task_id_get(osal_task_t *tsk, osal_task_id_t *id)
    @brief      Retrieves the unique id of a task.
    @param      tsk             The handle to the task.
    @param      id              The returned id. 
    @return     On success: osal_res_OK, otherwise: osal_res_NOK_nullpointer.
 **/
extern osal_result_t osal_task_id_get(osal_task_t *tsk, osal_task_id_t *id);


/** @fn         extern osal_result_t osal_task_extdata_set(osal_task_t *tsk, void *ext)
    @brief      Assign external data to be associated to the task.
    @param      tsk             The handle to the task.
    @param      ext             The external data associated to the task.
    @return     On success: osal_res_OK, otherwise: osal_res_NOK_nullpointer.
    
 **/
extern osal_result_t osal_task_extdata_set(osal_task_t *tsk, void *ext);


/** @fn         extern void* osal_task_extdata_get(osal_task_t *tsk)
    @brief      Retrieves the external data associated to a task.
    @param      tsk             The handle to the task.
    @return     The external associated to the task.
 **/
extern void* osal_task_extdata_get(osal_task_t *tsk);


/** @fn         extern osal_result_t osal_task_wait(osal_reltime_t time)
    @brief      Puts the calling task on wait for at least @e time microseconds. After that, the task can be scheduled
                by the system and run again if it has highest priority amongst active tasks. If called with @e time = 0
                then this call is equivalent to osal_task_pass().
    @param      time            The waiting time in microseconds. 
    @return     Always osal_res_OK.
**/
extern osal_result_t osal_task_wait(osal_reltime_t time);


/** @fn         extern osal_result_t osal_task_pass(void)
    @brief      Ask the scheduler to give control to the next task with the same priority as the calling one. 
                If no other task of the same priority is ready, then the control is given back to the caller.
    @return     Always osal_res_OK.
**/
extern osal_result_t osal_task_pass(void);


/** @fn         extern osal_result_t osal_task_period_set(osal_reltime_t period)
    @brief      The calling task set-up a periodic wait of @e period usec.
    @param      period          The required period.
    @return     Always osal_res_OK.
    @warning    If we use the osal_rtostype_iitmod_rtxarm, the time is limited to 2147 seconds if tick
                in osal_params_cfg_t is 1 usec (sic!), else to osal_timeMAX (4294 seconds).
 **/
extern osal_result_t osal_task_period_set(osal_reltime_t period);


/** @fn         extern osal_result_t osal_task_period_wait(void)
    @brief      The calling task waits until it is waken up every fixed amount of time as defined by 
                osal_task_period_set(). The wake-up time is indendent on the activity time after the sleep.
    @return     Always osal_res_OK.
 **/
extern osal_result_t osal_task_period_wait(void);


/** @fn         extern osal_result_t osal_task_delete(osal_task_t *tsk)
    @brief      Deletes a task and free its resources. If tsk is taken with osal_task_get(), then the task can
                delete itself. In such a case the function does not return and the scheduler internally executes 
                next task.  
    @param      tsk             The handle to the task.
    @return     On success osal_res_OK, osal_res_NOK_nullpointer if tsk is NULL, osal_res_NOK_generic if
                tsk is related to a task already deleted. It does not return if the task calling the function
                wants to delete itself.
 **/
extern osal_result_t osal_task_delete(osal_task_t *tsk);


/* @}            
    end of group osal_task  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

