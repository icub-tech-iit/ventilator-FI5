

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
#ifndef _RT_IIT_ADVTIMER_H_
#define _RT_IIT_ADVTIMER_H_


/* @file       rt_iit_AdvTimer.h
	@brief      This header file implements public interface to the IIT advaned tiemrs.
	@author     marco.accame@iit.it
	@date       08/20/2012
    @warning    the inspiration of the code in here comes from the file rt_Timer.c from the free distribution of the
                "CMSIS-RTOS RTX Implementation" version 4.20. of this file we have retained some ideas and some data 
                types but the implementation is different. even if this should be consider new code and not a modification
                of the original code owned by KEIL-ARM, we retain in here the orginal copyrigth notice.     
**/

/*----------------------------------------------------------------------------
 *      RL-ARM - RTX
 *----------------------------------------------------------------------------
 *      Name:    RT_TIMER.C
 *      Purpose: User timer functions
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
 
/* @defgroup advtimer Advanced timers
    
    @{		
 **/


// - external dependencies --------------------------------------------------------------------------------------------



// - public #define  --------------------------------------------------------------------------------------------------
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 

typedef void (*void_fp_voidp_voidp)               (void*, void*);

typedef struct OSIIT_XADVTMR {
   U8                   cb_type;
   struct OSIIT_ADVTMR  *next;
   TIME_t               tcnt;  
} *PIIT_XADVTMR;

typedef struct OSIIT_ADVTMR {
   U8                   cb_type;
   U8                   isrbusy;
   U8                   isactive;
   struct OSIIT_ADVTMR  *next;          /* Link pointer to Next timer              */
   TIME_t               tcnt;                   /* Timer delay count               */   // acemor: was U16
   TIME_t               period_iit;    // acemor added
   WIDETIME_t           startat_iit;   // acemor added
   WIDETIME_t           abstime_iit;   // acemor added 
   void_fp_voidp_voidp  cbk;           // acemor added 
   void                 *par;                   /* User defined call info          */   // acemor: was U16  and was info
} *PIIT_ADVTMR;

typedef struct 
{
    U8      capacity;
    U8      sizeofitem;
    U8      size;
    U8      dummy;
    U32     data[1];
} oosiit_arrayhead_t;


// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty section   

// - declaration of extern public functions ---------------------------------------------------------------------------


extern OS_ID rt_iit_advtmr_new(void);
extern OS_RESULT rt_iit_advtmr_start(OS_ID timer, void* timing, void* action);
extern OS_RESULT rt_iit_advtmr_stop(OS_ID timer);
extern OS_RESULT rt_iit_advtmr_isactive(OS_ID timer);
extern OS_ID rt_iit_advtmr_delete(OS_ID timer);


extern OS_RESULT isr_iit_advtmr_start(OS_ID timer, void* timing, void* action);
extern OS_RESULT isr_iit_advtmr_stop(OS_ID timer);
extern OS_RESULT isr_iit_advtmr_delete(OS_ID timer);


extern void rt_iit_advtmr_init(void);

extern U32 rt_iit_advtmr_next(void);
extern void rt_iit_advtmr_resume_from_sleep(U32 sleep_time);

extern void  rt_iit_advtmr_tick(void);

extern void rt_iit_advtmr_synchronise(U64 oldtime); 


extern void rt_advtmr_psh (OS_ID timer, U32 mode);

                                    


/* @}            
    end of group advtimer  
 **/

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

