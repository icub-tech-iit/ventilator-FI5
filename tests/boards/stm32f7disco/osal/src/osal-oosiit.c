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

/* @file       osal-oosiit.c
	@brief      This file keeps internal implementation of the osal module using open oosiit
	@author     marco.accame@iit.it
    @date       08/30/2012
**/



// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"
#include "string.h"

#include "oosiit.h"

#include "stdio.h"

#include "oosiit_hid.h"

// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "osal.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------



extern void osal_hid_reset_static_ram(void);
extern void osal_hid_entrypoint(void);

// i keep it extern to allow its name in microvision
void tINIT(void* p);



// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------




// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------


extern const osal_reltime_t osal_reltimeZERO        = OSAL_reltimeZERO;
extern const osal_reltime_t osal_reltime1ms         = OSAL_reltime1ms;
extern const osal_reltime_t osal_reltime1sec        = OSAL_reltime1sec;
extern const osal_reltime_t osal_reltimeMAX         = OSAL_reltimeMAX;
extern const osal_reltime_t osal_reltimeINFINITE    = OSAL_reltimeINFINITE;

extern const osal_abstime_t osal_abstimeNONE        = OSAL_abstimeNONE;



// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------

// we keep 12 bytes for a mutex
typedef uint32_t OOSIIT_MUT[3];


enum {osal_task_signature = 0x33};

struct osal_task_opaque_t 
{   // 4+1+1+2+4+4 = 16 bytes
    void*           rtostsk;
    uint8_t         signtsk;
    uint8_t         prio;
    uint16_t        stksize;
    uint64_t        *stkdata;
    void            *ext;
};


typedef struct
{   // 16 bytes
    void*           rtosobj;
    uint32_t        signature;   
} osal_obj_t;

enum {osal_timer_signature = 0x01234544};
typedef osal_obj_t osal_timer_opaque_t;

enum {osal_messagequeue_signature = 0x01234555};
typedef osal_obj_t osal_messagequeue_opaque_t;

enum {osal_mutex_signature = 0x01234566};
typedef osal_obj_t osal_mutex_opaque_t;

enum {osal_semaphore_signature = 0x01234577};
typedef osal_obj_t osal_semaphore_opaque_t;



// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------


static void s_osal_on_idle(void *p);

static void s_osal_error(osal_fatalerror_t err_code, const char *err_msg);

static void s_osal_on_fatalerror(osal_task_t *task, osal_fatalerror_t errorcode, const char * errormsg);

static void s_osal_fill_cfg(oosiit_cfg_t *oosiit_c, const osal_cfg_t *osal_c);

static uint32_t s_osal_timeout2tick(osal_reltime_t tout);
static uint32_t s_osal_delay2tick(osal_reltime_t delay);
static uint64_t s_osal_abstime2tick(osal_abstime_t time);
static uint32_t s_osal_period2tick(osal_reltime_t period);

static osal_obj_t* s_osal_osalobj_new(void *rtosobj, uint32_t signature);
static void* s_osal_rtosobj_get(osal_obj_t *osalobj, uint32_t signature);
static void s_osal_rtosobj_clr(osal_obj_t *osalobj);

static void s_osal_taskobj_init(void* memory);
static osal_task_t* s_osal_taskobj_new(void);
static void s_osal_taskobj_del(osal_task_t* taskobj);


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static const osal_cpufamily_t s_osal_cpufam = 
#if defined(OSAL_CPUFAM_CM3)
    osal_cpufam_armcm3;
#elif defined(OSAL_CPUFAM_CM4)
    osal_cpufam_armcm4;
#else
    255;
    #error -> must define an OSAL_CPUFAM_xxx
#endif

// mutex used to guarantee atomic access at user space in the osal layer
static osal_mutex_t *s_osal_mutex_api_protection = NULL;

// configuration of osiit
static oosiit_cfg_t s_osal_oosiit_cfg = {0};

// configuration of osal  
static osal_cfg_t  s_osal_osal_cfg;

// stack for launcher task
static uint64_t *s_osal_stacklauncher_data = NULL;
static uint16_t  s_osal_stacklauncher_size = 0;

// stack for idle task
static uint64_t *s_osal_stackidle_data = NULL;
static uint16_t  s_osal_stackidle_size = 0;


// memory used to store osal_task data.
static osal_task_t *s_osal_task_data = NULL; // its size is number of tasks+2 (idle, launcher, tasks)
static uint8_t s_osal_task_maxnum = 0;
static uint8_t s_osal_task_next = 0;

// special system tasks ...
static osal_task_t* s_osal_task_launcher = NULL;
static osal_task_t* s_osal_task_idle = NULL;

// indicates how many usec in a tick. used for time conversion
extern uint32_t osal_usec_in_tick = 0;

// pointer to the launcher function
static void (*s_osal_launcher_fn)(void) = NULL;

// status of the system
static osal_info_status_t s_osal_info_status = osal_info_status_zero;

// tables of resources
static uint16_t s_resources_free[osal_info_entity_numberof] = {0, 0, 0, 0, 0, 0, 0};
static uint16_t s_resources_used[osal_info_entity_numberof] = {0, 0, 0, 0, 0, 0, 0};

// number of mutexes required by the armcc99.
static uint8_t s_osal_arch_arm_armc99_sysmutex_number = 0;



// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern uint32_t osal_base_memory_getsize(const osal_cfg_t *cfg, uint32_t *size08aligned)
{
    uint16_t size08 = 0;
    uint16_t size04 = 0;
    uint32_t retval = 0;

    if(NULL == cfg)
    {
        if(NULL != size08aligned)
        {
            *size08aligned = 0;
        }
        return(0);
    }

    s_osal_fill_cfg(&s_osal_oosiit_cfg, cfg);   
    oosiit_memory_getsize(&s_osal_oosiit_cfg, &size04, &size08);
    
    retval = (size04+7)/8 + (size08+7)/8;
    
    // if memory mode is dynamic ... oosiit_memory_getsize will require 0 memory
    
    if(osal_memmode_static == cfg->memorymodel)
    {   // add the memory required for the osal
        retval += (cfg->launcherstacksize+7)/8 + (cfg->idlestacksize+7)/8 + ((cfg->tasknum+2)*sizeof(osal_task_t)+7)/8;
        retval *= 8;                
    }


    if(NULL != size08aligned)
    {
        *size08aligned = retval;
    }

    return(retval);
}


extern void* osal_base_memory_new(uint32_t size)
{
    return(oosiit_memory_new(size));
}

extern void* osal_base_memory_realloc(void* mem, uint32_t size)
{
    return(oosiit_memory_realloc(mem, size));
}

extern void osal_base_memory_del(void* mem)
{
//    oosiit_result_t rr = oosiit_memory_del(mem);
//    return((oosiit_res_OK == rr) ? (osal_res_OK) : (osal_res_NOK_generic));
    oosiit_memory_del(mem);
}

