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
#ifndef _OOSIIT_SVC_H_
#define _OOSIIT_SVC_H_


#ifdef __cplusplus
extern "C" {
#endif


/** @file       oosiit_svc.h
	@brief      This header file contains svc interface to the IIT extension to open source CMSIS-RTOS RTX kernel
                The code in here is taken from rt_CMSIS.c file, of which we retain the original copyright note
	@author     marco.accame@iit.it
	@date       07/23/2012
**/


/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    rt_CMSIS.c
 *      Purpose: CMSIS RTOS API
 *      Rev.:    V4.20
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

#define __CMSIS_GENERIC
//#undef __CMSIS_GENERIC

#if defined (__CORTEX_M4) || defined (__CORTEX_M4F)
  #include "core_cm4.h"
#elif defined (__CORTEX_M3)
  #include "core_cm3.h"
#elif defined (__CORTEX_M0)
  #include "core_cm0.h"
#else
  #error "Missing __CORTEX_Mx definition"
#endif

#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_Event.h"
#include "rt_List.h"
#include "rt_Time.h"
#include "rt_Mutex.h"
#include "rt_Semaphore.h"
#include "rt_Mailbox.h"
#include "rt_MemBox.h"
#include "rt_Memory.h"
#include "rt_HAL_CM.h"

//IIT-EXT: removed as we dont want to use the cmsis version and other things
//#define os_thread_cb OS_TCB
//
//#include "cmsis_os.h"
//
//#if (osFeature_Signals != 16)
//#error Invalid "osFeature_Signals" value!
//#endif
//#if (osFeature_Semaphore != 8)
//#error Invalid "osFeature_Semaphore" value!
//#endif
//#if (osFeature_Wait != 0)
//#error osWait not supported!
//#endif
//IIT-EXT: end

// ==== Enumeration, structures, defines ====


// Service Calls defines

//IIT-EXT: beginning of the definition of svc macros on compilers: armcc, gcc, iar
#if defined (__CC_ARM)          /* ARM Compiler */

#define __NO_RETURN __declspec(noreturn)

#define osEvent_type       osEvent
#define osEvent_ret_status ret
#define osEvent_ret_value  ret
#define osEvent_ret_msg    ret
#define osEvent_ret_mail   ret

#define osCallback_type    osCallback
#define osCallback_ret     ret

#define SVC_0_1(f,t,...)                                                       \
__svc_indirect(0) t  _##f (t(*)());                                            \
                  t     f (void);                                              \
__attribute__((always_inline))                                                 \
static __inline   t __##f (void) {                                             \
  return _##f(f);                                                              \
}

#define SVC_1_1(f,t,t1,...)                                                    \
__svc_indirect(0) t  _##f (t(*)(t1),t1);                                       \
                  t     f (t1 a1);                                             \
__attribute__((always_inline))                                                 \
static __inline   t __##f (t1 a1) {                                            \
  return _##f(f,a1);                                                           \
}

#define SVC_2_0(f,t,t1,t2,...)                                                 \
__svc_indirect(0) t  _##f (t(*)(t1,t2),t1,t2);                                 \
                  t     f (t1 a1, t2 a2);                                      \
__attribute__((always_inline))                                                 \
static __inline   t __##f (t1 a1, t2 a2) {                                     \
   _##f(f,a1,a2);                                                        \
}

#define SVC_2_1(f,t,t1,t2,...)                                                 \
__svc_indirect(0) t  _##f (t(*)(t1,t2),t1,t2);                                 \
                  t     f (t1 a1, t2 a2);                                      \
__attribute__((always_inline))                                                 \
static __inline   t __##f (t1 a1, t2 a2) {                                     \
  return _##f(f,a1,a2);                                                        \
}

#define SVC_3_1(f,t,t1,t2,t3,...)                                              \
__svc_indirect(0) t  _##f (t(*)(t1,t2,t3),t1,t2,t3);                           \
                  t     f (t1 a1, t2 a2, t3 a3);                               \
__attribute__((always_inline))                                                 \
static __inline   t __##f (t1 a1, t2 a2, t3 a3) {                              \
  return _##f(f,a1,a2,a3);                                                     \
}

#define SVC_4_1(f,t,t1,t2,t3,t4,...)                                           \
__svc_indirect(0) t  _##f (t(*)(t1,t2,t3,t4),t1,t2,t3,t4);                     \
                  t     f (t1 a1, t2 a2, t3 a3, t4 a4);                        \
__attribute__((always_inline))                                                 \
static __inline   t __##f (t1 a1, t2 a2, t3 a3, t4 a4) {                       \
  return _##f(f,a1,a2,a3,a4);                                                  \
}

