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
 
/* @file       rt_iit_changes.c
	@brief      This file implements internal implementation of the IIT extension for some calls of RTX.
	@author     marco.accame@iit.it
    @date       07/30/2012
    @warning    this file contains parts of code taken from various files coming from the free distribution of the
                "CMSIS-RTOS RTX Implementation" version 4.20 or successive. the code has been modified to add support to 32 bit 
                timing and to dynamic retrieval of memory used by the rtos objects. the modification is done in the
                following way:
                - if a function of RTX needs a different behaviour but keeps its declation, we just redefine it in this
                  file. obviously in its original file the funtion was made __weak.
                - if a function of RTX needs also different parameters in its number of type, we just create a new
                  function in this file with name prefixed with "iitchanged_". the old function is left unchanged in 
                  its original file. 
                hence, we attach in here the copyrigth notice of KEIL.
**/



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


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

// acemor: there are not include guards in the header files from keil. such a shame...


#include "stdlib.h"

#include "oosiit_hid.h"

#include "oosiit_storage_hid.h"


// - types
#include "rt_TypeDef.h"
#include "RTX_Config.h" 

// - rt_time
#include "rt_Task.h"
#include "rt_Time.h"

// - rt_mutex
#include "rt_List.h"
#include "rt_Mutex.h"

// - rt_event
#include "rt_System.h"
#include "rt_Event.h"


// - rt_mailbox
#include "rt_Mailbox.h"
#include "rt_HAL_CM.h"

// - rt_semaphore
#include "rt_Semaphore.h"


// - rt_robin
#include "rt_Robin.h"


#include "rt_iit_AdvTimer.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "rt_iit_changes.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

// i keep it extern to allow its name in microvision
void tIDLE(void);


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

struct OS_TSK os_tsk =
{
    .run = NULL,
    .new = NULL
};

struct OS_TCB os_idle_TCB = 
{
    .cb_type                = NOCB,
    .state                  = 0,
    .prio                   = 0,
    .task_id                = 0,
    .p_lnk                  = NULL,
    .p_rlnk                 = NULL,
    .p_dlnk                 = NULL,
    .p_blnk                 = NULL,
    .delta_time             = 0,
    .interval_time          = 0,
    .events                 = 0,
    .waits                  = 0,
    .msg                    = NULL,
    .p_mlnk                 = NULL,
    .prio_base              = 0,
    .ptr_perthread_libspace = NULL,
    .extdata                = NULL,

    .stack_frame            = 0,
    .priv_stack             = 0,
    .tsk_stack              = 0,
    .stack                  = NULL,
    .ptask                  = NULL
};



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

extern void rt_iit_debug_global_init(void);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------


extern void* rt_iit_memory_new(uint32_t size)
{
    return(oosiit_ext_calloc(size, 1));
}

extern void* rt_iit_memory_realloc(void* m, uint32_t size)
{
    return(oosiit_ext_realloc(m, size));
}

extern void rt_iit_memory_del(void* mem)
{
    oosiit_ext_free(mem);
}

extern void rt_iit_dynamic_mode_init(const oosiit_cfg_t *cfg)
{   
    // cannot initialise if i dont know a configuration
    if(NULL == cfg)
    {
        os_error(0x60000001);
    }



    os_maxtaskrun = 0; //oosiit_cfg_in_use->maxnumofusertasks+2; // adds two: idle and init
//     os_stackinfo  = (oosiit_cfg_in_use->checkStack << 24) |
//                     (oosiit_cfg_in_use->numTaskWithUserProvidedStack << 16) |
//                     (oosiit_cfg_in_use->sizeStack*4);
    
    os_stackinfo  = (oosiit_cfg_in_use->checkstackoverflow << 24) |
                    (os_maxtaskrun << 16) |
                    (0*4);    
    os_rrobin_use  = (0 == oosiit_cfg_in_use->roundrobinenabled) ? (0) : (1);
    os_rrobin_tout = oosiit_cfg_in_use->roundrobinnumberofticks;

    os_trv        = ((uint32_t)(((double)oosiit_cfg_in_use->cpufreq*(double)oosiit_cfg_in_use->ticktime)/1E6)-1);
    oosiit_num_units_of_systick = (os_trv+1);
    double tmp = ((double)oosiit_cfg_in_use->ticktime*1000000.0)/(double)(oosiit_num_units_of_systick);
    oosiit_picosec_per_unit_of_systick = (uint32_t)tmp;
    os_flags      = 1; //oosiit_cfg_in_use->priviledgeMode; 

    os_clockrate  =  oosiit_cfg_in_use->ticktime;
    //os_timernum   = (0 << 16) | oosiit_cfg_in_use->numTimer;    // in cm3, OS_TIMER is 0
    os_timernum   = (0 << 16) | 0;    // in cm3, OS_TIMER is 0
 
    // dyn: advanced timers
    oosiit_cfg_advtmr_ptrs   = NULL;
    oosiit_cfg_advtmr_size   = 0;
    oosiit_cfg_advtmr        = NULL;
    
    // in dynamic mode we check the value of following variables vs to NULL to understand that we must allocate memory and not deference them 
    // dyn: mutexes
    oosiit_cfg_mutex_size       = 0;
    oosiit_cfg_mutex            = NULL;

    // dyn: semaphores
    oosiit_cfg_semaphore_size   = 0;
    oosiit_cfg_semaphore        = NULL;
    
    // dyn: mboxex
    oosiit_cfg_mbox_size        = 0;
    oosiit_cfg_mbox             = NULL;

    // dyn: mutex_memory
    oosiit_cfg_mutex_memory     = NULL;

    // dyn: mp tcb 
    mp_tcb_size                 = 0;
    mp_tcb                      = NULL;
    
    // dyn: os-fifo  .... however we need to preallocate memory for the postpend commands fifo
    os_fifo_size = oosiit_cfg_in_use->capacityofpostpendcommandfifo;
    os_fifo = rt_iit_memory_new((sizeof(struct OS_PSQ)-sizeof(struct OS_PSFE))+oosiit_cfg_in_use->capacityofpostpendcommandfifo*sizeof(struct OS_PSFE));

    // dyn: active task-control-block table: we dont use it
    os_active_TCB               = NULL; 

    // dyn: tmr
    mp_tmr_size                 = 0;
    m_tmr                       = NULL;

    // dyn: globalstack
    oosiit_cfg_globstack_size    = 0;
    oosiit_cfg_globstack         = NULL;

    // dyn: mp_stk
    mp_stk_size    = 0;
    mp_stk         = NULL;

    std_libspace = NULL;


    // initialisation to zero of os variables
    {
        extern BIT os_lock;             //IIT-EXT
        extern BIT os_psh_flag;         //IIT-EXT
        extern U8  pend_flags;          //IIT-EXT

        os_lock = 0;
        os_psh_flag = 0;
        pend_flags = 0;    
    }
    
   
}

