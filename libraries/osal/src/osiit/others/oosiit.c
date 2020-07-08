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

/* @file       oosiit.c
	@brief      This file keeps hidden implementation of the oosiit. 
	@author     marco.accame@iit.it
    @date       07/23/2012
**/



// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "string.h"

//#if defined(OOSIIT_DBG_ENABLE)
#include "osal_scope.h"
//#endif

#include "oosiit_svc.h"


#include "rt_iit_changes.h"
#include "rt_iit_AdvTimer.h"
#include "rt_iit_memory.h"

#include "rtx_config.h"

#include "rt_TypeDef.h"

// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "oosiit.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------

#include "oosiit_hid.h"


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

//#define OOSIIT_USE_TIMEGET_UNDER_SVC

// used to avoid problems with the SVC declarations
#define FAKE_VOID_FP_VOIDP  void*
#define FAKE_VOID_FP_VOID   void*


#define IFN0(a, b)                  ((a)?(b):(0))
// used by: advanced timers in ram32size[0], mp_tcb in ram32size[5], normal timers m_tmr in ram32size[8]
#define SIZEINU32ofBOX4AL(sizeinbytes, cnt)           ((((sizeinbytes)+3)/4)*(cnt) + 3)
// used by: stack not externally provided in ram64size[1]
#define SIZEINU64ofBOX8AL(sizeinbytes, cnt)           ((((sizeinbytes)+7)/8)*(cnt) + 2)


#define SIZEOF_STDLIB32_ELEM            (24*4)
#define SIZEOF_ARRAY_HEAD               (sizeof(oosiit_arrayhead_t)-4)
#define SIZEOF_MUTEX_ELEM               (sizeof(struct OS_MUCB))
#define SIZEOF_SEMAPHORE_ELEM           (sizeof(struct OS_SCB))
#define SIZEOF_BASEMBOX_ELEM            (sizeof(struct OS_MCB)-4)
#define SIZEOF_POSTSERV_ELEM            (sizeof(struct OS_PSFE))
#define SIZEOF_POSTSERV_HEAD            (sizeof(struct OS_PSQ)-sizeof(struct OS_PSFE))
#define SIZEOF_TCBPOINTER               (4)
#define SIZEOF_ADVTIMER_ELEM            (sizeof(struct OSIIT_ADVTMR))
#define SIZEOF_TCB_ELEM                 (sizeof(struct OS_TCB))
#define SIZEOF_TMR_ELEM                 (sizeof(struct OS_TMR))

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------

extern const uint32_t oosiit_notimeout = OOSIIT_NOTIMEOUT;
extern const uint64_t oosiit_asaptime = OOSIIT_ASAPTIME;

// the configuration in use. it points to s_oosiit_cfg_in_use
extern const oosiit_cfg_t *oosiit_cfg_in_use = NULL;
// used for ram of objects specified in oosiit_cfg_in_use if static mode is used
extern       uint32_t *oosiit_params_ram32data = NULL;
// used for ram of objects specified in oosiit_cfg_in_use if static mode is used
extern       uint64_t *oosiit_params_ram64data = NULL;
// used for stdlib32
extern       uint32_t *oosiit_params_stdlib32data = NULL;
// the time of the system expressed in ticks
extern volatile uint64_t oosiit_time = 0;
// the duration in picosec of each unity of the systick register
extern uint64_t oosiit_picosec_per_unit_of_systick = 0;
// the max number of unity of the systick register
extern uint32_t oosiit_num_units_of_systick = 0;

// extern uint32_t oosiit_memory_sizeofallocated = 0;

// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------

static uint16_t s_oosiit_params_get_ram32size(const oosiit_cfg_t *cfg);
static uint16_t s_oosiit_params_get_ram64size(const oosiit_cfg_t *cfg);

static __INLINE void* isr_oosiit_memory_new(uint32_t size);
static __INLINE void* isr_oosiit_memory_realloc(void* m, uint32_t size);
static __INLINE oosiit_result_t isr_oosiit_memory_del(void* mem);
static __INLINE oosiit_tskptr_t isr_oosiit_tsk_self(void);
static __INLINE oosiit_result_t isr_oosiit_tsk_set_extdata(oosiit_tskptr_t tp, void* extdata);
static __INLINE void* isr_oosiit_tsk_get_extdata(oosiit_tskptr_t tp);
static __INLINE oosiit_result_t isr_oosiit_mbx_retrieve(oosiit_objptr_t mailbox, void** message);
static __INLINE void isr_oosiit_mbx_send(oosiit_objptr_t mailbox, void* message);
static __INLINE uint16_t isr_oosiit_mbx_available(oosiit_objptr_t mailbox);
static __INLINE uint16_t isr_oosiit_mbx_used(oosiit_objptr_t mailbox);
static __INLINE oosiit_result_t isr_oosiit_evt_set(uint32_t flags, oosiit_tskptr_t tskid);
static __INLINE oosiit_result_t isr_oosiit_sem_send(oosiit_objptr_t sem);
static __INLINE oosiit_result_t isr_oosiit_advtmr_start(oosiit_objptr_t timer, oosiit_advtmr_timing_t *timing, oosiit_advtmr_action_t *action);
static __INLINE oosiit_result_t isr_oosiit_advtmr_stop(oosiit_objptr_t timer);
static __INLINE oosiit_result_t isr_oosiit_advtmr_delete(oosiit_objptr_t timer);

static __INLINE oosiit_result_t s_oosiit_tsk_valid(oosiit_tskptr_t tp);
static __INLINE oosiit_result_t s_oosiit_mut_valid(oosiit_objptr_t op);
static __INLINE oosiit_result_t s_oosiit_sem_valid(oosiit_objptr_t op);
static __INLINE oosiit_result_t s_oosiit_mbx_valid(oosiit_objptr_t op);
static __INLINE oosiit_result_t s_oosiit_advtmr_valid(oosiit_objptr_t op);

#if defined(OOSIIT_USE_TIMEGET_UNDER_SVC)
static __INLINE oosiit_result_t s_oosiit_microtime_get(uint32_t* low, uint32_t* high);
static __INLINE oosiit_result_t s_oosiit_nanotime_get(uint32_t* low, uint32_t* high);
#endif

// --------------------------------------------------------------------------------------------------------------------
// - declaration of svc functions
// --------------------------------------------------------------------------------------------------------------------
//      fname                           rettype             arg1type            arg2type                arg3type        arg4type        return register   


// - memory
SVC_1_1(svc_oosiit_memory_new,          void*,             uint32_t,                                                                    RET_pointer);
SVC_2_1(svc_oosiit_memory_realloc,      void*,             void*,               uint32_t,                                               RET_pointer);
SVC_1_1(svc_oosiit_memory_del,          oosiit_result_t,   void*,                                                                       RET_int32_t);



SVC_1_1(svc_oosiit_memory_getstack,     uint64_t*,          uint16_t,                                                                   RET_pointer);



// - system
//SVC_4_1(svc_oosiit_sys_init_user,       int32_t,            void (*)(void),      uint8_t,                void*,          uint16_t,       RET_int32_t);
SVC_2_1(svc_oosiit_sys_start,           uint32_t,    oosiit_task_properties_t*, oosiit_task_properties_t*,                              RET_uint32_t);
//SVC_2_0(svc_oosiit_sys_start,           void,    oosiit_task_properties_t*, oosiit_task_properties_t*,                       RET_int32_t);
SVC_4_1(svc_oosiit_sys_init_user,       oosiit_result_t,    FAKE_VOID_FP_VOID,  uint8_t,                void*,          uint16_t,       RET_int32_t);
SVC_1_1(svc_oosiit_sys_timeofnextevent, oosiit_result_t,    uint32_t*,                                                                  RET_int32_t);
SVC_1_1(svc_oosiit_sys_suspend,         oosiit_result_t,    uint32_t*,                                                                  RET_int32_t);
SVC_1_1(svc_oosiit_sys_resume,          oosiit_result_t,    uint32_t,                                                                   RET_int32_t);


