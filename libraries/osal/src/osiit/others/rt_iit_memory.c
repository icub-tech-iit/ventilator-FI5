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
 
/* @file       rt_iit_memory.c
	@brief      This file implements internals of the memory module for oosiit
	@author     marco.accame@iit.it
    @date       07/30/2012
**/


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------


#include "rt_TypeDef.h"
#include "RTX_Config.h" 
#include "rt_MemBox.h"
#include "rt_Mutex.h"
#include "string.h"
#include "stdlib.h"


#include "oosiit_storage_hid.h"



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "rt_iit_memory.h"


// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

// define it if the calls to 
#define DONTUSE_INTERNAL_MUTEX

#if defined(DONTUSE_INTERNAL_MUTEX)    
    #define ALLOC_MUTEX()       NULL
    #define TAKE_MUTEX(m, t) 
    #define RELEASE_MUTEX(m)    
#else
    #define ALLOC_MUTEX()       rt_iit_memory_new(sizeof(struct OS_MUCB))
    #define TAKE_MUTEX(m, t)    (NULL == m) ? (;) : (iitchanged_rt_mut_wait(m, t))
    #define RELEASE_MUTEX(m)    (NULL == m) ? (;) : (rt_mut_release(m))
#endif  


// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------
static U32 *s_index_mutex = NULL;
static U32 *s_index_semaphore = NULL;
static U32 *s_index_mbox = NULL;
static U64 *s_index_stack = NULL;
static U32 *s_mutex_memory = NULL;




// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern void rt_iit_memory_init(void) 
{
    // init oosiit memory which could be accessed using an _alloc_box()
    if(NULL != oosiit_cfg_mutex)
    {
        s_index_mutex = (U32 *)oosiit_cfg_mutex;
        memset(s_index_mutex, 0, oosiit_cfg_mutex_size);
    }
    
    if(NULL != oosiit_cfg_semaphore)
    {
        s_index_semaphore = (U32 *)oosiit_cfg_semaphore;
        memset(s_index_semaphore, 0, oosiit_cfg_semaphore_size);
    }
   
    // init iit memory which cannot be accessed with fixed blocks.
    if(NULL != oosiit_cfg_mbox)
    {
        s_index_mbox = (U32 *)oosiit_cfg_mbox;
        memset(s_index_mbox, 0, oosiit_cfg_mbox_size);
    }
    
    if(NULL != oosiit_cfg_globstack)
    {
        s_index_stack = (U64 *)oosiit_cfg_globstack;
        memset(s_index_stack, 0x11, oosiit_cfg_globstack_size);
    }
    
    // init the mutex
    if(NULL != oosiit_cfg_mutex_memory)
    {
        s_mutex_memory = (U32 *)oosiit_cfg_mutex_memory;
        rt_mut_init(s_mutex_memory);            
    }
   
}


extern OS_ID rt_iit_memory_getmut(void) 
{
    void *ret = NULL;
       
    // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_mutex)
    {
        ret = rt_iit_memory_new(sizeof(struct OS_MUCB));
    }
    else
    {    
        if(((U32)s_index_mutex + sizeof(struct OS_MUCB) - (U32)oosiit_cfg_mutex) > oosiit_cfg_mutex_size)
        {
            RELEASE_MUTEX(s_mutex_memory);
            oosiit_sys_error(oosiit_error_memory_preallocated);
            return(NULL);   // dont have any memory anymore or it was empty (every var above is NULL or zero)
        }
        
        // ok, i have memory enough... assign the pointer to return value
        ret = s_index_mutex;
        s_index_mutex += (sizeof(struct OS_MUCB)/4);
    }
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
    
    // return value
    return(ret);
}

extern void rt_iit_memory_relmut(OS_ID mut) 
{
     // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_mutex)
    {
        rt_iit_memory_del(mut);
    }
    else
    {
        // unfortunately in static mode we cannot give memory back 
    }    
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
}

extern OS_ID rt_iit_memory_getsem(void) 
{
    void *ret = NULL;
    
    // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_semaphore)
    {
        ret = rt_iit_memory_new(sizeof(struct OS_SCB));
    }
    else
    {
    
        if(((U32)s_index_semaphore + sizeof(struct OS_SCB) - (U32)oosiit_cfg_semaphore) > oosiit_cfg_semaphore_size)
        {
            RELEASE_MUTEX(s_mutex_memory);   
            oosiit_sys_error(oosiit_error_memory_preallocated);
            return(NULL);   // dont have any memory anymore or it was empty (every var above is NULL or zero)
        }
        
        // ok, i have memory enough... assign the pointer to return value
        ret = s_index_semaphore;
        s_index_semaphore += (sizeof(struct OS_SCB)/4);  
        
    }    
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
    
    // return value
    return(ret);
}

extern void rt_iit_memory_relsem(OS_ID sem) 
{
     // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_semaphore)
    {
        rt_iit_memory_del(sem);
    }
    else
    {
        // unfortunately in static mode we cannot give memory back 
    }    
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
}

extern OS_ID rt_iit_memory_getmbx(U16 nitems) 
{
    void *ret = NULL;
        
    // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);  
    
    if(NULL == oosiit_cfg_mbox)
    {
        ret = rt_iit_memory_new((sizeof(struct OS_MCB)-sizeof(void*))+nitems*sizeof(void*));
    }
    else
    {
        
        if(((U32)s_index_mbox + ((sizeof(struct OS_MCB)-4)+4*nitems) - (U32)oosiit_cfg_mbox) > oosiit_cfg_mbox_size)
        {
            RELEASE_MUTEX(s_mutex_memory);
            oosiit_sys_error(oosiit_error_memory_preallocated);
            return(NULL);   // dont have any memory anymore or it was empty (every var above is NULL or zero)
        }
        
        
        // ok, i have memory enough... assign the pointer to return value
        ret = s_index_mbox;
        s_index_mbox += (((sizeof(struct OS_MCB)-4)+4*nitems)/4);
    
    }
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
    
    // return value
    return(ret);
}

extern void rt_iit_memory_relmbx(OS_ID mbx) 
{
     // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_mbox)
    {
        rt_iit_memory_del(mbx);
    }
    else
    {
        // unfortunately in static mode we cannot give memory back 
    }    
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
}


extern U64* rt_iit_memory_getstack(U16 nbytes) 
{
    U64 *ret = NULL;
    
    U16 items = (nbytes+7) / 8;
    
    // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_globstack)
    {
        ret = rt_iit_memory_new(8*items);
        memset(ret, 0x11, 8*items);
    }
    else
    {        
        if(((U32)s_index_stack + (items)*8 - (U32)oosiit_cfg_globstack) > oosiit_cfg_globstack_size)
        {
            RELEASE_MUTEX(s_mutex_memory); 
            oosiit_sys_error(oosiit_error_memory_preallocated);
            return(NULL);   // dont have any memory anymore or it was empty (every var above is NULL or zero)
        }
        
        
        // ok, i have memory enough... assign the pointer to return value
        ret = s_index_stack;
        s_index_stack += items;
    
    }
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
    
    // return value
    return(ret);
}

extern void rt_iit_memory_relstack(void* stack) 
{
     // take mutex
    TAKE_MUTEX(s_mutex_memory, 0xFFFFFFFF);
    
    if(NULL == oosiit_cfg_globstack)
    {
        rt_iit_memory_del(stack);
    }
    else
    {
        // unfortunately in static mode we cannot give memory back 
    }    
 
    // release mutex
    RELEASE_MUTEX(s_mutex_memory);
}


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section



// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