extern void rt_iit_params_init(void)
{
    uint16_t offset32 = 0;
    uint16_t offset64 = 0;
    uint16_t ram32size[9], ram64size[2];

    // cannot initialise if i dont know a configuration
    if(NULL == oosiit_cfg_in_use)
    {
        os_error(0x60000001);
    }

    // cannot initialise if i dont have ram
    if((NULL == oosiit_params_ram32data) || (NULL == oosiit_params_ram64data) || (NULL == oosiit_params_stdlib32data))
    {
        os_error(0x60000002);
    }

    oosiit_hid_params_get_ram32(oosiit_cfg_in_use, ram32size);

    oosiit_hid_params_get_ram64(oosiit_cfg_in_use, ram64size);

    os_maxtaskrun = oosiit_cfg_in_use->maxnumofusertasks + 2; // adds idle and init
//     os_stackinfo  = (oosiit_cfg_in_use->checkStack << 24) |
//                     (oosiit_cfg_in_use->numTaskWithUserProvidedStack << 16) |
//                     (oosiit_cfg_in_use->sizeStack*4);
    os_stackinfo  = (oosiit_cfg_in_use->checkstackoverflow << 24) |
                    ((os_maxtaskrun) << 16) |
                    (0*4);    
    os_rrobin_use  = (0 == oosiit_cfg_in_use->roundrobinenabled) ? (0) : (1);
    os_rrobin_tout = oosiit_cfg_in_use->roundrobinnumberofticks;

    os_trv        = ((uint32_t)(((double)oosiit_cfg_in_use->cpufreq*(double)oosiit_cfg_in_use->ticktime)/1E6)-1);
    oosiit_num_units_of_systick = (os_trv+1);
    double tmp = ((double)oosiit_cfg_in_use->ticktime*1000000.0)/(double)(oosiit_num_units_of_systick);
    oosiit_picosec_per_unit_of_systick = (uint32_t)tmp;    
    os_flags      = 1; //oosiit_cfg_in_use->priviledgeMode; 

    os_clockrate  =  oosiit_cfg_in_use->ticktime;
    os_timernum   = (0 << 16) | 0;    // in cm3, OS_TIMER is 0
 
    
    if(0 != ram32size[0])
    {
        uint16_t sizeofadvtmr_ptrs = (sizeof(oosiit_arrayhead_t)-4) + 4*oosiit_cfg_in_use->numAdvTimer;
        // in here we place the buffer for temporary pointers, which we init with its capacity and sizeofitem
        oosiit_cfg_advtmr_ptrs = &oosiit_params_ram32data[offset32];
        ((oosiit_arrayhead_t*)oosiit_cfg_advtmr_ptrs)->capacity = oosiit_cfg_in_use->numAdvTimer;
        ((oosiit_arrayhead_t*)oosiit_cfg_advtmr_ptrs)->sizeofitem = 4;
        // after that we put the buffer for the timers
        oosiit_cfg_advtmr_size = sizeof(uint32_t)*ram32size[0] - sizeofadvtmr_ptrs;
        oosiit_cfg_advtmr = &oosiit_params_ram32data[offset32+(sizeofadvtmr_ptrs/4)];
        offset32 += ram32size[0];
    }
    else
    {
        oosiit_cfg_advtmr_ptrs   = NULL;
        oosiit_cfg_advtmr_size   = 0;
        oosiit_cfg_advtmr        = NULL;
    }

   
    if(0 != ram32size[1])
    {   
        oosiit_cfg_mutex_size = sizeof(uint32_t)*ram32size[1];
        oosiit_cfg_mutex = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[1];
    }
    else
    {
        oosiit_cfg_mutex_size    = 0;
        oosiit_cfg_mutex         = NULL;
    }

    
    if(0 != ram32size[2])
    {   
        oosiit_cfg_semaphore_size = sizeof(uint32_t)*ram32size[2];
        oosiit_cfg_semaphore = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[2];
    }
    else
    {
        oosiit_cfg_semaphore_size    = 0;
        oosiit_cfg_semaphore         = NULL;
    }
    
    if(0 != ram32size[3])
    {   
        oosiit_cfg_mbox_size = sizeof(uint32_t)*ram32size[3];
        oosiit_cfg_mbox = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[3];
    }

    if(0 != ram32size[4])
    {
        oosiit_cfg_mutex_memory = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[4];
    }
    else
    {
        oosiit_cfg_mutex_memory    = 0;
    }

    if(0 != ram32size[5])
    {   
        mp_tcb_size = sizeof(uint32_t)*ram32size[5];
        mp_tcb = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[5];
    }
    else
    {
        mp_tcb_size    = 0;
        mp_tcb         = NULL;
    }

    if(0 != ram32size[6])
    {   
        // os_fifo_size contains the number of items, not the size in bytes of teh memory
        os_fifo_size = oosiit_cfg_in_use->capacityofpostpendcommandfifo;
        os_fifo = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[6];
    }
    else
    {
        os_fifo_size    = 0;
        os_fifo         = NULL;
    }


    if(0 != ram32size[7])
    {   
        os_active_TCB = (void**) &oosiit_params_ram32data[offset32];
        offset32 += ram32size[7];
    }
    else
    {
        os_active_TCB    = NULL;
    }


    if(0 != ram32size[8])
    {   
        mp_tmr_size = sizeof(uint32_t)*ram32size[8];
        m_tmr = &oosiit_params_ram32data[offset32];
        offset32 += ram32size[8];
    }
    else
    {
        mp_tmr_size    = 0;
        m_tmr         = NULL;
    }


    if(0 != ram64size[0])
    {   
        oosiit_cfg_globstack_size = sizeof(uint64_t)*ram64size[0];
        oosiit_cfg_globstack = &oosiit_params_ram64data[offset64];
        offset64 += ram64size[0];
    }
    else
    {
        oosiit_cfg_globstack_size    = 0;
        oosiit_cfg_globstack         = NULL;
    }

    if(0 != ram64size[1])
    {   
        mp_stk_size = sizeof(uint64_t)*ram64size[1];
        mp_stk = &oosiit_params_ram64data[offset64];
        offset64 += ram64size[1];
    }
    else
    {
        mp_stk_size    = 0;
        mp_stk         = NULL;
    }


   std_libspace = (U32 (*)[24])rt_iit_libspace_init();
   if(NULL == std_libspace)
   {
       os_error(0x60000002);    
   }


    // initialisation to zero of os variables
    {
        extern BIT os_lock;             //IIT-EXT
        extern BIT os_psh_flag;         //IIT-EXT
        extern U8  pend_flags;          //IIT-EXT

        os_lock = 0;
        os_psh_flag = 0;
        pend_flags = 0;    

    }
    
        
}


extern U32* rt_iit_libspace_init(void)
{
    uint16_t sizes[2];
    U32* ret = NULL;

    if(NULL == oosiit_cfg_in_use)
    {
        return(NULL);
    }

    oosiit_hid_params_get_stdlib32(oosiit_cfg_in_use, sizes);

    if(0 != sizes[0])
    {   
        if(NULL == oosiit_params_stdlib32data)
        {
            ret = oosiit_ext_calloc(sizes[0], 4);
        }
        else
        {
            ret = &oosiit_params_stdlib32data[0];
        }
    }
    else
    {
        ret = NULL;
    } 

    return(ret);

}


extern void rt_iit_time_set(U32 low, U32 high) 
{
    U64 target = 0; 
    U64 current = 0;

    // retrigger the systick
    (*((volatile U32 *)0xE000E018)) = 0;

    target = ((U64)high << 32) + (U64)low;
    current = oosiit_time;

    if(target == current)
    {
	    
        return;
    }
    
    // change the time
    oosiit_time = target;

    // adjust the absolute-time data-structure: advanced timers
    rt_iit_advtmr_synchronise(current);
	
}



// - system task management routines: rt_System.c ---------------------------------------------------------------------

#include "rt_Timer.h"

#define IIT_EXEC_ONPENDSV_IN_SYSTICK // IIT-EXT

// IIT-EXT: made them global
extern BIT os_lock;
extern BIT os_psh_flag;
extern U8  pend_flags;

// it contains the standard function rt_suspend() 
U32 rt_iit_timeofnextevent(U8 andsuspend)
{
  /* Suspend OS scheduler */
  U32 delta = 0xFFFFU;
//#ifdef __CMSIS_RTOS
//  U32 sleep;
//#endif


  if(1 == andsuspend)
  {
    rt_tsk_lock();
  }
  
  if (os_dly.p_dlnk) {
    delta = os_dly.delta_time;
  }
//#ifdef __CMSIS_RTOS
//  sleep = sysUserTimerWakeupTime();
//  if (sleep < delta) { delta = sleep; }
//#else
//  if (os_tmr.next) {
//    if (os_tmr.tcnt < delta) delta = os_tmr.tcnt;
//  }
//#endif
  
  // we get next advanced timer time ...
  
  U32 nexttimer = rt_iit_advtmr_next();

  // it returns either a tick or 0xffffffff
  if(nexttimer < delta)
  {
      delta = nexttimer;
  }

  return (delta);    
      
}


// overridden function
U32 rt_suspend (void) {
  return (rt_iit_timeofnextevent(1));
}



void rt_resume (U32 sleep_time) {
  /* Resume OS scheduler after suspend */
  P_TCB next;
  U32   delta;

  os_tsk.run->state = READY;
  rt_put_rdy_first (os_tsk.run);

  os_robin.task = NULL;

  /* Update delays. */
  if (os_dly.p_dlnk) {
    delta = sleep_time;
    if (delta >= os_dly.delta_time) {
      delta   -= os_dly.delta_time;
      os_time += os_dly.delta_time;
      oosiit_time += os_dly.delta_time;
      os_dly.delta_time = 1U;
      while (os_dly.p_dlnk) {
        // rt_dec_dly();
        iitchanged_rt_dec_dly();
        if (delta == 0U) { break; }
        delta--;
        os_time++;
        oosiit_time++;
      }
    } else {
      os_time           +=      delta;
      oosiit_time       +=      delta;
      os_dly.delta_time -= (TIME_t)delta; // IIT-EXT: made delta_time of type TIME_t
    }
  } else {
    os_time += sleep_time;
    oosiit_time += sleep_time;
  }

//  /* Check the user timers. */
//#ifdef __CMSIS_RTOS
//  sysUserTimerUpdate(sleep_time);
//#else
//  if (os_tmr.next) {
//    delta = sleep_time;
//    if (delta >= os_tmr.tcnt) {
//      delta   -= os_tmr.tcnt;
//      os_tmr.tcnt = 1U;
//      while (os_tmr.next) {
//        rt_tmr_tick();
//        if (delta == 0U) { break; }
//        delta--;
//      }
//    } else {
//      os_tmr.tcnt -= delta;
//    }
//  }
//#endif
  
  // check the advtimers
  
  rt_iit_advtmr_resume_from_sleep(sleep_time);
  

  /* Switch back to highest ready task */
  next = rt_get_first (&os_rdy);
  rt_switch_req (next);

  rt_tsk_unlock();
}

void rt_tsk_lock (void) {
  /* Prevent task switching by locking out scheduler */
  // IIT-EXT: we need to read the register in order to clear bit 16, the COUNTFLAG
  volatile U32 nvicstctrl = NVIC_ST_CTRL;  
  nvicstctrl = nvicstctrl;
    
  if (os_tick_irqn < 0) {
    OS_LOCK();
    os_lock = __TRUE;
    OS_UNPEND(pend_flags);
  } else {
    OS_X_LOCK(os_tick_irqn);
    os_lock = __TRUE;
    OS_X_UNPEND(pend_flags);
  }
}

void rt_tsk_unlock (void) {
  /* Unlock scheduler and re-enable task switching */
  // IIT-EXT: added the execution of the systick if the timer was expired in the meantime.
  // warning: the systick executes only one even if the lock lasted for more than one period
  volatile U32 nvicstctrl = NVIC_ST_CTRL;
  					 
  if(0x00010000 == (nvicstctrl & 0x00010000))
  { // bit 16 is the COUNTFLAG: reads as 1 if counter reaches 0 since this is the last time this register
    // is read. clear to 0 automatically when read or when current counter value is cleared.
	// in our case, OS_LOCK() cleared it.
  	pend_flags |= 1; // in position 1 there is the systick flag
  }

  if (os_tick_irqn < 0) {
    OS_UNLOCK();
    os_lock = __FALSE;
    OS_PEND(pend_flags, os_psh_flag);
    os_psh_flag = __FALSE;
  } else {
   OS_X_UNLOCK((U32)os_tick_irqn);
    os_lock = __FALSE;
    OS_X_PEND(pend_flags, os_psh_flag);
    os_psh_flag = __FALSE;
  }
  
}