// - task
//SVC_4_1(svc_oosiit_tsk_create,          oosiit_tskptr_t,    FAKE_VOID_FP_VOIDP, void*,                  void*,          uint32_t,       RET_pointer);
SVC_4_1(svc_oosiit_tsk_create,          oosiit_tskptr_t,    FAKE_VOID_FP_VOIDP, void*,                  void*,          void*,          RET_pointer);
SVC_1_1(svc_oosiit_tsk_delete,          oosiit_result_t,    oosiit_tskptr_t,                                                            RET_int32_t);
SVC_0_1(svc_oosiit_tsk_self,            oosiit_tskptr_t,                                                                                RET_pointer);
SVC_2_1(svc_oosiit_tsk_setprio,         oosiit_result_t,    oosiit_tskptr_t,    uint8_t,                                                RET_int32_t);
SVC_0_1(svc_oosiit_tsk_pass,            oosiit_result_t,                                                                                RET_int32_t);
SVC_1_1(svc_oosiit_tsk_get_perthread_libspace, void*,       oosiit_tskptr_t,                                                            RET_pointer);
SVC_2_1(svc_oosiit_tsk_set_extdata,     oosiit_result_t,    oosiit_tskptr_t,    void*,                                                  RET_int32_t);
SVC_1_1(svc_oosiit_tsk_get_extdata,     void*,              oosiit_tskptr_t,                                                            RET_pointer);

// - time
SVC_2_1(svc_oosiit_time_set,            oosiit_result_t,    uint32_t,           uint32_t,                                               RET_int32_t);
#if defined(OOSIIT_USE_TIMEGET_UNDER_SVC)
SVC_2_1(svc_oosiit_time_get,            oosiit_result_t,    uint32_t*,          uint32_t*,                                              RET_int32_t);
SVC_2_1(svc_oosiit_microtime_get,       oosiit_result_t,    uint32_t*,          uint32_t*,                                              RET_int32_t);
SVC_2_1(svc_oosiit_nanotime_get,        oosiit_result_t,    uint32_t*,          uint32_t*,                                              RET_int32_t);
#endif


// delay
SVC_1_1(svc_oosiit_dly_wait,            oosiit_result_t,    uint32_t,                                                                   RET_int32_t);
SVC_1_1(svc_oosiit_itv_set,             oosiit_result_t,    uint32_t,                                                                   RET_int32_t);
SVC_0_1(svc_oosiit_itv_wait,            oosiit_result_t,                                                                                RET_int32_t);


// mailbox
SVC_1_1(svc_oosiit_mbx_create,          void*,              uint16_t,                                                                   RET_pointer);
SVC_3_1(svc_oosiit_mbx_retrieve,        oosiit_result_t,    void*,              void**,                 uint32_t,                       RET_int32_t);
SVC_3_1(svc_oosiit_mbx_send,            oosiit_result_t,    void*,              void*,                  uint32_t,                       RET_int32_t);
SVC_1_1(svc_oosiit_mbx_available,       uint16_t,           void*,                                                                      RET_uint32_t);
SVC_1_1(svc_oosiit_mbx_used,            uint16_t,           void*,                                                                      RET_uint32_t);
SVC_1_1(svc_oosiit_mbx_delete,          oosiit_result_t,    void*,                                                                      RET_int32_t);

// event flags 
SVC_2_1(svc_oosiit_evt_set,             oosiit_result_t,    uint32_t,           oosiit_tskptr_t,                                        RET_int32_t);
SVC_3_1(svc_oosiit_evt_wait,            oosiit_result_t,    uint32_t,           uint32_t,               oosiit_evt_wait_mode_t,         RET_int32_t);
SVC_0_1(svc_oosiit_evt_get,             uint32_t,                                                                                       RET_uint32_t);
SVC_1_1(svc_oosiit_evt_clr,             oosiit_result_t,    uint32_t,                                                                   RET_int32_t);

// semaphore
SVC_2_1(svc_oosiit_sem_create,          void*,              uint8_t,            uint8_t,                                                RET_pointer);
SVC_2_1(svc_oosiit_sem_set,             oosiit_result_t,    void*,              uint8_t,                                                RET_int32_t);
SVC_1_1(svc_oosiit_sem_send,            oosiit_result_t,    void*,                                                                      RET_int32_t);
SVC_2_1(svc_oosiit_sem_wait,            oosiit_result_t,    void*,              uint32_t,                                               RET_int32_t);
SVC_1_1(svc_oosiit_sem_delete,          oosiit_result_t,    void*,                                                                      RET_int32_t);

// mutex
SVC_0_1(svc_oosiit_mut_create,          void*,                                                                                          RET_pointer);
SVC_2_1(svc_oosiit_mut_wait,            oosiit_result_t,    void*,              uint32_t,                                               RET_int32_t);
SVC_1_1(svc_oosiit_mut_release,         oosiit_result_t,    void*,                                                                      RET_int32_t);
SVC_1_1(svc_oosiit_mut_delete,          oosiit_result_t,    void*,                                                                      RET_int32_t);

// advanced timer
SVC_0_1(svc_oosiit_advtmr_new,          void*,                                                                                          RET_pointer);
SVC_3_1(svc_oosiit_advtmr_start,        oosiit_result_t,    void*,              oosiit_advtmr_timing_t*,oosiit_advtmr_action_t*,        RET_int32_t);
SVC_1_1(svc_oosiit_advtmr_stop,         oosiit_result_t,    void*,                                                                      RET_int32_t);
SVC_1_1(svc_oosiit_advtmr_delete,       oosiit_result_t,    void*,                                                                      RET_int32_t);



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static oosiit_cfg_t s_oosiit_cfg_in_use = {0};
static volatile uint8_t s_oosiit_started = 0;

// --------------------------------------------------------------------------------------------------------------------
// - declaration of externally defined functions whuch dont have a .h file
// --------------------------------------------------------------------------------------------------------------------

extern void os_set_env(void);


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern void* oosiit_memory_new(uint32_t size)
{
    void* ret = NULL;
    
    if(0 == size)
    {
        return(NULL);
    }    
    else if(0 != __get_IPSR()) 
    {   // inside isr
        ret = isr_oosiit_memory_new(size);
    }     
    else if(1 == s_oosiit_started)
    {   // not within an isr. os started     
        ret = __svc_oosiit_memory_new(size);
    }
    else
    {   // not within an isr. os not started   
        ret = rt_iit_memory_new(size);
    }
    
    if(NULL == ret)
    {
        oosiit_sys_error(oosiit_error_memory_allocation);
    }
    
    
    return(ret);
}

extern void* oosiit_memory_realloc(void* m, uint32_t size)
{
    void* ret = NULL;
        
    if(0 == size)
    {
        return(NULL);
    }    
    else if(0 != __get_IPSR()) 
    {   // inside isr
        ret = isr_oosiit_memory_realloc(m, size);
    }     
    else if(1 == s_oosiit_started)
    {   // not within an isr. os started     
        ret = __svc_oosiit_memory_realloc(m, size);
    }
    else
    {   // not within an isr. os not started   
        ret = rt_iit_memory_realloc(m, size);
    }
    
    if(NULL == ret)
    {
        oosiit_sys_error(oosiit_error_memory_allocation);
    }
    
    
    return(ret);
}