extern osal_result_t osal_base_initialise(const osal_cfg_t *cfg, uint64_t *data08aligned)
{
    uint16_t size08 = 0;
    uint16_t size04 = 0;
    uint32_t *oosiitdata04al;
    uint64_t *oosiitdata08al;

    if(NULL == cfg)
    {
        s_osal_error(osal_error_missingconfiguration, "osal: NULL cfg in osal_initialise()");
        //return(osal_res_NOK_nullpointer);    
    }    
    

    if((osal_memmode_static == cfg->memorymodel) && (NULL == data08aligned))
    {
        s_osal_error(osal_error_incorrectparameter, "osal: NULL memory in osal_initialise() for static memory model");
        //return(osal_res_NOK_nullpointer);
    }

    if(osal_rtostype_oosiit != cfg->rtostype)
    {
        s_osal_error(osal_error_unsupportedbehaviour, "osal: unsupported rtos");
        //return(osal_res_NOK_generic);
    }
    
    if(s_osal_cpufam != cfg->cpufam)
    {
        s_osal_error(osal_error_unsupportedbehaviour, "osal: cfg has a wrong cpu family");
        //return(osal_res_NOK_generic);        
    }

    if((osal_info_status_running == s_osal_info_status) || (osal_info_status_suspended == s_osal_info_status))
    {
        s_osal_error(osal_error_generic, "osal: called osal_initialise() after osal_start()");
        //return(osal_res_NOK_generic);
    }

    if((NULL != cfg->extfn.hal_sys_irqn_priority_set) && (cfg->prio > 15))
    {
        s_osal_error(osal_error_generic, "osal: if used, prio must be between 0 and 15");
        //return(osal_res_NOK_generic);
    }


    memcpy(&s_osal_osal_cfg, cfg, sizeof(osal_cfg_t));

    s_osal_fill_cfg(&s_osal_oosiit_cfg, cfg);
    
    oosiit_memory_getsize(&s_osal_oosiit_cfg, &size04, &size08);

    // memory for oosiit. 
    if((0 != size04) || (0 != size08))
    {
        oosiitdata04al = (uint32_t*)(data08aligned);
        oosiitdata08al = (uint64_t*)(data08aligned + (size04+7)/8);
    }
    else
    {
        oosiitdata04al = NULL;
        oosiitdata08al = NULL;
    }
    oosiit_memory_load(&s_osal_oosiit_cfg, oosiitdata04al, oosiitdata08al);
    
    // memory for launcher stack.
    if(osal_memmode_static == cfg->memorymodel)
    {
        s_osal_stacklauncher_data = (uint64_t*)(data08aligned + (size04+7)/8 + (size08+7)/8);
    }
    else
    {
        s_osal_stacklauncher_data = osal_base_memory_new(cfg->launcherstacksize);
    }
    s_osal_stacklauncher_size = cfg->launcherstacksize;
    memset(s_osal_stacklauncher_data, 0xEE, s_osal_stacklauncher_size); 
   
    // memory for idle stack.
    if(osal_memmode_static == cfg->memorymodel)
    {
        s_osal_stackidle_data = (uint64_t*)(data08aligned + (size04+7)/8 + (size08+7)/8 + (cfg->launcherstacksize+7)/8);
    }
    else
    {
        s_osal_stackidle_data = osal_base_memory_new(cfg->idlestacksize);
    }
    s_osal_stackidle_size = cfg->idlestacksize;
    memset(s_osal_stackidle_data, 0xEE, s_osal_stackidle_size);    
    
    // memory for task pointers
//     if(osal_memmode_static == cfg->memorymodel)
//     {
//         s_osal_task_data = (osal_task_t*)(data08aligned + (size04+7)/8 + (size08+7)/8 + (cfg->launcherstacksize+7)/8 + (cfg->idlestacksize+7)/8);
//         s_osal_task_idle = &s_osal_task_data[0];
//         s_osal_task_launcher = &s_osal_task_data[1];
//         s_osal_task_maxnum = cfg->tasknum + 2;
//         s_osal_task_next = 2;
//     } 
//     else
//     {   // only the launcher and the init
//         s_osal_task_data = NULL;
//         s_osal_task_idle = osal_base_memory_new(sizeof(osal_task_t));
//         s_osal_task_launcher = osal_base_memory_new(sizeof(osal_task_t));
//         //osal_base_memory_new(sizeof(osal_task_t)*(0 + 2)); // NULL; //osal_base_memory_new(sizeof(osal_task_t)*(cfg->tasknum + 2));
//     }

    s_osal_taskobj_init((data08aligned + (size04+7)/8 + (size08+7)/8 + (cfg->launcherstacksize+7)/8 + (cfg->idlestacksize+7)/8));
    
    s_osal_task_idle        = s_osal_taskobj_new();
    s_osal_task_launcher    = s_osal_taskobj_new();
    

    osal_usec_in_tick = cfg->tick;

    s_osal_info_status = osal_info_status_initialised;

    return(osal_res_OK);
}



extern osal_info_status_t osal_info_get_status(void)
{
    return(s_osal_info_status);
}


extern osal_reltime_t osal_info_get_tick(void)
{
    return(osal_usec_in_tick);
}

extern const osal_cfg_t* osal_info_get_config(void)
{
    if(osal_info_status_zero == s_osal_info_status)
    {
        return(NULL);
    }
    else
    {
        return(&s_osal_osal_cfg);
    }    
}


extern void osal_system_start(void (*launcher_fn)(void)) 
{
    oosiit_task_properties_t tskpinit;
    oosiit_task_properties_t tskpidle;

    if(s_osal_info_status != osal_info_status_initialised)
    {
        if(s_osal_info_status == osal_info_status_zero)
        {
            s_osal_error(osal_error_generic, "osal: called osal_start() w/out prior osal_initialise()"); 
        }
        else
        {
            s_osal_error(osal_error_generic, "osal: called osal_start() again");  
        }
           
    }

    s_osal_info_status = osal_info_status_running;

    s_osal_launcher_fn = launcher_fn; 
     
    if(NULL == s_osal_launcher_fn)
    {
        s_osal_error(osal_error_incorrectparameter, "osal: incorrect param in osal_start()");
    }
    
    // as we dont call an explicit oosiit_tsk_create() which gives a oosiit_tskptr_t, 
    // the task idle itself inside s_osal_on_idle() must call: s_osal_task_idle->rtostsk = oosiit_tsk_self()  
    s_osal_task_idle->rtostsk       = NULL;
    s_osal_task_idle->signtsk       = osal_task_signature;
    s_osal_task_idle->prio          = osal_prio_systsk_idle;
    s_osal_task_idle->stksize       = s_osal_stackidle_size;
    s_osal_task_idle->stkdata       = s_osal_stackidle_data;
    s_osal_task_idle->ext           = NULL;
    
    tskpidle.function   = s_osal_on_idle;
    tskpidle.param      = NULL;
    tskpidle.priority   = osal_prio_systsk_idle;
    tskpidle.stacksize  = s_osal_stackidle_size;
    tskpidle.stackdata  = s_osal_stackidle_data; 
    tskpidle.extdata    = s_osal_task_idle;    
    

    // as we dont call an explicit oosiit_tsk_create() which gives a oosiit_tskptr_t, 
    // the task launcher itself inside tINIT() must call: s_osal_task_launcher->rtostsk = oosiit_tsk_self()
    s_osal_task_launcher->rtostsk   = NULL;
    s_osal_task_launcher->signtsk   = osal_task_signature;
    s_osal_task_launcher->prio      = osal_prio_systsk_launcher;
    s_osal_task_launcher->stksize   = s_osal_stacklauncher_size; 
    s_osal_task_launcher->stkdata   = &s_osal_stacklauncher_data[0]; 
    s_osal_task_launcher->ext       = NULL;    
    
    tskpinit.function   = tINIT;
    tskpinit.param      = NULL;
    tskpinit.priority   = osal_prio_systsk_launcher;
    tskpinit.stacksize  = s_osal_stacklauncher_size;
    tskpinit.stackdata  = &s_osal_stacklauncher_data[0];
    tskpinit.extdata    = s_osal_task_launcher;



           
    // start the system
    oosiit_sys_start(&tskpinit, &tskpidle);
}



extern void osal_info_entities_get_stats(const uint16_t **used, const uint16_t **free)
{

    if((s_osal_info_status == osal_info_status_zero) || (s_osal_info_status == osal_info_status_initialised))
    {
        *used = NULL;
        *free = NULL;
        return;
    }
    
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);

    s_resources_free[osal_info_entity_globalstack]     =  s_osal_osal_cfg.globalstacksize  - s_resources_used[osal_info_entity_globalstack];  
    s_resources_free[osal_info_entity_task]            =  s_osal_osal_cfg.tasknum          - s_resources_used[osal_info_entity_task];
    s_resources_free[osal_info_entity_timer]           =  s_osal_osal_cfg.timernum         - s_resources_used[osal_info_entity_timer];
    s_resources_free[osal_info_entity_mutex]           =  s_osal_osal_cfg.mutexnum         - s_resources_used[osal_info_entity_mutex];
    s_resources_free[osal_info_entity_semaphore]       =  s_osal_osal_cfg.semaphorenum     - s_resources_used[osal_info_entity_semaphore];
    s_resources_free[osal_info_entity_messagequeue]    =  s_osal_osal_cfg.mqueuenum        - s_resources_used[osal_info_entity_messagequeue];
    s_resources_free[osal_info_entity_message]         =  s_osal_osal_cfg.mqueueelemnum    - s_resources_used[osal_info_entity_message];

    *used = s_resources_used;
    *free = s_resources_free;

    osal_mutex_release(s_osal_mutex_api_protection);
}