#define SVC_5_1(f,t,t1,t2,t3,t4,t5,...)                                           \
__svc_indirect(0) t  _##f (t(*)(t1,t2,t3,t4,t5),t1,t2,t3,t4,t5);                     \
                  t     f (t1 a1, t2 a2, t3 a3, t4 a4, t5 a5);                        \
__attribute__((always_inline))                                                 \
static __inline   t __##f (t1 a1, t2 a2, t3 a3, t4 a4, t5 a5) {                       \
  return _##f(f,a1,a2,a3,a4,a5);                                                  \
}

#define SVC_1_2 SVC_1_1 
#define SVC_1_3 SVC_1_1 
#define SVC_2_3 SVC_2_1 

#elif defined (__GNUC__)        /* GNU Compiler */

#define __NO_RETURN __attribute__((noreturn))

typedef uint32_t __attribute__((vector_size(8)))  ret64;
typedef uint32_t __attribute__((vector_size(16))) ret128;

#define RET_pointer    __r0
#define RET_int32_t    __r0
#define RET_uint32_t   __r0
#define RET_osStatus   __r0
#define RET_osPriority __r0
#define RET_osEvent    {(osStatus)__r0, {(uint32_t)__r1}, {(void *)__r2}}
#define RET_osCallback {(void *)__r0, (void *)__r1}

#define osEvent_type        ret128
#define osEvent_ret_status (ret128){ret.status}
#define osEvent_ret_value  (ret128){ret.status, ret.value.v}
#define osEvent_ret_msg    (ret128){ret.status, ret.value.v, (uint32_t)ret.def.message_id}
#define osEvent_ret_mail   (ret128){ret.status, ret.value.v, (uint32_t)ret.def.mail_id}

#define osCallback_type     ret64
#define osCallback_ret     (ret64) {(uint32_t)ret.fp, (uint32_t)ret.arg}

#define SVC_ArgN(n) \
  register int __r##n __asm("r"#n);

#define SVC_ArgR(n,t,a) \
  register t   __r##n __asm("r"#n) = a;

#define SVC_Arg0()                                                             \
  SVC_ArgN(0)                                                                  \
  SVC_ArgN(1)                                                                  \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg1(t1)                                                           \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgN(1)                                                                  \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg2(t1,t2)                                                        \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg3(t1,t2,t3)                                                     \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgR(2,t3,a3)                                                            \
  SVC_ArgN(3)

#define SVC_Arg4(t1,t2,t3,t4)                                                  \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgR(2,t3,a3)                                                            \
  SVC_ArgR(3,t4,a4)

#if (defined (__CORTEX_M0))
#define SVC_Call(f)                                                            \
  __asm volatile                                                                 \
  (                                                                            \
    "ldr r7,="#f"\n\t"                                                         \
    "mov r12,r7\n\t"                                                           \
    "svc 0"                                                                    \
    :               "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)         \
    :                "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)         \
    : "r7", "r12", "lr", "cc"                                                  \
  );
#else
#define SVC_Call(f)                                                            \
  __asm volatile                                                                 \
  (                                                                            \
    "ldr r12,="#f"\n\t"                                                        \
    "svc 0"                                                                    \
    :               "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)         \
    :                "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)         \
    : "r12", "lr", "cc"                                                        \
  );
#endif

#define SVC_0_1(f,t,rv)                                                        \
__attribute__((always_inline))                                                 \
static inline  t __##f (void) {                                                \
  SVC_Arg0();                                                                  \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_1_1(f,t,t1,rv)                                                     \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1) {                                               \
  SVC_Arg1(t1);                                                                \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_2_1(f,t,t1,t2,rv)                                                  \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2) {                                        \
  SVC_Arg2(t1,t2);                                                             \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_3_1(f,t,t1,t2,t3,rv)                                               \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3) {                                 \
  SVC_Arg3(t1,t2,t3);                                                          \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_4_1(f,t,t1,t2,t3,t4,rv)                                            \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3, t4 a4) {                          \
  SVC_Arg4(t1,t2,t3,t4);                                                       \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_1_2 SVC_1_1 
#define SVC_1_3 SVC_1_1 
#define SVC_2_3 SVC_2_1 

#elif defined (__ICCARM__)      /* IAR Compiler */

#define __NO_RETURN __noreturn

#define RET_osEvent        "=r"(ret.status), "=r"(ret.value), "=r"(ret.def)
#define RET_osCallback     "=r"(ret.fp), "=r"(ret.arg)

#define osEvent_type       osEvent
#define osEvent_ret_status ret
#define osEvent_ret_value  ret
#define osEvent_ret_msg    ret
#define osEvent_ret_mail   ret

#define osCallback_type    uint64_t
#define osCallback_ret     ((uint64_t)ret.fp | ((uint64_t)ret.arg)<<32)

#define SVC_Setup(f)                                                           \
  __asm(                                                                         \
    "mov r12,%0\n"                                                             \
    :: "r"(&f): "r12"                                                          \
  );

#define SVC_Ret3()                                                             \
  __asm(                                                                         \
    "ldr r0,[sp,#0]\n"                                                         \
    "ldr r1,[sp,#4]\n"                                                         \
    "ldr r2,[sp,#8]\n"                                                         \
  );

#define SVC_0_1(f,t,...)                                                       \
t f (void);                                                                    \
_Pragma("swi_number=0") __swi t _##f (void);                                   \
static inline t __##f (void) {                                                 \
  SVC_Setup(f);                                                                \
  return _##f();                                                               \
}