extern oosiit_result_t oosiit_memory_del(void* mem)
{
    if(NULL == mem)
    {
        return(oosiit_res_NOK);
    }    
    else if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_memory_del(mem));
    }
    else if(1 == s_oosiit_started) 
    {   // not within an isr. os started        
        return(__svc_oosiit_memory_del(mem));
    }
    else
    {   // not within an isr. os not started  
        rt_iit_memory_del(mem);
        return(oosiit_res_OK);
    }
}

extern oosiit_result_t oosiit_memory_getsize(const oosiit_cfg_t *cfg, uint16_t *size04aligned, uint16_t *size08aligned)
{
    if(NULL == cfg)
    {
        *size04aligned = *size08aligned = 0;
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    }  

    if(oosiit_memmode_dynamic == cfg->memorymode)
    {
        *size04aligned = *size08aligned = 0;
        return(oosiit_res_OK);
    }        
    
    *size04aligned = 4*s_oosiit_params_get_ram32size(cfg);
    *size08aligned = 8*s_oosiit_params_get_ram64size(cfg);
    
    return(oosiit_res_OK);
}

extern oosiit_result_t oosiit_memory_load(const oosiit_cfg_t *cfg, uint32_t *data04aligned, uint64_t *data08aligned)
{
    if(NULL == cfg)
    {
        return(oosiit_res_NOK);
    }   

    if((oosiit_memmode_static == cfg->memorymode) && ((NULL == data04aligned) || (NULL == data08aligned)))
    {
        return(oosiit_res_NOK);
    }    

    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    
    if(1 == s_oosiit_started)
    {   // already started
        return(oosiit_res_NOK);
    }
    
    memcpy(&s_oosiit_cfg_in_use, cfg, sizeof(oosiit_cfg_t));
    oosiit_cfg_in_use            = &s_oosiit_cfg_in_use;
    
 
    // need to initialise os_flags now because os_sys_init* calls os_set_env() which use it before rt_sys_init() is called
    os_flags      = 1; //oosiit_cfg_in_use->priviledgeMode;
    
    // set the environment to privileged / unprivileged
    os_set_env();
    
    // initialise the parameters and the memory straigth away.
    if(oosiit_memmode_static == cfg->memorymode)
    {
        oosiit_params_stdlib32data   = data04aligned;
        oosiit_params_ram32data      = data04aligned + ((SIZEOF_STDLIB32_ELEM/4)*cfg->maxnumofusertasks);   
        oosiit_params_ram64data      = data08aligned;                 
        rt_iit_params_init();             
        rt_iit_memory_init();  
    }    
    else if(oosiit_memmode_dynamic == cfg->memorymode)
    {
        oosiit_params_stdlib32data   = NULL;
        oosiit_params_ram32data      = NULL;   
        oosiit_params_ram64data      = NULL;        
        rt_iit_dynamic_mode_init(cfg);                  
    }
    else
    {   // error    
        oosiit_sys_error(oosiit_error_invalid_param);
    }

    return(oosiit_res_OK);
}


// - system functions -------------------------------------------------------------------------------------------------


extern oosiit_result_t oosiit_sys_start(oosiit_task_properties_t* tskinit, oosiit_task_properties_t* tskidle)
{
    if(1 == s_oosiit_started)
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else if((NULL == tskinit) || (NULL == tskidle) )
    {
        return(oosiit_res_NOK);
    }
    else if((NULL == tskinit->function) || (0 == tskinit->stacksize) || (NULL == tskinit->stackdata) ||
            (NULL == tskidle->function) || (0 == tskidle->stacksize) || (NULL == tskidle->stackdata) )
    { 
        return(oosiit_res_NOK);
    }
    else
    {
        // set the environment to privileged / unprivileged
        os_set_env();
        // set the os as started
        s_oosiit_started = 1;
        // init debug (if any)
        rt_iit_dbg_global_init();
        // call svc to start the system
        __svc_oosiit_sys_start(tskinit, tskidle);
    }
    
    // but in here it never goes because the __svc_oosiit_sys_start() never returns
    return(oosiit_res_OK);    
}

extern oosiit_result_t oosiit_sys_init(void (*inittskfn)(void), uint8_t inittskpriority, void* inittskstackdata, uint16_t inittskstacksize)
{   // old mode to start the system
    if(1 == s_oosiit_started)
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else if((NULL == inittskfn) || (NULL == inittskstackdata) || (0 == inittskstacksize) )
    {
        return(oosiit_res_NOK);
    }
    else
    { 
        // set the environment to privileged / unprivileged
        os_set_env();
        // set the os as started
        s_oosiit_started = 1;        
        // call svc
        __svc_oosiit_sys_init_user(inittskfn, inittskpriority, inittskstackdata, inittskstacksize);
    }
    
    // but in here it never goes because the __svc_oosiit_sys_init_user() never returns
    return(oosiit_res_OK);
}

extern uint32_t oosiit_sys_timeofnextevent(void)
{        
    uint32_t tt = 0;
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(0);
    } 
    else
    {   // call svc
        __svc_oosiit_sys_timeofnextevent(&tt);
    }
    
    return(tt);    
}

extern oosiit_result_t oosiit_sys_suspend(uint32_t *timeofnextevent)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
        __svc_oosiit_sys_suspend(timeofnextevent);
    }
    
    return(oosiit_res_OK);
}


extern oosiit_result_t oosiit_sys_resume(uint32_t timeofsuspension)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
        __svc_oosiit_sys_resume(timeofsuspension);
    }
    
    return(oosiit_res_OK);
}

__weak void extern oosiit_sys_error(oosiit_error_code_t errorcode)
{
    for(;;);
}

// - miscellanea ------------------------------------------------------------------------------------------------------

extern uint64_t* oosiit_memory_getstack(uint16_t bytes)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(NULL);
    } 
    else if(1 == s_oosiit_started)
    {   // call svc
        return(__svc_oosiit_memory_getstack(bytes));
    }
    else
    {   // dont call svc
        return(rt_iit_memory_getstack(bytes));
    }
    
}


// - task functions ---------------------------------------------------------------------------------------------------

extern oosiit_tskptr_t oosiit_tsk_create(oosiit_task_properties_t* tskprop)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(NULL);
    } 
    else if((NULL == tskprop) || (NULL == tskprop->function) || (NULL == tskprop->stackdata) || (0 == tskprop->stacksize) )
    {
        return(NULL);
    }
    else
    {   // call svc
        osiit_hid_tsk_create_other_args_t others;
        others.stacksize = tskprop->stacksize;
        others.priority = tskprop->priority;
        others.extdata = tskprop->extdata;
        return(__svc_oosiit_tsk_create(tskprop->function, tskprop->param, tskprop->stackdata, &others));
    }   
}


extern oosiit_result_t oosiit_tsk_delete(oosiit_tskptr_t tp)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_delete(tp));
    } 
}

extern oosiit_tskptr_t oosiit_tsk_self(void)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_tsk_self());
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_self());
    } 
}

extern oosiit_result_t oosiit_tsk_setprio(oosiit_tskptr_t tp, uint8_t tskpriority)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        //return(isr_oosiit_tsk_prio(tp, tskpriority));
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_setprio(tp, tskpriority));
    }   
}

extern oosiit_result_t oosiit_tsk_pass(void)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        //return(isr_oosiit_tsk_pass());
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_pass());
    }   
}

extern void* oosiit_tsk_get_perthread_libspace(oosiit_tskptr_t tp)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(NULL);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(rt_iit_tsk_perthread_libspace_get(tp));
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_get_perthread_libspace(tp));
    }     
}

extern oosiit_result_t oosiit_tsk_set_extdata(oosiit_tskptr_t tp, void* extdata)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_tsk_set_extdata(tp, extdata));
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_set_extdata(tp, extdata));
    }       
}


