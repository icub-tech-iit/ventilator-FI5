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
*   This template is responsible to initialize the system clock of the hardware,
*   to provide a timer tick for the Embedded Wizard UI application and to get
*   access to the realtime clock (RTC). Additionally, this file contains some
*   performance measurements to analyse the CPU usage of the UI application.
*
*******************************************************************************/

#include "ewconfig.h"
#include "stm32hal.h"
#include "stm32746g_discovery.h"

#include "ew_bsp_clock.h"

#include <string.h>

/* Defines to select adaquate clock source */
#define RTC_CLOCK_SOURCE_LSI
#undef RTC_CLOCK_SOURCE_LSE

#ifdef RTC_CLOCK_SOURCE_LSI
  #define RTC_ASYNCH_PREDIV   0x7F
  #define RTC_SYNCH_PREDIV    0xF9
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
  #define RTC_ASYNCH_PREDIV   0x7F
  #define RTC_SYNCH_PREDIV    0x00FF
#endif


#define RtcTicksPerSecond    ( 1UL )
#define RtcTicksPerMinute    ( RtcTicksPerSecond * 60UL )
#define RtcTicksPerHour      ( RtcTicksPerMinute * 60UL )
#define RtcTicksPerDay       ( RtcTicksPerHour   * 24UL )

const unsigned long DaysToMonth[] =
{
  0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

const unsigned long DaysToMonthInLeapYear[] =
{
  0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
};


/* timer tick for Embedded Wizard UI applications */
static volatile unsigned long EmWiSystemTicks = 0;

/* RTC handler declaration */
static RTC_HandleTypeDef      RtcHandle;

#if EW_CPU_LOAD_MEASURING == 1

  /* variables for cycle counter / CPU load measurement */
  static unsigned long        WorkingTime  = 0;
  static unsigned long        SleepingTime = 0;
  static unsigned long        PrevCycCnt   = 0;

  /* helper function to initialize measuring unit (cycle counter) */
  static void CycleCounterInit( void )
  {
    /* Enable TRC */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Unlock DWT registers */
    if ((*(uint32_t*)0xE0001FB4) & 1)
      *(uint32_t*)0xE0001FB0 = 0xC5ACCE55;

    /* clear the cycle counter */
    DWT->CYCCNT = 0;

    /* start the cycle counter */
    DWT->CTRL = 0x40000001;

    /* initialize counters */
    WorkingTime   = 0;
    SleepingTime  = 0;
    PrevCycCnt    = 0;
  }

  /* helper function to get cycle counter value since last call */
  static unsigned long GetCycleCounterDelta( void )
  {
    unsigned long cycCnt;
    unsigned long result;

    /* get delta between current counter value and last counter value */
    cycCnt = DWT->CYCCNT;
    result = cycCnt - PrevCycCnt;
    PrevCycCnt = cycCnt;

    return result;
  }

#endif


/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Configure LSE as RTC clock source ##################################*/
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    return;
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    return;
  }

  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();
}

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();
}


/*******************************************************************************
* FUNCTION:
*   EwBspClockInit
*
* DESCRIPTION:
*   Initialises the system clock and the real time clock.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspClockInit( void )
{
  RtcHandle.Instance = RTC;

  EmWiSystemTicks = 0;
    // IIT-ICUB-TECH: removed  HAL_SYSTICK_Config( SystemCoreClock / 1000 );

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
     - Hour Format    = Format 24
     - Asynch Prediv  = Value according to source clock
     - Synch Prediv   = Value according to source clock
     - OutPut         = Output Disable
     - OutPutPolarity = High Polarity
     - OutPutType     = Open Drain */
  RtcHandle.Init.HourFormat     = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv   = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv    = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut         = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;

  HAL_RTC_Init( &RtcHandle );
}

// IIT-ICUB-TECH: removed
/*******************************************************************************
* FUNCTION:
*   EwBspClockTickIncrement
*
* DESCRIPTION:
*   The function EwBspClockTickIncrement increments the millisecond counter,
*   which is used by the Runtime Environment (RTE) to trigger timer events.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
//void EwBspClockTickIncrement( void )
//{
//  EmWiSystemTicks++;
//}

//void EwBspClockTickIncrement10( void )
//{
//   EmWiSystemTicks+=10; 
//}


/*******************************************************************************
* FUNCTION:
*   EwBspClockGetTicks
*
* DESCRIPTION:
*   The function EwBspClockGetTicks returns the current ticks counter value.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current ticks counter value.
*
*******************************************************************************/
//unsigned long EwBspClockGetTicks( void )
//{
//  return EmWiSystemTicks;
//}


/*******************************************************************************
* FUNCTION:
*   EwBspClockGetTime
*
* DESCRIPTION:
*   Returns the current time in seconds since 01.01.1970.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current time in seconds since 01.01.1970.
*
*******************************************************************************/
unsigned long EwBspClockGetTime( void )
{
  RTC_TimeTypeDef currentTime;
  RTC_DateTypeDef currentDate;

  HAL_RTC_GetTime( &RtcHandle, &currentTime, RTC_FORMAT_BIN );
  HAL_RTC_GetDate( &RtcHandle, &currentDate, RTC_FORMAT_BIN );

  unsigned long   days;
  unsigned long   year   = currentDate.Year;
  unsigned long   month  = currentDate.Month;
  unsigned long   day    = currentDate.Date;
  unsigned long   hour   = currentTime.Hours;
  unsigned long   minute = currentTime.Minutes;
  unsigned long   second = currentTime.Seconds;
  unsigned long   timeInSeconds;

  /* year is the number of years since 2000 */
  /* calculate number of days since 01.01.1970 until begin of current
     year including the additional days of leap years */

  days = (year + 30) * 365 + (year + 27) / 4;

  if (year & 0x3)
  {
    days += DaysToMonth[month - 1];
  }
  else
  {
    days += DaysToMonthInLeapYear[month - 1];
  }

  days += day;

  timeInSeconds =
    days   * RtcTicksPerDay +
    hour   * RtcTicksPerHour +
    minute * RtcTicksPerMinute +
    second;

  return timeInSeconds;
}