#define SVC_1_1(f,t,t1,...)                                                    \
t f (t1 a1);                                                                   \
_Pragma("swi_number=0") __swi t _##f (t1 a1);                                  \
static inline t __##f (t1 a1) {                                                \
  SVC_Setup(f);                                                                \
  return _##f(a1);                                                             \
}

#define SVC_2_1(f,t,t1,t2,...)                                                 \
t f (t1 a1, t2 a2);                                                            \
_Pragma("swi_number=0") __swi t _##f (t1 a1, t2 a2);                           \
static inline t __##f (t1 a1, t2 a2) {                                         \
  SVC_Setup(f);                                                                \
  return _##f(a1,a2);                                                          \
}

#define SVC_3_1(f,t,t1,t2,t3,...)                                              \
t f (t1 a1, t2 a2, t3 a3);                                                     \
_Pragma("swi_number=0") __swi t _##f (t1 a1, t2 a2, t3 a3);                    \
static inline t __##f (t1 a1, t2 a2, t3 a3) {                                  \
  SVC_Setup(f);                                                                \
  return _##f(a1,a2,a3);                                                       \
}

#define SVC_4_1(f,t,t1,t2,t3,t4,...)                                           \
t f (t1 a1, t2 a2, t3 a3, t4 a4);                                              \
_Pragma("swi_number=0") __swi t _##f (t1 a1, t2 a2, t3 a3, t4 a4);             \
static inline t __##f (t1 a1, t2 a2, t3 a3, t4 a4) {                           \
  SVC_Setup(f);                                                                \
  return _##f(a1,a2,a3,a4);                                                    \
}

#define SVC_1_2(f,t,t1,rr)                                                     \
uint64_t f (t1 a1);                                                            \
_Pragma("swi_number=0") __swi uint64_t _##f (t1 a1);                           \
static inline t __##f (t1 a1) {                                                \
  t ret;                                                                       \
  SVC_Setup(f);                                                                \
  _##f(a1);                                                                    \
  __asm("" : rr : :);                                                            \
  return ret;                                                                  \
}

#define SVC_1_3(f,t,t1,rr)                                                     \
t f (t1 a1);                                                                   \
void f##_ (t1 a1) {                                                            \
  f(a1);                                                                       \
  SVC_Ret3();                                                                  \
}                                                                              \
_Pragma("swi_number=0") __swi void _##f (t1 a1);                               \
static inline t __##f (t1 a1) {                                                \
  t ret;                                                                       \
  SVC_Setup(f##_);                                                             \
  _##f(a1);                                                                    \
  __asm("" : rr : :);                                                            \
  return ret;                                                                  \
}

#define SVC_2_3(f,t,t1,t2,rr)                                                  \
t f (t1 a1, t2 a2);                                                            \
void f##_ (t1 a1, t2 a2) {                                                     \
  f(a1,a2);                                                                    \
  SVC_Ret3();                                                                  \
}                                                                              \
_Pragma("swi_number=0") __swi void _##f (t1 a1, t2 a2);                        \
static inline t __##f (t1 a1, t2 a2) {                                         \
  t ret;                                                                       \
  SVC_Setup(f##_);                                                             \
  _##f(a1,a2);                                                                 \
  __asm("" : rr : :);                                                            \
  return ret;                                                                  \
}

#endif

//IIT-EXT: end of the definition of svc macros on compilers: armcc, gcc, iar


//IIT-EXT: removed the rest of the file rt_CMSIS.c as it contains a wrapper to functions which we dont need

 
#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 

#endif  // include-guard

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