extern void* oosiit_tsk_get_extdata(oosiit_tskptr_t tp)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(NULL);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_tsk_get_extdata(tp));
    } 
    else
    {   // call svc
        return(__svc_oosiit_tsk_get_extdata(tp));
    }        
}

extern uint8_t oosiit_tsk_get_id(oosiit_tskptr_t tp)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(0);
    } 

    P_TCB task_context = (P_TCB)tp;
    if(255 == task_context->task_id)
    {
        return(ev_ID_idle);
    }
    return(task_context->task_id+ev_ID_first_ostask-1);  
}



// - time management functions ----------------------------------------------------------------------------------------

extern oosiit_result_t oosiit_time_set(uint64_t target)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        //return(isr_oosiit_tsk_pass());
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
        return(__svc_oosiit_time_set(target&0xffffffff, (target>>32)&0xffffffff));
    }   
}


#if     defined(OOSIIT_USE_TIMEGET_UNDER_SVC)

extern uint64_t oosiit_time_get(void)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_time);
    } 
    else
    {   // call svc
        uint32_t low = 0;
        uint32_t high = 0;
        uint64_t res = 0;
        __svc_oosiit_time_get(&low, &high);
        res = high;
        res <<= 32;
        res += low;
        return(res);
    }   
}


extern uint64_t oosiit_microtime_get(void)
{       
    uint32_t low = 0;
    uint32_t high = 0;
    uint64_t res = 0;
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        s_oosiit_microtime_get(&low, &high);
    } 
    else
    {   // call svc
        __svc_oosiit_microtime_get(&low, &high);
    }  
    
    res = high;
    res <<= 32;
    res += low;
    return(res);    
}

extern uint64_t oosiit_nanotime_get(void)
{
    uint32_t low = 0;
    uint32_t high = 0;
    uint64_t res = 0;
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        s_oosiit_nanotime_get(&low, &high);
    } 
    else
    {   // call svc
        __svc_oosiit_nanotime_get(&low, &high);
    }  
    
    res = high;
    res <<= 32;
    res += low;
    return(res); 
}

#else//!defined(OOSIIT_USE_TIMEGET_UNDER_SVC)

extern uint64_t oosiit_time_get(void)
{
    return(oosiit_time);
}

extern uint64_t oosiit_microtime_get(void)
{
    uint64_t microsecs;
    uint64_t tmp;
    volatile uint32_t reg0xE000E018 = *((volatile uint32_t *)0xE000E018);
    
    // add to microsecs the content of register systick_current_value_reg properly scaled.

    // tmp is in pico
    tmp = oosiit_picosec_per_unit_of_systick * (oosiit_num_units_of_systick - reg0xE000E018);
    tmp /= 1000000LL; // now micro
    microsecs = (oosiit_time * oosiit_cfg_in_use->ticktime) + tmp;

    return(microsecs);    
}

extern uint64_t oosiit_nanotime_get(void)
{
    uint64_t nanosecs;
    volatile uint32_t reg0xE000E018 = *((volatile uint32_t *)0xE000E018);

    // add to nanosecs the content of register systick_current_value_reg properly scaled.

    nanosecs = oosiit_picosec_per_unit_of_systick * (oosiit_num_units_of_systick - reg0xE000E018);
    nanosecs /= 1000; // before it was pico, now it is nano
    nanosecs += (oosiit_time * oosiit_cfg_in_use->ticktime * 1000LL);

    return(nanosecs);
}



#endif//!defined(OOSIIT_USE_TIMEGET_UNDER_SVC)


// - delay functions --------------------------------------------------------------------------------------------------

extern void oosiit_dly_wait(uint32_t delay)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return;
    } 
    else
    {   // call svc
        __svc_oosiit_dly_wait(delay);
    }
}

extern void oosiit_itv_set(uint32_t period)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return;
    } 
    else
    {   // call svc
        __svc_oosiit_itv_set(period);
    }
}

extern void oosiit_itv_wait(void)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return;
    } 
    else
    {   // call svc
        __svc_oosiit_itv_wait();
    }
}


// - mailbox functions ------------------------------------------------------------------------------------------------

extern oosiit_objptr_t oosiit_mbx_create(uint16_t numofmessages)
{   // get memory and initialises it
    
    if(0 == numofmessages)
    {
        return(NULL);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(NULL);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mbx_create(numofmessages));
    }
}

extern oosiit_result_t oosiit_mbx_delete(oosiit_objptr_t mailbox)
{
    if(oosiit_res_NOK == s_oosiit_mbx_valid(mailbox))
    {
        return(oosiit_res_NOK);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mbx_delete(mailbox));
    }         
}

extern oosiit_result_t oosiit_mbx_retrieve(oosiit_objptr_t mailbox, void** message, uint32_t timeout)
{
    if(oosiit_res_NOK == s_oosiit_mbx_valid(mailbox))
    {
        return(oosiit_res_NOK);
    } 
    
    if(NULL == message)
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_mbx_retrieve(mailbox, message));
    } 
    else
    {   // call svc
         return(__svc_oosiit_mbx_retrieve(mailbox, message, timeout));
    }
}

extern oosiit_result_t oosiit_mbx_send(oosiit_objptr_t mailbox, void* message, uint32_t timeout)
{
    if(oosiit_res_NOK == s_oosiit_mbx_valid(mailbox))
    {
        return(oosiit_res_NOK);
    } 
 
// i must be able to send messages which are zero, thus i remove the control.   
//     if(NULL == message)
//     {
//         return(oosiit_res_NOK);
//     }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        isr_oosiit_mbx_send(mailbox, message);
        return(oosiit_res_OK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mbx_send(mailbox, message, timeout));
    }
}

extern uint16_t oosiit_mbx_available(oosiit_objptr_t mailbox)
{
    if(oosiit_res_NOK == s_oosiit_mbx_valid(mailbox))
    {
        return(0);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_mbx_available(mailbox));
    } 
    else
    {   // call svc
         return(__svc_oosiit_mbx_available(mailbox));
    }
}

extern uint16_t oosiit_mbx_used(oosiit_objptr_t mailbox)
{
    if(oosiit_res_NOK == s_oosiit_mbx_valid(mailbox))
    {
        return(0);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_mbx_used(mailbox));
    } 
    else
    {   // call svc
         return(__svc_oosiit_mbx_used(mailbox));
    }
}


// - event flag functions ---------------------------------------------------------------------------------------------


extern oosiit_result_t oosiit_evt_set(uint32_t flags, oosiit_tskptr_t tp)
{
    if(oosiit_res_NOK == s_oosiit_tsk_valid(tp))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_evt_set(flags, tp));
    } 
    else
    {   // call svc
        return(__svc_oosiit_evt_set(flags, tp));
    }
}

extern oosiit_result_t oosiit_evt_wait(uint32_t waitflags,  uint32_t timeout, oosiit_evt_wait_mode_t waitmode)
{   
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_evt_wait(waitflags, timeout, waitmode));
    }
}

extern uint32_t oosiit_evt_get(void)
{   
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(0);
    } 
    else
    {   // call svc
         return(__svc_oosiit_evt_get());
    }
}

extern oosiit_result_t oosiit_evt_clr(uint32_t flags)
{
    if(0 == flags)
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_evt_clr(flags));
    }
}


// - semaphore functions ----------------------------------------------------------------------------------------------

extern oosiit_objptr_t oosiit_sem_create(uint8_t maxtokens, uint8_t ntokens)
{   // get memory and initialises it
    
    if(0 == maxtokens)
    {
        return(NULL);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(NULL);
    } 
    else
    {   // call svc
         return(__svc_oosiit_sem_create(maxtokens, ntokens));
    }
}

