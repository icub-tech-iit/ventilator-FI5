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

/* @file       oosiit_storage.c
	@brief      This file keeps ram storage and implementation of some internals of oosiit and rl-rtx
	@author     marco.accame@iit.it
    @date       07/30/2012
**/


// this file contains changes from the original RTX_CM_lib.h by Keil. we include in here its copyright notice.
/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RTX_CM_LIB.H
 *      Purpose: RTX Kernel System Configuration
 *      Rev.:    V4.50
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 1999-2009 KEIL, 2009-2012 ARM Germany GmbH
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
 
// --------------------------------------------------------------------------------------------------------------------
// - optimisation
// -------------------------------------------------------------------------------------------------------------------- 

#if   defined (__CC_ARM)
#pragma O3
#define __USED __attribute__((used))
#elif defined (__GNUC__)
#pragma GCC optimize ("O3")
#define __USED __attribute__((used))
#elif defined (__ICCARM__)
#define __USED __root
#endif

// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "oosiit.h"
#include "stdlib.h"
#include "rt_TypeDef.h"

// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "oosiit_storage_hid.h"


// --------------------------------------------------------------------------------------------------------------------
// - declarations of extern functions defined in other modules
// --------------------------------------------------------------------------------------------------------------------

// - the types needed by some funtions 
typedef uint32_t OS_TID;
typedef uint32_t OS_MUT[sizeof(struct OS_MUCB)/4];
typedef void*    OS_ID;
typedef void*   OS_TPTR;      // points to a struct OS_TCB
typedef uint32_t OS_RESULT;
typedef uint32_t TIME_t;

// - some dependency for functions used in here. these functions are defined inside oosiit.lib
extern OS_TPTR rt_iit_tsk_self (void);
extern void      rt_mut_init(OS_ID mutex);
extern OS_RESULT iitchanged_rt_mut_wait(OS_ID mutex, TIME_t timeout);
extern OS_RESULT rt_mut_release(OS_ID mutex);
extern void* rt_iit_tsk_perthread_libspace_get(OS_TPTR taskp);

// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#define mutex_init(m)   rt_mut_init(m)
#define mutex_wait(m)   iitchanged_rt_mut_wait(m, OOSIIT_NOTIMEOUT)
#define mutex_rel(m)    rt_mut_release(m)


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

// holders for static memory

// - advanced timers
uint32_t* oosiit_cfg_advtmr_ptrs                = NULL;
uint32_t* oosiit_cfg_advtmr                     = NULL;
uint16_t  oosiit_cfg_advtmr_size                = 0;

// - mutexes
uint32_t* oosiit_cfg_mutex                      = NULL;
uint16_t  oosiit_cfg_mutex_size                 = 0;

// - semaphores
uint32_t* oosiit_cfg_semaphore                  = NULL;
uint16_t  oosiit_cfg_semaphore_size             = 0;

// - mailboxes
uint32_t* oosiit_cfg_mbox                       = NULL;
uint16_t  oosiit_cfg_mbox_size                  = 0;

// - mutex for memory
uint32_t* oosiit_cfg_mutex_memory               = NULL;

// memory for tcb allocation
uint32_t* mp_tcb                                = NULL;      
uint16_t mp_tcb_size                            = 0;

// -- fifo queue buffer for isr requests
uint32_t* os_fifo                               = NULL;
uint8_t  os_fifo_size                           = 0;

// - array of active task pointers
void**    os_active_TCB                         = NULL;

// memory for simple timers: dont use them
uint32_t* m_tmr                                 = NULL;
uint16_t  mp_tmr_size                           = 0;

// - globalstack
uint64_t* oosiit_cfg_globstack                  = NULL;
uint16_t  oosiit_cfg_globstack_size             = 0;

// - system stack allocation
uint64_t* mp_stk                                = NULL;
uint32_t  mp_stk_size                           = 0;