extern osal_abstime_t osal_info_idletime_get(void)
{
    return(osal_usec_in_tick * 0);
}


// extern void * osal_system_launcherbuffer_get(uint16_t *size)
// {
//     if(NULL == size)
//     {
//         return(NULL);
//     }

//     if(NULL == s_osal_stacklauncher_data)
//     {   // osal_initialise() not yet called
//         *size = 0;
//         return(NULL);
//     }

//     if(0 != s_osal_stacklauncher_data[0])
//     {   // the signature is not-zero: the launcher task is either not running yet or not terminated
//         *size = 0;
//         return(NULL);
//     }
//     else 
//     {
//         *size = s_osal_stacklauncher_size-8;
//         return(&s_osal_stacklauncher_data[1]);
//     }
// }



extern void osal_system_ticks_abstime_set(osal_abstime_t tot)
{
    oosiit_time_set(s_osal_abstime2tick(tot));
}


extern osal_abstime_t osal_system_ticks_abstime_get(void)
{
    return((uint64_t)osal_usec_in_tick * oosiit_time_get());
}


extern osal_abstime_t osal_system_abstime_get(void)
{
    return(oosiit_microtime_get());
}

extern osal_nanotime_t osal_system_nanotime_get(void)
{
    return(oosiit_nanotime_get());
}

extern uint64_t osal_system_millitime_get(void)
{
    return (osal_usec_in_tick*oosiit_time) / 1000;
}

extern void osal_system_scheduling_suspend(void)
{
    if(osal_info_status_running == s_osal_info_status)
    {   // order is important in here
        oosiit_sys_suspend(NULL);
        s_osal_info_status = osal_info_status_suspended;
    }
}

extern void osal_system_scheduling_restart(void)
{
    if(osal_info_status_suspended == s_osal_info_status)
    {   // order is important in here
        s_osal_info_status = osal_info_status_running;
        oosiit_sys_resume(0);
    }
}


extern osal_reltime_t osal_system_timeofnextevent(void)
{
    osal_reltime_t nextevent = 0;
    if(osal_info_status_running == s_osal_info_status)
    {   
        nextevent = oosiit_sys_timeofnextevent();
    }
    
    return(osal_usec_in_tick*nextevent);    
}


extern osal_reltime_t osal_system_suspend(void)
{
    osal_reltime_t nextevent = 0;
    if(osal_info_status_running == s_osal_info_status)
    {   // order is important in here
        oosiit_sys_suspend(&nextevent);
        s_osal_info_status = osal_info_status_suspended;
    }
    
    return(osal_usec_in_tick*nextevent);
}

extern void osal_system_resume(osal_reltime_t timeslept)
{
    if(osal_info_status_suspended == s_osal_info_status)
    {   // order is important in here
        s_osal_info_status = osal_info_status_running;
        oosiit_sys_resume(timeslept/osal_usec_in_tick);
    }
}

extern osal_task_t * osal_task_new1(osal_task_properties_t *tskprop)
{
    if(NULL == tskprop)
    {
        return(NULL);
    }
    return(osal_task_new(tskprop->function, tskprop->param, tskprop->priority, tskprop->stacksize));
}


extern osal_task_t * osal_task_new(void (*run_fn)(void*), void *run_arg, uint8_t prio, uint16_t stksize)
{
    uint64_t *stack = NULL;
    osal_task_t* retval = NULL;    
    void *rtostsk = NULL;
    oosiit_task_properties_t tskprop;


    if((0 == stksize) || (prio<osal_prio_usrtsk_min) || (prio>osal_prio_usrtsk_max) || (NULL == run_fn))
    {
        //return(NULL);
        s_osal_error(osal_error_incorrectparameter, "osal: incorr param in osal_task_new()");
    }

    // protect vs concurrent call
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    
//     // can we proceed in getting the task? if dynamic mode yes. if static mode ... lets see
//     if(NULL != s_osal_task_data)
//     {
//         if(s_osal_task_next >= s_osal_task_maxnum)
//         {
//             // cannot get a task
//             s_osal_error(osal_error_missingmemory, "osal: missing mem for osal_task_t (static mode) in osal_task_new()");
//             osal_mutex_release(s_osal_mutex_api_protection);
//             return(NULL);        
//         }
//         retval = &s_osal_task_data[s_osal_task_next++];
//     }
//     else
//     {
//         retval = osal_base_memory_new(sizeof(osal_task_t)); 
//     }
//     
    retval = s_osal_taskobj_new();    
    if(NULL == retval)
    {
        s_osal_error(osal_error_missingmemory, "osal: missing mem for osal_task_t in osal_task_new()");
        osal_mutex_release(s_osal_mutex_api_protection);
        return(NULL);        
    }
       
    
    // get the stack    
    stack = oosiit_memory_getstack(stksize);
    if(NULL == stack)
    {
        // cannot get a stack
        s_osal_error(osal_error_missingmemory, "osal: missing mem for stack in osal_task_new()");
        osal_mutex_release(s_osal_mutex_api_protection);
        //return(NULL);
    }
    // increment used stacksize
    s_resources_used[osal_info_entity_globalstack] += stksize;
        

     
    // get a oosiit task
    tskprop.function        = run_fn;
    tskprop.param           = run_arg;
    tskprop.priority        = osal_prio_systsk_usrwhencreated; // use priority osal_prio_systsk_usrwhencreated = 1 to avoid that the run_fn starts straigth away
    tskprop.stacksize       = stksize;
    tskprop.stackdata       = stack;
    tskprop.extdata         = retval; // by crating teh task w/ this argument i avoid calling oosiit_tsk_set_extdata(rtostsk, retval) after
    
    rtostsk = oosiit_tsk_create(&tskprop);

    if(NULL == rtostsk)
    {
        // cannot get a task
        s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos task in osal_task_new()");
        osal_mutex_release(s_osal_mutex_api_protection);
        //return(NULL);
    }
    
    // ok. i can increment the tasks.
    s_resources_used[osal_info_entity_task] ++;
    
    
    // init the returning osal task
    retval->rtostsk    = rtostsk;
    retval->signtsk    = osal_task_signature;
    retval->prio       = prio;
    retval->stksize    = stksize;
    retval->stkdata    = stack;
    retval->ext        = NULL;
    
    // store inside the rtostsk the pointer to the osal task
    //oosiit_tsk_set_extdata(rtostsk, retval);

    // can release now
    osal_mutex_release(s_osal_mutex_api_protection);

    // the destiny of the task is now given to the scheduler. it can start now if prio is higher than the priority of the caller  
    oosiit_tsk_setprio(rtostsk, prio);

    return(retval);
}



extern osal_result_t osal_task_wait(osal_reltime_t time)
{
    if(0 == time)
    {
        oosiit_tsk_pass();
    }
    else
    {
        oosiit_dly_wait(s_osal_delay2tick(time));
    }

    return(osal_res_OK);
}

extern osal_result_t osal_task_pass(void)
{
    oosiit_tsk_pass();
    return(osal_res_OK);
}

extern osal_result_t osal_task_period_set(osal_reltime_t period)
{
    oosiit_itv_set(s_osal_period2tick(period));
    return(osal_res_OK);
}


extern osal_result_t osal_task_period_wait(void)
{
    oosiit_itv_wait();
    return(osal_res_OK);
}