extern oosiit_result_t oosiit_sem_set(oosiit_objptr_t sem, uint8_t ntokens)
{
    if(oosiit_res_NOK == s_oosiit_sem_valid(sem))
    {
        return(oosiit_res_NOK);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_sem_set(sem, ntokens));
    }
}

extern oosiit_result_t oosiit_sem_send(oosiit_objptr_t sem)
{
    if(oosiit_res_NOK == s_oosiit_sem_valid(sem))
    {
        return(oosiit_res_NOK);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_sem_send(sem));
    } 
    else
    {   // call svc
         return(__svc_oosiit_sem_send(sem));
    }
}

extern oosiit_result_t oosiit_sem_wait(oosiit_objptr_t sem, uint32_t timeout)
{
    if(oosiit_res_NOK == s_oosiit_sem_valid(sem))
    {
        return(oosiit_res_NOK);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_sem_wait(sem, timeout));
    }
}

extern oosiit_result_t oosiit_sem_delete(oosiit_objptr_t sem)
{
    if(oosiit_res_NOK == s_oosiit_sem_valid(sem))
    {
        return(oosiit_res_NOK);
    } 
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_sem_delete(sem));
    }    
}


// - mutex functions --------------------------------------------------------------------------------------------------

extern oosiit_objptr_t oosiit_mut_create(void)
{   // get memory and initialises it
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(NULL);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mut_create());
    }
}

extern oosiit_result_t oosiit_mut_wait(oosiit_objptr_t mutex, uint32_t timeout)
{
    if(oosiit_res_NOK == s_oosiit_mut_valid(mutex))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mut_wait(mutex, timeout));
    }
}

extern oosiit_result_t oosiit_mut_release(oosiit_objptr_t mutex)
{
    if(oosiit_res_NOK == s_oosiit_mut_valid(mutex))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mut_release(mutex));
    }
}

extern oosiit_result_t oosiit_mut_delete(oosiit_objptr_t mutex)
{
    if(oosiit_res_NOK == s_oosiit_mut_valid(mutex))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(oosiit_res_NOK);
    } 
    else
    {   // call svc
         return(__svc_oosiit_mut_delete(mutex));
    }    
}


// - advanced timer functions -----------------------------------------------------------------------------------------

extern oosiit_objptr_t oosiit_advtmr_new(void)
{
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(NULL);
    } 
    else
    {   // call svc
         return(__svc_oosiit_advtmr_new());
    }
}

extern oosiit_result_t oosiit_advtmr_start(oosiit_objptr_t timer, oosiit_advtmr_timing_t *timing, oosiit_advtmr_action_t *action)
{
    if(oosiit_res_NOK == s_oosiit_advtmr_valid(timer))
    {
        return(oosiit_res_NOK);
    }    
    
    if((NULL == timing) || (NULL == action))
    {
        return(oosiit_res_NOK);
    }
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_advtmr_start(timer, timing, action));
    } 
    else
    {   // call svc
         return(__svc_oosiit_advtmr_start(timer, timing, action));
    }
}

extern oosiit_result_t oosiit_advtmr_stop(oosiit_objptr_t timer)
{
    if(oosiit_res_NOK == s_oosiit_advtmr_valid(timer))
    {
        return(oosiit_res_NOK);
    }  
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_advtmr_stop(timer));
    } 
    else
    {   // call svc
         return(__svc_oosiit_advtmr_stop(timer));
    }
}

extern oosiit_result_t oosiit_advtmr_isactive(oosiit_objptr_t timer)
{
    if(oosiit_res_NOK == s_oosiit_advtmr_valid(timer))
    {
        return(oosiit_res_NOK);
    }  
    
    return((OS_R_OK == rt_iit_advtmr_isactive(timer)) ? (oosiit_res_OK) : (oosiit_res_NOK));
    
//    if(0 != __get_IPSR()) 
//    {   // inside isr
//        return(isr_oosiit_advtmr_isactive(timer));
//    } 
//    else
//    {   // call svc
//         return(__svc_oosiit_advtmr_isactive(timer));
//    }
}

