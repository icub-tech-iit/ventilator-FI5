/*
 * Copyright (C) 2011 Department of Robotics Brain and Cognitive Sciences - Istituto Italiano di Tecnologia
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
#ifndef _OSAL_CFG_H_
#define _OSAL_CFG_H_

// --------------------------------------------------------------------------------------------------------------------
//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------


// <h> Configuration of OSAL
// <i> It holds configuration for objects used in OSAL


// <h> Porting specifics 
// <i> sssssssss
//   <o> RTOS type         <0=>   IITmodified-RTXARM    <1=>    OOSIIT
//   <i> Only IITmodified-RTXARM and OOSIIT are so far supported.
#ifndef OSAL_RTOSTYPE
 #define OSAL_RTOSTYPE      1
#endif


//   <o> CPU family         <0=>   Cortex M3 <1=>   Cortex M4
//   <i> Only Cortex M3 and M4 are now supported.
#ifndef OSAL_CPUFAM
 #define OSAL_CPUFAM      0
#endif

//   <o> CPU Frequency [Hz] <1-1000000000>
//   <i> Specify CPU frequency.
//   <i> Default: 72000000  (72MHz on STM32F107, 168Mhz on STM32F407)
#ifndef OSAL_CPUFREQ
 #define OSAL_CPUFREQ       72000000
#endif



// </h>Porting specifics


// <h> Scheduler 
// <i> sssssssss


//   <o> Priority of the tick handler <1-15>
//   <i> Set the priority of teh tick handler. Lower number is higher priority.
//   <i> Default: 15
#ifndef OSAL_PRIO
 #define OSAL_PRIO       15
#endif

//   <o> Timer tick value [us] <1-1000000>
//   <i> Set the timer tick, the base of time for OSAL.
//   <i> Default: 1000  (1ms)
#ifndef OSAL_TICK
 #define OSAL_TICK        1000
#endif


// <e>Round-Robin Task switching
// <i> Enable Round-Robin
// <i> Default: Disabled
#ifndef OSAL_RROBIN
 #define OSAL_RROBIN       1
#endif

// <o> Round-Robin tick time [ticks] <1-1000>
// <i> Define how long a task will execute before a task switch.
// <i> Default: 5
#ifndef OSAL_RROBINTICK
 #define OSAL_RROBINTICK   10
#endif

// </e>



// </h>Scheduler


// <h> System resources 
// <i> sssssssss


//   <o> Stack size for launcher task [bytes] <200-4096:8>
//   <i> Set the stack size for launcher task.
//   <i> Default: 128
#ifndef OSAL_LAUNSTKSIZE
 #define OSAL_LAUNSTKSIZE     1024
#endif

//   <o> Stack size for idle task [bytes] <200-4096:8>
//   <i> Set the stack size for idle task.
//   <i> Default: 128
#ifndef OSAL_IDLESTKSIZE
 #define OSAL_IDLESTKSIZE     256
#endif


// </h>System resources


// <h> Memory model 
// <i> sssssssss


//   <o> Memory model         <0=>   static allocation      <1=>   dynamic allocation    
//   <i> with static allocation all the memory needed by OSAL objects is allocate at startup and retrieved when required.
//   <i> with dynamic allocation the memry is retrieved just when it is needed
#ifndef OSAL_MEMMODEL
 #define OSAL_MEMMODEL      1
#endif



// </h>Memory model



// <h> Max number of OSAL resources for static allocation mode
// <i> This section is relevant only if the memory model is static.

//   <o> Number of user tasks <0-250>
//   <i> Maximum number of tasks that will run at the same time.
//   <i> Default: 6
#ifndef OSAL_TASKNUM
 #define OSAL_TASKNUM     8
#endif

//   <o> Total stack size for all the user tasks [bytes] <256-16384:8>
//   <i> Define max. size in bytes of the global stack.
//   <i> Default: 256  (only one task for instance)
#ifndef OSAL_GLOBSTKSIZE
 #define OSAL_GLOBSTKSIZE    8192
#endif


//   <o> Number of timers <0-250>
//   <i> Define max number of timers.
//   <i> Default: 0  (User timers disabled)
#ifndef OSAL_TIMERNUM
 #define OSAL_TIMERNUM    8
#endif


//   <o> Number of mutexes <0-250>
//   <i> Define max. number of mutexes that will run at the same time.
//   <i> Default: 0  (Mutexes not enabled)
#ifndef OSAL_MUTEXNUM
 #define OSAL_MUTEXNUM    8
#endif


//   <o> Number of semaphores <0-250>
//   <i> Define max. number of semaphores that will run at the same time.
//   <i> Default: 0  (Semaphores not enabled)
#ifndef OSAL_SEMAPHORENUM
 #define OSAL_SEMAPHORENUM    8
#endif


//   <o> Number of message queues <0-250>
//   <i> Define max. number of message queues that that will run at the same time.
//   <i> Default: 0  (Message queues not enabled)
#ifndef OSAL_MQUEUENUM
 #define OSAL_MQUEUENUM    8
#endif

//   <o> Total number of messages in message queues <0-1000>
//   <i> Define max. number of messages that can be contained in all the message queues.
//   <i> Default: 0  (Message queues not enabled)
#ifndef OSAL_MQUEUEELEMNUM
 #define OSAL_MQUEUEELEMNUM    64
#endif



// </h>OSAL objects



// </h>


// --------------------------------------------------------------------------------------------------------------------
//------------- <<< end of configuration section >>> ------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------


// - some controls ----------------------------------------------------------------------------------------------------

#if(OSAL_RTOSTYPE > 1)
    #error only arm-rtx modified by iit and oosiit are supported so far
#endif

#if(OSAL_RTOSTYPE == 0)
    #if(OSAL_MEMMODEL == 1)
        #error only oosiit supports dynamic memory model
    #endif

    #if(OSAL_MQUEUEELEMNUM < OSAL_MQUEUENUM)
        #error more messagequeues than messages ...
    #endif

#endif



// - end of controls --------------------------------------------------------------------------------------------------

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------


