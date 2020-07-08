/*******************************************************************************
*
* E M B E D D E D   W I Z A R D   P R O J E C T
*
*                                                Copyright (c) TARA Systems GmbH
*                                    written by Paul Banach and Manfred Schweyer
*
********************************************************************************
*
* This software is delivered "as is" and shows the usage of other software
* components. It is provided as an example software which is intended to be
* modified and extended according to particular requirements.
*
* TARA Systems hereby disclaims all warranties and conditions with regard to the
* software, including all implied warranties and conditions of merchantability
* and non-infringement of any third party IPR or other rights which may result
* from the use or the inability to use the software.
*
********************************************************************************
*
* DESCRIPTION:
*   This file is part of the interface (glue layer) between an Embedded Wizard
*   generated UI application and the board support package (BSP) of a dedicated
*   target.
*   Please note: The implementation of this module is partially based on
*   examples that are provided within the STM32 cube firmware. In case you want
*   to adapt this module to your custom specific hardware, please adapt the
*   hardware initialization code according your needs or integrate the generated
*   initialization code created by using the tool CubeMX.
*   This template provides a system event mechanism, that can be used in
*   combination with an operating system to sleep and to continue (resume) the
*   operation of the UI main loop.
*
*******************************************************************************/

#include "ewconfig.h"
#include "stm32hal.h"
#include "stm32746g_discovery.h"

#if EW_USE_FREE_RTOS == 1

  #include "cmsis_os.h"

  #define OS_SIGNAL_WAKEUP_UI  0x00000001

  static osThreadId ThreadId = 0;

#endif

#include "ew_bsp_clock.h"
#include "ew_bsp_event.h"

/*******************************************************************************
* FUNCTION:
*   EwBspEventWait
*
* DESCRIPTION:
*   The function EwBspEventWait should be called from the Embedded
*   Wizard main loop in case there are no pending events, signals or timers that
*   have to be processed by the UI application.
*   The function EwBspEventWait is used to sleep the given time span or
*   to suspend the UI task. The function returns as soon as a new system event
*   occurs or when the given timeout value is expired.
*   Typically, a system event is a touch event or a key event or any event
*   from your device driver.
*
* ARGUMENTS:
*   aTimeout - timeout value in milliseconds.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspEventWait( int aTimeout )
{
  if ( aTimeout > 0 )
  {
    CPU_LOAD_SET_IDLE();

    #if EW_USE_FREE_RTOS == 1

      /* save the thread ID for signaling */
      if ( ThreadId == 0 )
        ThreadId = osThreadGetId();

      /* Important note: Within this template, the timeout value is limited to
         10 ms in order to ensure that the UI application is continuously working,
         e.g. to get new touch values or to receive data from a device driver.
         This makes the usage of this template very simple.
         If you want to implement a completely event driven system, you can use
         the given timeout without limiting it. In this case you have to ensure
         that the touch driver or your device drivers are resuming the UI task
         by calling TriggerSystemEvent().
      */
      if ( aTimeout > 10 )
        aTimeout = 10;

      osSignalWait( OS_SIGNAL_WAKEUP_UI, aTimeout );

    #else

      /* in bare metal maximum wait time is 10 ms to keep main loop running */
      if ( aTimeout > 10 )
        aTimeout = 10;

      HAL_Delay( aTimeout );

    #endif

    CPU_LOAD_SET_ACTIVE();
  }
}


/*******************************************************************************
* FUNCTION:
*   EwBspEventTrigger
*
* DESCRIPTION:
*   The function EwBspEventTrigger is used in combination with an
*   operating system to continue (resume) the operation of the UI main loop.
*   Typically, a system event is a touch event or a key event or any event
*   from your device driver.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspEventTrigger( void )
{
  #if EW_USE_FREE_RTOS == 1

    osSignalSet( ThreadId, OS_SIGNAL_WAKEUP_UI );

  #endif
}


/* msy */