extern oosiit_result_t oosiit_advtmr_delete(oosiit_objptr_t timer)
{
    if(oosiit_res_NOK == s_oosiit_advtmr_valid(timer))
    {
        return(oosiit_res_NOK);
    }  
    
    if(0 != __get_IPSR()) 
    {   // inside isr
        return(isr_oosiit_advtmr_delete(timer));
    } 
    else
    {   // call svc
         return(__svc_oosiit_advtmr_delete(timer));
    }
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------

extern uint16_t oosiit_hid_params_get_stdlib32(const oosiit_cfg_t *cfg, uint16_t *stdlib32size)
{
    uint16_t stdlib32datasizetot = 0;

    if(NULL == cfg)
    {
        return(0);
    }

    stdlib32size[0] = (SIZEOF_STDLIB32_ELEM/4)*cfg->maxnumofusertasks;
    stdlib32datasizetot += stdlib32size[0];

    return(stdlib32datasizetot);
}

extern uint16_t oosiit_hid_params_get_ram32(const oosiit_cfg_t *cfg, uint16_t *ram32size)
{
    uint16_t ram32datasizetot = 0;

    if(NULL == cfg)
    {
        return(0);
    }
    // the following use membox 4-aligned: adv-timers [0], task-control-block [5], timers [8]
    ram32size[0] = (SIZEOF_ARRAY_HEAD/4 + cfg->numAdvTimer) + IFN0(cfg->numAdvTimer, SIZEINU32ofBOX4AL(SIZEOF_ADVTIMER_ELEM, cfg->numAdvTimer));  
    ram32datasizetot += ram32size[0];
    ram32size[1] = (SIZEOF_MUTEX_ELEM/4)*cfg->numMutex;   
    ram32datasizetot += ram32size[1];
    ram32size[2] = (SIZEOF_SEMAPHORE_ELEM/4)*cfg->numSemaphore;       
    ram32datasizetot += ram32size[2];
    ram32size[3] = IFN0(cfg->numMessageBox*cfg->numMessageBoxElements, (SIZEOF_BASEMBOX_ELEM/4)*cfg->numMessageBox+cfg->numMessageBoxElements); 
    ram32datasizetot += ram32size[3];
    ram32size[4] = IFN0(cfg->numMutex+cfg->numSemaphore+cfg->numMessageBox+cfg->sizeof64alignedStack, (SIZEOF_MUTEX_ELEM/4));    
    ram32datasizetot += ram32size[4];
    ram32size[5] = IFN0(cfg->maxnumofusertasks+1, SIZEINU32ofBOX4AL(SIZEOF_TCB_ELEM, cfg->maxnumofusertasks+1)); 
    ram32datasizetot += ram32size[5];
    ram32size[6] = cfg->capacityofpostpendcommandfifo*(SIZEOF_POSTSERV_ELEM/4) + (SIZEOF_POSTSERV_HEAD/4);      
    ram32datasizetot += ram32size[6];
    ram32size[7] = (SIZEOF_TCBPOINTER/4)*(cfg->maxnumofusertasks+2);
    ram32datasizetot += ram32size[7];
    //ram32size[8] = IFN0(cfg->numTimer, SIZEINU32ofBOX4AL(SIZEOF_TMR_ELEM, cfg->numTimer));
    ram32size[8] = IFN0(0, SIZEINU32ofBOX4AL(SIZEOF_TMR_ELEM, 0));
    ram32datasizetot += ram32size[8];

    return(ram32datasizetot);
}

extern uint16_t oosiit_hid_params_get_ram64(const oosiit_cfg_t *cfg, uint16_t *ram64size)
{
    uint16_t ram64datasizetot = 0;

    if(NULL == cfg)
    {
        return(0);
    }

    // the following use membox 8-aligned: stack not externally provided [1]
    ram64size[0] = (cfg->sizeof64alignedStack+7)/8;    
    ram64datasizetot += ram64size[0];
    //ram64size[1] = SIZEINU64ofBOX8AL(4*cfg->sizeStack, cfg->maxnumofusertasks-cfg->numTaskWithUserProvidedStack+1);
    ram64size[1] = 0;
    ram64datasizetot += ram64size[1];

    return(ram64datasizetot);
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static uint16_t s_oosiit_params_get_ram32size(const oosiit_cfg_t *cfg)
{
    uint16_t ram32datasizetot = 0;

    if(NULL == cfg)
    {
        return(0);
    }

    // for oosiit_params_stdlib32data
    ram32datasizetot += ((SIZEOF_STDLIB32_ELEM/4)*cfg->maxnumofusertasks);   
    
    // the following use membox 4-aligned: adv-timers [0], task-control-block [5], timers [8]
    // for oosiit_params_ram32data
    // 0. advanced timers
    ram32datasizetot += ((SIZEOF_ARRAY_HEAD/4 + cfg->numAdvTimer) + IFN0(cfg->numAdvTimer, SIZEINU32ofBOX4AL(SIZEOF_ADVTIMER_ELEM, cfg->numAdvTimer)));
     // 1. mutex
    ram32datasizetot += (SIZEOF_MUTEX_ELEM/4)*cfg->numMutex; 
    // 2. semaphore           
    ram32datasizetot += (SIZEOF_SEMAPHORE_ELEM/4)*cfg->numSemaphore;
    // 3. mbox        
    ram32datasizetot += IFN0(cfg->numMessageBox*cfg->numMessageBoxElements, (SIZEOF_BASEMBOX_ELEM/4)*cfg->numMessageBox+cfg->numMessageBoxElements); 
    // 4. mutex for ram
    ram32datasizetot += IFN0(cfg->numMutex+cfg->numSemaphore+cfg->numMessageBox+cfg->sizeof64alignedStack, (SIZEOF_MUTEX_ELEM/4));  
    // 5. memory for tcb: we need one for each user task plus one for the init (the idle use a dedicated one)
    ram32datasizetot += IFN0(cfg->maxnumofusertasks+1, SIZEINU32ofBOX4AL(SIZEOF_TCB_ELEM, cfg->maxnumofusertasks+1));
    // 6. isr post service
    ram32datasizetot += cfg->capacityofpostpendcommandfifo*(SIZEOF_POSTSERV_ELEM/4) + (SIZEOF_POSTSERV_HEAD/4);  
    // 7. pointers to task control blocks: one for each user task plus one per idle task plus one per init task
    ram32datasizetot += (SIZEOF_TCBPOINTER/4)*(cfg->maxnumofusertasks+2); 
    // 8. timers  
    ram32datasizetot += IFN0(0, SIZEINU32ofBOX4AL(SIZEOF_TMR_ELEM, 0));

    return(ram32datasizetot);
}

static uint16_t s_oosiit_params_get_ram64size(const oosiit_cfg_t *cfg)
{
    uint16_t ram64datasizetot = 0;

    if(NULL == cfg)
    {
        return(0);
    }

    // the following use membox 8-aligned: stack not externally provided [1]
    ram64datasizetot += (cfg->sizeof64alignedStack+7)/8; 

    return(ram64datasizetot);
}

// ------------------------------------------- ovverridden functions ---------------------------------------------------

void rt_stk_check(void) 
{  
    if(0 == s_oosiit_cfg_in_use.checkstackoverflow)
    {
        return;
    }
    
    /* Check for stack overflow. */
    if((os_tsk.run->tsk_stack < (U32)os_tsk.run->stack) || (os_tsk.run->stack[0] != MAGIC_WORD)) 
    {
        os_error (OS_ERR_STK_OVF);
    }
}    

// ------------------------------------------- svc functions -----------------------------------------------------------


// - memory

extern void* svc_oosiit_memory_new(uint32_t size)
{
    return(oosiit_ext_calloc(size, 1));
}

extern void* svc_oosiit_memory_realloc(void* m, uint32_t size)
{
    return(oosiit_ext_realloc(m, size));
}

extern oosiit_result_t svc_oosiit_memory_del(void* mem)
{
    oosiit_ext_free(mem);
    return(oosiit_res_OK);
}

extern uint64_t* svc_oosiit_memory_getstack(uint16_t bytes)
{
    uint64_t *ret;
    rt_iit_dbg_svc_enter();
    
    ret = rt_iit_memory_getstack(bytes);
    
    rt_iit_dbg_svc_exit();
    return(ret);
}



// - system 


extern uint32_t svc_oosiit_sys_start(oosiit_task_properties_t* inittsk, oosiit_task_properties_t* idletsk)
{ 
    rt_iit_dbg_svc_enter();
     
    rt_iit_sys_start(inittsk, idletsk);
    
    rt_iit_dbg_svc_exit();
    
    // trick to have in the stack of idle_task the param in the position of r0.
    // follow with debugger hal_cm3.c after return of the function to discover why.
    return((uint32_t)idletsk->param);
}


extern oosiit_result_t svc_oosiit_sys_init_user(FAKE_VOID_FP_VOID inittskfn, uint8_t inittskpriority, void* inittskstackdata, uint16_t inittskstacksize)
{   // void (*inittskfn)(void)
    uint32_t stacksize24priority08 = ((((uint32_t)(inittskstacksize)) << 8)&0xffffff00) | (inittskpriority&0xff);
    
    rt_iit_dbg_svc_enter();
     
    rt_sys_init((FUNCP)inittskfn, stacksize24priority08, inittskstackdata);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}


extern oosiit_result_t svc_oosiit_sys_timeofnextevent(uint32_t *timeofnextevent)
{
    rt_iit_dbg_svc_enter();
    //#warning --> now rtx has a new rt_suspend() ...
    // rt_tsk_lock();
    uint32_t tt = rt_iit_timeofnextevent(0);
    
    if(NULL != timeofnextevent)
    {
        *timeofnextevent = tt;
    }
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern oosiit_result_t svc_oosiit_sys_suspend(uint32_t *timeofnextevent)
{
    rt_iit_dbg_svc_enter();
    //#warning --> now rtx has a new rt_suspend() ...
    // rt_tsk_lock();
    uint32_t tt = rt_iit_timeofnextevent(1); // or ... rt_suspend();
    
    if(NULL != timeofnextevent)
    {
        *timeofnextevent = tt;
    }
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern oosiit_result_t svc_oosiit_sys_resume(uint32_t timeofsuspension)
{
    rt_iit_dbg_svc_enter();
    //#warning --> now rtx has a new rt_resume() ...
   // rt_tsk_unlock();
    rt_resume(timeofsuspension);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}


// - task


extern oosiit_tskptr_t svc_oosiit_tsk_create(FAKE_VOID_FP_VOIDP tskfn, void *tskfnarg, void *tskstackdata, void* others)
{   // rt_tsk_create_user_ex has tskfnarg as last argument
    // void (*tskfn)(void *)
    oosiit_tskptr_t tp;
    rt_iit_dbg_svc_enter();
       
    tp = rt_iit_tsk_create((FUNCP)tskfn, tskfnarg, tskstackdata, others);
       
    rt_iit_dbg_svc_exit();
    return(tp);
}

extern oosiit_result_t svc_oosiit_tsk_delete(oosiit_tskptr_t tp)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
    
    res = rt_iit_tsk_delete(tp);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);
}

extern oosiit_tskptr_t svc_oosiit_tsk_self(void)
{
    oosiit_tskptr_t tp;
    rt_iit_dbg_svc_enter();
    
    tp = rt_iit_tsk_self();
    
    rt_iit_dbg_svc_exit();
    return(tp);
}

extern oosiit_result_t svc_oosiit_tsk_setprio(oosiit_tskptr_t tp, uint8_t tskpriority)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
    
    res = rt_iit_tsk_prio(tp, tskpriority);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);
}

