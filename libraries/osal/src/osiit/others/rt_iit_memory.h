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
#ifndef _RT_IIT_MEMORY_H_
#define _RT_IIT_MEMORY_H_


/* @file       rt_iit_memory.h
	@brief      This header file implements public interface to the IIT memory allocation.
	@author     marco.accame@iit.it
	@date       04/27/2010
**/

/* @defgroup memory Memory
    
    @{		
 **/


// - external dependencies --------------------------------------------------------------------------------------------



// - public #define  --------------------------------------------------------------------------------------------------
// empty-section
  

// - declaration of public user-defined types ------------------------------------------------------------------------- 

// damn to the lack of include guards in keil files .... i must redefine in here the U64 etc that i need.
typedef char               S8;
typedef unsigned char      U8;
typedef short              S16;
typedef unsigned short     U16;
typedef int                S32;
typedef unsigned int       U32;
typedef long long          S64;
typedef unsigned long long U64;
typedef unsigned char      BIT;
typedef unsigned int       BOOL;
typedef void               (*FUNCP)(void);
typedef U32     OS_TID;
typedef void    *OS_ID;
typedef U32     OS_RESULT;

// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section    

// - declaration of extern public functions ---------------------------------------------------------------------------

extern void rt_iit_memory_init(void);

extern OS_ID rt_iit_memory_getmut(void);
extern void rt_iit_memory_relmut(OS_ID mut);

extern OS_ID rt_iit_memory_getsem(void);
extern void rt_iit_memory_relsem(OS_ID sem);

extern OS_ID rt_iit_memory_getmbx(U16 nitems);
extern void rt_iit_memory_relmbx(OS_ID mbx);

extern U64* rt_iit_memory_getstack(U16 nbytes);
extern void rt_iit_memory_relstack(void* stack);

                                    


/* @}            
    end of group memory  
 **/

#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



