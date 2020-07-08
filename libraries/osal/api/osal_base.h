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
#ifndef _OSAL_BASE_H_
#define _OSAL_BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_base.h
    @brief      This header file implements public interface to the os abstraction layer: base services and types
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_base OS abstraction layer: base services and types
    The OSAL is an abstraction layer towards a real-time operative system.
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "stdint.h"
#include "osal_arch_cfg.h"


// - public #define  --------------------------------------------------------------------------------------------------

/** @def        OSAL_reltimeZERO
    @brief      Equals 0 micro seconds and can be used to avoid blocking in functions with timeout
 **/
#define     OSAL_reltimeZERO                               (0)

/** @def        OSAL_reltime1ms
    @brief      Equals 1 ms 
 **/
#define     OSAL_reltime1ms                               (1000)

/** @def        OSAL_reltime1sec
    @brief      Equals 1 sec 
 **/
#define     OSAL_reltime1sec                               (1000000)

/** @def        OSAL_abstimeMAX
    @brief      Equals 4294 seconds and is the maximum relative time available
 **/
#define     OSAL_reltimeMAX                             (0xfffffffe)

/** @def        OSAL_abstimeINFINITE
    @brief      Infinite relative time in OSAL. It can be used to specify not timeout.
 **/
#define     OSAL_reltimeINFINITE                        (0xffffffff)

/** @def        OSAL_abstimeNONE
    @brief      Contains an invalid value for the life time (absolute time measure in micro-seconds)
 **/
#define     OSAL_abstimeNONE                            (0xffffffffffffffff)


 
// - declaration of public user-defined types ------------------------------------------------------------------------- 


/** @typedef    typedef enum osal_result_t 
    @brief      osal_result_t contains all possible results from a function
 **/ 
typedef enum 
{
    osal_res_OK                     = 0,    /**< correct result */
    osal_res_NOK_generic            = -1,   /**< generic error */
    osal_res_NOK_nullpointer        = -2,   /**< error due to a null pointer */
    osal_res_NOK_timeout            = -3    /**< the task did not get the resource after it waited for some time (or also zero time). */
} osal_result_t;


/** @typedef    typedef enum osal_cpufamily_t 
    @brief      osal_cpufamily_t contains all possible families of CPUs supported by OSAL
 **/ 
typedef enum
{
    osal_cpufam_armcm3              = 0,    /**< ARM Cortex M3 */
    osal_cpufam_armcm4              = 1,    /**< ARM Cortex M4 with FPU */
    osal_cpufam_dspic               = 2     /**< not supported */
} osal_cpufamily_t;


/** @typedef    typedef enum osal_cputype_t 
    @brief      osal_cputype_t contains a type of CPU within a given family. we leave 32 possible CPU types for each
                CPU family.
 **/ 
typedef enum
{
    osal_cputyp_stm32f1x            = 0,    /**< STM32F1x processors */
    osal_cputyp_stm32f2x            = 1,    /**< STM32F2x processors */
    osal_cputyp_stm32f4x            = 32,   /**< STM32F4x processors */
    osal_cputyp_dspic33             = 64    /**< not supported */
} osal_cputype_t;


/** @typedef    typedef enum osal_rtostype_t 
    @brief      osal_rtostype_t contains all possible RTOSes supported by OSAL
 **/ 
typedef enum
{
    osal_rtostype_iitmod_rtxarm     = 0,    /**< a version of commercial RTX by Keil, enhanced by IIT with some extra features */
    osal_rtostype_oosiit            = 1,    /**< a version of the open-source RTX by Keil, enhanced by IIT with some extra features */
    osal_rtostype_iitmod_freertos   = 2,    /**< not supported so far */
    osal_rtostype_iitmod_ucosii     = 3,    /**< not supported so far */
    osal_rtostype_iitmod_ucosiii    = 4     /**< not supported so far */
} osal_rtostype_t;