extern oosiit_result_t svc_oosiit_tsk_pass(void)
{
    rt_iit_dbg_svc_enter();
    
    rt_tsk_pass();
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern void* svc_oosiit_tsk_get_perthread_libspace(oosiit_tskptr_t tp)
{
    return(rt_iit_tsk_perthread_libspace_get(tp));
}


extern oosiit_result_t svc_oosiit_tsk_set_extdata(oosiit_tskptr_t tp, void* extdata)
{
    rt_iit_dbg_svc_enter();
    
    isr_oosiit_tsk_set_extdata(tp, extdata);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern void* svc_oosiit_tsk_get_extdata(oosiit_tskptr_t tp)
{
    void* ret = NULL;
    rt_iit_dbg_svc_enter();
    
    ret = isr_oosiit_tsk_get_extdata(tp);
    
    rt_iit_dbg_svc_exit();
    return(ret);
}


// time

extern oosiit_result_t svc_oosiit_time_set(uint32_t low, uint32_t high)
{
    rt_iit_dbg_svc_enter();
    
    rt_iit_time_set(low, high);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

#if     defined(OOSIIT_USE_TIMEGET_UNDER_SVC)

extern oosiit_result_t svc_oosiit_time_get(uint32_t* low, uint32_t* high)
{
    rt_iit_dbg_svc_enter();
    
    *low = oosiit_time & 0xffffffff;
    *high = (oosiit_time >> 32) & 0xffffffff;
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern oosiit_result_t svc_oosiit_microtime_get(uint32_t* low, uint32_t* high)
{
    rt_iit_dbg_svc_enter();
    s_oosiit_microtime_get(low, high);
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);    
}


extern oosiit_result_t svc_oosiit_nanotime_get(uint32_t* low, uint32_t* high)
{
    rt_iit_dbg_svc_enter();
    s_oosiit_nanotime_get(low, high);
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);    
}

#endif//defined(OOSIIT_USE_TIMEGET_UNDER_SVC)


// delay

extern oosiit_result_t svc_oosiit_dly_wait(uint32_t delay)
{
    rt_iit_dbg_svc_enter();
    
    iitchanged_rt_dly_wait(delay);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern oosiit_result_t svc_oosiit_itv_set(uint32_t period)
{
    rt_iit_dbg_svc_enter();
    
    iitchanged_rt_itv_set(period);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern oosiit_result_t svc_oosiit_itv_wait(void)
{
    rt_iit_dbg_svc_enter();
    
    iitchanged_rt_itv_wait();
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}


// mailbox

extern void* svc_oosiit_mbx_create(uint16_t numofmessages)
{
    void *ret;
    rt_iit_dbg_svc_enter();
    
    ret = rt_iit_memory_getmbx(numofmessages);
    if(NULL != ret)
    {
        iitchanged_rt_mbx_init(ret, numofmessages);
    }
    
    rt_iit_dbg_svc_exit();
    return(ret);   
}


extern oosiit_result_t svc_oosiit_mbx_delete(oosiit_objptr_t mailbox)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_iit_mbx_delete(mailbox);
    
    rt_iit_memory_relmbx(mailbox);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}

extern oosiit_result_t svc_oosiit_mbx_retrieve(oosiit_objptr_t mailbox, void** message, uint32_t timeout)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
    
    res = iitchanged_rt_mbx_wait(mailbox, message, timeout);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);
}

extern oosiit_result_t svc_oosiit_mbx_send(oosiit_objptr_t mailbox, void* message, uint32_t timeout)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
    
    res = iitchanged_rt_mbx_send(mailbox, message, timeout);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);
}

extern uint16_t svc_oosiit_mbx_available(oosiit_objptr_t mailbox)
{
    uint16_t res;
    rt_iit_dbg_svc_enter();
    
    res = (uint16_t)rt_mbx_check(mailbox);
    
    rt_iit_dbg_svc_exit();
    return(res);
}

extern uint16_t svc_oosiit_mbx_used(oosiit_objptr_t mailbox)
{
    uint16_t res;
    rt_iit_dbg_svc_enter();
    
    res = (uint16_t)rt_iit_mbx_count(mailbox);
    
    rt_iit_dbg_svc_exit();
    return(res);
}

// events

extern oosiit_result_t svc_oosiit_evt_set(uint32_t flags, oosiit_tskptr_t tp)
{
    rt_iit_dbg_svc_enter();
    
    iitchanged_rt_evt_set(flags, tp);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);
}

extern oosiit_result_t svc_oosiit_evt_wait(uint32_t waitflags,  uint32_t timeout, oosiit_evt_wait_mode_t waitmode)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
    
    res = iitchanged_rt_evt_wait(waitflags, timeout, (oosiit_evt_wait_mode_all == waitmode) ? (1) : (0));
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);
}

extern uint32_t svc_oosiit_evt_get(void)
{
    uint32_t res;
    rt_iit_dbg_svc_enter();
    
    res = iitchanged_rt_evt_get();
    
    rt_iit_dbg_svc_exit();
    return(res);
}


extern oosiit_result_t svc_oosiit_evt_clr(uint32_t flags)
{
    rt_iit_dbg_svc_enter();
    
    iitchanged_rt_evt_clr_runningtask(flags);
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_NOK);
}


// semaphore

extern void* svc_oosiit_sem_create(uint8_t maxtokens, uint8_t ntokens)
{
    void *ret;
    rt_iit_dbg_svc_enter();
    
    ret = rt_iit_memory_getsem();
    if(NULL != ret)
    {
        iitchanged_rt_sem_init(ret, maxtokens, ntokens);  
    }
    
    rt_iit_dbg_svc_exit();
    return(ret);   
}


extern oosiit_result_t svc_oosiit_sem_set(oosiit_objptr_t sem, uint8_t ntokens)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_iit_sem_set(sem, ntokens);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}


extern oosiit_result_t svc_oosiit_sem_send(oosiit_objptr_t sem)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = iitchanged_rt_sem_send(sem);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}


extern oosiit_result_t svc_oosiit_sem_wait(oosiit_objptr_t sem, uint32_t timeout)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = iitchanged_rt_sem_wait(sem, timeout);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}

extern oosiit_result_t svc_oosiit_sem_delete(oosiit_objptr_t sem)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_iit_sem_delete(sem);
    
    rt_iit_memory_relsem(sem);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}

// mutex

extern void* svc_oosiit_mut_create(void)
{
    void *ret;
    rt_iit_dbg_svc_enter();
    
    ret = rt_iit_memory_getmut();
    if(NULL != ret)
    {
        rt_mut_init(ret);  
    }
    
    rt_iit_dbg_svc_exit();
    return(ret);   
}

extern oosiit_result_t svc_oosiit_mut_wait(oosiit_objptr_t mutex, uint32_t timeout)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = iitchanged_rt_mut_wait(mutex, timeout);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}

extern oosiit_result_t svc_oosiit_mut_release(oosiit_objptr_t mutex)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_mut_release(mutex);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}

extern oosiit_result_t svc_oosiit_mut_delete(oosiit_objptr_t mutex)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_mut_delete(mutex);
    rt_iit_memory_relmut(mutex);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}


// advanced timer

extern void* svc_oosiit_advtmr_new(void)
{
    void *ret;
    rt_iit_dbg_svc_enter();
    
    ret = rt_iit_advtmr_new();
     
    rt_iit_dbg_svc_exit();
    return(ret);   
}