#ifdef IIT_EXEC_ONPENDSV_IN_SYSTICK
void rt_iit_pop_req_base (void) {
  struct OS_XCB *p_CB;
  U32  idx;

  idx = os_psq->last;
  while (os_psq->count) {
    p_CB = os_psq->q[idx].id;
    if (p_CB->cb_type == TCB) {
      /* Is of TCB type */
      iitchanged_rt_evt_psh ((U32)p_CB, (U32)os_psq->q[idx].arg);    // IIT-EXT: it was rt_evt_psh ((P_TCB)p_CB, (U16)os_psq->q[idx].arg)
    }
    else if (p_CB->cb_type == MCB) {
      /* Is of MCB type */
      rt_mbx_psh ((P_MCB)p_CB, (void *)os_psq->q[idx].arg);
    }
    else if (p_CB->cb_type == ATCB) { //IIT-EXT
      rt_advtmr_psh ((OS_ID)p_CB, (U32)os_psq->q[idx].arg); //IIT-EXT
    } //IIT-EXT
    else if (p_CB->cb_type == SCB) { //IIT-EXT
      /* Must be of SCB type */
      iitchanged_rt_sem_psh((P_SCB)p_CB); // IIT-EXT: it was: rt_sem_psh ((P_SCB)p_CB)
    } // else ... it may be NOCB or an error
    if (++idx == os_psq->size) idx = 0;
    rt_dec (&os_psq->count);
  }
  os_psq->last = idx;

}
#endif 

#ifdef IIT_EXEC_ONPENDSV_IN_SYSTICK
void rt_iit_pop_req_inside_systick (void) {

  	if((1<<28) == ((1<<28) & (NVIC_INT_CTRL)))
  	{
		rt_iit_pop_req_base();
  		// clear
  		NVIC_INT_CTRL  |= (1<<27);
	}

}
#endif

void rt_pop_req (void) {
  /* Process an ISR post service requests. */
#ifdef IIT_EXEC_ONPENDSV_IN_SYSTICK
  
  P_TCB next;

  rt_iit_dbg_pendsv_enter(); // IIT-EXT

  os_tsk.run->state = READY;
  rt_put_rdy_first (os_tsk.run);

  rt_iit_pop_req_base();

  next = rt_get_first (&os_rdy);
  rt_switch_req (next);

  rt_iit_dbg_pendsv_exit(); // IIT-EXT

#else
  // original rtx's rt_pop_req() w/ some changes
  /* Process an ISR post service requests. */
  struct OS_XCB *p_CB;
  P_TCB next;
  U32  idx;

  rt_iit_dbg_pendsv_enter(); // IIT-EXT

  os_tsk.run->state = READY;
  rt_put_rdy_first (os_tsk.run);

  idx = os_psq->last;
  while (os_psq->count) {
    p_CB = os_psq->q[idx].id;
    if (p_CB->cb_type == TCB) {
      /* Is of TCB type */
      rt_iit_evt_psh ((U32)p_CB, (U32)os_psq->q[idx].arg);    // IIT-EXT: it was: rt_evt_psh ((P_TCB)p_CB, (U16)os_psq->q[idx].arg)
    }
    else if (p_CB->cb_type == MCB) {
      /* Is of MCB type */
      rt_mbx_psh ((P_MCB)p_CB, (void *)os_psq->q[idx].arg);
    }
    else if (p_CB->cb_type == ATCB) { // IIT-EXT
      rt_advtmr_psh ((OS_ID)p_CB, (U32)os_psq->q[idx].arg); // IIT-EXT: added
    } // IIT-EXT
    else if (p_CB->cb_type == SCB) { // IIT-EXT
      /* Must be of SCB type */
      rt_iit_sem_psh((P_SCB)p_CB); // IIT-EXT: it was: rt_sem_psh ((P_SCB)p_CB)
    } // else ... error
    if (++idx == os_psq->size) { idx = 0U; }
    rt_dec (&os_psq->count);
  }
  os_psq->last = (U8)idx;

  next = rt_get_first (&os_rdy);
  rt_switch_req (next);

  rrt_iit_dbg_pendsv_exit(); // IIT-EXT

#endif
}

#ifdef __CMSIS_RTOS
extern void sysTimerTick(void);
#endif
// made as much consistent as possible with version V4.81
void rt_systick (void) {
  /* Check for system clock update, suspend running task. */
  P_TCB next;

  rt_iit_dbg_systick_enter();               // IIT-EXT

  os_tsk.run->state = READY;
  rt_put_rdy_first (os_tsk.run);

  /* Check Round Robin timeout. */
  rt_chk_robin ();		        // IIT-EXT

  /* Update delays. */
  os_time++;
  oosiit_time++;				            // IIT-EXT
  iitchanged_rt_dec_dly ();			        // IIT-EXT


//  /* Check the user timers. */
//#ifdef __CMSIS_RTOS
//  sysTimerTick();
//
//#else
//  rt_tmr_tick ();
//#endif

  rt_iit_advtmr_tick();			            // IIT-EXT: we use only the advanced timers


#ifdef IIT_EXEC_ONPENDSV_IN_SYSTICK	
  rt_iit_pop_req_inside_systick();          // IIT-EXT
#endif

  /* Switch back to highest ready task */
  next = rt_get_first (&os_rdy);
  rt_switch_req (next);


  rt_iit_dbg_systick_exit();	            // IIT-EXT
}


// - time management routines: rt_Time.c ------------------------------------------------------------------------------

void iitchanged_rt_dly_wait (TIME_t delay_time) {
  /* Delay task by "delay_time" */
  iitchanged_rt_block (delay_time, WAIT_DLY);   
} 

void iitchanged_rt_itv_set (TIME_t interval_time) {
  /* Set interval length and define start of first interval */
  os_tsk.run->interval_time = interval_time;
  os_tsk.run->delta_time = interval_time + oosiit_time;
} 

void iitchanged_rt_itv_wait (void) {
  /* Wait for interval end and define start of next one */
  TIME_t delta;

  delta = os_tsk.run->delta_time - oosiit_time;
  os_tsk.run->delta_time += os_tsk.run->interval_time;
  if ((delta & MSB_TIME_t) == 0) {
    iitchanged_rt_block (delta, WAIT_ITV);
  }
} 


// - mutex management routines: rt_Mutex.c ----------------------------------------------------------------------------

// changed: type of timeout, call to iitchanged_rt_block().
OS_RESULT iitchanged_rt_mut_wait (OS_ID mutex, TIME_t timeout) {
  /* Wait for a mutex, continue when mutex is free. */
  P_MUCB p_MCB = mutex;

  if (p_MCB->level == 0U) {
    p_MCB->owner  = os_tsk.run;
    p_MCB->p_mlnk = os_tsk.run->p_mlnk;
    os_tsk.run->p_mlnk = p_MCB; 
    goto inc;
  }
  if (p_MCB->owner == os_tsk.run) {
    /* OK, running task is the owner of this mutex. */
inc:p_MCB->level++;
    return (OS_R_OK);
  }
  /* Mutex owned by another task, wait until released. */
  if (timeout == 0U) {
    return (OS_R_TMO);
  }
  /* Raise the owner task priority if lower than current priority. */
  /* This priority inversion is called priority inheritance.       */
  if (p_MCB->owner->prio < os_tsk.run->prio) {
    p_MCB->owner->prio = os_tsk.run->prio;
    rt_resort_prio (p_MCB->owner);
  }
  if (p_MCB->p_lnk != NULL) {
    rt_put_prio ((P_XCB)p_MCB, os_tsk.run);
  }
  else {
    p_MCB->p_lnk = os_tsk.run;
    os_tsk.run->p_lnk  = NULL;
    os_tsk.run->p_rlnk = (P_TCB)p_MCB;
  }
  iitchanged_rt_block(timeout, WAIT_MUT);
  return (OS_R_TMO);
}

//// we want it inside oosiit but function rt_mut_delete() is fine as it is
//OS_RESULT rt_iit_mut_delete (OS_ID mutex) {
//  return(rt_mut_delete(mutex));
//}


// - event flag management routines: rt_Event.c -----------------------------------------------------------------------

// changed type of event flags, of timeout and used iitchanged_rt_block()
OS_RESULT iitchanged_rt_evt_wait (EVENT_t wait_flags, TIME_t timeout, BOOL and_wait) {
  /* Wait for one or more event flags with optional time-out.                */
  /* "wait_flags" identifies the flags to wait for.                          */
  /* "timeout" is the time-out limit in system ticks (0xffff if no time-out) */
  /* "and_wait" specifies the AND-ing of "wait_flags" as condition to be met */
  /* to complete the wait. (OR-ing if set to 0).                             */
  U32 block_state;

  if (and_wait) {
    /* Check for AND-connected events */
    if ((os_tsk.run->events & wait_flags) == wait_flags) {
      os_tsk.run->events &= ~wait_flags;
      return (OS_R_EVT);
    }
    block_state = WAIT_AND;
  }
  else {
    /* Check for OR-connected events */
    if (os_tsk.run->events & wait_flags) {
      os_tsk.run->waits = os_tsk.run->events & wait_flags;
      os_tsk.run->events &= ~wait_flags;
      return (OS_R_EVT);
    }
    block_state = WAIT_OR;
  }
  /* Task has to wait */
  os_tsk.run->waits = wait_flags;
  iitchanged_rt_block (timeout, (U8)block_state);
  return (OS_R_TMO);
} 