/** @typedef    typedef enum osal_memorymodel_t 
    @brief      osal_memorymodel_t contains all possible memory models supported by OSAL
    @details    With static memory mode, we mean a behaviour in which the OSAL loads beforehands all the memory it needs to offer 
                the services specified by a given configuration.  For instance, if the configuration specifies
                7 user tasks, then the OSAL needs to be initialised with all the externally provided memory for the 7 user tasks.
                At the creation of a new object, a task in our example, the OSAL shall use the memory buffer it has loaded and does not
                perform any dynamical allocation (e.g., with malloc() or the likes of it).
                With dynamic memory mode, we mean a a behaviour in which the OSAL does not load any memory beforehands and it performs
                dynamically allocation in runtime as long as the user needs an object.
 **/ 
typedef enum
{
    osal_memmode_static             = 0,    /**< uses statically allocated memory. the only supported so far */
    osal_memmode_dynamic            = 1     /**< uses memory allocated at run time from the heap. not supported so far */,
} osal_memorymodel_t;



/** @typedef    typedef enum osal_fatalerror_t 
    @brief      osal_fatalerror_t contains system specific error codes used in user-defined function usr_on_fatal_error()
 **/ 
typedef enum 
{
    osal_error_generic              = 0xE0,             
    osal_error_missingconfiguration = 0xE1,       
    osal_error_missingmemory        = 0xE2,       
    osal_error_incorrectparameter   = 0xE3,       
    osal_error_unsupportedbehaviour = 0xE4,
    osal_error_stackoverflow        = 0xE5,
    osal_error_invalidcall          = 0xE6,
    osal_error_internal0            = 0xF0,
    osal_error_internal1            = 0xF1,
    osal_error_internal2            = 0xF2,
    osal_error_internal3            = 0xF3,             
    osal_error_internal4            = 0xF4,
    osal_error_internal5            = 0xF5,
    osal_error_internal6            = 0xF6,
    osal_error_internal7            = 0xF7,
    osal_error_unknownfromrtos      = 0xD0
} osal_fatalerror_t;


/** @typedef    typedef enum osal_boolval_t 
    @brief      osal_boolval_t contains values for bool type (osal_bool_t).
 **/ 
typedef enum
{
    osal_false = 0,      /**< false */
    osal_true  = 1       /**< true */
} osal_boolval_t;


/** @typedef    typedef enum osl_bool_t 
    @brief      osal_bool_t is the bool type. 
 **/ 
typedef uint8_t     osal_bool_t;


/** @typedef    typedef uint32_t osal_reltime_t 
    @brief      osal_reltime_t is the relative time used by OSAL. It is expressed in micro-seconds and it max value
                is around 4294 seconds.
 **/  
typedef uint32_t    osal_reltime_t;


/** @typedef    typedef osal_reltime_t  osal_time_t
    @brief      osl_time_t is an alias for osl_reltime_t
 **/ 
typedef osal_reltime_t  osal_time_t;   
         

/** @typedef    typedef uint64_t osal_abstime_t
    @brief      osal_abstime_t is the absolute time used by OSAL. It is expressed in micro-seconds and it max value
                is around 5848 centuries.
 **/
typedef uint64_t osal_abstime_t;    


/** @typedef    typedef uint64_t osal_nanotime_t 
    @brief      osal_nanotime_t express very precise relative time in OSAL in form of nsec (nano-seconds).
                A osl_nanotime_t variable keeps time up to 584 years.
 **/  
typedef uint64_t    osal_nanotime_t;

 
/** @typedef    typedef enum osal_caller_t 
    @brief      osal_caller_t contains all possible callers of an osal function
 **/  
typedef enum 
{
    osal_callerTSK                          = 0,     /** < the caller is a task */
    osal_callerISR                          = 1,     /** < the caller is an ISR */
    osal_callerTMRMAN                       = 2,     /** < the caller is the timer manager */
    osal_callerAUTOdetect                   = 3      /** < the OSAL automatcically detects the caller (supported only with OOSIIT on CMx) */
} osal_caller_t; 


/** @typedef    typedef enum osal_priorityvalues_t 
    @brief      osal_priorityvalues_t contains all possible states of OSAL
 **/ 
