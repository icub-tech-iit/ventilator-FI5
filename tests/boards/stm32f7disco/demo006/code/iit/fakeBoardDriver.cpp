 
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
*/

#include "board_driver.h"

#include "app_tCTRL.h"

#include "vnt_os_Timer.h"
#include "vnt_core.h"
#include "vnt_bsp.h"



static vnt::os::Timer *s_tmr {nullptr};
constexpr vnt::core::relTime s_timeInit {10*vnt::core::time1millisec};
constexpr vnt::core::relTime s_timeRead {6*vnt::core::time1millisec};
constexpr vnt::core::relTime s_timeAct  {200*vnt::core::time1microsec};

void s_wrapper(void *p)
{
    board_read_callback_t cbk = reinterpret_cast<board_read_callback_t>(p);
    if(nullptr != cbk)
    {
        cbk(RC_OK); // and maybe sometimes RC_SENSOR_ERROR or RC_BUS_ERROR
    }
}


int board_init(void)
{
    // init the timer we use to simulate the async reading

    s_tmr = new vnt::os::Timer;

    // just apply a delay of ...
    vnt::bsp::delay(s_timeInit);

    return RC_OK;
}


int board_read_sensors(board_sensor_data_t* in_data)
{
    return RC_ERROR;
}


int board_read_sensors_async(board_sensor_data_t* in_data, board_read_callback_t read_completed)
{   
    if((nullptr == in_data) || (nullptr == read_completed))
    {
        return RC_ERROR;
    }
 
    static int v = 2;
    in_data->flow1 = v++;


    // starting a timer. at its expire it calls s_wrapper which calls read_completed
    vnt::os::Action cbk2thread {vnt::os::CallbackToThread(s_wrapper, reinterpret_cast<void*>(read_completed), nullptr)};
    s_tmr->start({s_timeRead, cbk2thread, vnt::os::Timer::Mode::oneshot, 0});
    return RC_OK;
}

int board_apply_actuation(board_actuation_data_t* out_data)
{
    // just apply a delay of ...
    vnt::bsp::delay(s_timeAct);
    return RC_OK;
}



// --------------------------------------------------------------------------------------------------------------------
// - end-of-file (leave a blank line after)
// --------------------------------------------------------------------------------------------------------------------


