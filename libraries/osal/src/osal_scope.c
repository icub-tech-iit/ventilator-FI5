
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code is a custumization for the ventilator project 
 *          of code originally published in https://github.com/robotology/icub-firmware
 *
*/




// --------------------------------------------------------------------------------------------------------------------
// - external dependencies
// --------------------------------------------------------------------------------------------------------------------

#include "stdlib.h"

 
// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern public interface
// --------------------------------------------------------------------------------------------------------------------

#include "osal_scope.h"

extern void evIDLE(void);



// --------------------------------------------------------------------------------------------------------------------
// - declaration of extern hidden interface 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - #define with internal scope
// --------------------------------------------------------------------------------------------------------------------

#define EVENTVIEWER_USE_CORTEX_M3M4

#define ENTITY_ID_IDLE  ev_ID_idle


// -- specific for cortex m3/m4 ----

#if defined(EVENTVIEWER_USE_CORTEX_M3M4)

#define DEMCR_TRCENA    0x01000000
#define ITM_ITMENA      0x00000001

/* Core Debug registers */
#define DEMCR           (*((volatile uint32_t *)0xE000EDFC))

/* ITM registers */
#define ITM_CONTROL     (*((volatile uint32_t *)0xE0000E80))
#define ITM_ENABLE      (*((volatile uint32_t *)0xE0000E00))
#define ITM_PORT30_U32  (*((volatile uint32_t *)0xE0000078))
#define ITM_PORT31_U32  (*((volatile uint32_t *)0xE000007C))
#define ITM_PORT31_U16  (*((volatile uint16_t *)0xE000007C))
#define ITM_PORT31_U8   (*((volatile uint8_t  *)0xE000007C))

#endif

// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of extern variables, but better using _get(), _set() 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - typedef with internal scope
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------
// - declaration of static functions
// --------------------------------------------------------------------------------------------------------------------



// --------------------------------------------------------------------------------------------------------------------
// - definition (and initialisation) of static variables
// --------------------------------------------------------------------------------------------------------------------

static osal_scope_ID_t s_osal_scope_currentid = ev_ID_idle;
static uint8_t s_osal_scope_initted = 0;


// --------------------------------------------------------------------------------------------------------------------
// - definition of extern public functions
// --------------------------------------------------------------------------------------------------------------------


extern osal_result_t osal_scope_init(void)
{
    if(1 == s_osal_scope_initted)
    {
        return(osal_res_OK);
    }

    if((DEMCR & DEMCR_TRCENA)       && 
       (ITM_CONTROL & ITM_ITMENA)   &&
       (ITM_ENABLE & (1UL << 31))
      ) 
    {
        s_osal_scope_initted = 1;
        osal_scope_load(ev_ID_idle, evIDLE);
        osal_scope_switch_to(ev_ID_idle);
        s_osal_scope_currentid = ev_ID_idle;
        return(osal_res_OK);
    }
    return(osal_res_NOK_generic);
}


extern void osal_scope_load(osal_scope_ID_t id, evEntityName_t name)
{
    if(0 == s_osal_scope_initted)
    {
        osal_scope_init();
    }

    while (ITM_PORT31_U32 == 0);
    ITM_PORT31_U32 = (uint32_t)name;
    while (ITM_PORT31_U32 == 0);
    ITM_PORT31_U16 = (1 << 8) | (uint8_t)id;
}

extern void osal_scope_unload(osal_scope_ID_t id, evEntityName_t name)
{
    if(0 == s_osal_scope_initted)
    {
        osal_scope_init();
    }
    
    while (ITM_PORT31_U32 == 0);
    ITM_PORT31_U32 = (uint32_t)name;
    while (ITM_PORT31_U32 == 0);
    ITM_PORT31_U16 = (0 << 8) | (uint8_t)id;
}


extern osal_scope_ID_t osal_scope_switch_to(osal_scope_ID_t id)
{
    osal_scope_ID_t prev = s_osal_scope_currentid;
    s_osal_scope_currentid = id;
    
    if(0 == s_osal_scope_initted)
    {
        return(prev);
    }
    
    while (ITM_PORT31_U32 == 0);
    ITM_PORT31_U8 = (uint8_t)s_osal_scope_currentid;
        
    return(prev);
}








// --------------------------------------------------------------------------------------------------------------------
// - definition of extern hidden functions 
// --------------------------------------------------------------------------------------------------------------------


// ---- isr of the module: begin ----
// empty-section
// ---- isr of the module: end ------

extern void evIDLE(void)
{

}

// --------------------------------------------------------------------------------------------------------------------
// - definition of static functions 
// --------------------------------------------------------------------------------------------------------------------
// empty-section


// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------