// - memory space for arm standard library + its mutexes
#if defined (__CC_ARM) && !defined (__MICROLIB)
uint32_t (*std_libspace)[96/4]                  = NULL;
OS_MUT*   std_libmutex                          = NULL;
uint8_t   max_nr_mutex                          = 0; 
extern void  *__libspace_start;                 // just a declaration, as it is defined elsewhere
#endif


// - global variables required by rtx

uint16_t os_maxtaskrun                          = 0;
uint32_t os_stackinfo                           = 0;
uint32_t os_rrobin                              = 0;
uint32_t os_rrobin_tout                         = 0;
uint8_t  os_rrobin_use                          = 0;
uint32_t os_trv                                 = 0;
uint8_t  os_flags                               = 0;


// - export to uvision debugger
__USED uint32_t os_clockrate                    = 0;
__USED uint32_t os_timernum                     = 0;





// --------------------------------------------------------------------------------------------------------------------
// - definition of extern functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------

// - standard library multithreading interface: begin

#if     defined (__CC_ARM) && !defined (__MICROLIB)

#if     defined(OOSIIT_USE_EXTERNAL_TREADSAFETY)
    // if defined we dont re-define in here.
#else

/*--------------------------- __user_perthread_libspace ---------------------*/

void *__user_perthread_libspace (void) {
  /* Provide a separate libspace for each task. */
#if 0    
  uint32_t idx;

  idx = runtask_id ();
  if (idx == 0) {
    /* RTX not running yet. */
    return (&__libspace_start);
  }

    if(NULL == std_libspace)
    {
        oosiit_sys_error(oosiit_error_internal_stdlibspace);    
    }

  return ((void *)&std_libspace[idx-1]);
#else
    oosiit_tskptr_t tp = rt_iit_tsk_self();
    if(NULL == tp)
    {
        // oosiit not running yet
        return (&__libspace_start);
    }

    return(rt_iit_tsk_perthread_libspace_get(tp));      
#endif    
}


/*--------------------------- _mutex_initialize modified by IIT --------- */

#define OSIIT_SYSMUTEXCNT_HIDDEN  8

uint8_t nr_mutex_hidden = 0;

int _mutex_initialize(OS_ID *mutex) 
{
    // allocate and initialize a system mutex. 

    static OS_MUT sysmutexes_hidden[OSIIT_SYSMUTEXCNT_HIDDEN];

    if(nr_mutex_hidden >= OSIIT_SYSMUTEXCNT_HIDDEN) 
    {
        // if in here, you need to increase OSIIT_SYSMUTEXCNT_HIDDEN
        oosiit_sys_error(oosiit_error_internal_sysmutex);
    }


    *mutex = &sysmutexes_hidden[nr_mutex_hidden++];

    mutex_init(*mutex);

    // _mutex_initialize must return a nonzero value on success so that, at runtime, 
    // the library knows that it is being used in a multithreaded environment 
    return (1);
}


/*--------------------------- _mutex_acquire --------------------------------*/

__attribute__((used)) void _mutex_acquire (OS_ID *mutex) {
  /* Acquire a system mutex, lock stdlib resources. */
//  if (runtask_id ()) {
  if(NULL != rt_iit_tsk_self()) {
    /* RTX running, acquire a mutex. */
    mutex_wait (*mutex);
  }
}


/*--------------------------- _mutex_release --------------------------------*/

__attribute__((used)) void _mutex_release (OS_ID *mutex) {
  /* Release a system mutex, unlock stdlib resources. */
//  if (runtask_id ()) {
  if(NULL != rt_iit_tsk_self()) {
    /* RTX runnning, release a mutex. */
    mutex_rel (*mutex);
  }
}

#endif//!defined(OOSIIT_USE_EXTERNAL_TREADSAFETY)


#endif //defined (__CC_ARM) && !defined (__MICROLIB)

// - standard library multithreading interface: end




// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------



