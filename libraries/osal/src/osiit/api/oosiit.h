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
#ifndef _OOSIIT_H_
#define _OOSIIT_H_


#ifdef __cplusplus
extern "C" {
#endif


/** @file       oosiit.h
	@brief      This header file implements public interface to the IIT extension to open source CMSIS-RTOS RTX kernel
	@author     marco.accame@iit.it
	@date       07/20/2012
**/

/* @defgroup oosiit IIT extension to open source CMSIS-RTOS RTX kernel
    The IIT extension to the open source CMSIS-RTOS RTX kernel allows the support to time with 32 bits range, event flags 
    extended to 32 bits, plus advanced timers with individual callback execution and reload capabilities. It also allows
    using two memory modes: static and dynamic. In the former, the memory of every resource that will be used by the oosiit 
    is allocated at startup. In dynamic mode, the memory is allocated only when it is effectively required.    
   
    @{		
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "stdint.h"


// - public #define  --------------------------------------------------------------------------------------------------

#define OOSIIT_MAXWAIT      0xfffffffe
#define OOSIIT_NOTIMEOUT    0xffffffff
#define OOSIIT_ASAPTIME     0xffffffffffffffff


// - declaration of public user-defined types ------------------------------------------------------------------------- 

/** @typedef    typedef enum oosiit_result_t 
    @brief      contains all possible results for the oosiit functions. negative values means error.
 **/ 
typedef enum
{
    oosiit_res_OK       = 0x00,     /**< generic success */
    oosiit_res_NOK      = 0xff,     /**< generic error */
    oosiit_res_TMO      = 0x01,     /**< the function returned for a timeout */
    oosiit_res_EVT      = 0x02,     /**< the function successfully received an event before the timeout */
    oosiit_res_SEM      = 0x03,     /**< the function has successfully acquired a semaphore, but waited for some time no longer than the timeout */  
    oosiit_res_MBX      = 0x04,     /**< the function has successfully retrieved a mailbox, but waited for some time no longer than the timeout */
    oosiit_res_MUT      = 0x05      /**< the function has successfully acquired a mutex, but waited for some time no longer than the timeout */
} oosiit_result_t;

//#warning WIP-> think about fixing result_t so that nok are negative (nok and tmo) and ok are positive or zero.

/** @typedef    typedef uint32_t oosiit_taskid_t 
    @brief      contains the identifier of a oosiit task.
 **/ 
//typedef uint32_t oosiit_taskid_t; 

typedef void* oosiit_tskptr_t;


/** @typedef    typedef void* oosiit_objptr_t 
    @brief      contains the handle to oosiit entities.
 **/ 
typedef void* oosiit_objptr_t;


/** @typedef    typedef enum oosiit_memorymode_t 
    @brief      contains modes for memory allocation.
 **/ 
typedef enum
{
    oosiit_memmode_static       = 0,
    oosiit_memmode_dynamic      = 1  
} oosiit_memorymode_t;


/** @typedef    typedef struct oosiit_cfg_t
    @brief      keeps configuration of oosiit which can be loaded in runtime.
 **/
typedef struct
{  
    uint32_t        cpufreq;                        /**< the clock speed of the CPU in Hz. */                                
    uint32_t        ticktime;                       /**< the tick time of the oosiit in micro-seconds. */   
    uint8_t         capacityofpostpendcommandfifo;  /**< the capacity of FIFO which contains events to be processed in the postPEND following a call of a oosiit function from inside an ISR */     
    uint8_t         checkstackoverflow;             /**< if 1 stack overflow is verified at every context switch. */  
    uint8_t         memorymode;                     /**< use oosiit_memorymode_t. if oosiit_memmode_static the ram used by oosiit object is preallocated. else, it is allcated in runtime */
    uint8_t         roundrobinenabled;              /**< if 1 it allows round-robin between the tasks of same priority. */                                                 
    uint32_t        roundrobinnumberofticks;        /**< the timeslice assigned to each round-robin task expressed in number of ticks */                             

    // the following are not relevant if memoryallocation is oosiit_memalloc_atruntime
    uint8_t         maxnumofusertasks;              /**< for oosiit_memmode_static case only: max number of user tasks (init and idle task are not counted as they are system tasks) */
    uint8_t         numAdvTimer;                    /**< for oosiit_memmode_static case only: max number of advanced timers */                            
    uint8_t         numMutex;                       /**< for oosiit_memmode_static case only: max number of mutexes */                                              
    uint8_t         numSemaphore;                   /**< for oosiit_memmode_static case only: max number of semaphores */                           
    uint8_t         numMessageBox;                  /**< for oosiit_memmode_static case only: max number of message boxes */                      
    uint16_t        numMessageBoxElements;          /**< for oosiit_memmode_static case only: max number of messages in all the message boxes */                  
    uint32_t        sizeof64alignedStack;           /**< for oosiit_memmode_static case only: size of internally allocated memory for the stack of tasks */                    
} oosiit_cfg_t;


/** @typedef    typedef enum oosiit_evt_wait_mode_t 
    @brief      contains all possible wait modes for an event
 **/ 
typedef enum
{
    oosiit_evt_wait_mode_any        = 0,
    oosiit_evt_wait_mode_all        = 1
} oosiit_evt_wait_mode_t;


/** @typedef    typedef struct oosiit_advtmr_timing_t
    @brief      keeps timing configuration of advanced timers. time is in in system ticks.
 **/
typedef struct
{
    uint64_t        startat;
    uint32_t        firstshot;
    uint32_t        othershots;
} oosiit_advtmr_timing_t;


/** @typedef    typedef struct oosiit_advtmr_action_t
    @brief      keeps the action executed by the advanced timer.
 **/
typedef struct
{
    void(*cbk)(void* tmr, void* par);
    void *par;
} oosiit_advtmr_action_t;


typedef void (*oosiit_task_fn_t)(void *);

/** @typedef    typedef struct oosiit_task_properties_t
    @brief      keeps properties used to launch a task.
 **/
typedef struct
{
    oosiit_task_fn_t    function;       /**< the function executed by the task. typically is a forever loop. */
    void*               param;          /**< the param of the function executed by the task.  */
    uint8_t             priority;       /**< the priority of the task. */
    uint16_t            stacksize;      /**< the stack size */
    uint64_t*           stackdata;      /**< the stack used by the task */  
    void*               extdata;        /**< external data which may be useful link to the task */
} oosiit_task_properties_t;


/** @typedef    typedef enum oosiit_error_code_t 
    @brief      contains all possible types of error managed by oosiit.
 **/ 
typedef enum
{
    oosiit_error_unknown                = 0,        /**< nasty one */
    oosiit_error_stackoverflow          = 1,        /**< a task has used all its stack */
    oosiit_error_isrfifooverflow        = 2,        /**< too many calls from an ISR */
    oosiit_error_mbxoverflow            = 3,        /**< .... */
    oosiit_error_internal_stdlibspace   = 10,       /**< there is no stdlib space available */
    oosiit_error_internal_sysmutex      = 11,       /**< system mutexes are not enough */
    oosiit_error_memory_allocation      = 12,       /**< there is not heap anymore */
    oosiit_error_memory_preallocated    = 13,       /**< there is not preallocated memory anymore */
    oosiit_error_invalid_tskptr         = 14,       /**< detected the use of an incorrect oosiit_tskptr_t */
    oosiit_error_invalid_objptr         = 15,       /**< detected the use of an incorrect oosiit_objptr_t */
    oosiit_error_invalid_param          = 16,       /**< detected the use of an incorrect parameter */
    oosiit_error_invalid_call           = 17        /**< detected the use of an invalid internal call */
} oosiit_error_code_t;


    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------

extern const uint32_t oosiit_maxwait;   // = OOSIIT_MAXWAIT
extern const uint32_t oosiit_notimeout; // = OOSIIT_NOTIMEOUT
extern const uint64_t oosiit_asaptime;  // = OOSIIT_ASAPTIME


// - declaration of extern public functions ---------------------------------------------------------------------------

/** @fn         extern void* oosiit_ext_calloc(uint32_t s, uint32_t n)
    @brief      memory allocator such as the normal calloc(). It must be externally defined for use in 
                oosiit_memory_new()
 **/ 
extern void* oosiit_ext_calloc(uint32_t s, uint32_t n);

/** @fn         extern void* oosiit_ext_realloc(void* m, uint32_t s)
    @brief      memory reallocator such as the normal realloc(). It must be externally defined for use in 
                oosiit_memory_realloc()
 **/ 
extern void* oosiit_ext_realloc(void* m, uint32_t s);

/** @fn         extern void oosiit_ext_free(void* m)
    @brief      memory deallocator such as the normal free(). It must be externally defined for use in 
                oosiit_memory_del()
 **/ 
extern void oosiit_ext_free(void* m);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// memory functions

/** @fn         extern void* oosiit_memory_new(uint32_t size)
    @brief      Thread safe memory allocator. 
    @param      size            the size of the requested memory in bytes.
    @return     if successful a proper 8-aligned memory pointer, otherwise it returns NULL.
    @warning    needs the definition of an external oosiit_ext_calloc() function.
 **/ 
extern void* oosiit_memory_new(uint32_t size);


/** @fn         extern void* oosiit_memory_realloc(void* m, uint32_t size)
    @brief      Thread safe memory reallocator. 
    @param      m               the memory to reallocate. if NULL, then the function behaves as oosiit_memory_new().
    @param      size            the size of the requested memory in bytes.
    @return     if successful a proper 8-aligned memory pointer, otherwise it returns NULL.
    @warning    needs the definition of an external oosiit_ext_realloc() function.
 **/ 
extern void* oosiit_memory_realloc(void* m, uint32_t size);


/** @fn         extern oosiit_result_t oosiit_memory_del(void* mem)
    @brief      Thread-safe memory free. 
    @param      mem             the pointer to the memory to be de-allocated.
    @return     if successful oosiit_res_OK, otherwise it returns oosiit_res_NOK (for instance if called with NULL pointer)
    @warning    needs the definition of an external oosiit_ext_free() function.
 **/ 
extern oosiit_result_t oosiit_memory_del(void* mem);


/** @fn         extern void oosiit_memory_getsize(const oosiit_params_cfg_t *cfg, uint16_t *num04aligned, uint16_t *num08aligned)
    @brief      Gives back the size in bytes of the memory required by the oosiit. 
    @param      cfg             A pointer to a configuration. If NULL, return values are non-valid (both zero).
    @param      size04aligned   The number of bytes of four-aligned required memory.
    @param      size08aligned   The number of bytes of eigth-aligned required memory.
    @return     if successful oosiit_res_OK, otherwise it returns oosiit_res_NOK (for instance if called with incorrect parameters)
 **/ 
extern oosiit_result_t oosiit_memory_getsize(const oosiit_cfg_t *cfg, uint16_t *size04aligned, uint16_t *size08aligned);


/** @fn         extern oosiit_result_t oosiit_memory_set(const oosiit_cfg_t *cfg, uint32_t *data04aligned, uint64_t *data08aligned)
    @brief      Loads to the internals of oosiit the references to the configuration and of required memory. 
                If any is NULL, any successive attempt to run the oosiit will fail due to lack of memory.
    @param      cfg             A pointer to the wanted configuration.
    @param      data04aligned   The four-aligned required memory.
    @param      data08aligned   The eigth-aligned required memory.
    @return     if succesuful oosiit_res_OK, otherwise it returns oosiit_res_NOK (for instance if called with incorrect parameters)
 **/ 
extern oosiit_result_t oosiit_memory_load(const oosiit_cfg_t *cfg, uint32_t *data04aligned, uint64_t *data08aligned);


/** @fn         extern uint64_t* oosiit_memory_getstack(uint16_t bytes)
    @brief      Gets @e bytes of memory to be used as stack for a task. If @e bytes is not a multiple
                of eight the memory returned will be the smallest multiple of eight bigger than @e bytes.   
    @param      bytes        The size in bytes. It must be a multiple of eight.
    @return     Not NULL pointer to memory, NULL if no memory is available.
 **/ 
extern uint64_t* oosiit_memory_getstack(uint16_t bytes);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// system functions


/** @fn         extern oosiit_result_t oosiit_sys_start(oosiit_task_properties_t* tskinit, oosiit_task_properties_t* tskidle)
    @brief      starts the system and executes the init and the idle tasks as defined by the function parameters. 
                the init task is responsible to start any other task and then, either (a) delete itself and return control to the scheduler,
                or (b) enter a forever loop possibly adjusting its priority and / or blocking itself by waiting a message / event / timeout / trigger of
                a period. 
                the idle task shall always have zero priority and executes when no other task is scheduled. 
    @param      tskinit             descriptor of the init task.
    @param      tskidle             descriptor of the idle task.
    @return     if succesuful it never returns, otherwise it returns oosiit_res_NOK (for instance if called from within an ISR or if parameters are incorrect)
 **/ 
extern oosiit_result_t oosiit_sys_start(oosiit_task_properties_t* tskinit, oosiit_task_properties_t* tskidle);


/** @fn         extern uint32_t oosiit_sys_timeofnextevent(void)
    @brief      tells how long to next event (delay timeout or timer expiry). 
    @return     the number of ticks to next event (0 if called from an ISR) 
 **/ 
extern uint32_t oosiit_sys_timeofnextevent(void); 


/** @fn         extern oosiit_result_t oosiit_sys_suspend(uint32_t *timeofnextevent)
    @brief      suspends scheduling. 
    @param      timeofnextevent     if non-NULL it keeps the time in system ticks from the moment of this call to the next expected system event
    @return     if succesuful oosiit_res_OK, otherwise it returns oosiit_res_NOK (for instance if called from within an ISR)
 **/ 
extern oosiit_result_t oosiit_sys_suspend(uint32_t *timeofnextevent); 


/** @fn         extern oosiit_result_t oosiit_sys_resume(uint32_t timeofsuspension)
    @brief      resume scheduling and changes system time and all events (delays and timers). 
    @param      timeofsuspension    time spent from suspension (in system ticks)
    @return     if succesuful oosiit_res_OK, otherwise it returns oosiit_res_NOK (for instance if called from within an ISR)
 **/
extern oosiit_result_t oosiit_sys_resume(uint32_t timeofsuspension);


/** @fn         extern void oosiit_sys_error(oosiit_error_code_t errorcode)
    @brief      called in case of errors. the user can redefine it as it is internally defined as weak.
    @param      errorcode       tells what happened.    
 **/ 
extern void oosiit_sys_error(oosiit_error_code_t errorcode);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// task functions

 
/** @fn         extern oosiit_tskptr_t oosiit_tsk_create(oosiit_task_properties_t* tskprop)
    @brief      Creates and starts a task.
    @param      tskprop     the properties that a task must have
    @return     The descriptor of the task or NULL upon failure. 
    @warning    The function cannot be called from within an ISR. In oosiit_memmode_static memory mode this operation consumes one task 
                control block and thus  reduces the total amount of still available user tasks specified in configuration loaded with 
                oosiit_memory_load()
 **/ 
extern oosiit_tskptr_t oosiit_tsk_create(oosiit_task_properties_t* tskprop);


/** @fn         extern oosiit_result_t oosiit_tsk_delete(oosiit_tskptr_t tp)
    @brief      deletes a task and free one task control block.
    @param      tp       the descriptor of the task
    @return     if succesuful oosiit_res_OK, otherwise it returns oosiit_res_NOK. 
    @warning    The function cannot be called from within an ISR. The stack of the function is NOT automatically released
                and if needed must be managed by the user. After this operation, instead, the number of available user tasks
                is incread by one.
 **/
extern oosiit_result_t oosiit_tsk_delete(oosiit_tskptr_t tp);


/** @fn         extern oosiit_tskptr_t oosiit_tsk_self(void)
    @brief      retrieves the descriptor of the calling task.
    @return     The descriptor of the task or NULL upon failure. 
    @warning    The function CAN be called from within an ISR.
 **/ 
extern oosiit_tskptr_t oosiit_tsk_self(void);


/** @fn         extern oosiit_result_t oosiit_tsk_setprio(oosiit_tskptr_t tp, uint8_t tskpriority)
    @brief      changes the priority of the task specified.
    @param      tp       the descriptor of the task
    @param      tskpriority     the new priority
    @return     if succesuful oosiit_res_OK, otherwise it returns oosiit_res_NOK. 
    @warning    The function cannot be called from within an ISR.
 **/
extern oosiit_result_t oosiit_tsk_setprio(oosiit_tskptr_t tp, uint8_t tskpriority);


/** @fn         extern oosiit_result_t oosiit_tsk_pass(void)
    @brief      used in round-robin to force a context switch and thus passing execution to the next task of same priority
    @return     if successful oosiit_res_OK, otherwise it returns oosiit_res_NOK. 
    @warning    The function cannot be called from within an ISR.
 **/
extern oosiit_result_t oosiit_tsk_pass(void);


/** @fn         extern void* oosiit_tsk_get_perthread_libspace(oosiit_tskptr_t tp)
    @brief      returns a pointer to 96 bytes used to store data that is local to this thread.
    @param      tp       the descriptor of the task
    @return     if successful the pointer, otherwise it returns NULL. 
 **/
extern void* oosiit_tsk_get_perthread_libspace(oosiit_tskptr_t tp);


/** @fn         extern oosiit_result_t oosiit_tsk_set_extdata(oosiit_tskptr_t tp, void* extdata)
    @brief      allows associating some external data to the task.
    @param      tp       the descriptor of the task
    @param      extdata  the pointer to the data to be associated to the task
    @return     if successful oosiit_res_OK, otherwise it returns oosiit_res_NOK 
 **/
extern oosiit_result_t oosiit_tsk_set_extdata(oosiit_tskptr_t tp, void* extdata);


/** @fn         extern void* oosiit_tsk_get_extdata(oosiit_tskptr_t tp)
    @brief      allows retrieving the external data associated to the task.
    @param      tp       the descriptor of the task
    @param      extdata  the pointer to the data to be associated to the task
    @return     if successful the pointer, otherwise it returns NULL. 
 **/
extern void* oosiit_tsk_get_extdata(oosiit_tskptr_t tp);


/** @fn         extern uint8_t oosiit_tsk_get_id(oosiit_tskptr_t tp)
    @brief      allows retrieving the internal id of the task.
    @param      tp       the descriptor of the task
    @return     if successful the id, otherwise it returns 0. 
 **/
extern uint8_t oosiit_tsk_get_id(oosiit_tskptr_t tp);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// time functions


/** @fn         extern oosiit_result_t oosiit_time_set(uint64_t target)
    @brief      Changes by brute-force the time since last bootstrap in system ticks.
    @param      target          The target time in sys ticks.
 **/
extern oosiit_result_t oosiit_time_set(uint64_t target);


/** @fn         extern uint64_t oosiit_time_get(void)
    @brief      Returns the time since last bootstrap in system ticks.
    @return     System ticks since bootstrap.
 **/
extern uint64_t oosiit_time_get(void);


/** @fn         extern uint64_t oosiit_microtime_get(void)
    @brief      Returns the number of microseconds elapsed since last bootstrap.
    @return     microseconds since bootstrap.
 **/
extern uint64_t oosiit_microtime_get(void);


/** @fn         extern uint64_t oosiit_nanotime_get(void)
    @brief      Gets the number of nanoseconds elapsed since last bootstrap
    @return     nanoseconds since bootstrap.
 **/
extern uint64_t oosiit_nanotime_get(void);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// delay functions


/** @fn         extern void oosiit_dly_wait(uint32_t delay)
    @brief      Pauses the calling task for a specified interval
    @param      delay      the delay in system ticks.
 **/
extern void oosiit_dly_wait(uint32_t delay);


/** @fn         extern void oosiit_itv_set(uint32_t period)
    @brief      Sets up the calling task for periodic wake-up 
    @param      period      the period in system ticks.
 **/
extern void oosiit_itv_set(uint32_t period);


/** @fn         extern void oosiit_itv_wait(void)
    @brief      Waits for the period set with oosiit_itv_set() to expire.
 **/
extern void oosiit_itv_wait(void);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mailbox functions


/** @fn         extern oosiit_objptr_t oosiit_mbx_create(uint16_t numofmessages)
    @brief      Retrieves a pointer to memory which contains a mailbox with @e numofmessages messages.
                It also initialialises the memory for its use with other mailbox functions
    @param      numofmessages   The number of messages the mailbox can contain.
    @return     the handle to the mailbox or NULL upon insuccess.
 **/
extern oosiit_objptr_t oosiit_mbx_create(uint16_t numofmessages); 


/** @fn         extern oosiit_result_t oosiit_mbx_delete(oosiit_objptr_t mailbox)
    @brief      It deletes a mailbox. 
    @param      mailbox         The handler to the mailbox
    @return     oosiit_res_OK upon success, oosiit_res_NOK upon failure (e.g., if called from an ISR)

 **/ 
extern oosiit_result_t oosiit_mbx_delete(oosiit_objptr_t mailbox);


/** @fn         extern oosiit_result_t oosiit_mbx_retrieve(oosiit_objptr_t mailbox, void **message, uint32_t timeout)
    @brief      Gets a pointer to a message from the mailbox. The function can be called from inside an ISR. In such a case it is used as if timeout were zero.
    @param      mailbox         the mailbox.
    @param      message         location where to store the message pointer
    @param      timeout         the timeout.
    @return     oosiit_res_OK upon immediate success, oosiit_res_MBX upon success after some wait time, oosiit_res_TMO if timeout failure
                (or if there is no msg when called by an ISR), oosiit_res_NOK if mailbox is invalid
 **/
extern oosiit_result_t oosiit_mbx_retrieve(oosiit_objptr_t mailbox, void** message, uint32_t timeout); 


/** @fn         extern oosiit_result_t oosiit_mbx_send (oosiit_objptr_t mailbox, void* message, uint32_t timeout)
    @brief      puts the pointer to a message inside the mailbox.
    @param      mailbox         the mailbox.                                                 
    @param      message         the message
    @param      timeout         the timeout in system ticks
    @return     oosiit_res_OK upon success, oosiit_res_TMO if timeout failure 
 **/ 
extern oosiit_result_t oosiit_mbx_send(oosiit_objptr_t mailbox, void* message, uint32_t timeout); // osiit_



/** @fn         extern uint16_t oosiit_mbx_available(oosiit_objptr_t mailbox)
    @brief      Gives the number of messages which can still be put inside the mailbox
                It can be called also by an ISR.
    @param      mailbox         The mailbox
    @return     The number of messages which can be put inside the mailbox. Zero if mailbox is full (or invalid).
 **/
extern uint16_t oosiit_mbx_available(oosiit_objptr_t mailbox);


/** @fn         extern uint16_t oosiit_mbx_used(oosiit_objptr_t mailbox)
    @brief      Gives the number of available messages stored inside the mailbox
                It can be called also by an ISR.
    @param      mailbox         The mailbox
    @return     The number of messages stored inside the mailbox. Zero if mailbox is empty (or invalid).
 **/
extern uint16_t oosiit_mbx_used(oosiit_objptr_t mailbox); 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// event flag functions


/** @fn         extern oosiit_result_t oosiit_evt_set(uint32_t flags, oosiit_tskptr_t tp)
    @brief      It sends the event flags specified by @e flags to the task @e taskid. The function can be called also by an ISR.
    @param      flags           the mask of the event flags to be sent to the task.
    @param      tskid           the task.
    @return     oosiit_res_NOK if the specified task is invalid, oosiit_res_OK in all other cases.
 **/ 
extern oosiit_result_t oosiit_evt_set(uint32_t flags, oosiit_tskptr_t tp);


/** @fn         extern oosiit_result_t oosiit_evt_wait(uint32_t waitflags,  uint32_t timeout, oosiit_evt_wait_mode_t waitmode)
    @brief      It forces the caller task to wait for the event flags specified by @e waitflags in the mode specified by @e waitmode
                with timeout specified by @e timeout.
    @param      waitflags       the mask of the event flags to be waited.
    @param      timeout         the timeout in system ticks.
    @param      waitmode        if oosiit_evt_wait_mode_any, the function waits until any of the flags becomes available; 
                                else if oosiit_evt_wait_mode_all, it waits until all the flags become available
    @return     oosiit_res_TMO if the function exits for timeout. oosiit_res_EVT if the condition specified by @e waitflag and @e waitmode
                is satisfied within the specified timeout. oosiit_res_NOK if the function is called from inside an ISR.
    @warning    the internal flags are not reset. the caller must retrieve them with oosiit_evt_get() and reset them with oosiit_evt_clr().
 **/ 
extern oosiit_result_t oosiit_evt_wait(uint32_t waitflags,  uint32_t timeout, oosiit_evt_wait_mode_t waitmode); 


/** @fn         extern uint32_t oosiit_evt_get(void)
    @brief      It retrieves the mask of event flags received by the calling task.
    @return     the mask of received event flags. 0 if called by an ISR.
 **/ 
extern uint32_t oosiit_evt_get(void);


/** @fn         extern oosiit_result_t oosiit_evt_clr(uint32_t flags)
    @brief      clears in the calling task all the events specified by @e flags . 
    @param      flags           the flag mask
    @return     oosiit_res_OK or oosiit_res_NOK if called from inside an ISR.
 **/ 
extern oosiit_result_t oosiit_evt_clr(uint32_t flags); 


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// semaphore functions


/** @fn         extern oosiit_objptr_t oosiit_sem_create(uint8_t maxtokens, uint8_t ntokens)
    @brief      It creates and initialises a semaphore to contain @e maxtokens and have its initial value set to @e ntokens. 
    @param      maxtokens       Maximum number of tokens.
    @param      ntokens         Number of tokens (must be lower or equal than @e maxtokens, otherwise it is clipped to its value)
    @return     the semaphore or NULL if no memory is available.

 **/ 
extern oosiit_objptr_t oosiit_sem_create(uint8_t maxtokens, uint8_t ntokens);


/** @fn         extern oosiit_result_t oosiit_sem_delete(oosiit_objptr_t sem)
    @brief      It deletes a semaphore. 
    @param      sem             The handler to the semaphore
    @return     oosiit_res_OK upon success, oosiit_res_NOK upon failure (e.g., if called from an ISR)

 **/ 
extern oosiit_result_t oosiit_sem_delete(oosiit_objptr_t sem);


/** @fn         extern oosiit_result_t oosiit_sem_set(oosiit_objptr_t sem, uint8_t ntokens)
    @brief      It sets the number of tokens to @e ntokens. The function wakes up a task that may wait if the
                number of tokens @e notokens is one or higher.
    @param      sem             As with the os_ version..
    @param      ntokens         Number of tokens.
    @warning    the semaphore must have been previously initialised with oosiit_sem_init().
    @return     oosiit_res_NOK if @ntokens is higher than the number of tokens that the semaphore can contain, oosiit_res_NOK
                if the semaphore is invalid or the function is called from inside an ISR. oosiit_res_OK otherwise
 **/ 
extern oosiit_result_t oosiit_sem_set(oosiit_objptr_t sem, uint8_t ntokens);


/** @fn         extern oosiit_result_t oosiit_sem_send(oosiit_objptr_t sem)
    @brief      It adds a token to the semaphore, but it fails if the number of tokens inside
                has reached its maximum number
    @param      sem             The handler to the semaphore
    @return     oosiit_res_NOK if the number of tokens are already the maximum number, or oosiit_res_OK otherwise
 **/ 
extern oosiit_result_t oosiit_sem_send(oosiit_objptr_t sem);


/** @fn         extern oosiit_result_t oosiit_sem_wait(oosiit_objptr_t sem, uint32_t timeout)
    @brief      It removes a token from the semaphore with a given timeout.
    @param      sem             The handler to the semaphore
    @param      timeout         maximum wait time in system ticks.
    @return     oosiit_res_OK upon immediate success, oosiit_res_SEM upon success after some wait time, oosiit_res_TMO if timeout failure,
                finally oosiit_res_NOK if called from an ISR or if the semaphore is invalid
 **/ 
extern oosiit_result_t oosiit_sem_wait(oosiit_objptr_t sem, uint32_t timeout);



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// mutex functions


/** @fn         extern oosiit_objptr_t oosiit_mut_create(void)
    @brief      It creates and initialises a mutex.
    @return     the mutex or NULL if no memory is available.
 **/ 
extern oosiit_objptr_t oosiit_mut_create(void);


/** @fn         extern oosiit_result_t oosiit_mut_delete(oosiit_objptr_t mutex)
    @brief      It deletes a mutex. 
    @param      sem             The handler to the mutex
    @return     oosiit_res_OK upon success, oosiit_res_NOK upon failure (e.g., if called from an ISR)

 **/
extern oosiit_result_t oosiit_mut_delete(oosiit_objptr_t mutex);


/** @fn         extern oosiit_result_t oosiit_mut_wait(oosiit_objptr_t mutex, uint32_t timeout)
    @brief      It takes a mutex with priority inheritance.
    @param      mutex           the handle to the mutex
    @param      timeout         maximum wait time. in system ticks
    @return     oosiit_res_OK upon immediate success, oosiit_res_MUT upon success after some wait time, oosiit_res_TMO if timeout failure. 
                oosiit_res_NOK on invalid mutex or if the function is called from inside an ISR.
 **/
extern oosiit_result_t oosiit_mut_wait(oosiit_objptr_t mutex, uint32_t timeout);


/** @fn         extern oosiit_result_t oosiit_mut_release(oosiit_objptr_t mutex)
    @brief      It releases a mutex.
    @param      mutex           the handle to the mutex
    @return     oosiit_res_OK upon success, oosiit_res_NOK upon error (i.e., the caller is not the same task which called oosiit_mut_wait() or
                there is unbalanced release).
 **/
extern oosiit_result_t oosiit_mut_release(oosiit_objptr_t mutex);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// advanced timer functions


    
/** @fn         extern oosiit_objptr_t oosiit_advtmr_new(void)
    @brief      Creates but NOT starts an advanced timer.
    @return     The handle to the advanced timer or NULL upon failure. The function cannot be called from within an ISR.
 **/
extern oosiit_objptr_t oosiit_advtmr_new(void);


/** @fn         extern oosiit_result_t oosiit_advtmr_start(oosiit_objptr_t timer, oosiit_advtmr_timing_t *timing, oosiit_advtmr_action_t *action)
    @brief      Starts an advanced timer which expires for the first time after timing->firstshot ticks 
                and the other times after timing->othershots ticks. At each expiry it calls the
                user-defined callback function action->cbk(timer, action->arg). If timing->othershots is zero then the timer 
                is one-shot. If timing->firstshot is zero the first shot is not executed.
                Moreover, if timing->startat is equal to ASAPTIME then the start of the timer is relative to the moment it is
                called the oosiit_advtmr_start() function. Otherwise everything is shifter to begin at the absolute time given 
                by timing->startat
                The functions can be called also from within an ISR with a limitation: there can not be two or more calls to the same 
                timer inside any ISR before the post-ISR is processed. In such a case the timer is simply not started.
    @param      timing          keeps the timing information. The values in timing->firstshot and timing->othershots indicates
                                when the expiry shall happen relative to teh value timing->startat.
                                If timing->startat is equal to ASAPTIME, then the timer is relative to the calling time of
                                oosiit_advtmr_start(), otehrwise to teh value in timing->startat.
                                In this latter case, any expiry of a single-shot due in the past relative to current absolute 
                                time is not going to cause any trigger. In case of periodic timer, instead the rsult is that
                                the expiries can happen at given times equispaced by timing->othershots.
    @param      action          Specified the callback function executed at each expiry of the advanced timer and its second
                                argument. As this function is executed directly by the scheduler, some limitations arise: keep 
                                it short and calls only oosiit_isr* or isr* functions.
    @return     oosiit_res_OK if the timer was succesfully started or oosiit_res_NOK otherwise.
    @warning    A call of oosiit_time_set() that changes the current time shall also shifts the timers started with timing->startat
                different from ASAPTIME.
 **/
extern oosiit_result_t oosiit_advtmr_start(oosiit_objptr_t timer, oosiit_advtmr_timing_t *timing, oosiit_advtmr_action_t *action);


/** @fn         extern oosiit_result_t oosiit_advtmr_stop(oosiit_objptr_t timer)
    @brief      Stops an advanced timer. 
                The functions can be called also from within an ISR with a limitation: there can not be two or more calls to the same 
                timer inside any ISR before the post-ISR is processed. In such a case the timer is simply not stopped.
    @param      timer           the advanced timer.
    @return     oosiit_res_OK if the timer was succesfully stopped or oosiit_res_NOK otherwise.
 **/
extern oosiit_result_t oosiit_advtmr_stop(oosiit_objptr_t timer);


/** @fn         extern oosiit_result_t oosiit_advtmr_isactive(oosiit_objptr_t timer)
    @brief      tells if an advanced timer is active or not (not yet started or started as single shot and expired
                or also stopped. The functions can be called also from within an ISR.
    @param      timer           the advanced timer.
    @return     oosiit_res_OK if the timer is active or oosiit_res_NOK otherwise.
 **/
extern oosiit_result_t oosiit_advtmr_isactive(oosiit_objptr_t timer);


/** @fn         extern oosiit_result_t oosiit_advtmr_delete(oosiit_objptr_t timer)
    @brief      Deletes an advanced timer.
                The functions can be called also from within an ISR with a limitation: there can not be two or more calls to the same 
                timer inside any ISR before the post-ISR is processed. In such a case the timer is simply not deleted.
    @param      timer           the advanced timer.
    @return     Always oosiit_res_OK 
 **/
extern oosiit_result_t oosiit_advtmr_delete(oosiit_objptr_t timer);



/* @}            
    end of group oosiit  
 **/
 
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

