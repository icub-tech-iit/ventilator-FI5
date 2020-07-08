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
*   This template is responsible to initialize the touch driver of the display
*   hardware and to receive the touch events for the UI application.
*
*******************************************************************************/

#include "stm32hal.h"
#include "stm32746g_discovery.h"
#include "ft5336.h"

#include <string.h>

#include "ewconfig.h"
#include "ewrte.h"

#include "ew_bsp_clock.h"
#include "ew_bsp_touch.h"

#define NO_OF_FINGERS                   FT5336_MAX_DETECTABLE_TOUCH
#define DELTA_TOUCH                     16
#define DELTA_TIME                      500

/* additional touch flag to indicate idle state */
#define EW_BSP_TOUCH_IDLE               0

/* additional touch flag to indicate hold state */
#define EW_BSP_TOUCH_HOLD               4

/* structure to store internal touch information for one finger */
typedef struct
{
  int           XPos;      /* horizontal position in pixel */
  int           YPos;      /* vertical position in pixel */
  unsigned long Ticks;     /* time of recent touch event */
  unsigned char TouchId;   /* constant touch ID provided by touch controller */
  unsigned char State;     /* current state within a touch cycle */
} XTouchData;


static int           TouchAreaWidth  = 0;
static int           TouchAreaHeight = 0;

static XTouchEvent   TouchEvent[ NO_OF_FINGERS ];
static XTouchData    TouchData[ NO_OF_FINGERS ];


typedef struct
{
  uint8_t  touchDetected;              /*!< Total number of active touches detected at last scan */
  uint16_t touchX[NO_OF_FINGERS];      /*!< Touch X[0], X[1] coordinates on 12 bits */
  uint16_t touchY[NO_OF_FINGERS];      /*!< Touch Y[0], Y[1] coordinates on 12 bits */
  uint8_t  touchId[NO_OF_FINGERS];
} TS_StateTypeDef;


static void BSP_TS_GetState(TS_StateTypeDef *TS_State)
{
  static uint32_t _x[NO_OF_FINGERS] = {0, 0};
  static uint32_t _y[NO_OF_FINGERS] = {0, 0};
  uint16_t x[NO_OF_FINGERS];
  uint16_t y[NO_OF_FINGERS];
  uint16_t brute_x;
  uint16_t brute_y;
  uint16_t x_diff;
  uint16_t y_diff;
  uint32_t index;
  uint8_t id;

  /* Check and update the number of touches active detected */
  TS_State->touchDetected = ft5336_TS_DetectTouch(TS_I2C_ADDRESS);

  if(TS_State->touchDetected)
  {
    for(index=0; index < TS_State->touchDetected; index++)
    {
      /* Get each touch coordinates */
      ft5336_TS_GetXY(TS_I2C_ADDRESS, &brute_x, &brute_y, &id );

      /* coordinates are provided swapped */
      y[index] = brute_x;
      x[index] = brute_y;

      x_diff = x[index] > _x[index]? (x[index] - _x[index]): (_x[index] - x[index]);
      y_diff = y[index] > _y[index]? (y[index] - _y[index]): (_y[index] - y[index]);

      if ((x_diff + y_diff) > 5)
      {
        _x[index] = x[index];
        _y[index] = y[index];
      }

      /* store the coordinates */
      TS_State->touchX[index] = x[index];
      TS_State->touchY[index] = y[index];

      /* store the unique touch id */
      TS_State->touchId[index] = id;
    }
  }
}


/*******************************************************************************
* FUNCTION:
*   EwBspTouchInit
*
* DESCRIPTION:
*   Initializes the touch driver.
*
* ARGUMENTS:
*   aWidth  - Width of the toucharea (framebuffer) in pixel.
*   aHeight - Height of the toucharea (framebuffer) in pixel.
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspTouchInit( int aWidth, int aHeight )
{
  TouchAreaWidth  = aWidth;
  TouchAreaHeight = aHeight;

  /* Read ID and verify if the touch screen driver is ready */
  ft5336_Init(TS_I2C_ADDRESS);
  if(ft5336_ReadID(TS_I2C_ADDRESS) == FT5336_ID_VALUE)
  {
    /* Initialize the TS driver */
    ft5336_TS_Start(TS_I2C_ADDRESS);
  }
  else
    EwPrint("EwBspTouchInit: Initialization of FT5336 failed\n" );

  /* clear all touch state variables */
  memset( TouchData, 0, sizeof( TouchData ));
}


/*******************************************************************************
* FUNCTION:
*   EwBspTouchDone
*
* DESCRIPTION:
*   Terminates the touch driver.
*
* ARGUMENTS:
*   None
*
* RETURN VALUE:
*   None
*
*******************************************************************************/
void EwBspTouchDone( void )
{
}


