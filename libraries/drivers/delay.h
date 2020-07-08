#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG_HAL
/* MOCK */
void mdelay(int ms);
#else
#if defined(USE_STM32HAL)
#include "stm32hal.h"
#else
#include "stm32f7xx_hal.h"
#endif
#define mdelay(ms) HAL_Delay(ms)
#endif

#ifdef __cplusplus
}
#endif

#endif
