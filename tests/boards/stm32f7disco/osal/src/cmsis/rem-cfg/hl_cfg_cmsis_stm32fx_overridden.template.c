/*
 * Copyright (C) 2013 iCub Facility - Istituto Italiano di Tecnologia
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

// this is a template file for advanced configuration of cmsis.
// it is possible to redefine two functions which are weakly defined in system_xxxx.c:
// void SystemInit(void):            is called in the reset handler before jumping to main and is responsible to initialise 
//                                   the system (e.g., set internal clocks, external memory etc).
// void SystemCoreClockUpdate(void): is responsible to update the variable SystemCoreClock according to configured registers. 


#include "cmsis_stm32fx.h"    // contains declarations in cmsis 

// // redefined the SystemCoreClock to have value ...
// uint32_t SystemCoreClock = HSI_VALUE;
// //
// void SystemInit(void)
// {   
//     // does nothing, so it relies upon default configuration of registers
//     // then we just update the SystemCoreClock, so that we have correct timing at least in systick
//     // ... we must call SystemCoreClockUpdate() again inside main() otherwise SystemCoreClock is not updated.
//     SystemCoreClockUpdate();    
// }

// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------






