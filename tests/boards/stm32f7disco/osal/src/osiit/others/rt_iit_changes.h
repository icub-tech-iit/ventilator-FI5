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
#ifndef _RT_IIT_CHANGES_H_
#define _RT_IIT_CHANGES_H_


// this file contains parts of code taken from various files coming from the free distribution of the
// "CMSIS-RTOS RTX Implementation" version 4.20. the code has been modified to add support to 32 bit 
// timing and to dynamic retrieval of memory used by the rtos objects.
// as such, we attach in here the copyrigth notice which applies only to functions with prefix: iitchanged_

/*-----------------------------------------------------------------------------

    License for CMSIS-RTOS RTX Implementation

    Copyright (c) 1999-2009 KEIL, 2009-2011 ARM Germany GmbH
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
     - Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     - Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     - Neither the name of ARM  nor the names of its contributors may be used 
       to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
    
-----------------------------------------------------------------------------*/


/* @file       rt_iit_changes.h
	@brief      This header file implements public interface to the IIT extension for some calls of RTX.
	@author     marco.accame@iit.it
	@date       07/27/2012
    @warning    this file contains parts of code taken from various files coming from the free distribution of the
                "CMSIS-RTOS RTX Implementation" version 4.20. the code has been modified to add support to 32 bit 
                timing and to dynamic retrieval of memory used by the rtos objects. the modification is done in the
                following way:
                - if a function of RTX needs a different behaviour but keeps its declation, we just redefine it in this
                  file. obviously in its original file the funtion was made __weak.
                - if a function of RTX needs also different parameters in its number of type, we just create a new
                  function in this file with name prefixed with "iitchanged_". the old function is left unchanged in 
                  its original file. 
                hence, we attach in here the copyrigth notice of KEIL.
**/

