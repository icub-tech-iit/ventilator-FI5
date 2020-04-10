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
#ifndef _OSAL_ARCH_ARM_H_
#define _OSAL_ARCH_ARM_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file       osal_arch_arm.h
    @brief      This header file implements public interface to the os abstraction layer: arch arm
    @author     marco.accame@iit.it
    @date       12/05/2011
**/

/* @defgroup osal_arch_arm OS abstraction layer: arch arm
    cecece.
    
    @{        
 **/


// - external dependencies --------------------------------------------------------------------------------------------

#include "osal_base.h"


// - public #define  --------------------------------------------------------------------------------------------------
// empty-section


 
// - declaration of public user-defined types ------------------------------------------------------------------------- 
// empty-section

    
// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section


// - declaration of extern public functions ---------------------------------------------------------------------------


/** @fn         extern void * osal_arch_arm_armc99stdlib_getlibspace(uint8_t firstcall)
    @brief      It can be used to redefine the function __user_perthread_libspace() used by ARM C standard libray to obtain
                a user space for each thread (aka osal_task_t entity), as described in 
                http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdicbfj.html
                The redefinition of __user_perthread_libspace() shall be placed in a .c file and have the following code
                void * __user_perthread_libspace(void) 
                { static volatile uint8_t fc = 1; void *ret = osal_arch_arm_armc99stdlib_getlibspace(fc);
                  fc = 0; return(ret);
                }
    @param      firstcall       Must be 1 only the first time        
    @return     a pointer to be used as return value for __user_perthread_libspace().
**/
extern void * osal_arch_arm_armc99stdlib_getlibspace(uint8_t firstcall);


/** @fn         extern int osal_arch_arm_armc99stdlib_mutex_initialize(void *m)
    @brief      It can be used to redefine the function _mutex_initialize() used by ARM C standard libray to obtain
                a mutex for protection of the non-reentrant functions of the standard library, such as malloc(), as 
                described in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdfjddj.html
                The redefinition of _mutex_initialize() shall be placed in a .c file and have the following code
                int _mutex_initialize(void *m) 
                { 
                  return(osal_arch_arm_armc99stdlib_mutex_initialize(m)); 
                } 
    @param      m           Pointer to the mutex, as ... "For the C library, a mutex is specified as a single 32-bit word 
                            of memory that can be placed anywhere. However, if your mutex implementation requires more space 
                            than this, or demands that the mutex be in a special memory area, then you must treat the default 
                            mutex as a pointer to a real mutex".      
    @return     a non-zero value as ... "in a multithreaded application, _mutex_initialize() must return a nonzero value 
                on success so that at runtime, the library knows that it is being used in a multithreaded environment".
**/
extern int osal_arch_arm_armc99stdlib_mutex_initialize(void *m);


/** @fn         extern void osal_arch_arm_armc99stdlib_mutex_acquire(void *m)
    @brief      It can be used to redefine the function _mutex_acquire() used by ARM C standard libray to acquire
                a mutex for protection of the non-reentrant functions of the standard library, such as malloc(), as 
                described in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdfjddj.html
                The redefinition of _mutex_acquire() shall be placed in a .c file and have the following code
                void _mutex_acquire(void *m) 
                { 
                  osal_arch_arm_armc99stdlib_mutex_acquire(m); 
                } 
    @param      m           Pointer to the mutex, as ... "For the C library, a mutex is specified as a single 32-bit word 
                            of memory that can be placed anywhere. However, if your mutex implementation requires more space 
                            than this, or demands that the mutex be in a special memory area, then you must treat the default 
                            mutex as a pointer to a real mutex".      
**/
extern void osal_arch_arm_armc99stdlib_mutex_acquire(void *m);


/** @fn         extern void osal_arch_arm_armc99stdlib_mutex_release(void *m)
    @brief      It can be used to redefine the function _mutex_release() used by ARM C standard libray to release
                a mutex for protection of the non-reentrant functions of the standard library, such as malloc(), as 
                described in http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0475c/Chdfjddj.html
                The redefinition of _mutex_release() shall be placed in a .c file and have the following code
                void _mutex_release(void *m) 
                { 
                  osal_arch_arm_armc99stdlib_mutex_release(m); 
                } 
    @param      m           Pointer to the mutex, as ... "For the C library, a mutex is specified as a single 32-bit word 
                            of memory that can be placed anywhere. However, if your mutex implementation requires more space 
                            than this, or demands that the mutex be in a special memory area, then you must treat the default 
                            mutex as a pointer to a real mutex".      
**/
extern void osal_arch_arm_armc99stdlib_mutex_release(void *m);



/* @}            
    end of group osal_arch_cfg  
 **/
 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