extern osal_result_t osal_task_delete(osal_task_t *tsk)
{
    void* rtostsk = NULL;
    
    if(NULL == tsk)
    {
        return(osal_res_NOK_nullpointer);
    }
    
    // some tasks could call it at the same time ... better to protect
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);   
        
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {   // if the caller has used a proper tsk. it may be that another task has already or just deleted the task.
        osal_mutex_release(s_osal_mutex_api_protection);
        return(osal_res_NOK_generic);
    }
    
    rtostsk = tsk->rtostsk;
    
    s_resources_used[osal_info_entity_task] --;
    if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
    {
        s_resources_used[osal_info_entity_globalstack] -= tsk->stksize;
    }
    
    // reset the data pointed by tsk so that another task calling osal_tsk_* functions with the same tsk
    // exits gracefully ... until the same memory is used for something else....    
    tsk->rtostsk        = NULL;
    tsk->signtsk        = 0;
    tsk->prio           = 0;
    tsk->stksize        = 0;
    tsk->stkdata        = NULL;
    tsk->ext            = NULL;       
    
    s_osal_taskobj_del(tsk);      
 
    osal_mutex_release(s_osal_mutex_api_protection);
    
    // MUST be the last operation because ... in case of auto-delete the function
    // oosiit_tsk_delete() internally forces a contex switch and current task is not executed anymore
    // thus isntructions after oosiit_tsk_delete() are not executed 
    oosiit_tsk_delete(rtostsk);     
    // in case of auto-delete any instruction after this comment is not executed .....  

    
    return(osal_res_OK);    
}

extern osal_result_t osal_task_priority_get(osal_task_t *tsk, uint8_t *prio)
{

    if((NULL == tsk) || (NULL == prio))
    {
        return(osal_res_NOK_nullpointer);
    }
    
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {
        return(osal_res_NOK_generic);
    }
    
    *prio = tsk->prio;

    return(osal_res_OK);
}

extern osal_result_t osal_task_priority_set(osal_task_t *tsk, uint8_t prio)
{
    if(NULL == tsk)
    {
        return(osal_res_NOK_nullpointer);
    }

    if((prio<osal_prio_usrtsk_min) || (prio>osal_prio_usrtsk_max))
    {
        return(osal_res_NOK_generic);
    }
      
   
    // some tasks could call it at the same time ... better to protect
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
        
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {
        osal_mutex_release(s_osal_mutex_api_protection);
        return(osal_res_NOK_generic);
    }      

    tsk->prio = prio;

    oosiit_tsk_setprio(tsk->rtostsk, prio);

    osal_mutex_release(s_osal_mutex_api_protection);

    return(osal_res_OK);
}

extern osal_result_t osal_task_id_get(osal_task_t *tsk, osal_task_id_t *id)
{
    if((NULL == tsk) || (NULL == id))
    {
        return(osal_res_NOK_nullpointer);
    }
 
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {
        return(osal_res_NOK_generic);
    } 
    
    *id = oosiit_tsk_get_id(tsk->rtostsk);

    return(osal_res_OK);
}

extern osal_result_t osal_task_extdata_set(osal_task_t *tsk, void *ext)
{
    if(NULL == tsk)
    {
        return(osal_res_NOK_nullpointer);
    }

    // some tasks could call it at the same time ... better to protect
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {
        osal_mutex_release(s_osal_mutex_api_protection);
        return(osal_res_NOK_generic);
    }     

    tsk->ext = ext;

    osal_mutex_release(s_osal_mutex_api_protection);

    return(osal_res_OK);
}

extern void* osal_task_extdata_get(osal_task_t *tsk)
{
    if(NULL == tsk)
    {
        return(NULL);
    }
    
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {
        return(NULL);
    }
    
    return(tsk->ext);
}


extern osal_task_t * osal_task_get(osal_caller_t caller)
{    
//     osal_task_t* retval = NULL;
//     void* rtostsk = oosiit_tsk_self();       
//     // i had associated the osal task to the rtos task
//     retval = oosiit_tsk_get_extdata(rtostsk);
//     return(retval);
    return(oosiit_tsk_get_extdata(oosiit_tsk_self()));
}

extern void * osal_task_stack_get1(osal_task_t *tsk, uint16_t *size)
{
    if(NULL == tsk)
    {
        return(NULL);
    }
    
    if((NULL == tsk->rtostsk) || (osal_task_signature != tsk->signtsk))
    {
        return(NULL);
    }
        
    if(NULL != size)
    {
        *size = tsk->stksize;
    }

    return(tsk->stkdata);
}

extern osal_messagequeue_t * osal_messagequeue_new(uint16_t maxmsg)
{
    osal_messagequeue_t *retptr = NULL;

    if(maxmsg == 0)
    {
        s_osal_error(osal_error_incorrectparameter, "osal: incorr param in osal_messagequeue_new()");
        //return(NULL);
    }
    
    
//     retptr = osal_base_memory_new(sizeof(osal_messagequeue_t));
//     
//     if(NULL == retptr)
//     {
//         s_osal_error(osal_error_missingmemory, "osal: missing mem for osal_messagequeue_t in osal_messagequeue_new()");
//         //return(NULL);
//     }
//     
//   
//     retptr->rtosmbx = oosiit_mbx_create(maxmsg);
//     retptr->signmbx = osal_messagequeue_signature;

//     if(NULL == retptr->rtosmbx)
//     {
//         s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos msg queue in osal_messagequeue_new()");
//         //return(NULL);
//     }
    
    
    
    void* rtosobj = oosiit_mbx_create(maxmsg);
    if(NULL == rtosobj)
    {
        s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos obj in osal_messagequeue_new()");
        //return(NULL);
    }        
    retptr = (osal_messagequeue_t*)s_osal_osalobj_new(rtosobj, osal_messagequeue_signature);
    

    // protect he because the osiit_* calls are already protected as they are executes by svc
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);

    s_resources_used[osal_info_entity_messagequeue] ++;
    s_resources_used[osal_info_entity_message] += maxmsg;

    osal_mutex_release(s_osal_mutex_api_protection);

    return(retptr);
}


extern osal_result_t osal_messagequeue_get(osal_messagequeue_t *mq, osal_message_t *pmsg, osal_reltime_t tout, osal_caller_t caller)
{
    osal_result_t res = osal_res_NOK_timeout; 
    oosiit_result_t rr;
    void *p = NULL;

    if((NULL == mq) || (NULL == pmsg))
    {
        return(osal_res_NOK_nullpointer);
    }
    
   
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mq, osal_messagequeue_signature)))
    {
        return(osal_res_NOK_generic);
    }
    

    *pmsg = (osal_message_t)0;
    
    // any caller ...
    rr = oosiit_mbx_retrieve(rtosobj, &p, s_osal_timeout2tick(tout));
    
    *pmsg = (osal_message_t)p;
    
    switch(rr)
    {
        case oosiit_res_OK:         res = osal_res_OK;              break;
        case oosiit_res_MBX:        res = osal_res_OK;              break;
//        case oosiit_res_TMO:        res = (osal_callerTSK == caller) ? (osal_res_NOK_timeout) : (osal_res_NOK_isrnowait);             break;
        case oosiit_res_TMO:        res = osal_res_NOK_timeout;     break;
        case oosiit_res_NOK: 
//        case oosiit_res_OBJDEL:
        default:                    res = osal_res_NOK_nullpointer; break; 
    }
    
    return(res);
}


extern osal_message_t osal_messagequeue_getquick(osal_messagequeue_t *mq, osal_reltime_t tout, osal_caller_t caller)
{
    void *p = NULL;

    if(NULL == mq)
    {
        return((osal_message_t)0);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mq, osal_messagequeue_signature)))
    {
        return((osal_message_t)0);
    }    
    
    // any caller
    oosiit_mbx_retrieve(rtosobj, &p, s_osal_timeout2tick(tout));

    
    return((osal_message_t)p);
}

extern uint16_t osal_messagequeue_available(osal_messagequeue_t *mq, osal_caller_t caller)
{
    if(NULL == mq)
    {
        return(0);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mq, osal_messagequeue_signature)))
    {
        return(0);
    }      

    return( oosiit_mbx_available(rtosobj) );    
}


