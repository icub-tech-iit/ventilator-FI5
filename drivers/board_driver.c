#include "board_driver.h"

#include "i2c_api.h"
#include "hsc.h"

/****************************************************************/
/* Local definitions.                                           */
/****************************************************************/
#define PRESSURE_SENSOR_1_ADDRESS       (0x29)
#define PRESSURE_SENSOR_2_ADDRESS       (0x29)
#define PRESSURE_SENSOR_3_ADDRESS       (0x29)

#define PRESSURE_SENSOR_1_CHANNEL       (0x00)
#define PRESSURE_SENSOR_2_CHANNEL       (0x01)
#define PRESSURE_SENSOR_3_CHANNEL       (0x02)

#define I2C_MUX_CHANNEL_NONE            (0xFF)

/****************************************************************/
/* Local type definitions.                                      */
/****************************************************************/
typedef struct 
{
    uint8_t i2c_channel;
    hsc_handle_t sensor;
} pressure_sensor_t;

typedef struct
{
    pressure_sensor_t pressure_sensor1;
    pressure_sensor_t pressure_sensor2;
    pressure_sensor_t pressure_sensor3;
} board_device_t;

/****************************************************************/
/* Local functions declarations.                                */
/****************************************************************/
static int i2c_xfer(i2c_xfer_list_t *xfers, int addr);
static int i2c_mux_select(int channel);
static int board_read_pressure(pressure_sensor_t* sensor, 
                               uint16_t* pressure,
                               uint16_t* temperature);

/****************************************************************/
/* Local data.                                                  */
/****************************************************************/
static board_device_t board_dev;

/****************************************************************/
/* Exported APIs.                                               */
/****************************************************************/
int board_init(void)
{
    // Initialize the I2C mux channel mapping
    board_dev.pressure_sensor1.i2c_channel = PRESSURE_SENSOR_1_CHANNEL;
    board_dev.pressure_sensor1.i2c_channel = PRESSURE_SENSOR_2_CHANNEL;
    board_dev.pressure_sensor1.i2c_channel = PRESSURE_SENSOR_3_CHANNEL;

    // Initialize the pressure sensors handles
    hsc_init(&board_dev.pressure_sensor1.sensor, i2c_xfer, PRESSURE_SENSOR_1_ADDRESS);
    hsc_init(&board_dev.pressure_sensor2.sensor, i2c_xfer, PRESSURE_SENSOR_2_ADDRESS);
    hsc_init(&board_dev.pressure_sensor3.sensor, i2c_xfer, PRESSURE_SENSOR_3_ADDRESS);

    // Initialize the sensor' properties.
	hsc_config_device(&board_dev.pressure_sensor1.sensor, &HSCDANN150PG2A5);
	hsc_config_device(&board_dev.pressure_sensor2.sensor, &HSCMAND160MD2A5);
	hsc_config_device(&board_dev.pressure_sensor3.sensor, &HSCMAND160MD2A5);

    return RC_OK;
}

int board_read_sensors(board_sensor_data_t* in_data)
{
    int ret_code;

    // Read pressure sensor 1
    ret_code = board_read_pressure(&board_dev.pressure_sensor1, 
                                   &in_data->pressure1,
                                   &in_data->temperature1);
    if(ret_code != RC_OK)
    {
        return ret_code;
    }

    // Read pressure sensor 2
    ret_code = board_read_pressure(&board_dev.pressure_sensor2, 
                                   &in_data->pressure2,
                                   &in_data->temperature2);
    if(ret_code != RC_OK)
    {
        return ret_code;
    }

    // Read pressure sensor 3
    ret_code = board_read_pressure(&board_dev.pressure_sensor3, 
                                   &in_data->pressure3,
                                   &in_data->temperature3);
    if(ret_code != RC_OK)
    {
        return ret_code;
    }

    return RC_OK;
}

int board_apply_actuation(board_actuation_data_t* out_data)
{
    // TODO: call the PWM driver here
    return RC_OK;
}

/****************************************************************/
/* Local functions definitions.                                 */
/****************************************************************/
static int i2c_xfer(i2c_xfer_list_t *xfers, int addr)
{
    // Write here HAL I2C function wrapper
    return 0;
}

static int i2c_mux_select(int channel)
{
    // Call the I2C mux driver here
    return 0;
}

static int board_read_pressure(pressure_sensor_t* sensor, 
                               uint16_t* pressure,
                               uint16_t* temperature)
{
    float sensor_pressure;
    float sensor_temperature;
    int result;
    int ret_code;

    // Switch the I2C mux
    if(sensor->i2c_channel != I2C_MUX_CHANNEL_NONE)
    {
        result = i2c_mux_select(sensor->i2c_channel);

        if(result != RC_OK)
        {
            return result;
        }
    }

    // Read the sensor data
    result = hsc_read(&sensor->sensor, &sensor_pressure, &sensor_temperature);

    // Check the reading result 
    if(result == 0)
    {
        // Convert the data in some way and store the result
        // into the output variable.
        // ...
        *pressure    = (uint16_t)sensor_pressure;
        *temperature = (uint16_t)sensor_temperature;

        // Mark the measure reading as good.
        ret_code = RC_OK;
    }
    else
    {
        // Sensor reading went wrong
        ret_code = RC_SENSOR_ERROR;
    }

    return ret_code;
}