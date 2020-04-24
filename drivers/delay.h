#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef DEBUG_HAL
/* MOCK */
void mdelay(int ms);
#else
#include "stm32f7xx_hal.h"
#define mdelay(ms) HAL_Delay(ms)
#endif

#endif