extern uint16_t osal_messagequeue_size(osal_messagequeue_t *mq, osal_caller_t caller)
{
    if(NULL == mq)
    {
        return(0);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mq, osal_messagequeue_signature)))
    {
        return(0);
    }      

    return( oosiit_mbx_used(rtosobj) );    
}


extern osal_result_t osal_messagequeue_put(osal_messagequeue_t *mq, osal_message_t msg, osal_reltime_t tout, osal_caller_t caller)
{
    oosiit_result_t rr;

    if(NULL == mq)
    {
        return(osal_res_NOK_nullpointer);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mq, osal_messagequeue_signature)))
    { 
        return(osal_res_NOK_generic);
    }    
    
    rr = oosiit_mbx_send(rtosobj, (void*)msg, s_osal_timeout2tick(tout));

    if(rr == oosiit_res_OK)
    {
        return(osal_res_OK);
    }
    else
    {
        return(osal_res_NOK_timeout);
    }
}

extern osal_result_t osal_messagequeue_delete(osal_messagequeue_t *mq)
{
    if(NULL == mq)
    {
        return(osal_res_NOK_nullpointer); 
    }

    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mq, osal_messagequeue_signature)))
    { 
        return(osal_res_NOK_generic);
    }       
    
    oosiit_mbx_delete(rtosobj);
    
    s_osal_rtosobj_clr((osal_obj_t*)mq);
        
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    s_resources_used[osal_info_entity_messagequeue] --;
    osal_mutex_release(s_osal_mutex_api_protection);

    return(osal_res_OK);
}

extern osal_result_t osal_eventflag_set(osal_eventflag_t flag, osal_task_t * totask, osal_caller_t caller)
{
    if(NULL == totask)
    {
        return(osal_res_NOK_nullpointer);
    }
    
    oosiit_evt_set(flag, totask->rtostsk);

    return(osal_res_OK);
}


extern osal_result_t osal_eventflag_get(osal_eventflag_t waitmsk, osal_eventflag_waitmode_t waitmode, osal_eventflag_t *rxmsk, osal_reltime_t tout)
{
    osal_result_t res = osal_res_NOK_timeout; 
    oosiit_result_t rr;

    if(NULL == rxmsk)
    {
        return(osal_res_NOK_nullpointer);
    }

    *rxmsk = 0;
    
    rr = oosiit_evt_wait(waitmsk, s_osal_timeout2tick(tout), (osal_waitALLflags == waitmode) ? (oosiit_evt_wait_mode_all) : (oosiit_evt_wait_mode_any));
    
    if(oosiit_res_EVT == rr)
    {
         res = osal_res_OK;
         *rxmsk = (osal_waitALLflags == waitmode) ? (waitmsk) : (oosiit_evt_get());
    }

    return(res);

}


extern osal_eventflag_t osal_eventflag_getany(osal_reltime_t tout)
{

    if(oosiit_res_EVT == oosiit_evt_wait(0xFFFFFFFF, s_osal_timeout2tick(tout), oosiit_evt_wait_mode_any))
    {
        return(oosiit_evt_get());
    }
    else
    {
        return(0);
    }

}


extern osal_mutex_t * osal_mutex_new(void) 
{
    osal_mutex_t *retptr = NULL;
    
//     retptr = osal_base_memory_new(sizeof(osal_mutex_t));
//     
//     if(NULL == retptr)
//     {
//         s_osal_error(osal_error_missingmemory, "osal: missing mem for osal_mutex_t in osal_mutex_new()");
//         //return(NULL);
//     }    
//     
//     retptr->rtosmtx = oosiit_mut_create();
//     retptr->signmtx = osal_mutex_signature;
//     
//     if(NULL == retptr)
//     {
//         s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos mutex in osal_mutex_new()");
//         //return(NULL);
//     }
    
    void* rtosobj = oosiit_mut_create();
    if(NULL == rtosobj)
    {
        s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos obj in osal_mutex_new()");
        //return(NULL);
    }        
    retptr = (osal_mutex_t*)s_osal_osalobj_new(rtosobj, osal_mutex_signature);
    

    if(NULL != s_osal_mutex_api_protection)
    {
        osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
        s_resources_used[osal_info_entity_mutex] ++;
        osal_mutex_release(s_osal_mutex_api_protection);
    }
 
    // ok, all done
    return(retptr);
} 


extern osal_result_t osal_mutex_take(osal_mutex_t *mutex, osal_reltime_t tout) 
{
    oosiit_result_t rr;
    
    if(NULL == mutex)
    {
        return(osal_res_NOK_nullpointer);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mutex, osal_mutex_signature)))
    { 
        return(osal_res_NOK_generic);
    }       
    
    rr = oosiit_mut_wait(rtosobj, s_osal_timeout2tick(tout));
    
    if((oosiit_res_OK == rr) || (oosiit_res_MUT == rr))
    {
        return(osal_res_OK);
    }
    else
    {
        return(osal_res_NOK_timeout);
    }

}


extern osal_result_t osal_mutex_release(osal_mutex_t *mutex) 
{
    if(NULL == mutex)
    {
        return(osal_res_NOK_nullpointer);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mutex, osal_mutex_signature)))
    { 
        return(osal_res_NOK_generic);
    }  
    
    return((oosiit_res_NOK == oosiit_mut_release(rtosobj)) ? (osal_res_NOK_generic) : (osal_res_OK));
}

extern osal_result_t osal_mutex_delete(osal_mutex_t *mutex)
{
    if(NULL == mutex)
    {
        return(osal_res_NOK_nullpointer);
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)mutex, osal_mutex_signature)))
    { 
        return(osal_res_NOK_generic);
    }  
    
    oosiit_mut_delete(rtosobj);
    
    s_osal_rtosobj_clr((osal_obj_t*)mutex);
        
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    s_resources_used[osal_info_entity_mutex] --;
    osal_mutex_release(s_osal_mutex_api_protection);

    return(osal_res_OK);
}


extern osal_semaphore_t * osal_semaphore_new(uint8_t maxtokens, uint8_t tokens)
{
    osal_semaphore_t *retptr = NULL;

    if((0 == maxtokens) || (tokens > maxtokens))
    {
        s_osal_error(osal_error_incorrectparameter, "osal: value of maxtokens or tokens are incorrect in osal_semaphore_new()");
        //return(NULL);
    }
    
//     retptr = osal_base_memory_new(sizeof(osal_semaphore_t));
//     
//     if(NULL == retptr)
//     {
//         s_osal_error(osal_error_missingmemory, "osal: missing mem for osal_semaphore_t in osal_semaphore_new()");
//         //return(NULL);
//     }
//     
//     retptr->rtossem = oosiit_sem_create(maxtokens, tokens);
//     retptr->signsem = osal_semaphore_signature;


//     if(NULL == retptr)
//     {
//         s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos sem in osal_semaphore_new()");
//         //return(NULL);
//     }


    void* rtosobj = oosiit_sem_create(maxtokens, tokens);
    if(NULL == rtosobj)
    {
        s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos obj in osal_semaphore_new()");
        //return(NULL);
    }        
    retptr = (osal_semaphore_t*)s_osal_osalobj_new(rtosobj, osal_semaphore_signature);    
    
    
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    s_resources_used[osal_info_entity_semaphore] ++;
    osal_mutex_release(s_osal_mutex_api_protection);
    
    // ok, all done
    return(retptr);
}

  
extern osal_result_t osal_semaphore_set(osal_semaphore_t *sem, uint8_t tokens)
{
    if(NULL == sem)
    {
        return(osal_res_NOK_nullpointer); 
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)sem, osal_semaphore_signature)))
    { 
        return(osal_res_NOK_generic);
    }       
    
    oosiit_sem_set(rtosobj, tokens);

    // ok, all done
    return(osal_res_OK);
}


extern osal_result_t osal_semaphore_decrement(osal_semaphore_t *sem, osal_reltime_t tout)
{
    oosiit_result_t rr;
    
    if(NULL == sem)
    {
        return(osal_res_NOK_nullpointer); 
    }
    
    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)sem, osal_semaphore_signature)))
    { 
        return(osal_res_NOK_generic);
    }     
    
    rr = oosiit_sem_wait(rtosobj, s_osal_timeout2tick(tout));
    
    if((oosiit_res_OK == rr) || (oosiit_res_SEM == rr))
    {
        return(osal_res_OK);
    }
    else
    {
        return(osal_res_NOK_timeout);
    }
    

}

