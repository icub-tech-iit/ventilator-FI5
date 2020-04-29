#ifndef _IIT_BOARD_DRIVER_H_
#define _IIT_BOARD_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define BOARD_PRESSURE_1        (0x0001)
#define BOARD_PRESSURE_2        (0x0002)
#define BOARD_PRESSURE_3        (0x0004)
#define BOARD_PRESSURE_4        (0x0008)
#define BOARD_FLOW_1            (0x0010)
#define BOARD_FLOW_2            (0x0020)
#define BOARD_O2                (0x0040)
#define BOARD_TEMPERATURE_1     (0x0080)
#define BOARD_TEMPERATURE_2     (0x0100)
#define BOARD_TEMPERATURE_3     (0x0200)
#define BOARD_TEMPERATURE_4     (0x0400)
#define BOARD_GPIO              (0x0800)
#define BOARD_ENCODER           (0x1000)
#define BOARD_ANALOG            (0x2000)
#define BOARD_ALL_SENSORS       (0x3FFF)
#define BOARD_NO_SENSOR         (0x0000)

#define BOARD_BUTTON_ENCODER    (0x0001)

enum 
{
    RC_OK = 0,
    RC_ERROR,
    RC_SENSOR_ERROR,
    RC_BUS_ERROR,
    RC_PWM_ERROR,
    RC_GPIO_ERROR,
    RC_NOT_INSTALLED_ERROR,
    RC_UNSUPPORTED_ERROR
};

enum
{
    PRESSURE_SENSOR_TYPE_NONE = 0,
    PRESSURE_SENSOR_TYPE_HSCDANN150PG2A5,
    PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
    PRESSURE_SENSOR_TYPE_HSCSANN100PA2A5
};

enum
{
    FLOW_SENSOR_TYPE_NONE = 0,
    FLOW_SENSOR_TYPE_ZEPHYR
};

enum
{
    O2_SENSOR_TYPE_NONE = 0
};

enum
{
    GPIO_EXPANDER_TYPE_NONE = 0,
    GPIO_EXPANDER_TYPE_MCP23017
};

typedef struct
{
    int type;
    int address;
} device_config_t;

typedef struct
{
    device_config_t pressure_sensor1;
    device_config_t pressure_sensor2;
    device_config_t pressure_sensor3;
    device_config_t pressure_sensor4;
    device_config_t flow_sensor1;
    device_config_t flow_sensor2;
    device_config_t o2_sensor;
    device_config_t gpio_expander;
} board_config_t;

typedef struct
{
    int16_t pressure1;
    int16_t pressure2;
    int16_t pressure3;
    int16_t pressure4;
    uint16_t flow1;
    uint16_t flow2;
    uint16_t o2;
    int16_t temperature1;
    int16_t temperature2;
    int16_t temperature3;
    int16_t temperature4;
    uint16_t gpio;
    int32_t encoder;
    uint32_t buttons;
    uint16_t analog_input[4];
    uint32_t read_mask;
} board_sensor_data_t;

typedef struct
{
    uint32_t valve1;
    uint32_t valve2;
    uint32_t buzzer;
    uint16_t gpio;
} board_actuation_data_t;

typedef void (*board_read_callback_t)(int);

int board_init(board_config_t* config);
int board_read_sensors(board_sensor_data_t* in_data);
int board_read_sensors_async(board_sensor_data_t* in_data, board_read_callback_t read_completed);
int board_apply_actuation(board_actuation_data_t* out_data);
void i2c_xfer_completed(int retcode);

#ifdef __cplusplus
}
#endif

#endif //_IIT_BOARD_DRIVER_H_