typedef enum
{
    osal_prio_systsk_idle                   = 0,       
    osal_prio_systsk_usrwhencreated         = 1,
    osal_prio_usrtsk_min                    = 2,
    osal_prio_usrtsk_max                    = 251,
    osal_prio_systsk_launcher               = 254,
    osal_prio_systsk_scheduler              = 255,
    osal_prio_systsk_timerman_iitmod_rtxarm = 255   /**< in osal_rtostype_iitmod_rtxarm the timer manager is executed by the scheduler */       
} osal_priorityvalues_t;



/** @typedef    typedef struct osal_extfn_cfg_t
    @brief      osal_extfn_cfg_t keeps pointers to functions which OSAL internally calls and that can be redefined by the user 
 **/
typedef struct
{
    /** Used to change the HW priority of the ISRs used for the scheduler. If NULL then the lowest priority is used. Use types from HAL */ 
    void            (*hal_sys_irqn_priority_set)(int32_t irqn, uint8_t priority);    
    /** When a fatal error is detected, at first the scheduler is disabled, then it is called this function usr_on_fatal_error()
       (if not NULL) and then the system enters in a forever loop. First parameter is the calling task, second is the error code, 
       third an error message from the OSAL. */ 
    void            (*usr_on_fatal_error)(void* task, osal_fatalerror_t errorcode, const char * errormsg);   
    /** The function executed when no task is scheduled. If not redefined, it just runs a forever loop. */ 
    void            (*usr_on_idle)(void);
} osal_extfn_cfg_t;


/** @typedef    typedef struct osal_cfg_t
    @brief      osal_cfg_t keeps the configuration of OSAL. This type should be used to instantiate one constant
                variable, possibly using osal_cfg.[h, c] files.
 **/
typedef struct
{
    osal_rtostype_t     rtostype;               /**< The underlying RTOS   */   
    osal_memorymodel_t  memorymodel;            /**< The used memory model */
    uint8_t             prio;                   /**< the HW priority of the OSAL (aka the priority of the handler of osal_ calls)    */                  
    osal_cpufamily_t    cpufam;                 /**< The CPU type                     */                                 
    uint32_t            cpufreq;                /**< The CPU frequency in Hz                                    */ 
    osal_reltime_t      tick;                   /**< The tick of the OSAL expressed in micro-sec  */ 
    uint16_t            launcherstacksize;      /**< The stack size (in bytes) of the launcher task             */               
    uint16_t            idlestacksize;          /**< The stack size (in bytes) of the idle task                 */
    osal_bool_t         roundrobin;             /**< If osal_true the OSAL uses round robin on tasks of equal priority, else it does not  */
    osal_reltime_t      roundrobintick;         /**< The maximum timeslot used for round robin expressed in micro-sec */

    // sizes relevant only for memorymodel osal_memmode_static    
    uint8_t             tasknum;                /**< The maximum number of user tasks managed by OSAL (apart launcher and idle tasks) */
    uint16_t            globalstacksize;        /**< The global stack size (in bytes) reserved for user tasks        */
    uint8_t             timernum;               /**< The maximum number of timers managed by OSAL */
    uint8_t             mutexnum;               /**< The maximum number of mutexes managed by OSAL */    
    uint8_t             semaphorenum;           /**< The maximum number of semaphores managed by OSAL */
    uint8_t             mqueuenum;              /**< The maximum number of messagequeues managed by OSAL */
    uint16_t            mqueueelemnum;          /**< The maximum total number of messages contained in all the possible message queues */
    
    // additional configuration
    osal_arch_cfg_t     arch;                   /**< the configuration part relative to the architecture specified in cpufam */
    osal_extfn_cfg_t    extfn;                  /**< User-defined behaviour defined with external function pointers */
} osal_cfg_t;

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------


/** @var        extern const osal_reltime_t osal_reltimeZERO
    @brief      Equals 0 micro seconds and can be used to avoid blocking in functions with timeout
 **/
extern const osal_reltime_t osal_reltimeZERO;

/** @var        extern const osal_reltime_t osal_reltime1ms
    @brief      Equals 1 ms
 **/
extern const osal_reltime_t osal_reltime1ms;

