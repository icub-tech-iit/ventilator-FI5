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
#ifndef _OSAL_TIMER_H_
#define _OSAL_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_timer.h
    @brief      This header file implements public interface to the os abstraction layer, timer
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_timer OS abstraction layer: timer
    crcrcrm.
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


 
// - declaration of public user-defined types ------------------------------------------------------------------------- 

/** @typedef    typedef struct osal_timer_opaque_t          osal_timer_t
    @brief      osal_timer_t is the opaque definition of a timer. It has usec resolution, it lasts at max 4294 seconds, 
                it has execution of callback at expiry, and also auto retrigger. 
 **/
typedef struct osal_timer_opaque_t          osal_timer_t;           


/** @typedef    typedef enum osal_timer_mode_t 
    @brief      osal_timer_mode_t contains all possible modes with which an osal timer can be used
 **/  
typedef enum 
{
    osal_tmrmodeONESHOT = 0,        /** < the timer expiries only once */          
    osal_tmrmodeFOREVER = 1         /** < the timer expiries forever */ 
} osal_timer_mode_t;


/** @typedef    typedef struct osal_timer_timing_t
    @brief      osal_timer_timing_t contains the timing of the osal timer.
 **/
typedef struct
{
    osal_abstime_t          startat;    /**< the timer starts to count down at this absolute time */
    osal_reltime_t          count;      /**< the count down value */
    osal_timer_mode_t       mode;       /**< tells if the timer is one-shot or in forever mode */
} osal_timer_timing_t;     


/** @typedef    typedef struct osal_timer_onexpiry_t
    @brief      osal_timer_onexpiry_t contains the action to be performed at expiry of the timer.
 **/
typedef struct
{
    void(*cbk)(osal_timer_t* tmr, void* par);   /**< first argument is the firing timer, second is par of this struct */
    void *par;                                  /**< the second argument of function cbk() of this struct */
} osal_timer_onexpiry_t;     

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section
                 


// - declaration of extern public functions ---------------------------------------------------------------------------

/** @fn         extern osal_timer_t * osal_timer_new(void)
    @brief      Creates a timer which will allow the task timermanager to execute a callback function at its priority
                osal_prio_systsk_timerman_*.  In osal_rtostype_iitmod_rtxarm and osal_rtostype_oosiit the timer manager 
                is directly run by the scheduler, hence there is maximum priority.
    @return     A not NULL handle to the timer or NULL upon failure.
**/
extern osal_timer_t * osal_timer_new(void);


/** @fn         extern osal_result_t osal_timer_start(osal_timer_t *timer, osal_timer_timing_t *timing, 
                                                      osal_timer_onexpiry_t *onexpiry, osal_caller_t caller)
    @brief      Starts a timer with expiry time specified by @e timing, actions to be done on expiry defined by
                @e onexpiry. 
                The callback function should thus be: not blocking, fast, and use a small amount of stack.
                In its inside it can be used only not-blocking osal_* functions and only with osal_callerISR mode.
                For more elaborate operations, the callback can send a message or an event to a working task
                which will execute an action in task mode.
    @param      timer           The timer.
    @param      timing          The timing modality.
    @param      onexpiry        The action to be executed at expiry. The callback onexpiry->cbk(timer, onexpiry->arg) 
                                has the tiemr itself as first argument and a user provided parameter as second argument.
                                It is executed directly by the timer manager (the scheduler in iitmod_rtxarm), hence it 
    @param      caller          Specifies the entity which calls the function osal_timer_new().
    @return     The value osal_res_NOK_nullpointer if any parameter is NULL, osal_res_NOK_generic if the timer is not valid,
                osal_res_OK if the timer succesfully starts.
**/
extern osal_result_t osal_timer_start(osal_timer_t *timer, osal_timer_timing_t *timing, osal_timer_onexpiry_t *onexpiry, osal_caller_t caller);


/** @fn         extern osal_result_t osal_timer_stop(osal_timer_t *timer, osal_caller_t caller) 
    @brief      Stops a timer
    @param      timer           The timer.
    @param      caller          Specifies the entity which calls the function osal_timer_stop().
    @return     The value osal_res_NOK_nullpointer if any parameter is NULL, osal_res_NOK_generic if the timer is not valid,
                osal_res_OK if the timer succesfully stops.
**/
extern osal_result_t osal_timer_stop(osal_timer_t *timer, osal_caller_t caller);


/** @fn         extern osal_result_t osal_timer_isactive(osal_timer_t *timer, osal_caller_t caller) 
    @brief      Tells if a timer is active, that si to say if it is going to expire.
    @param      timer           The timer.
    @param      caller          Specifies the entity which calls the function osal_timer_isactive().
    @return     The value osal_res_NOK_nullpointer if any parameter is NULL, osal_res_NOK_generic if the timer is not valid
                or not yet started or started but already stopped or if it was one-shot and the shot has already happened,
                osal_res_OK if the timer was started as one shot and has to expire yet or created as periodic an not stopped yet.
**/
extern osal_result_t osal_timer_isactive(osal_timer_t *timer, osal_caller_t caller);


/** @fn         extern osal_result_t osal_timer_delete(osal_timer_t *timer)
    @brief      Deletes a timer.
    @param      timer           A valid handle to a timer.
    @return     Upon correct deletion: osal_res_OK. Otherwise: osal_res_NOK_nullpointer or osal_res_NOK_generic.
**/
extern osal_result_t osal_timer_delete(osal_timer_t *timer);


/* @}            
    end of group osal_timer  
 **/
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

