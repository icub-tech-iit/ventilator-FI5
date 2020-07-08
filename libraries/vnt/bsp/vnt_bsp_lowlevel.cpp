
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was adapted from the embot library in https://github.com/robotology/icub-firmware for use
 *          in the ventilator project
*/

// --------------------------------------------------------------------------------------------------------------------
// - public interface
// --------------------------------------------------------------------------------------------------------------------

#include "vnt_bsp_lowlevel.h"


// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stm32hal.h" // just to see STM32HAL_STM32F7, STM32HAL_STM32H7, STM32HAL_STM32L4 etc

using namespace std;


// keep it without optimisation, otherwise the compiler may wipe away apparently useless operations
#if defined(__ARMCOMPILER_VERSION)
    #if defined(__OPTIMIZE__)
    #error  THIS FILE MUST NOT BE OPTIMISED: use -O0 (otherwise the compiler may wipe away apparently useless operations)
    #endif
#else
    #error  THIS CODE MUST NOT BE OPTIMISED: use -O0 and adjust this code for your compiler
    // for gcc it may be: GCC_VERSION  __OPTIMIZE__ 
#endif
// --------------------------------------------------------------------------------------------------------------------
// - all the rest
// --------------------------------------------------------------------------------------------------------------------

namespace vnt { namespace bsp { namespace lowlevel {
    
#if defined(STM32HAL_STM32L4) || defined(STM32HAL_STM32H7) || defined(STM32HAL_STM32F7)

#if __ARMCOMPILER_VERSION > 6000000
int _execOperations(uint32_t i)
{
  int res = 0;
  __asm 
  (
    "DOWAITLOOP:                            \t\n"
    "SUBS %[input_i], %[input_i], #1        \t\n"
    "BNE DOWAITLOOP                         \t\n"
    : [result] "=&r" (res)
    : [input_i] "r" (i)
  );
  return 0;
}
#else
__asm void _execOperations(uint32_t numberof) 
{
   align
dowaitloop
   subs r0,r0,#1
   bne dowaitloop
   bx lr 
   align    
}
#endif

#else 
    #error you must define a _execOperations() function
#endif

    void asmEXECcycles(std::uint32_t numcycles)
    {
        _execOperations(numcycles);        
    }
  

}}} // namespace vnt { namespace bsp { namespace lowlevel {


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