/** @var        extern const osal_reltime_t osal_reltime1sec
    @brief      Equals 1 sec
 **/
extern const osal_reltime_t osal_reltime1sec;

/** @var        extern const osal_reltime_t osal_reltimeMAX
    @brief      Equals 4294 seconds and is the maximum relative time available
 **/
extern const osal_reltime_t osal_reltimeMAX;

/** @var        extern const osal_reltime_t osal_reltimeINFINITE
    @brief      Infinite relative time in OSAL. It can be used to specify not timeout.
 **/
extern const osal_reltime_t osal_reltimeINFINITE;

/** @def        extern const osal_abstime_t osal_abstimeNONE
    @brief      Contains an invalid value for the abstime (absolute time measure in micro-seconds)
 **/
extern const osal_abstime_t osal_abstimeNONE;


// - declaration of extern public functions ---------------------------------------------------------------------------

/** @fn         extern void* osal_ext_calloc(uint32_t s, uint32_t n)
    @brief      memory allocator such as the normal calloc(). It must be externally defined for use in 
                osal_base_memory_new()
 **/ 
extern void* osal_ext_calloc(uint32_t s, uint32_t n);


/** @fn         extern void* osal_ext_realloc(void* m, uint32_t s)
    @brief      memory reallocator such as the normal realloc(). It must be externally defined for use in 
                osal_base_memory_realloc()
 **/ 
extern void* osal_ext_realloc(void* m, uint32_t s);


/** @fn         extern void osal_ext_free(void* m)
    @brief      memory deallocator such as the normal free(). It must be externally defined for use in 
                osal_base_memory_del()
 **/ 
extern void osal_ext_free(void* m);


/** @fn         extern uint32_t osal_base_memory_getsize(const osal_cfg_t *cfg, uint32_t *size08aligned)
    @brief      Gets the size of the 8-byte aligned memory required by the osal for configuration @e cfg.
    @param      cfg             Pointer to the requested configuration.
    @param      size08aligned   Size of the 8-byte aligned memory which is required to run the osal. If 
                                the passed value is NULL, then the function does not use it to store the
                                return value.
    @return     The size the 8-byte aligned memory which is required to run the osal
 **/
extern uint32_t osal_base_memory_getsize(const osal_cfg_t *cfg, uint32_t *size08aligned);


/** @fn         extern void* osal_base_memory_new(uint32_t size)
    @brief      Thread safe memory allocator. It cannot be called from within an ISR. 
    @param      size            the size of the requested memory in bytes.
    @return     if successful a proper 8-aligned memory pointer, otherwise it returns NULL.
 **/ 
extern void* osal_base_memory_new(uint32_t size);


/** @fn         extern void* osal_base_memory_realloc(void* mem, uint32_t size)
    @brief      Thread safe memory reallocator. It cannot be called from within an ISR. 
    @param      mem             the memory to reallocate. if NULL, it is allocated.
    @param      size            the size of the requested memory in bytes.
    @return     if successful a proper 8-aligned memory pointer, otherwise it returns NULL.
 **/ 
extern void* osal_base_memory_realloc(void* mem, uint32_t size);


/** @fn         extern void osal_base_memory_del(void* mem)
    @brief      Thread-safe memory free. It cannot be called from within an ISR. 
    @param      mem             the pointer to the memory to be de-allocated.
 **/ 
extern void osal_base_memory_del(void* mem);


/** @fn         extern osal_result_t osal_base_initialise(const osal_cfg_t *cfg, uint64_t *data08aligned)
    @brief      Initialise the osal with the memory required by the configuration @e cfg. The function can be called
                multiple times, but not after osal_start().
    @param      cfg             Pointer to the requested configuration.
    @param      data08aligned   8-byte aligned memory which is required to run the osal.
    @return     On success osal_res_OK, osal_res_nullpointer in case of invalid arguments, it issues a fatal error if
                called after osal_start().
 **/
extern osal_result_t osal_base_initialise(const osal_cfg_t *cfg, uint64_t *data08aligned);


/* @}            
    end of group osal_base  
 **/
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