extern oosiit_result_t svc_oosiit_advtmr_start(oosiit_objptr_t timer, oosiit_advtmr_timing_t *timing, oosiit_advtmr_action_t *action)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_iit_advtmr_start(timer, timing, action);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}


extern oosiit_result_t svc_oosiit_advtmr_stop(oosiit_objptr_t timer)
{
    OS_RESULT res;
    rt_iit_dbg_svc_enter();
 
    res = rt_iit_advtmr_stop(timer);
    
    rt_iit_dbg_svc_exit();
    return((oosiit_result_t)res);   
}


extern oosiit_result_t svc_oosiit_advtmr_delete(oosiit_objptr_t timer)
{
    void* ret;
    rt_iit_dbg_svc_enter();
 
    ret = rt_iit_advtmr_delete(timer);
    ret =  ret;
    
    rt_iit_dbg_svc_exit();
    return(oosiit_res_OK);   
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of static inline functions 
// --------------------------------------------------------------------------------------------------------------------

static __INLINE void* isr_oosiit_memory_new(uint32_t size)
{
    return(rt_iit_memory_new(size));
}


static __INLINE void* isr_oosiit_memory_realloc(void* m, uint32_t size)
{
    return(rt_iit_memory_realloc(m, size));
}

static __INLINE oosiit_result_t isr_oosiit_memory_del(void* mem)
{
    rt_iit_memory_del(mem);
    return(oosiit_res_OK);
}

static __INLINE oosiit_tskptr_t isr_oosiit_tsk_self(void)
{
    return(rt_iit_tsk_self());
}

static __INLINE oosiit_result_t isr_oosiit_tsk_set_extdata(oosiit_tskptr_t tp, void* extdata)
{
    P_TCB tcb = (P_TCB)tp;
    tcb->extdata = extdata;
    return(oosiit_res_OK);  
}

static __INLINE void* isr_oosiit_tsk_get_extdata(oosiit_tskptr_t tp)
{
    P_TCB tcb = (P_TCB)tp;
    return(tcb->extdata);    
}

static __INLINE oosiit_result_t isr_oosiit_mbx_retrieve(oosiit_objptr_t mailbox, void** message)
{
    int32_t r = isr_mbx_receive(mailbox, message);
    
    if(OS_R_OK == r)
    {
        r = oosiit_res_TMO;
    }
    else
    {
        r = oosiit_res_OK;
    }
    return((oosiit_result_t)r);
}


static __INLINE void isr_oosiit_mbx_send(oosiit_objptr_t mailbox, void* message)
{
    isr_mbx_send(mailbox, message);
}


static __INLINE uint16_t isr_oosiit_mbx_available(oosiit_objptr_t mailbox)
{
    return((uint16_t)rt_mbx_check(mailbox));
}

static __INLINE uint16_t isr_oosiit_mbx_used(oosiit_objptr_t mailbox)
{
    return((uint16_t)rt_iit_mbx_count(mailbox));    
}

static __INLINE oosiit_result_t isr_oosiit_evt_set(uint32_t flags, oosiit_tskptr_t tp)
{
    iitchanged_isr_evt_set(flags, tp);
    return(oosiit_res_OK);
}

static __INLINE oosiit_result_t isr_oosiit_sem_send(oosiit_objptr_t sem)
{
    isr_sem_send(sem);
    return(oosiit_res_OK);
}

static __INLINE oosiit_result_t isr_oosiit_advtmr_start(oosiit_objptr_t timer, oosiit_advtmr_timing_t *timing, oosiit_advtmr_action_t *action)
{
    return((oosiit_result_t)isr_iit_advtmr_start(timer, timing, action));
}

static __INLINE oosiit_result_t isr_oosiit_advtmr_stop(oosiit_objptr_t timer)
{
    return((oosiit_result_t)isr_iit_advtmr_stop(timer));
}

static __INLINE oosiit_result_t isr_oosiit_advtmr_delete(oosiit_objptr_t timer)
{
    return((oosiit_result_t)isr_iit_advtmr_delete(timer));
}


static __INLINE oosiit_result_t s_oosiit_tsk_valid(oosiit_tskptr_t tp)
{
    if((NULL == tp) || (TCB != *((uint8_t*)tp)))
    {
        oosiit_sys_error(oosiit_error_invalid_tskptr);
        return(oosiit_res_NOK);
    }
    return(oosiit_res_OK);    
}

static __INLINE oosiit_result_t s_oosiit_mut_valid(oosiit_objptr_t op)
{
    if((NULL == op) || (MUCB != *((uint8_t*)op)))
    {
        oosiit_sys_error(oosiit_error_invalid_objptr);
        return(oosiit_res_NOK);
    }
    return(oosiit_res_OK);    
}


static __INLINE oosiit_result_t s_oosiit_sem_valid(oosiit_objptr_t op)
{
    if((NULL == op) || (SCB != *((uint8_t*)op)))
    {
        oosiit_sys_error(oosiit_error_invalid_objptr);
        return(oosiit_res_NOK);
    }
    return(oosiit_res_OK);    
}

static __INLINE oosiit_result_t s_oosiit_mbx_valid(oosiit_objptr_t op)
{
    if((NULL == op) || (MCB != *((uint8_t*)op)))
    {
        oosiit_sys_error(oosiit_error_invalid_objptr);
        return(oosiit_res_NOK);
    }
    return(oosiit_res_OK);    
}

static __INLINE oosiit_result_t s_oosiit_advtmr_valid(oosiit_objptr_t op)
{
    if((NULL == op) || (ATCB != *((uint8_t*)op)))
    {
        oosiit_sys_error(oosiit_error_invalid_objptr);
        return(oosiit_res_NOK);
    }
    return(oosiit_res_OK);    
}

#if     defined(OOSIIT_USE_TIMEGET_UNDER_SVC)

static __INLINE oosiit_result_t s_oosiit_microtime_get(uint32_t* low, uint32_t* high)
{
    uint64_t microsecs;
    uint64_t tmp;
    volatile uint32_t reg0xE000E018 = *((volatile uint32_t *)0xE000E018);
    
    // add to microsecs the content of register systick_current_value_reg properly scaled.

    // tmp is in pico
    tmp = oosiit_picosec_per_unit_of_systick * (oosiit_num_units_of_systick - reg0xE000E018);
    tmp /= 1000000LL;   // now in micro
    microsecs = (oosiit_time * oosiit_cfg_in_use->ticktime) + tmp;
    
    *low = microsecs & 0xffffffff;
    *high = (microsecs >> 32) & 0xffffffff;

    return(oosiit_res_OK);    
}


static __INLINE oosiit_result_t s_oosiit_nanotime_get(uint32_t* low, uint32_t* high)
{
    uint64_t nanosecs;
    volatile uint32_t reg0xE000E018 = *((volatile uint32_t *)0xE000E018);

    // add to nanosecs the content of register systick_current_value_reg properly scaled.
    // if it is equal to systick_reload_value_reg, then extranano = 0;
    // if it is equal to 1, then extranano = ... (ticktime*1000)/(systick_reload_value_reg+1)

    nanosecs = oosiit_picosec_per_unit_of_systick * (oosiit_num_units_of_systick - reg0xE000E018);
    nanosecs /= 1000LL; // before it was pico, now it is nano
    nanosecs += (oosiit_time * oosiit_cfg_in_use->ticktime * 1000LL);

    *low = nanosecs & 0xffffffff;
    *high = (nanosecs >> 32) & 0xffffffff;
    
    return(oosiit_res_OK);    
}

#endif//defined(OOSIIT_USE_TIMEGET_UNDER_SVC)

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------