// using task pointer instead of task id
void iitchanged_rt_evt_set (EVENT_t event_flags, OS_TPTR taskp) {
  /* Set one or more event flags of a selectable task. */
  P_TCB p_tcb;

  p_tcb = (P_TCB) taskp;
  if (p_tcb == NULL) {
    return;
  }
  p_tcb->events |= event_flags;
  event_flags    = p_tcb->waits;
  /* If the task is not waiting for an event, it should not be put */
  /* to ready state. */
  if (p_tcb->state == WAIT_AND) {
    /* Check for AND-connected events */
    if ((p_tcb->events & event_flags) == event_flags) {
      goto wkup;
    }
  }
  if (p_tcb->state == WAIT_OR) {
    /* Check for OR-connected events */
    if (p_tcb->events & event_flags) {
      p_tcb->waits  &= p_tcb->events;
wkup: p_tcb->events &= ~event_flags;
      rt_rmv_dly (p_tcb);
      p_tcb->state   = READY;
#ifdef __CMSIS_RTOS
      rt_ret_val2(p_tcb, 0x08U/*osEventSignal*/, p_tcb->waits); 
#else
      rt_ret_val (p_tcb, OS_R_EVT);
#endif
      rt_dispatch (p_tcb);
    }
  }
}

// changed: the type of the flags and the way the task is retrieved: not from id but from its pointer
void iitchanged_rt_evt_clr (EVENT_t clear_flags, OS_TPTR taskp) {
  /* Clear one or more event flags (identified by "clear_flags") of a */
  /* selectable task (identified by "task"). */
  P_TCB task = (P_TCB)taskp;

  if (task == NULL) {
    return;
  }
  task->events &= ~clear_flags;
}

void iitchanged_rt_evt_clr_runningtask(EVENT_t flags)
{
    os_tsk.run->events &= ~flags;
}


// changed: the type of the flags and the way the task is retrieved
void iitchanged_isr_evt_set (EVENT_t event_flags, OS_TPTR taskp) {
  /* Same function as "os_evt_set", but to be called by ISRs. */
  P_TCB p_tcb = (P_TCB)taskp;

  if (p_tcb == NULL) {
    return;
  }
  rt_psq_enq (p_tcb, event_flags);
  rt_psh_req ();
}

// changed: the type of the flags
EVENT_t iitchanged_rt_evt_get (void) {
  /* Get events of a running task after waiting for OR connected events. */
  return (os_tsk.run->waits);
}

// changed the first argument and the type of flags
void iitchanged_rt_evt_psh (U32 arg_u32, U32 set_flags) {
  /* Check if task has to be waken up */
  U32 event_flags;
  P_TCB p_CB = (P_TCB)arg_u32; 

  p_CB->events |= set_flags;
  event_flags = p_CB->waits;
  if (p_CB->state == WAIT_AND) {
    /* Check for AND-connected events */
    if ((p_CB->events & event_flags) == event_flags) {
      goto rdy;
    }
  }
  if (p_CB->state == WAIT_OR) {
    /* Check for OR-connected events */
    if (p_CB->events & event_flags) {
      p_CB->waits  &= p_CB->events;
rdy:  p_CB->events &= ~event_flags;
      rt_rmv_dly (p_CB);
      p_CB->state   = READY;
#ifdef __CMSIS_RTOS
      rt_ret_val2(p_CB, 0x08U/*osEventSignal*/, p_CB->waits); 
#else
      rt_ret_val (p_CB, OS_R_EVT);
#endif
      rt_put_prio (&os_rdy, p_CB);
    }
  }
}

// - mailbox management routines: rt_Mailbox.c ------------------------------------------------------------------------
// in here we change rt_mbx_init() by simply passing number of messages instead of ...
void iitchanged_rt_mbx_init (OS_ID mailbox, U16 numofmessages) {
  /* Initialize a mailbox */
  P_MCB p_MCB = mailbox;

  p_MCB->cb_type = MCB;
  p_MCB->state   = 0U;
  p_MCB->isr_st  = 0U;
  p_MCB->p_lnk   = NULL;
  p_MCB->first   = 0U;
  p_MCB->last    = 0U;
  p_MCB->count   = 0U;
  p_MCB->size    = numofmessages;
}

// developed from scratch
OS_RESULT rt_iit_mbx_delete (OS_ID mailbox) 
{
    // delete mailbox
    P_MCB p_MCB = mailbox;
    P_TCB p_TCB;
 
    while (p_MCB->p_lnk != NULL) 
    {
        if ((p_MCB->p_lnk != NULL) && (p_MCB->state == 1)) 
        {   // a task is waiting for a message:  we wake it up and give error if the mbx is empty. otherwise we give it the message
            p_TCB = rt_get_first ((P_XCB)p_MCB);
            if (p_MCB->count) 
            {
                *p_TCB->msg = p_MCB->msg[p_MCB->last];
                if (++p_MCB->last == p_MCB->size) {
                    p_MCB->last = 0;
                }
                rt_dec (&p_MCB->count); 
                //rt_ret_val (p_TCB, OS_R_MBX);  
                rt_ret_val(p_TCB, OS_R_OBJDELETED);
            }
            else
            {
                *p_TCB->msg = NULL;
                rt_ret_val (p_TCB, OS_R_OBJDELETED);
            }
            rt_rmv_dly (p_TCB);
            rt_dispatch (p_TCB);               
        }
        else if ((p_MCB->p_lnk != NULL) && (p_MCB->state == 2)) 
        {
            // a task is waiting to send a message: the mbx cannot accept it, thus we return an error 
            p_TCB = rt_get_first ((P_XCB)p_MCB);
            rt_ret_val(p_TCB, OS_R_OBJDELETED);
            rt_rmv_dly (p_TCB);
            rt_dispatch (p_TCB);
        }
    }

    if (os_rdy.p_lnk && (os_rdy.p_lnk->prio > os_tsk.run->prio)) 
    {
        /* preempt running task */
        rt_put_prio (&os_rdy, os_tsk.run);
        os_tsk.run->state = READY;
        rt_dispatch (NULL);
    }

    p_MCB->cb_type = NOCB;

    return (OS_R_OK);
}

// in here we just use TIME_t for type of timeout and use iitchanged_rt_block() 
OS_RESULT iitchanged_rt_mbx_send (OS_ID mailbox, void *p_msg, TIME_t timeout) {
  /* Send message to a mailbox */
  P_MCB p_MCB = mailbox;
  P_TCB p_TCB;

  if ((p_MCB->p_lnk != NULL) && (p_MCB->state == 1U)) {
    /* A task is waiting for message */
    p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
    rt_ret_val2(p_TCB, 0x10U/*osEventMessage*/, (U32)p_msg);
#else
    *p_TCB->msg = p_msg;
    rt_ret_val (p_TCB, OS_R_MBX);
#endif
    rt_rmv_dly (p_TCB);
    rt_dispatch (p_TCB);
  }
  else {
    /* Store message in mailbox queue */
    if (p_MCB->count == p_MCB->size) {
      /* No free message entry, wait for one. If message queue is full, */
      /* then no task is waiting for message. The 'p_MCB->p_lnk' list   */
      /* pointer can now be reused for send message waits task list.    */
      if (timeout == 0U) {
        return (OS_R_TMO);
      }
      if (p_MCB->p_lnk != NULL) {
        rt_put_prio ((P_XCB)p_MCB, os_tsk.run);
      }
      else {
        p_MCB->p_lnk = os_tsk.run;
        os_tsk.run->p_lnk  = NULL;
        os_tsk.run->p_rlnk = (P_TCB)p_MCB;
        /* Task is waiting to send a message */      
        p_MCB->state = 2U;
      }
      os_tsk.run->msg = p_msg;
      iitchanged_rt_block (timeout, WAIT_MBX);
      return (OS_R_TMO);
    }
    /* Yes, there is a free entry in a mailbox. */
    p_MCB->msg[p_MCB->first] = p_msg;
    rt_inc (&p_MCB->count);
    if (++p_MCB->first == p_MCB->size) {
      p_MCB->first = 0U;
    }
  }
  return (OS_R_OK);
}


// in here we just use TIME_t for type of timeout and use iitchanged_rt_block() 
OS_RESULT iitchanged_rt_mbx_wait (OS_ID mailbox, void **message, TIME_t timeout) {
  /* Receive a message; possibly wait for it */
  P_MCB p_MCB = mailbox;
  P_TCB p_TCB;

  /* If a message is available in the fifo buffer */
  /* remove it from the fifo buffer and return. */
  if (p_MCB->count) {
    *message = p_MCB->msg[p_MCB->last];
    if (++p_MCB->last == p_MCB->size) {
      p_MCB->last = 0U;
    }
    if ((p_MCB->p_lnk != NULL) && (p_MCB->state == 2U)) {
      /* A task is waiting to send message */
      p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
      rt_ret_val(p_TCB, 0U/*osOK*/);
#else
      rt_ret_val(p_TCB, OS_R_OK);
#endif
      p_MCB->msg[p_MCB->first] = p_TCB->msg;
      if (++p_MCB->first == p_MCB->size) {
        p_MCB->first = 0U;
      }
      rt_rmv_dly (p_TCB);
      rt_dispatch (p_TCB);
    }
    else {
      rt_dec (&p_MCB->count);
    }
    return (OS_R_OK);
  }
  /* No message available: wait for one */
  if (timeout == 0U) {
    return (OS_R_TMO);
  }
  if (p_MCB->p_lnk != NULL) {
    rt_put_prio ((P_XCB)p_MCB, os_tsk.run);
  }
  else {
    p_MCB->p_lnk = os_tsk.run;
    os_tsk.run->p_lnk = NULL;
    os_tsk.run->p_rlnk = (P_TCB)p_MCB;
    /* Task is waiting to receive a message */      
    p_MCB->state = 1U;
  }
  iitchanged_rt_block(timeout, WAIT_MBX);
#ifndef __CMSIS_RTOS
  os_tsk.run->msg = message;
#endif
  return (OS_R_TMO);
}