/*******************************************************************************
* FUNCTION:
*   EwBspClockSetTime
*
* DESCRIPTION:
*   Sets the given time in seconds since 01.01.1970 at real time clock (RTC).
*
* ARGUMENTS:
*   aTime - the time in seconds since 01.01.1970 to set in real time clock.
*
* RETURN VALUE:
*   None.
*
*******************************************************************************/
void EwBspClockSetTime( unsigned long aTime )
{
  #define LeapYearsFromBeginToNext400Year   7
  #define DaysPer4Years       ( 4 * 365 + 1 )

  RTC_TimeTypeDef rtcTime;
  RTC_DateTypeDef rtcDate;

  unsigned int year;
  unsigned int year4;
  unsigned int month;
  unsigned int day;
  unsigned int hour;
  unsigned int minute;
  unsigned int second;

  hour    = ( unsigned int ) (( aTime / RtcTicksPerHour    ) % 24 );
  minute  = ( unsigned int ) (( aTime / RtcTicksPerMinute  ) % 60 );
  second  = ( unsigned int ) (( aTime / RtcTicksPerSecond  ) % 60 );

  /* calculation of the date */
  day       = ( unsigned int ) ( aTime / RtcTicksPerDay + 365);

  if ( day >= 48213 )
    day++;

  year4     = day / DaysPer4Years;
  day      -= year4 * DaysPer4Years;
  year      = day / 365;

  if ( year == 4 )
  {
    year = 3;

    if ( !( year4 % 100 == LeapYearsFromBeginToNext400Year )
       && ( year4 %  25 == LeapYearsFromBeginToNext400Year ))
      year = 4;
  }

  day  -= year * 365;
  year  = year4 * 4 + year + 1969;

  /* all months have less than 32 days, days >> 5 is a good estimate
     for the month. */

  month = day >> 5;

  /* correct month if necessary */
  if ( ( year % 4 != 0 ) ||
      (( year % 100 == 0 ) && ( year % 400 != 0 )) )
  {
    /* no leap year */
    if ( day >= DaysToMonth[ month + 1 ] )
      month++;

    day -= DaysToMonth[ month ];
  }
  else
  {
    /* leap year */
    if ( day >= DaysToMonthInLeapYear[ month + 1 ] )
      month++;

    day -= DaysToMonthInLeapYear[ month ];
  }

  memset( &rtcTime, 0, sizeof( rtcTime ));
  memset( &rtcDate, 0, sizeof( rtcDate ));
  rtcTime.Hours   = hour;
  rtcTime.Minutes = minute;
  rtcTime.Seconds = second;
  rtcDate.Date    = day + 1;
  rtcDate.Month   = month + 1;
  rtcDate.Year    = year - 2000;

  HAL_RTC_SetTime( &RtcHandle, &rtcTime, RTC_FORMAT_BIN );
  HAL_RTC_SetDate( &RtcHandle, &rtcDate, RTC_FORMAT_BIN );
}


/*******************************************************************************
* FUNCTION:
*   EwBspClockGetCpuLoad
*
* DESCRIPTION:
*   Returns the current CPU load as percent value.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   The current CPU load.
*
*******************************************************************************/
 int EwBspClockGetCpuLoad( void )
{
  int cpuLoad = 0;

  #if EW_CPU_LOAD_MEASURING == 1

    /* get actual counter value */
    WorkingTime += GetCycleCounterDelta();

    /* calculate CPU load as percent value */
    cpuLoad = ( int ) ( WorkingTime / (( SleepingTime + WorkingTime ) / 100 ));

    /* clear accumulated counter values */
    WorkingTime  = 0;
    SleepingTime = 0;

  #endif

  return cpuLoad;
}


/*******************************************************************************
* FUNCTION:
*   EwBspClockCpuLoadSetActive
*
* DESCRIPTION:
*   Starts the CPU load counting. Call this function whenever CPU processing
*   is needed.
*
* ARGUMENTS:
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspClockCpuLoadSetActive( void )
{
  #if EW_CPU_LOAD_MEASURING == 1

    static char initialized = 0;

    if ( !initialized )
    {
      CycleCounterInit();
      initialized = 1;
    }

    SleepingTime += GetCycleCounterDelta();

  #endif
}


/*******************************************************************************
* FUNCTION:
*   EwBspClockCpuLoadSetIdle
*
* DESCRIPTION:
*   Stops the CPU load counting. Call this function whenever CPU processing is
*   currently not needed since the program execution is waiting for some system
*   event.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspClockCpuLoadSetIdle( void )
{
  #if EW_CPU_LOAD_MEASURING == 1

    WorkingTime += GetCycleCounterDelta();

  #endif
}


/* msy */