/*******************************************************************************
* FUNCTION:
*   EwBspTouchGetEvents
*
* DESCRIPTION:
*   The function EwBspTouchGetEvents reads the current touch positions from the
*   touch driver and returns the current touch position and touch status of the
*   different fingers. The returned number of touch events indicates the number
*   of XTouchEvent that contain position and status information.
*   The orientation of the touch positions is adjusted to match GUI coordinates.
*   If the hardware supports only single touch, the finger number is always 0.
*
* ARGUMENTS:
*   aTouchEvent - Pointer to return array of XTouchEvent.
*
* RETURN VALUE:
*   Returns the number of detected touch events, otherwise 0.
*
*******************************************************************************/
int EwBspTouchGetEvents( XTouchEvent** aTouchEvent )
{
  static TS_StateTypeDef touchPadState;
  int           x, y;
  int           t;
  int           f;
  unsigned long ticks;
  int           noOfEvents = 0;
  int           finger;
  char          identified[ NO_OF_FINGERS ];
  XTouchData*   touch;

  /* access touch driver to receive current touch status and position */
  CPU_LOAD_SET_IDLE();
  BSP_TS_GetState( &touchPadState );
  CPU_LOAD_SET_ACTIVE();

  /* all fingers have the state unidentified */
  memset( identified, 0, sizeof( identified ));

  /* get current time in ms */
  ticks = EwGetTicks();

  /* iterate through all touch events from the hardware */
  for ( t = 0; t < touchPadState.touchDetected; t++ )
  {
    /* check for valid coordinates */
    if (( touchPadState.touchX[ t ] <= 0 ) || ( touchPadState.touchX[ t ] > TouchAreaWidth ) ||
        ( touchPadState.touchY[ t ] <= 0 ) || ( touchPadState.touchY[ t ] > TouchAreaHeight ))
      continue;

    #if ( EW_SURFACE_ROTATION == 90 )

      x = touchPadState.touchY[ t ];
      y = TouchAreaWidth  - touchPadState.touchX[ t ];

    #elif ( EW_SURFACE_ROTATION == 270 )

      x = TouchAreaHeight - touchPadState.touchY[ t ];
      y = touchPadState.touchX[ t ];

    #elif ( EW_SURFACE_ROTATION == 180 )

      x = TouchAreaWidth  - touchPadState.touchX[ t ];
      y = TouchAreaHeight - touchPadState.touchY[ t ];

    #else

      x = touchPadState.touchX[ t ];
      y = touchPadState.touchY[ t ];

    #endif

    /* iterate through all fingers to find a finger that matches with the provided touch event */
    for ( finger = -1, f = 0; f < NO_OF_FINGERS; f++ )
    {
      touch = &TouchData[ f ];

      /* check if the finger is already active */
      if (( touch->State != EW_BSP_TOUCH_IDLE ) && ( touch->TouchId == touchPadState.touchId[ t ]))
      {
        finger = f;
        break;
      }

      /* check if the finger was used within the recent time span and if the touch position is in the vicinity */
      if (( touch->State == EW_BSP_TOUCH_IDLE ) && ( ticks < touch->Ticks + DELTA_TIME )
        && ( x > touch->XPos - DELTA_TOUCH ) && ( x < touch->XPos + DELTA_TOUCH )
        && ( y > touch->YPos - DELTA_TOUCH ) && ( y < touch->YPos + DELTA_TOUCH ))
        finger = f;

      /* otherwise take the first free finger */
      if (( touch->State == EW_BSP_TOUCH_IDLE ) && ( finger == -1 ))
        finger = f;
    }

    /* determine the state within a touch cycle and assign the touch parameter to the found finger */
    if ( finger >= 0 )
    {
      touch = &TouchData[ finger ];
      identified[ finger ] = 1;

      /* check for start of touch cycle */
      if ( touch->State == EW_BSP_TOUCH_IDLE )
        touch->State = EW_BSP_TOUCH_DOWN;
      else
      {
        /* check if the finger has moved */
        if (( touch->XPos != x ) || ( touch->YPos != y ))
          touch->State = EW_BSP_TOUCH_MOVE;
        else
          touch->State = EW_BSP_TOUCH_HOLD;
      }

      /* store current touch parameter */
      touch->XPos    = x;
      touch->YPos    = y;
      touch->TouchId = touchPadState.touchId[ t ];
      touch->Ticks   = ticks;
    }
  }

  /* prepare sequence of touch events suitable for Embedded Wizard GUI application */
  for ( f = 0; f < NO_OF_FINGERS; f++ )
  {
    touch = &TouchData[ f ];

    /* begin of a touch cycle */
    if ( identified[ f ] && ( touch->State == EW_BSP_TOUCH_DOWN ))
      TouchEvent[ noOfEvents ].State = EW_BSP_TOUCH_DOWN;

    /* move within a touch cycle */
    else if ( identified[ f ] && ( touch->State == EW_BSP_TOUCH_MOVE ))
      TouchEvent[ noOfEvents ].State = EW_BSP_TOUCH_MOVE;

    /* end of a touch cycle */
    else if ( !identified[ f ] && ( touch->State != EW_BSP_TOUCH_IDLE ))
    {
      TouchEvent[ noOfEvents ].State = EW_BSP_TOUCH_UP;
      touch->State = EW_BSP_TOUCH_IDLE;
    }
    else
      continue;

    TouchEvent[ noOfEvents ].XPos   = touch->XPos;
    TouchEvent[ noOfEvents ].YPos   = touch->YPos;
    TouchEvent[ noOfEvents ].Finger = f;

    // EwPrint( "Touch event for finger %d with state %d ( %4d, %4d )\n", f, TouchEvent[ noOfEvents ].State, TouchEvent[ noOfEvents ].XPos, TouchEvent[ noOfEvents ].YPos );

    noOfEvents++;
  }

  /* return the prepared touch events and the number of prepared touch events */
  if ( aTouchEvent )
    *aTouchEvent = TouchEvent;

  return noOfEvents;
}


/* msy */