// not present in distribution of CMSIS-RTOS RTX
OS_RESULT rt_iit_mbx_count(OS_ID mailbox) {

  /* Gives the used space in a mailbox. Returns the number of messages     */
  /* currently stored in the mailbox. It returns 0 when mailbox is empty.   */
  P_MCB p_MCB = mailbox;
  return (p_MCB->count);
}

// not present in distribution of CMSIS-RTOS RTX
OS_RESULT rt_iit_isr_mbx_count(OS_ID mailbox) {

  /* Gives the used space in a mailbox. Returns the number of messages     */
  /* currently stored in the mailbox. It returns 0 when mailbox is empty.   */
  P_MCB p_MCB = mailbox;
  return (p_MCB->count);
}



// - semaphore management routines: rt_Semaphore.c --------------------------------------------------------------------


void iitchanged_rt_sem_init (OS_ID semaphore, U8 token_max, U8 token_count) {
  /* Initialize a semaphore */
  P_SCB p_SCB = semaphore;

  if(token_count > token_max)
  {
    token_count = token_max;
  }

  p_SCB->cb_type = SCB;
  p_SCB->p_lnk  = NULL;
  p_SCB->tokens = token_count;
  p_SCB->maxtokens = token_max;
}


// not present in distribution of CMSIS-RTOS RTX
OS_RESULT rt_iit_sem_set (OS_ID semaphore, U8 ntokens) {
  /* Return some tokens to semaphore */
  P_SCB p_SCB = semaphore;
  P_TCB p_TCB;
  U16 tkns = p_SCB->tokens;


  if(ntokens > p_SCB->maxtokens)
  { // cannot change ...
    return (OS_R_NOK);
  }

  if(ntokens == tkns)
  { // nothing changes, thus ... no action to do
    return (OS_R_OK);
  }

  if(p_SCB->p_lnk == NULL)
  { // no task is waiting, thus we can change the number without problems ...
    p_SCB->tokens = ntokens;
    return (OS_R_OK);
  }

  // we change the number of tokens and one or more tasks are waiting ...
  // in this case for sure p_SCB->tokens is zero. what we have to do is to
  // signal m waiting tasks (where m <= ntokens) that they could get the semaphore
  // and then set p_SCB->tokens to (ntokens - m)

  while((p_SCB->p_lnk == NULL) && (ntokens > 0))
  {
    p_TCB = rt_get_first ((P_XCB)p_SCB);
#ifdef __CMSIS_RTOS
    rt_ret_val(p_TCB, 1);
#else
    rt_ret_val(p_TCB, OS_R_SEM);
#endif
    rt_rmv_dly (p_TCB);
    rt_dispatch (p_TCB);
    //os_tsk.run->ret_val = OS_R_OK;
    ntokens --;
  }

  p_SCB->tokens = ntokens;
 
  return (OS_R_OK);
}

// changed so that we have control over maxtokens
OS_RESULT iitchanged_rt_sem_send (OS_ID semaphore) {
  /* Return a token to semaphore */
  P_SCB p_SCB = semaphore;
  P_TCB p_TCB;

  if (p_SCB->p_lnk != NULL) {
    /* A task is waiting for token */
    p_TCB = rt_get_first ((P_XCB)p_SCB);
#ifdef __CMSIS_RTOS
    rt_ret_val(p_TCB, 1U);
#else
    rt_ret_val(p_TCB, OS_R_SEM);
#endif
    rt_rmv_dly (p_TCB);
    rt_dispatch (p_TCB);
  }
  else {
    /* Store token. */
    if(p_SCB->tokens == p_SCB->maxtokens)
    {
      return(OS_R_NOK);
    }
    else
    {
      p_SCB->tokens++;
    }
  }
  return (OS_R_OK);
}

// changes are in timeout and iitchanged_rt_block()
OS_RESULT iitchanged_rt_sem_wait (OS_ID semaphore, TIME_t timeout) {
  /* Obtain a token; possibly wait for it */
  P_SCB p_SCB = semaphore;

  if (p_SCB->tokens) {
    p_SCB->tokens--;
    return (OS_R_OK);
  }
  /* No token available: wait for one */
  if (timeout == 0) {
    return (OS_R_TMO);
  }
  if (p_SCB->p_lnk != NULL) {
    rt_put_prio ((P_XCB)p_SCB, os_tsk.run);
  }
  else {
    p_SCB->p_lnk = os_tsk.run;
    os_tsk.run->p_lnk = NULL;
    os_tsk.run->p_rlnk = (P_TCB)p_SCB;
  }
  iitchanged_rt_block(timeout, WAIT_SEM);
  return (OS_R_TMO);
}


// changes are in argument (now void*) and in managing maxtokens
void iitchanged_rt_sem_psh (void *p) {
  P_SCB p_CB = (P_SCB) p;
  /* Check if task has to be waken up */
  P_TCB p_TCB;

  if (p_CB->p_lnk != NULL) {
    /* A task is waiting for token */
    p_TCB = rt_get_first ((P_XCB)p_CB);
    rt_rmv_dly (p_TCB);
    p_TCB->state   = READY;
#ifdef __CMSIS_RTOS
    rt_ret_val(p_TCB, 1U);
#else
    rt_ret_val(p_TCB, OS_R_SEM);
#endif
    rt_put_prio (&os_rdy, p_TCB);
  }
  else {
    /* Store token */
    if(p_CB->tokens < p_CB->maxtokens)
    {
      p_CB->tokens++;
    }
  }
}

// changes are in return value only
OS_RESULT rt_iit_sem_delete (OS_ID semaphore) {
  /* Delete semaphore */
  P_SCB p_SCB = semaphore;
  P_TCB p_TCB;

  while (p_SCB->p_lnk != NULL) {
    /* A task is waiting for token */
    p_TCB = rt_get_first ((P_XCB)p_SCB);
    rt_ret_val(p_TCB, OS_R_OBJDELETED);
    rt_rmv_dly(p_TCB);
    p_TCB->state = READY;
    rt_put_prio (&os_rdy, p_TCB);
  }

  if ((os_rdy.p_lnk != NULL) && (os_rdy.p_lnk->prio > os_tsk.run->prio)) {
    /* preempt running task */
    rt_put_prio (&os_rdy, os_tsk.run);
    os_tsk.run->state = READY;
    rt_dispatch (NULL);
  }

  p_SCB->cb_type = NOCB;

  return (OS_R_OK);
}


// - from rt_task: rt_Task.c ------------------------------------------------------------------------------------------

#include "rt_MemBox.h"
#include "rt_iit_memory.h"

static U8 osiit_init_task_started = 0; // IIT-EXT: allows to give tid 1 only to init_task

// redefined
OS_TID rt_get_TID (void) {
  U32 tid;
  static U32 last = 1;
    
  if(NULL == os_active_TCB) {
    return(last++);
  }
  
  for (tid = (1+osiit_init_task_started); tid <= os_maxtaskrun; tid++) {       // IIT-EXT: added
    if (os_active_TCB[tid-1] == NULL) {
      return ((OS_TID)tid);
    }
  }
  return (0);    
}


// redefined: to reflect initialisation of added members and ... stack cannot be automatically allocated in here
void rt_init_context (P_TCB p_TCB, U8 priority, FUNCP task_body) {
  /* Initialize general part of the Task Control Block. */
  p_TCB->cb_type        = TCB;
  p_TCB->state          = READY;
  p_TCB->prio           = priority;
  p_TCB->prio_base      = priority;
  p_TCB->p_lnk          = NULL;
  p_TCB->p_rlnk         = NULL;
  p_TCB->p_dlnk         = NULL;
  p_TCB->p_blnk         = NULL;
  p_TCB->p_mlnk         = NULL;
  p_TCB->ptr_perthread_libspace = p_TCB->ptr_perthread_libspace;
  p_TCB->extdata        = p_TCB->extdata;
  p_TCB->delta_time     = 0;
  p_TCB->interval_time  = 0;
  p_TCB->events         = 0;
  p_TCB->waits          = 0;
  p_TCB->stack_frame    = 0;

  if ((p_TCB->stack == NULL) || (p_TCB->priv_stack == 0)) {
    // stack must be passes as non-NULL pointer and priv_stack must contain its non-zero size
    os_error(4); // invalid call
  }
  rt_init_stack (p_TCB, task_body);
}


extern BIT os_lock;             //IIT-EXT