extern osal_result_t osal_semaphore_increment(osal_semaphore_t *sem, osal_caller_t caller)
{
    if(NULL == sem)
    {
        return(osal_res_NOK_nullpointer); 
    }

    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)sem, osal_semaphore_signature)))
    { 
        return(osal_res_NOK_generic);
    }       
    
    return((oosiit_res_OK == oosiit_sem_send(rtosobj)) ? (osal_res_OK) : (osal_res_NOK_generic));
}


extern osal_result_t osal_semaphore_delete(osal_semaphore_t *sem)
{
    if(NULL == sem)
    {
        return(osal_res_NOK_nullpointer); 
    }

    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)sem, osal_semaphore_signature)))
    { 
        return(osal_res_NOK_generic);
    }       
    
    oosiit_sem_delete(rtosobj);
    
    s_osal_rtosobj_clr((osal_obj_t*)sem);
        
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    s_resources_used[osal_info_entity_semaphore] --;
    osal_mutex_release(s_osal_mutex_api_protection);

    return(osal_res_OK);

}


extern osal_timer_t * osal_timer_new(void)
{
    osal_timer_t *retptr = NULL;
 
    void* rtosobj = oosiit_advtmr_new();    
    if(NULL == rtosobj)
    { 
        s_osal_error(osal_error_missingmemory, "osal: missing mem for rtos obj in osal_timer_new()");
        //return(NULL);
    }    
    retptr = (osal_timer_t*)s_osal_osalobj_new(rtosobj, osal_timer_signature);
    
       
//     if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
//     {
//         retval = osal_base_memory_new(sizeof(osal_timer_t));   
//         if(NULL == retval)
//         { 
//             s_osal_error(osal_error_missingmemory, "osal: missing mem for osal_timer_t in osal_timer_new()");
//             //return(NULL);
//         }  
//         retval->rtostmr = rtostmr;
//         retval->signtmr = osal_timer_signature;
//     }
//     else
//     {
//         retval = rtostmr;
//     }

    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    s_resources_used[osal_info_entity_timer] ++;
    osal_mutex_release(s_osal_mutex_api_protection);

    return(retptr);
}



extern osal_result_t osal_timer_start(osal_timer_t *timer, osal_timer_timing_t *timing, osal_timer_onexpiry_t *onexpiry, osal_caller_t caller)
{
    oosiit_advtmr_timing_t tim;
    oosiit_advtmr_action_t act;
    oosiit_result_t rr;
 
    if((NULL == timer) || (NULL == timing) || (NULL == onexpiry))
    { 
        return(osal_res_NOK_nullpointer);
    }
    //#warning -> protect it w/ mutex?
    

    void* rtosobj = NULL;
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)timer, osal_timer_signature)))
    {
        return(osal_res_NOK_generic);
    }
    
    
//     if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
//     {
//         if((NULL == timer->rtostmr) || (osal_timer_signature != timer->signtmr))
//         { 
//             return(osal_res_NOK_generic);
//         } 
//
//         rtostmr = timer->rtostmr;       
//     } 
//     else
//     {
//         rtostmr = timer; 
//     }
 

    tim.startat     = s_osal_abstime2tick(timing->startat);
    tim.firstshot   = s_osal_delay2tick(timing->count);
    tim.othershots  = (osal_tmrmodeONESHOT == timing->mode) ? (0) : (tim.firstshot);
    
    act.cbk         = (void(*)(void*, void*))onexpiry->cbk;
    act.par         = onexpiry->par;
    
    
    rr = oosiit_advtmr_start(rtosobj, &tim, &act);


    return((oosiit_res_NOK == rr) ? (osal_res_NOK_generic) : (osal_res_OK));
}


extern osal_result_t osal_timer_stop(osal_timer_t *timer, osal_caller_t caller)
{   
    void* rtosobj = NULL;
    
    if(NULL == timer)
    { 
        return(osal_res_NOK_nullpointer);
    }
    
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)timer, osal_timer_signature)))
    {
        return(osal_res_NOK_generic);
    }
    
    return((oosiit_res_NOK == oosiit_advtmr_stop(rtosobj)) ? (osal_res_NOK_generic) : (osal_res_OK));
}


extern osal_result_t osal_timer_isactive(osal_timer_t *timer, osal_caller_t caller)
{
    void* rtosobj = NULL;
    
    if(NULL == timer)
    { 
        return(osal_res_NOK_nullpointer);
    }
    
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)timer, osal_timer_signature)))
    {
        return(osal_res_NOK_generic);
    }     

    return((oosiit_res_NOK == oosiit_advtmr_isactive(rtosobj)) ? (osal_res_NOK_generic) : (osal_res_OK));
}

extern osal_result_t osal_timer_delete(osal_timer_t *timer)
{
    void* rtosobj = NULL;
    
    if(NULL == timer)
    { 
        return(osal_res_NOK_nullpointer);
    }
    
    if(NULL == (rtosobj = s_osal_rtosobj_get((osal_obj_t*)timer, osal_timer_signature)))
    {
        return(osal_res_NOK_generic);
    }      

    oosiit_advtmr_delete(rtosobj);
    
    s_osal_rtosobj_clr((osal_obj_t*)timer);
        
    osal_mutex_take(s_osal_mutex_api_protection, OSAL_reltimeINFINITE);
    s_resources_used[osal_info_entity_timer] --;
    osal_mutex_release(s_osal_mutex_api_protection);

    return(osal_res_OK);
}




extern void  *__libspace_start;
extern uint32_t (*std_libspace)[96/4];

// as in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdicbfj.html
// you must call that inside void *__user_perthread_libspace (void)
// { static uint8_t firstcall = 1; void * ret; ret = osal_threadsafe_stdlib_mutex_initialize(firstcall); 
//   if(1 == firstcall) { firstcall = 0;} 
//   return(ret); 
// }
extern void * osal_arch_arm_armc99stdlib_getlibspace(uint8_t firstcall) 
{
    // provide a separate libspace for each task
    oosiit_tskptr_t tp = NULL;

    if(1 == firstcall)
    {
        osal_hid_reset_static_ram();
    }


    if(s_osal_info_status < osal_info_status_running)
    {
        // osal not running yet
        return(&__libspace_start);
    }

    // we have osal running, thus also oosiit. we get the osiit task id and we give it its memory
    tp = oosiit_tsk_self();

    return(oosiit_tsk_get_perthread_libspace(tp));
    
}

extern void rt_mut_init(void* mutex);

// as in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdfjddj.html ...
// one should use it inside a void _mutex_acquire(void *m) function
extern int osal_arch_arm_armc99stdlib_mutex_initialize(void *m) 
{
    // this function accepts a pointer to a 32-bit word and initializes it as a valid mutex

    #define OSAL_SYSMUTEXCNT_HIDDEN  8
    
    static OOSIIT_MUT s_osal_sysmutexes_hidden[OSAL_SYSMUTEXCNT_HIDDEN];
    void *mutex = NULL;

   
    if(s_osal_arch_arm_armc99_sysmutex_number >= OSAL_SYSMUTEXCNT_HIDDEN) 
    {
        // if in here, you need to increase OSIIT_SYSMUTEXCNT_HIDDEN
         s_osal_error(osal_error_internal3, "armc99stdlib: not enough sys mutex");
    }

    // i get a pointer to a mutex
    mutex = (void*) &s_osal_sysmutexes_hidden[s_osal_arch_arm_armc99_sysmutex_number++];
    // i initialise the ram pointed by the mutex. i use rt_mut_init because it does not use the svc handler
    rt_mut_init(mutex);

    // now i write into *mutex the address of the initted mutex
    *((uint32_t*)m) = (uint32_t)mutex;


    // must return a nonzero value on success so that, at runtime, 
    // the library knows that it is being used in a multithreaded environment 
    return(1);
}