/* @defgroup changes Changes and add-ons for CMSIS-RTOS RTX
    
    @{		
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "stdint.h"

#include "oosiit.h"
#include "oosiit_hid.h"

#if defined(OOSIIT_DBG_ENABLE_ALL)
#define OOSIIT_DBG_ENABLE
#define OOSIIT_DBG_SYSTICK
#define OOSIIT_DBG_SVC
#define OOSIIT_DBG_PENDSV
#endif

#if defined(OOSIIT_DBG_ENABLE)
#include "osal_scope.h"
#endif


// - public #define  --------------------------------------------------------------------------------------------------

#define NOCB            0x00
#define TCB             0x60
#define MCB             0x61
#define SCB             0x62
#define MUCB            0x63
#define HCB             0x64
#define ATCB            0x65 // IIT-EXT: for AdvancedTimers

#define OS_R_OBJDELETED 0x10



#if defined(OOSIIT_DBG_ENABLE)
#define	RT_IIT_SYSCALL_ID_SYSTICK   ev_ID_systick	
#define	RT_IIT_SYSCALL_ID_PENDSV    ev_ID_pendsv
#define	RT_IIT_SYSCALL_ID_SVC       ev_ID_svc
#endif
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 


#if 1

// damn to the lack of include guards in keil files .... i must redefine in here the U64 etc that i need.
typedef char               S8;
typedef unsigned char      U8;
typedef short              S16;
typedef unsigned short     U16;
typedef int                S32;
typedef unsigned int       U32;
typedef long long          S64;
typedef unsigned long long U64;
typedef unsigned char      BIT;
typedef unsigned int       BOOL;
typedef void               (*FUNCP)(void);
typedef U32     OS_TID;
typedef void    *OS_ID;
typedef void*   OS_TPTR;      // points to a struct OS_TCB
typedef U32     OS_RESULT;

typedef struct OS_TCB *P_TCB_Opaque; 

#endif


typedef U32 TIME_t;
typedef U64 WIDETIME_t;
typedef U32 EVENT_t;
#define MSB_TIME_t      0x80000000
#define NOTIMEOUT       0xffffffff



    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------

extern void* rt_iit_memory_new(uint32_t size);
extern void* rt_iit_memory_realloc(void* m, uint32_t size);
extern void rt_iit_memory_del(void* mem);

// - sys routines -
extern void rt_iit_sys_start(oosiit_task_properties_t* inittsk, oosiit_task_properties_t* idletsk);
OS_TPTR rt_iit_tsk_create (FUNCP task, void *taskfnarg, void *taskstackdata, osiit_hid_tsk_create_other_args_t* others);
extern OS_TPTR rt_iit_tsk_self (void);
extern OS_RESULT rt_iit_tsk_prio (OS_TPTR taskp, U8 new_prio);
extern OS_RESULT rt_iit_tsk_delete (OS_TPTR taskp);
extern void* rt_iit_tsk_perthread_libspace_get(OS_TPTR taskp);

extern U32 rt_iit_timeofnextevent(U8 andsuspend);

// - init routines -
extern void rt_iit_dynamic_mode_init(const oosiit_cfg_t *cfg);
extern void rt_iit_params_init(void);
extern U32* rt_iit_libspace_init(void);
extern U32* rt_iit_libmutex_init(U8 *nrmutex);

// - time routines -
extern void rt_iit_time_set(U32 low, U32 high);

// - time management routines -
extern void      iitchanged_rt_dly_wait(TIME_t delay_time);
extern void      iitchanged_rt_itv_set(TIME_t interval_time);
extern void      iitchanged_rt_itv_wait(void);

// - mutex management routines -
extern OS_RESULT iitchanged_rt_mut_wait(OS_ID mutex, TIME_t timeout);

// - event flag management routines -
extern OS_RESULT iitchanged_rt_evt_wait(EVENT_t wait_flags,  TIME_t timeout, BOOL and_wait); 
extern void iitchanged_rt_evt_set (EVENT_t event_flags, OS_TPTR taskp);
extern void iitchanged_rt_evt_clr (EVENT_t clear_flags, OS_TPTR taskp);
extern void iitchanged_rt_evt_clr_runningtask(EVENT_t flags);
extern void iitchanged_isr_evt_set (EVENT_t event_flags, OS_TPTR taskp);
extern EVENT_t iitchanged_rt_evt_get (void);
extern void iitchanged_rt_evt_psh (U32 arg_u32, U32 set_flags);

// - mailbox management routines -
extern void iitchanged_rt_mbx_init (OS_ID mailbox, U16 numofmessages);
extern OS_RESULT rt_iit_mbx_delete (OS_ID mailbox);
extern OS_RESULT iitchanged_rt_mbx_send(OS_ID mailbox, void *p_msg,    TIME_t timeout);
extern OS_RESULT iitchanged_rt_mbx_wait(OS_ID mailbox, void **message, TIME_t timeout);
extern OS_RESULT rt_iit_mbx_count(OS_ID mailbox);
extern OS_RESULT rt_iit_isr_mbx_count(OS_ID mailbox);

// - semaphore management routines -
extern void iitchanged_rt_sem_init (OS_ID semaphore, U8 token_max, U8 token_count);
extern OS_RESULT rt_iit_sem_set(OS_ID semaphore, U8 ntokens);
extern OS_RESULT iitchanged_rt_sem_send (OS_ID semaphore);
extern OS_RESULT iitchanged_rt_sem_wait(OS_ID semaphore, TIME_t timeout);
extern void iitchanged_isr_sem_send (OS_ID semaphore);
extern void iitchanged_rt_sem_psh (void* p);
extern OS_RESULT rt_iit_sem_delete (OS_ID semaphore);


// - task delays management routines -
extern void iitchanged_rt_block(TIME_t timeout, U8 block_state);
extern void  iitchanged_rt_put_dly(P_TCB_Opaque p_task, TIME_t delay);
extern void iitchanged_rt_dec_dly(void);

// - round robin routines -


// - debug routines -
extern void rt_iit_dbg_global_init(void);
extern void rt_iit_dbg_init(void);
extern void rt_iit_dbg_task_notify(void* ptcb, BOOL create);
extern void rt_iit_dbg_task_switch(U32 task_id);
extern void rt_iit_dbg_syscall_register (U8 id);
extern void rt_iit_dbg_systick_enter(void);
extern void rt_iit_dbg_systick_exit(void);
extern void rt_iit_dbg_pendsv_enter(void);
extern void rt_iit_dbg_pendsv_exit(void);
extern void rt_iit_dbg_svc_enter(void);
extern void rt_iit_dbg_svc_exit(void);
//extern void rt_iit_dbg_syscall_enter (U8 id);
extern void rt_iit_dbg_syscall_exit (void);

                                    


/* @}            
    end of group addons  
 **/

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