// changed the timeout type and used iitchanged_rt_put_dly() 
void iitchanged_rt_block (TIME_t timeout, U8 block_state) {
   /* Block running task and choose next ready task.                         */
  /* "timeout" sets a time-out value or is 0xffff-ffff (=no time-out).           */
  /* "block_state" defines the appropriate task state */
  P_TCB next_TCB;

  if (timeout) {
    if (timeout < NOTIMEOUT) {
       iitchanged_rt_put_dly (os_tsk.run, timeout);
    }
    // marco accame added:
    // in case a task t_one locks the scheduler and before unlocking it, it nastily calls any blocking function, 
    // then there is a context switch to the next highest priority task etc. this is an erroneous behaviour.
    // the task t_one should never make such blocking calls when it has disabled scheduling.
    // solution 1: enter in error mode
    // solution 2: reenable scheduling.
    // we use solution 1.

    if(os_lock == __TRUE)
    {
      os_error(4); // invalid call
    }
    os_tsk.run->state = block_state;
    next_TCB = rt_get_first (&os_rdy);
    rt_switch_req (next_TCB);
  }
}

// implemented: similar to rt_tsk_self(), but it returns a pointer to running task an not an id 
OS_TPTR rt_iit_tsk_self (void) {
  /* Return own task identifier value. */
  if (os_tsk.run == NULL) {
    return (NULL);
  }
  return (os_tsk.run);
}

// implemented: similar to rt_tsk_prio(), but uses a pointer to running task an not an id 
OS_RESULT rt_iit_tsk_prio (OS_TPTR taskp, U8 new_prio) {
  /* Change execution priority of a task to "new_prio". */
  P_TCB p_task = (P_TCB)taskp;
    
  if(NULL == p_task)
  {
    return (OS_R_NOK);    
  }

  if (p_task == os_tsk.run) {
    /* Change execution priority of calling task. */
    os_tsk.run->prio      = new_prio;
    os_tsk.run->prio_base = new_prio;
run:if (rt_rdy_prio() > new_prio) {
      rt_put_prio (&os_rdy, os_tsk.run);
      os_tsk.run->state   = READY;
      rt_dispatch (NULL);
    }
    return (OS_R_OK);
  }
  // IIT-EXT: removed because we dont need to find it anymore 
//  /* Find the task in the "os_active_TCB" array. */
//  if (task_id > os_maxtaskrun || os_active_TCB[task_id-1] == NULL) {
//    /* Task with "task_id" not found or not started. */
//    return (OS_R_NOK);
//  }
//  p_task = os_active_TCB[task_id-1]; 
  p_task->prio      = new_prio;
  p_task->prio_base = new_prio;
  if (p_task == os_tsk.run) {
    goto run;
  }
  rt_resort_prio (p_task);
  if (p_task->state == READY) {
    /* Task enqueued in a ready list. */
    p_task = rt_get_first (&os_rdy);
    rt_dispatch (p_task);
  }
  return (OS_R_OK);
}


// implemented: similar to rt_tsk_create(), but it returns a pointer to running task an not an id 
OS_TPTR rt_iit_tsk_create (FUNCP task, void *taskfnarg, void *taskstackdata, osiit_hid_tsk_create_other_args_t* others) {
  /* Start a new task declared with "task". */
  P_TCB task_context;
  U32 i;
  U8 prio = others->priority;
  U16 stacksize = others->stacksize;
  
    
  // in case of static allocation the tid is 1.... max_num_of_tasks+1.
  // in case of dynamic allocation the tid is 1 .... inf
  if(0 == (i = rt_get_TID ())) 
  { // i move it earliear from (**1234**) because: if i use dynamic memory mode the rt_iit_memory_new() is succesful even if we dont have a valid TID 
    return(0);
  }      


//   /* Priority 0 is reserved for idle task! */
//   if ((prio_stksz & 0xFF) == 0) {
//     prio_stksz += 1;
//   }
  
  if(0 == prio)
  {
      prio = 1;
  }
  
  if(NULL != mp_tcb)
  {
    task_context = rt_alloc_box (mp_tcb);
    if(NULL == task_context)
    {
       oosiit_sys_error(oosiit_error_memory_preallocated);
    }
  }
  else
  {
    task_context = rt_iit_memory_new(sizeof(struct OS_TCB));    
  }
  
  if (task_context == NULL) {
    return (0);
  }
  
  task_context->ptr_perthread_libspace = NULL;      // it will be set on demand inside rt_iit_tsk_perthread_libspace_get()
  task_context->extdata = others->extdata;
  
  /* If "size != 0" use a private user provided stack. */
  task_context->stack      = taskstackdata;
  //was: task_context->priv_stack = prio_stksz >> 8;
  task_context->priv_stack = stacksize;
  /* Pass parameter 'argv' to 'rt_init_context' */
  task_context->msg = taskfnarg;
  /* For 'size == 0' system allocates the user stack from the memory pool. */
  // was: rt_init_context (task_context, prio_stksz & 0xFF, task);
  // and then it became: rt_init_context (task_context, (U8)(prio_stksz & 0xFFU), task);
  rt_init_context (task_context, prio, task);

  /* Find a free entry in 'os_active_TCB' table. */
  // was: i = rt_get_TID (); // moved earlier in (**1234**)
  if(NULL != os_active_TCB)
    os_active_TCB[i-1] = task_context;
  else
    task_context = task_context;
  
  task_context->task_id = i;
  DBG_TASK_NOTIFY(task_context, __TRUE);
  rt_dispatch (task_context);
  return ((OS_TPTR)task_context);
}

// implemented: similar to rt_tsk_delete(), but uses a pointer to running task an not an id 
OS_RESULT rt_iit_tsk_delete (OS_TPTR taskp) {
  /* Terminate the task identified with "task_id". */
  P_TCB task_context = (P_TCB)taskp;
  P_TCB  p_TCB;
  P_MUCB p_MCB, p_MCB0;
  OS_TID task_id = 0;
    
  if(NULL == task_context)
    return(OS_R_NOK);
  
  task_id = task_context->task_id;

  if ((task_id == 0U) || (task_id == os_tsk.run->task_id)) {
    /* Terminate itself. */
    os_tsk.run->state     = INACTIVE;
    os_tsk.run->tsk_stack = rt_get_PSP ();
    rt_stk_check ();
    p_MCB = os_tsk.run->p_mlnk;
    while (p_MCB) {
      /* Release mutexes owned by this task */
      if (p_MCB->p_lnk) {
        /* A task is waiting for mutex. */
        p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
        rt_ret_val(p_TCB, 0U/*osOK*/);
#else
        rt_ret_val(p_TCB, OS_R_MUT); 
#endif
        rt_rmv_dly (p_TCB);
        p_TCB->state = READY;
        rt_put_prio (&os_rdy, p_TCB);
        /* A waiting task becomes the owner of this mutex. */
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 1U;
        p_MCB->owner  = p_TCB;
        p_MCB->p_mlnk = p_TCB->p_mlnk;
        p_TCB->p_mlnk = p_MCB; 
        p_MCB = p_MCB0;
      }
      else {
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 0U;
        p_MCB->owner  = NULL;
        p_MCB->p_mlnk = NULL;
        p_MCB = p_MCB0;
      }
    }
    if(NULL != os_active_TCB)
        os_active_TCB[os_tsk.run->task_id-1U] = NULL;
    if(NULL != mp_stk)
        rt_free_box (mp_stk, os_tsk.run->stack);
    else
        rt_iit_memory_relstack(os_tsk.run->stack);
    os_tsk.run->stack = NULL;
    DBG_TASK_NOTIFY(os_tsk.run, __FALSE);
    os_tsk.run->cb_type = NOCB; // useful only in case of static mode, where the memory stay unchanged. the free changes content of pointer memory
    if(NULL != mp_tcb)
    {
        task_context->ptr_perthread_libspace = NULL;   
        rt_free_box (mp_tcb, os_tsk.run);
    }
    else
    {
        if(NULL != task_context->ptr_perthread_libspace)
        {
            rt_iit_memory_del(task_context->ptr_perthread_libspace);
            task_context->ptr_perthread_libspace = NULL;
        }
        rt_iit_memory_del(os_tsk.run);
    }
    os_tsk.run = NULL;
    rt_dispatch (NULL);
    /* The program should never come to this point. */
  }
  else {
    /* Find the task in the "os_active_TCB" array. */
//    if (task_id > os_maxtaskrun || os_active_TCB[task_id-1] == NULL) {
//      /* Task with "task_id" not found or not started. */
//      return (OS_R_NOK);
//    }
//    task_context = os_active_TCB[task_id-1];
    rt_rmv_list (task_context);
    rt_rmv_dly (task_context);
    p_MCB = task_context->p_mlnk;
    while (p_MCB) {
      /* Release mutexes owned by this task */
      if (p_MCB->p_lnk) {
        /* A task is waiting for mutex. */
        p_TCB = rt_get_first ((P_XCB)p_MCB);
#ifdef __CMSIS_RTOS
        rt_ret_val (p_TCB, 0/*osOK*/);
#else
        rt_ret_val (p_TCB, OS_R_MUT); 
#endif
        rt_rmv_dly (p_TCB);
        p_TCB->state = READY;
        rt_put_prio (&os_rdy, p_TCB);
        /* A waiting task becomes the owner of this mutex. */
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 1U;
        p_MCB->owner  = p_TCB;
        p_MCB->p_mlnk = p_TCB->p_mlnk;
        p_TCB->p_mlnk = p_MCB; 
        p_MCB = p_MCB0;
      }
      else {
        p_MCB0 = p_MCB->p_mlnk;
        p_MCB->level  = 0U;
        p_MCB->owner  = NULL;
        p_MCB->p_mlnk = NULL;
        p_MCB = p_MCB0;
      }
    }
    if(NULL != os_active_TCB)
        os_active_TCB[task_id-1U] = NULL;
    if(NULL != mp_stk)
        rt_free_box (mp_stk, task_context->stack);
    else
        rt_iit_memory_relstack(task_context->stack);
    task_context->stack = NULL;
    DBG_TASK_NOTIFY(task_context, __FALSE);
    task_context->cb_type = NOCB; // useful only in case of static mode, where the memory stay unchanged. the free changes content of pointer memory
    if(NULL != mp_tcb)
    {
        task_context->ptr_perthread_libspace = NULL;
        rt_free_box (mp_tcb, task_context);
    }
    else
    {
        if(NULL == task_context->ptr_perthread_libspace)
        {
            rt_iit_memory_del(task_context->ptr_perthread_libspace);
        }
        rt_iit_memory_del(task_context);
    }
    if (rt_rdy_prio() > os_tsk.run->prio) {
      /* Ready task has higher priority than running task. */
      os_tsk.run->state = READY;
      rt_put_prio (&os_rdy, os_tsk.run);
      rt_dispatch (NULL);
    }
  }
  return (OS_R_OK);
}