// as in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdfjddj.html ...
// one should use it inside a void _mutex_acquire(mutex *m) function
extern void osal_arch_arm_armc99stdlib_mutex_acquire(void *m)
{
    // acquire a system mutex to lock stdlib resources only if osal is running
    if(s_osal_info_status >= osal_info_status_running) 
    {
        // the 32 bit pointed by m are a valid osal mutex
        oosiit_objptr_t *mutex = (oosiit_objptr_t*)m;
        // OOSIIT running, acquire a mutex
        oosiit_mut_wait(*mutex, OOSIIT_NOTIMEOUT);
    }
}

// as in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdfjddj.html ...
// one should use it inside a void _mutex_release(mutex *m) function
extern void osal_arch_arm_armc99stdlib_mutex_release(void *m)
{
    // release a system mutex to unlock stdlib resources
    if(s_osal_info_status >= osal_info_status_running) 
    {
        // the 32 bit pointerd by m are a valid osal mutex only if osal is running
        oosiit_objptr_t *mutex = (oosiit_objptr_t*)m;
        // OOSIIT runnning, release a mutex. 
        oosiit_mut_release(*mutex);
    }
}


extern void* oosiit_ext_calloc(uint32_t s, uint32_t n)
{
    return(osal_ext_calloc(s, n));
}


extern void* oosiit_ext_realloc(void* m, uint32_t s)
{
    return(osal_ext_realloc(m, s));
}


