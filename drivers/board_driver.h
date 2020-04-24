#ifndef _IIT_BOARD_DRIVER_H_
#define _IIT_BOARD_DRIVER_H_

#include <stdint.h>

#define BOARD_PRESSURE_1        (0x0001)
#define BOARD_PRESSURE_2        (0x0002)
#define BOARD_PRESSURE_3        (0x0004)
#define BOARD_FLOW_1            (0x0008)
#define BOARD_FLOW_2            (0x0010)
#define BOARD_O2                (0x0020)
#define BOARD_TEMPERATURE_1     (0x0040)
#define BOARD_TEMPERATURE_2     (0x0080)
#define BOARD_TEMPERATURE_3     (0x0100)
#define BOARD_GPIO              (0x0200)
#define BOARD_ALL_SENSORS       (0x03FF)
#define BOARD_NO_SENSOR         (0x0000)

enum 
{
    RC_OK = 0,
    RC_ERROR,
    RC_SENSOR_ERROR,
    RC_BUS_ERROR,
    RC_PWM_ERROR
};

typedef struct
{
    int16_t pressure1;
    int16_t pressure2;
    int16_t pressure3;
    uint16_t flow1;
    uint16_t flow2;
    uint16_t o2;
    int16_t temperature1;
    int16_t temperature2;
    int16_t temperature3;
    uint16_t gpio;
    uint32_t read_mask;
} board_sensor_data_t;

typedef struct
{
    uint32_t valve1;
    uint32_t valve2;
    uint16_t gpio;
} board_actuation_data_t;

typedef void (*board_read_callback_t)(int);

int board_init(void);
int board_read_sensors(board_sensor_data_t* in_data);
int board_read_sensors_async(board_sensor_data_t* in_data, board_read_callback_t read_completed);
int board_apply_actuation(board_actuation_data_t* out_data);

void i2c_xfer_completed(int retcode);
#endif //_IIT_BOARD_DRIVER_H_