extern U32 os_time;     // IIT-EXT
extern struct OS_XTMR os_tmr; // IIT-EXT

// IIT-EXT: added a proper name in uvision
FUNCP idlefptr = NULL;
void tIDLE()
{
    if(NULL != idlefptr)
    {
        idlefptr();
    }    
}
    
// implemented: it is derived from rt_sys_init()
void rt_iit_sys_start(oosiit_task_properties_t* inittsk, oosiit_task_properties_t* idletsk) 
{ 
    uint32_t i;
  
    // initialising some global variables
    os_time                 = 0;            // IIT-EXT
    os_tsk.run              = NULL;         // IIT-EXT
    os_tsk.new              = NULL;         // IIT-EXT
    osiit_init_task_started = 0;            // IIT-EXT
    oosiit_time             = 0;            // IIT-EXT
    os_tmr.next             = NULL;         // IIT-EXT: but we dont use old timers anymore
    os_tmr.tcnt             = 0;            // IIT-EXT: but we dont use old timers anymore

    // Initialize dynamic memory and task TCB pointers to NULL. 
    if(NULL != os_active_TCB)
    {
        for(i=0; i<os_maxtaskrun; i++) 
        {
            os_active_TCB[i] = NULL;
        }
    }
    if(NULL != mp_tcb)
    {
        rt_init_box(mp_tcb, mp_tcb_size, sizeof(struct OS_TCB));                // IIT-EXT: removed the & before mp_tcb because it did not make external memory work
    }
    if(NULL != mp_stk)
    {
        rt_init_box(mp_stk, mp_stk_size, BOX_ALIGN_8 | (U16)(os_stackinfo));    // IIT-EXT: removed the & before mp_stk because it did not make external memory work
    }
    if(NULL != m_tmr)
    {
        rt_init_box((U32 *)m_tmr, mp_tmr_size, sizeof(struct OS_TMR));
    }

    idlefptr = (FUNCP)idletsk->function;

    // Set up TCB of idle demon
    os_idle_TCB.task_id         = 255;
    os_idle_TCB.stack           = (uint32_t*)idletsk->stackdata;        // IIT-EXT: now it is passed externally 
    os_idle_TCB.priv_stack      = idletsk->stacksize;                   // IIT-EXT: now it is passed externally 
    os_idle_TCB.msg             = idletsk->param;                       // IIT-EXT: and has a parameter !! 
    os_idle_TCB.extdata         = idletsk->extdata;
    rt_init_context (&os_idle_TCB, 0, (FUNCP)tIDLE);                    // IIT-EXT: as well as an externally passed function
    DBG_TASK_NOTIFY(&os_idle_TCB, __TRUE);
    
    /* Set up ready list: initially empty */
    os_rdy.cb_type = HCB;
    os_rdy.p_lnk   = NULL;
    /* Set up delay list: initially empty */
    os_dly.cb_type = HCB;
    os_dly.p_dlnk  = NULL;
    os_dly.p_blnk  = NULL;
    os_dly.delta_time = 0;


    /* Fix SP and system variables to assume idle task is running  */
    /* Transform main program into idle task by assuming idle TCB */
#ifndef __CMSIS_RTOS    
    rt_set_PSP (os_idle_TCB.tsk_stack+32);
#endif
    os_tsk.run = &os_idle_TCB;
    os_tsk.run->state = RUNNING;

    /* Initialize ps queue */
    os_psq->first = 0;
    os_psq->last  = 0;
    os_psq->size  = os_fifo_size;

    rt_init_robin ();        // IIT-EXT
    
    rt_iit_advtmr_init();	            // IIT-EXT: initialising advanced timers
    
#ifndef __CMSIS_RTOS
    /* Initialize SVC and PendSV */
    rt_svc_init ();

    /* Initialize and start system clock timer */
    os_tick_irqn = os_tick_init ();
    if (os_tick_irqn >= 0) {
        OS_X_INIT((U32)os_tick_irqn);
    }

    rt_psh_req();					    // IIT-EXT: just to force a pendsv call after this function call
    
    /* Start up first user task before entering the endless loop */
    osiit_hid_tsk_create_other_args_t others;
    others.priority = inittsk->priority;
    others.stacksize = inittsk->stacksize;
    others.extdata = inittsk->extdata;    
    rt_iit_tsk_create((FUNCP)inittsk->function, inittsk->param, inittsk->stackdata, &others); 
#endif    
}




// implemented: to give a libspace to the task. this function is called by higher level function oosiit_tsk_get_perthread_libspace() or __user_perthread_libspace()
// and allocates libspace ram only when called
void* rt_iit_tsk_perthread_libspace_get(OS_TPTR taskp)
{
    //P_TCB tc = os_active_TCB[task_id-1];
    P_TCB tc = (P_TCB)taskp;
    if(NULL == tc)
    {
        return(NULL);
    }
    if(NULL == tc->ptr_perthread_libspace)
    {   // if we dont have ever initted the pointer ....
        if(NULL == std_libspace)
        {   // use heap
            tc->ptr_perthread_libspace = rt_iit_memory_new(96); // 96 because ... see arm documentation
        }
        else
        {   // use preallocated std_libspace
            tc->ptr_perthread_libspace = (void *)&std_libspace[tc->task_id-1];
        }
    }
    return(tc->ptr_perthread_libspace);  
}


// - from rt_list: rt_List.c ------------------------------------------------------------------------------------------

// changed: p_task is a proper type and times and delays are 32 bits
void iitchanged_rt_put_dly (P_TCB_Opaque p_task, TIME_t delay) {
  /* Put a task identified with "p_task" into chained delay wait list using */
  /* a delay value of "delay".                                              */
  P_TCB p;
  WIDETIME_t delta,idelay = delay;

  p = (P_TCB)&os_dly;
  if (p->p_dlnk == NULL) {
    /* Delay list empty */
    delta = 0;
    goto last;
  }
  delta = os_dly.delta_time;
  while (delta < idelay) {
    if (p->p_dlnk == NULL) {
      /* End of list found */
last: p_task->p_dlnk = NULL;
      p->p_dlnk = p_task;
      p_task->p_blnk = p;
      p->delta_time = (TIME_t)(idelay - delta);
      p_task->delta_time = 0;
      return;
    }
    p = p->p_dlnk;
    delta += p->delta_time;
  }
  /* Right place found */
  p_task->p_dlnk = p->p_dlnk;
  p->p_dlnk = p_task;
  p_task->p_blnk = p;
  if (p_task->p_dlnk != NULL) {
    p_task->p_dlnk->p_blnk = p_task;
  }
  p_task->delta_time = (TIME_t)(delta - idelay);
  p->delta_time -= p_task->delta_time;
} 

// changed: used oosiit_time instead of (U16)os_time
void iitchanged_rt_dec_dly (void) {
  /* Decrement delta time of list head: remove tasks having a value of zero.*/
  P_TCB p_rdy;

  if (os_dly.p_dlnk == NULL) {
    return;
  }
  os_dly.delta_time--;
  while ((os_dly.delta_time == 0U) && (os_dly.p_dlnk != NULL)) {
    p_rdy = os_dly.p_dlnk;
    if (p_rdy->p_rlnk != NULL) {
      /* Task is really enqueued, remove task from semaphore/mailbox */
      /* timeout waiting list. */
      p_rdy->p_rlnk->p_lnk = p_rdy->p_lnk;
      if (p_rdy->p_lnk != NULL) {
        p_rdy->p_lnk->p_rlnk = p_rdy->p_rlnk;
        p_rdy->p_lnk = NULL;
      }
      p_rdy->p_rlnk = NULL;
    }
    rt_put_prio (&os_rdy, p_rdy);
    os_dly.delta_time = p_rdy->delta_time;
    if (p_rdy->state == WAIT_ITV) {
      /* Calculate the next time for interval wait. */
      p_rdy->delta_time = p_rdy->interval_time + oosiit_time;
    }
    p_rdy->state   = READY;
    os_dly.p_dlnk = p_rdy->p_dlnk;
    if (p_rdy->p_dlnk != NULL) {
      p_rdy->p_dlnk->p_blnk = (P_TCB)&os_dly;
      p_rdy->p_dlnk = NULL;
    }
    p_rdy->p_blnk = NULL;
  }
} 



// - from rt_robin: rt_Robin.c ----------------------------------------------------------------------------------------