extern void oosiit_ext_free(void* m)
{
    osal_ext_free(m);    
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------


// used only by osiit
void oosiit_sys_error(oosiit_error_code_t errorcode) 
{
    char str[100];
    switch(errorcode)
    {
        case oosiit_error_unknown:
        {
            s_osal_error(osal_error_unknownfromrtos, "osal: unknown");    
        } break;
        
        case oosiit_error_stackoverflow:
        {
            s_osal_error(osal_error_stackoverflow, "osal: stack overflow");
        } break;
        
        case oosiit_error_isrfifooverflow:
        {
             s_osal_error(osal_error_internal0, "osal: known error from rtos (isrfifooverflow)");
        } break;  

        case oosiit_error_mbxoverflow:
        {
            s_osal_error(osal_error_internal1, "osal: known error from rtos (mbxoverflow)");
        } break;
        
        case oosiit_error_internal_stdlibspace:
        {
            s_osal_error(osal_error_internal2, "osal: known error from rtos (stdlibspace)");
        } break;
        
        case oosiit_error_internal_sysmutex:
        {
            s_osal_error(osal_error_internal3, "osal: known error from rtos (sysmutex)");
        } break;
        
        case oosiit_error_memory_allocation:
        case oosiit_error_memory_preallocated:
        {
            s_osal_error(osal_error_missingmemory, "osal: memory is missing");
        } break;
        
        case oosiit_error_invalid_tskptr:
        {
            s_osal_error(osal_error_internal4, "osal: known error from rtos (used an invalid tskptr)");
        } break;
        
        case oosiit_error_invalid_objptr:
        {
            s_osal_error(osal_error_internal5, "osal: known error from rtos (used an invalid objptr)");
        } break;

        case oosiit_error_invalid_param:
        {
            s_osal_error(osal_error_internal6, "osal: known error from rtos (used an invalid param)");
        } break;
        
        case oosiit_error_invalid_call:
        {
            s_osal_error(osal_error_internal7, "osal: known error from rtos (used an invalid call)");
        } break;
 
        default:
        {
            snprintf(str, sizeof(str), "osal: error 0x%x from rtos", errorcode);
            s_osal_error(osal_error_generic, str);
        } break;
    }
}



// used only by the shalib as an entrypoint
extern void osal_hid_entrypoint(void)
{
    osal_base_initialise(NULL, NULL);
}


extern void osal_hid_reset_static_ram(void)
{
    // init static ram. it is necessary to do that only if we use a shalib.

    memset(&s_osal_oosiit_cfg, 0, sizeof(oosiit_cfg_t));  
    memset(&s_osal_osal_cfg, 0, sizeof(osal_cfg_t));

    s_osal_mutex_api_protection     = NULL;

    s_osal_info_status              = osal_info_status_zero;
    s_osal_stacklauncher_data       = NULL;
    s_osal_stacklauncher_size       = 0;

    s_osal_task_data                = NULL; // its size is number of tasks+2 (idle, launcher, tasks)
    s_osal_task_maxnum              = 0;
    s_osal_task_next                = 0;

    osal_usec_in_tick             = 0;
    s_osal_launcher_fn              = NULL;

    memset(s_resources_free, 0, osal_info_entity_numberof);
    memset(s_resources_used, 0, osal_info_entity_numberof);

    s_osal_arch_arm_armc99_sysmutex_number = 0;

    s_osal_launcher_fn = NULL;

    // values in oosiit whcih cannot wait for a call of the osiit init function called by osiit_memory_load()
    // which is called by osal_initialise()
    {
        extern struct OS_TSK os_tsk;
        memset(&os_tsk, 0, 2*sizeof(void*));        // trick .... it has two pointers
    }
}

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------


static void s_osal_on_fatalerror(osal_task_t *task, osal_fatalerror_t errorcode, const char * errormsg)
{
    if(NULL != s_osal_osal_cfg.extfn.usr_on_fatal_error)
    {
        s_osal_osal_cfg.extfn.usr_on_fatal_error(task, errorcode, errormsg);
        for(;;);
    }
    else
    {
        for(;;)
        {
            errorcode = errorcode;
        }
    }
}

static void s_osal_on_idle(void* p)
{
    p = p;
    
    s_osal_task_idle->rtostsk = oosiit_tsk_self();
    
    if(NULL != s_osal_osal_cfg.extfn.usr_on_idle)
    {
        s_osal_osal_cfg.extfn.usr_on_idle();
        for(;;);
    }
    else
    {
        for(;;);
    }
}


static void s_osal_error(osal_fatalerror_t err_code, const char *err_msg) 
{
    osal_task_t *task = NULL;

    osal_system_scheduling_suspend();
    
    if((osal_info_status_running == s_osal_info_status) || (osal_info_status_suspended == s_osal_info_status))
    { 
        task = osal_task_get(osal_callerAUTOdetect);
    }
    else
    {
        task = NULL;
    }


    if(NULL == err_msg)
    {   // use default message
        s_osal_on_fatalerror(task, err_code, "unspecified message");
    }
    else
    {   // use specific error
        s_osal_on_fatalerror(task, err_code, err_msg);   
    }

}


static void s_osal_fill_cfg(oosiit_cfg_t *oosiit_c, const osal_cfg_t *osal_c)
{
#if 0    
    oosiit_c->maxnumofusertasks                 = osal_c->tasknum;
    oosiit_c->checkStack                        = 1;   // always check stack overflow w/ osiit
    oosiit_c->sizeISRFIFO                       = 16; // ???? the enqueable requests done to rtos within an ISR
    oosiit_c->roundRobin                        = osal_c->roundrobin;
    oosiit_c->osClock                           = osal_c->cpufreq;
    oosiit_c->osTick                            = osal_c->tick;
    oosiit_c->roundRobinTimeout                 = osal_c->roundrobintick;
    
    oosiit_c->numAdvTimer                       = osal_c->timernum;
    oosiit_c->numMutex                          = osal_c->mutexnum + 1;  // +1 for internal sys mutex s_osal_mutex_api_protection
    oosiit_c->numSemaphore                      = osal_c->semaphorenum;
    oosiit_c->numMessageBox                     = osal_c->mqueuenum;
    oosiit_c->numMessageBoxElements             = osal_c->mqueueelemnum;
    oosiit_c->sizeof64alignedStack              = osal_c->globalstacksize; 
#else
    #if defined(OSAL_CPUFAM_CM4 )
    #define ISRQUEUESIZE 128
    #else
    #define ISRQUEUESIZE 16
    #endif
    oosiit_c->cpufreq                           = osal_c->cpufreq;
    oosiit_c->ticktime                          = osal_c->tick;
    oosiit_c->capacityofpostpendcommandfifo     = ISRQUEUESIZE; // ???? the enqueable requests done to rtos within an ISR
    oosiit_c->checkstackoverflow                = 1;   // always check stack overflow w/ osiit
    oosiit_c->memorymode                        = (osal_memmode_static == osal_c->memorymodel) ? oosiit_memmode_static : oosiit_memmode_dynamic; //oosiit_memmode_dynamic;
    oosiit_c->roundrobinenabled                 = osal_c->roundrobin;    
    oosiit_c->roundrobinnumberofticks           = osal_c->roundrobintick;    
    oosiit_c->maxnumofusertasks                 = osal_c->tasknum;
       
    oosiit_c->numAdvTimer                       = osal_c->timernum;
    oosiit_c->numMutex                          = osal_c->mutexnum + 1;  // +1 for internal sys mutex s_osal_mutex_api_protection
    oosiit_c->numSemaphore                      = osal_c->semaphorenum;
    oosiit_c->numMessageBox                     = osal_c->mqueuenum;
    oosiit_c->numMessageBoxElements             = osal_c->mqueueelemnum;
    oosiit_c->sizeof64alignedStack              = osal_c->globalstacksize; 
#endif    
}

//     oosiit_c->cpufreq                           = osal_c->cpufreq;
//     oosiit_c->ticktime                          = osal_c->tick;
//     oosiit_c->capacityofpostpendcommandfifo     = 16; // ???? the enqueable requests done to rtos within an ISR
//     oosiit_c->checkstackoverflow                = 1;   // always check stack overflow w/ osiit
//     oosiit_c->memoryallocation                  = oosiit_memalloc_atstartup;
//     oosiit_c->roundrobinenabled                 = osal_c->roundrobin;    
//     oosiit_c->roundrobinticktime                = osal_c->roundrobintick;
//     
//     oosiit_c->maxnumofusertasks                 = osal_c->tasknum;
//        
//     oosiit_c->numAdvTimer                       = osal_c->timernum;
//     oosiit_c->numMutex                          = osal_c->mutexnum + 1;  // +1 for internal sys mutex s_osal_mutex_api_protection
//     oosiit_c->numSemaphore                      = osal_c->semaphorenum;
//     oosiit_c->numMessageBox                     = osal_c->mqueuenum;
//     oosiit_c->numMessageBoxElements             = osal_c->mqueueelemnum;
//     oosiit_c->sizeof64alignedStack              = osal_c->globalstacksize; 

void tINIT(void* p)
{
    const int32_t s_SVCall_IRQn  = -5;
    //const int32_t s_PendSV_IRQn  = -2;
    //const int32_t s_SysTick_IRQn = -1;
    
    p = p;

    s_osal_mutex_api_protection = osal_mutex_new();

    if(NULL == s_osal_mutex_api_protection)
    {
        s_osal_error(osal_error_missingmemory, "osal: cannot create the s_osal_mutex_api_protection");
    }
    
    // must retrieve the rtostsk in here because we did not call a oosiit_tsk_create(). the system did
    s_osal_task_launcher->rtostsk = oosiit_tsk_self();

    // change the priority of the svc only. the pendsv and systick have fixed value. 
	// the default in osiit for svc, pendsv and systick is 14, 15, 15. 
    if(NULL != s_osal_osal_cfg.extfn.hal_sys_irqn_priority_set)
    {
        s_osal_osal_cfg.extfn.hal_sys_irqn_priority_set(s_SVCall_IRQn, s_osal_osal_cfg.prio);
        // the others must stay at priority 15....
        //s_osal_osal_cfg.extfn.hal_sys_irqn_priority_set(s_PendSV_IRQn, s_osal_osal_cfg.prio);
        //s_osal_osal_cfg.extfn.hal_sys_irqn_priority_set(s_SysTick_IRQn, s_osal_osal_cfg.prio);
    }
   

    // execute the user-defined function. it may never return ...
    s_osal_launcher_fn();

    // but if we are in here it means that the user wants to terminate the launcher task ...
    // thus i mark its stack as available by resetting its signature.
    //s_osal_stacklauncher_data[0] = (uint64_t)0;
    
    
    // delete the task ...

    // delete the launcher task
    oosiit_tsk_delete(oosiit_tsk_self());
    
}

static uint32_t s_osal_period2tick(osal_reltime_t period)
{
    period /= osal_usec_in_tick;

    if(osal_reltimeZERO == period)
    {   // cannot be zero
        period = 1; 
    }
    else if(period >= 0x80000000)
    {
        // there is a bug if called with period higher than 0x80000000
        // however, we have this value only if osal_usec_in_tick is either 1 or 2
        // and in case of osal_usec_in_tick = 1, the period is 2147 seconds.
        period = 0x80000000 - 1;
    }

    return(period);
}

static uint32_t s_osal_delay2tick(osal_reltime_t delay)
{
    return(delay / osal_usec_in_tick);
}

static uint64_t s_osal_abstime2tick(osal_abstime_t time)
{
    if(osal_abstimeNONE == time)
    {
        return(OOSIIT_ASAPTIME);
    }

    return(time / osal_usec_in_tick);
}

static uint32_t s_osal_timeout2tick(osal_reltime_t tout)
{
    if(osal_reltimeINFINITE == tout)
    {     // first, because it is likely to be the most probable
        return(OOSIIT_NOTIMEOUT); 
    }
    else if(osal_reltimeZERO == tout)
    {
        // second, because it is likely to be the next most probable
        return(0);
    }
    else
    {
        // else  ... must do division.
        return(tout / osal_usec_in_tick);
    }

}

static osal_obj_t* s_osal_osalobj_new(void *rtosobj, uint32_t signature)
{
    osal_obj_t* retval = NULL;
    
    if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
    {
        retval = osal_base_memory_new(sizeof(osal_obj_t));   
        if(NULL == retval)
        { 
            s_osal_error(osal_error_missingmemory, "osal: missing mem for osal object in osal_xxx_new()");
            //return(NULL);
        }  
        retval->rtosobj     = rtosobj;
        retval->signature   = signature;
    }
    else
    {
        retval = rtosobj;
    }
    
    return(retval);
}

static void* s_osal_rtosobj_get(osal_obj_t *osalobj, uint32_t signature)
{
    void* rtosobj = NULL;
        
    if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
    {
        if((NULL == osalobj->rtosobj) || (signature != osalobj->signature))
        { 
            return(NULL);
        } 

        rtosobj = osalobj->rtosobj;       
    } 
    else
    {
        rtosobj = osalobj; 
    }
    
    return(rtosobj);
}

static void s_osal_rtosobj_clr(osal_obj_t *osalobj)
{
    if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
    {
        osalobj->rtosobj    = NULL;
        osalobj->signature  = 0;
        osal_base_memory_del(osalobj);
    } 
}

static void s_osal_taskobj_init(void* memory)
{
    if(osal_memmode_static == s_osal_osal_cfg.memorymodel)
    {
        s_osal_task_data    = memory;
        s_osal_task_maxnum  = s_osal_osal_cfg.tasknum + 2;
        s_osal_task_next    = 0; 
    }
    else
    {
        s_osal_task_data    = NULL;
        s_osal_task_maxnum  = 0;
        s_osal_task_next    = 0; 
    }    
}

static osal_task_t* s_osal_taskobj_new(void)
{
    osal_task_t* taskobj = NULL;

    if(osal_memmode_static == s_osal_osal_cfg.memorymodel)
    {
        if(s_osal_task_next >= s_osal_task_maxnum)
        {
            // cannot get a task
            return(NULL);        
        }
        taskobj = &s_osal_task_data[s_osal_task_next++];
    }
    else if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
    {
        taskobj = osal_base_memory_new(sizeof(osal_task_t));   
    }        
    
    return(taskobj);   
}

static void s_osal_taskobj_del(osal_task_t* taskobj)
{
    if(osal_memmode_dynamic == s_osal_osal_cfg.memorymodel)
    {  
        osal_base_memory_del(taskobj);
    }      
}


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------




