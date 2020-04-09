#ifndef _IIT_BOARD_DRIVER_H_
#define _IIT_BOARD_DRIVER_H_

#include <stdint.h>

enum 
{
    RC_OK = 0,
    RC_ERROR,
    RC_SENSOR_ERROR,
    RC_BUS_ERROR
};

typedef struct
{
    int16_t pressure1;
    int16_t pressure2;
    int16_t pressure3;
    int16_t flow1;
    int16_t flow2;
    uint16_t o2;
    int16_t temperature1;
    int16_t temperature2;
    int16_t temperature3;
} board_sensor_data_t;

typedef struct
{
    uint16_t valve1;
    uint16_t valve2;
} board_actuation_data_t;

int board_init(void);
int board_read_sensors(board_sensor_data_t* in_data);
int board_apply_actuation(board_actuation_data_t* out_data);

#endif //_IIT_BOARD_DRIVER_H_