// redefined so that it use time w/ 32 bits
void rt_init_robin (void) {
  /* Initialize Round Robin variables. */
  os_robin.task = NULL;
  os_robin.tout = (TIME_t)os_rrobin;
}

// using oosiit_time and added a control vs not using round robin
void rt_chk_robin (void) {
  /* Check if Round Robin timeout expired and switch to the next ready task.*/
  P_TCB p_new;

  if(0 == os_rrobin_use)
  {
    return;
  }
  if (os_robin.task != os_rdy.p_lnk) {
    /* New task was suspended, reset Round Robin timeout. */
    os_robin.task = os_rdy.p_lnk;
    os_robin.time = oosiit_time + os_robin.tout - 1U;
  }
  if (os_robin.time == oosiit_time) {
    /* Round Robin timeout has expired, swap Robin tasks. */
    os_robin.task = NULL;
    p_new = rt_get_first (&os_rdy);
    rt_put_prio ((P_XCB)&os_rdy, p_new);
  }
}


// - extra functions --------------------------------------------------------------------------------------------------

void os_tmr_call(uint16_t info) 
{
  /* This function is called when the user timer has expired. Parameter   */
  /* 'info' holds the value, defined when the timer was created.          */
  info = info;
}

void os_idle_demon (void) {
  /* The idle demon is a system thread, running when no other thread is      */
  /* ready to run.                                                           */

  for (;;) {
  /* HERE: include optional user code to be executed when no thread runs.*/
  }
}
//uint32_t oosiit_error_lastone = 0xffffffff;
void os_error(uint32_t errorcode) 
{
    if( (OS_ERR_STK_OVF == errorcode) || (OS_ERR_FIFO_OVF == errorcode) || (OS_ERR_MBX_OVF == errorcode) )
    {
        oosiit_sys_error((oosiit_error_code_t) errorcode);
    }
    else if( 4 == errorcode )
    {
        oosiit_sys_error(oosiit_error_invalid_call);
    }
    else 
    {
//        oosiit_error_lastone = errorcode;
        oosiit_sys_error(oosiit_error_unknown);
    }

}



// - management of system calls with itm ------------------------------------------------------------------------------

// added funtions to signal to itm manager the execution of sys calls: systick, pendsv, svc

#ifdef OOSIIT_DBG_ENABLE

extern void oosiit_systick(void)
{
	volatile uint8_t aa = 0;
	aa = aa;
}

extern void oosiit_pendsv(void)
{
	volatile uint8_t aa = 0;
	aa = aa;
}

extern void oosiit_svc(void)
{
	volatile uint8_t aa = 0;
	aa = aa;
}

static uint8_t oosiit_dbg_initted = 0;

extern void rt_iit_dbg_init(void)
{
//   if ((DEMCR & DEMCR_TRCENA)     && 
//       (ITM_CONTROL & ITM_ITMENA) &&
//       (ITM_ENABLE & (1UL << 31))) {
//     oosiit_dbg_initted = 1;
//   } 
//   else {
//     oosiit_dbg_initted = 0;
//   } 
    if(osal_res_OK == osal_scope_init())
    {
        oosiit_dbg_initted = 1;
    } 
    else
    {
        oosiit_dbg_initted = 0;
    }
    oosiit_dbg_initted = oosiit_dbg_initted; 
}




extern void rt_iit_dbg_task_notify(void* ptcb, BOOL create)
{
    P_TCB p_tcb = (P_TCB) ptcb;
    osal_scope_ID_t id = 0;
    if(0 == oosiit_dbg_initted)
    {
        return;
    }
    
    id = (255==p_tcb->task_id) ? (ev_ID_idle) : (ev_ID_first_ostask+(U8)p_tcb->task_id-1);
    //id = ev_ID_first_ostask+p_tcb->task_id-1;

    
    if(1 == create)
    {
        osal_scope_load(id, p_tcb->ptask);
    }
    else
    {
        osal_scope_unload(id, p_tcb->ptask);
    }
}

extern void rt_iit_dbg_task_switch(U32 task_id)
{
    U8 id = (255==task_id) ? (ev_ID_idle) : (ev_ID_first_ostask+(U8)task_id-1);
    
    if((0 == oosiit_dbg_initted) || (os_tsk.new == os_tsk.run))
    {
        return;
    }
    
    osal_scope_switch_to(id);
}

extern void rt_iit_dbg_syscall_register(U8 id) 
{
  FUNCP fptr = NULL;
  uint8_t idev = 0;
  switch(id) {
    case RT_IIT_SYSCALL_ID_SYSTICK:   fptr = oosiit_systick;    idev = ev_ID_systick;   break;
	case RT_IIT_SYSCALL_ID_PENDSV:    fptr = oosiit_pendsv;     idev = ev_ID_pendsv;    break;
	case RT_IIT_SYSCALL_ID_SVC:       fptr = oosiit_svc;        idev = ev_ID_svc;       break;
	default: 				          fptr = NULL;       	    idev = 0;               break;
  };
  osal_scope_load(idev, fptr);
}

#if defined(OOSIIT_DBG_SYSTICK) | defined(OOSIIT_DBG_PENDSV) | defined(OOSIIT_DBG_SVC)

static uint8_t s_dbg_previous_id = ev_ID_idle;

//extern void rt_iit_dbg_syscall_enter (U8 id) {
//  while (ITM_PORT31_U32 == 0);
//  ITM_PORT31_U8 = id;
//}
//
extern void rt_iit_dbg_syscall_exit (void)
{
//  if(os_tsk.new==os_tsk.run)
//    dbg_task_switch(os_tsk.run->task_id);
//  else
//  ;
  if(os_tsk.new==os_tsk.run)
  {
    //uint8_t id = (255 == os_tsk.run->task_id) ? (ev_ID_idle) : (ev_ID_first_osaltask+os_tsk.run->task_id); 
    //osal_scope_switch_to(id); 
    osal_scope_switch_to(s_dbg_previous_id);
  }
  else
  {
    //s_dbg_previous_id;
  }
  //osal_scope_switch_to
}

#endif

#ifdef OOSIIT_DBG_SYSTICK
extern void rt_iit_dbg_systick_enter(void)
{
	//rt_iit_dbg_syscall_enter(RT_IIT_SYSCALL_ID_SYSTICK);
    s_dbg_previous_id = osal_scope_switch_to(ev_ID_systick);
}
extern void rt_iit_dbg_systick_exit(void)
{
 	rt_iit_dbg_syscall_exit();
    //osal_scope_switch_to(s_dbg_previous_id);
}
#else//OOSIIT_DBG_SYSTICK
extern void rt_iit_dbg_systick_enter(void){;}
extern void rt_iit_dbg_systick_exit(void){;}
#endif//OOSIIT_DBG_SYSTICK


#ifdef OOSIIT_DBG_PENDSV
extern void rt_iit_dbg_pendsv_enter(void)
{
	//rt_iit_dbg_syscall_enter(RT_IIT_SYSCALL_ID_PENDSV);
    s_dbg_previous_id = osal_scope_switch_to(ev_ID_pendsv);
}
extern void rt_iit_dbg_pendsv_exit(void)
{
 	rt_iit_dbg_syscall_exit();
    //osal_scope_switch_to(s_dbg_previous_id);
}
#else//OOSIIT_DBG_PENDSV
extern void rt_iit_dbg_pendsv_enter(void){;}
extern void rt_iit_dbg_pendsv_exit(void){;}
#endif//OOSIIT_DBG_PENDSV


#ifdef OOSIIT_DBG_SVC
extern void rt_iit_dbg_svc_enter(void)
{
	//rt_iit_dbg_syscall_enter(RT_IIT_SYSCALL_ID_SVC);
    s_dbg_previous_id = osal_scope_switch_to(ev_ID_svc);
}
extern void rt_iit_dbg_svc_exit(void)
{
 	rt_iit_dbg_syscall_exit();
    //osal_scope_switch_to(s_dbg_previous_id);
}
#else
extern void rt_iit_dbg_svc_enter(void){;}
extern void rt_iit_dbg_svc_exit(void){;}
#endif//OOSIIT_DBG_SVC


#else//OOSIIT_DBG_ENABLE is not defined

extern void rt_iit_dbg_init(void){;}

extern void rt_iit_dbg_task_notify(void* ptcb, BOOL create){;}
extern void rt_iit_dbg_task_switch(U32 task_id){;}
    
extern void rt_iit_dbg_syscall_register (U8 id){;}
//extern void rt_iit_dbg_syscall_enter (U8 id){;}
extern void rt_iit_dbg_syscall_exit (void){;}

extern void rt_iit_dbg_systick_enter(void){;}
extern void rt_iit_dbg_systick_exit(void){;}
extern void rt_iit_dbg_pendsv_enter(void){;}
extern void rt_iit_dbg_pendsv_exit(void){;}
extern void rt_iit_dbg_svc_enter(void){;}
extern void rt_iit_dbg_svc_exit(void){;}

#endif





// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------

extern void rt_iit_dbg_global_init(void)
{
#ifdef OOSIIT_DBG_ENABLE  
    rt_iit_dbg_init();              
#endif
    
#ifdef OOSIIT_DBG_SYSTICK
    rt_iit_dbg_syscall_register(RT_IIT_SYSCALL_ID_SYSTICK);		
#endif
#ifdef OOSIIT_DBG_PENDSV
    rt_iit_dbg_syscall_register(RT_IIT_SYSCALL_ID_PENDSV);		
#endif
#ifdef OOSIIT_DBG_SVC
    rt_iit_dbg_syscall_register(RT_IIT_SYSCALL_ID_SVC);			
#endif    
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


