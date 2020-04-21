#include "board_driver.h"

#include "i2c_api.h"
#include "i2c_mux.h"
#include "hsc.h"
#include "pwm.h"
#include "zephyr.h"

#ifdef DEBUG_HAL
    #include "board_sim.h"
#endif

/****************************************************************/
/* Local definitions.                                           */
/****************************************************************/
#define I2C_MUX_ADDRESS                 (0x70)

#define PRESSURE_SENSOR_1_ADDRESS       (0x29)
#define PRESSURE_SENSOR_2_ADDRESS       (0x29)
#define PRESSURE_SENSOR_3_ADDRESS       (0x29)

#define PRESSURE_SENSOR_1_CHANNEL       (0x00)
#define PRESSURE_SENSOR_2_CHANNEL       (0x01)
#define PRESSURE_SENSOR_3_CHANNEL       (0x02)

#define FLOW_SENSOR_1_ADDRESS           (0x49)
#define FLOW_SENSOR_2_ADDRESS           (0x49)

/****************************************************************/
/* Local type definitions.                                      */
/****************************************************************/
typedef struct 
{
    hsc_handle_t sensor;
    i2c_mux_handle_t* i2c_mux;
    uint8_t i2c_channel;
} pressure_sensor_t;

typedef struct
{
    i2c_xfer_cb_t cb;
    void* args;
} i2c_xfer_cb_data_t;

typedef struct
{
    // Sensor devices
    pressure_sensor_t pressure_sensor1;
    pressure_sensor_t pressure_sensor2;
    pressure_sensor_t pressure_sensor3;
    zephyr_handle_t flow_sensor1;
    zephyr_handle_t flow_sensor2;
    i2c_mux_handle_t i2c_mux;

    // Actuator devices
    pwm_handle_t pwm_valve1;
    pwm_handle_t pwm_valve2;
} board_device_t;

typedef struct
{
    int state;
    board_sensor_data_t* in_data;
    board_read_callback_t read_completed;
    int result;

    int16_t* pressure;
    int16_t* temperature;
    float sensor_pressure;
    float sensor_temperature;
} board_fsm_t;

enum
{
    BOARD_STATE_IDLE = 0,
    BOARD_STATE_CHANNEL1,
    BOARD_STATE_PRESSURE1,
    BOARD_STATE_FLOW1,
    BOARD_STATE_CHANNEL2,
    BOARD_STATE_PRESSURE2,
    BOARD_STATE_FLOW2,
    BOARD_STATE_CHANNEL3,
    BOARD_STATE_PRESSURE3,
    BOARD_STATE_COMPLETED
};

/****************************************************************/
/* Local functions declarations.                                */
/****************************************************************/
static int i2c_xfer(i2c_xfer_list_t *xfers, int addr);
static int i2c_xfer_async(i2c_xfer_list_t *xfers, int addr, i2c_xfer_cb_t cb, void* args);
static int i2c_mux_select(i2c_mux_handle_t* mux, int channel, i2c_mux_callback_t callback);
static void i2c_mux_select_completed(int status);
static int i2c_mux_select_wait(void);

static int board_read_pressure(pressure_sensor_t* sensor, 
                               int16_t* pressure,
                               int16_t* temperature);

static int board_read_flow(zephyr_handle_t* sensor, uint16_t* flow);

static int board_pressure_data_received(void);

static void board_fsm_start(void);
static void board_fsm_clear(int ret_code);
static void board_fsm_update(int status);

static void board_read_completed_callback(int status);

void i2c_xfer_completed(int retcode);

/****************************************************************/
/* Local data.                                                  */
/****************************************************************/
static board_device_t board_dev;
static board_fsm_t board_fsm;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern I2C_HandleTypeDef hi2c1;

static pwm_config_t pwm1_cfg = {
    .timer_frequency = 108000000,
    .pwm_frequency = 10000,
    .channel = 1,
    .input_voltage = 24000
};

static pwm_config_t pwm2_cfg = {
    .timer_frequency = 54000000,
    .pwm_frequency = 10000,
    .channel = 1,
    .input_voltage = 24000
};

static i2c_xfer_cb_data_t i2c_xfer_cb_data = 
{
    .cb = 0,
    .args = 0
};

static int i2c_mux_semaphore = 0;

/****************************************************************/
/* Exported APIs.                                               */
/****************************************************************/
int board_init(void)
{
    // Initialize the I2C mux channel mapping
    board_dev.pressure_sensor1.i2c_mux = &board_dev.i2c_mux;
    board_dev.pressure_sensor2.i2c_mux = &board_dev.i2c_mux;
    board_dev.pressure_sensor3.i2c_mux = &board_dev.i2c_mux;

    board_dev.pressure_sensor1.i2c_channel = PRESSURE_SENSOR_1_CHANNEL;
    board_dev.pressure_sensor2.i2c_channel = PRESSURE_SENSOR_2_CHANNEL;
    board_dev.pressure_sensor3.i2c_channel = PRESSURE_SENSOR_3_CHANNEL;

    // Initialize the I2C mux device handles
    i2c_mux_init(&board_dev.i2c_mux, i2c_xfer_async, I2C_MUX_ADDRESS);

    // Initialize the pressure sensors handles
    hsc_init(&board_dev.pressure_sensor1.sensor, i2c_xfer_async, PRESSURE_SENSOR_1_ADDRESS);
    hsc_init(&board_dev.pressure_sensor2.sensor, i2c_xfer_async, PRESSURE_SENSOR_2_ADDRESS);
    hsc_init(&board_dev.pressure_sensor3.sensor, i2c_xfer_async, PRESSURE_SENSOR_3_ADDRESS);

    // Initialize the sensor' properties.
	hsc_config_device(&board_dev.pressure_sensor1.sensor, &HSCDANN150PG2A5);
	hsc_config_device(&board_dev.pressure_sensor2.sensor, &HSCMAND160MD2A5);
	hsc_config_device(&board_dev.pressure_sensor3.sensor, &HSCMAND160MD2A5);

    // Initialize the flow sensor
    i2c_mux_select(&board_dev.i2c_mux, 0, i2c_mux_select_completed);
    i2c_mux_select_wait();
    zephyr_init(&board_dev.flow_sensor1, i2c_xfer_async, FLOW_SENSOR_1_ADDRESS);

    i2c_mux_select(&board_dev.i2c_mux, 1, i2c_mux_select_completed);
    i2c_mux_select_wait();
    zephyr_init(&board_dev.flow_sensor2, i2c_xfer_async, FLOW_SENSOR_2_ADDRESS);

    // Initialize the PWM devices
    pwm_init(&board_dev.pwm_valve1, (void*)&htim1, &pwm1_cfg);
    pwm_init(&board_dev.pwm_valve2, (void*)&htim2, &pwm2_cfg);

    // Set to zero the PWM output voltage
    pwm_set(&board_dev.pwm_valve1, 0);
    pwm_set(&board_dev.pwm_valve2, 0);

    // Start the PWM channels
    pwm_start(&board_dev.pwm_valve1);
    pwm_start(&board_dev.pwm_valve2);

    return RC_OK;
}

int board_read_sensors(board_sensor_data_t* in_data)
{
    int ret_code;

    // Start the asyncrhronous read of all sensors
    ret_code = board_read_sensors_async(in_data, board_read_completed_callback);
    if(ret_code != RC_OK)
    {
        return ret_code;
    }

    // Wait for the operation to be completed
    while(board_fsm.state != BOARD_STATE_IDLE) 
    {
        HAL_Delay(1);
        // asm("wfi");
    }

    return board_fsm.result;
}

int board_read_sensors_async(board_sensor_data_t* in_data, board_read_callback_t read_completed)
{
    board_fsm.in_data = in_data;
    board_fsm.read_completed = read_completed;

    board_fsm_start();

    return RC_OK;
}

int board_apply_actuation(board_actuation_data_t* out_data)
{
    int ret_code;

    ret_code = pwm_set(&board_dev.pwm_valve1, out_data->valve1);
    if(ret_code != 0)
    {
        return RC_PWM_ERROR;
    }
    
    ret_code = pwm_set(&board_dev.pwm_valve2, out_data->valve2);
    if(ret_code != 0)
    {
        return RC_PWM_ERROR;
    }

    return RC_OK;
}

/****************************************************************/
/* Local functions definitions.                                 */
/****************************************************************/
static int i2c_xfer(i2c_xfer_list_t *xfers, int addr)
{
    int retcode;
    int i;

    for(i = 0; i < xfers->xfer_num; ++i)
    {
        i2c_xfer_t* xfer = &xfers->xfers[i];

        if(xfer->direction == WRITE)
        {
            retcode = HAL_I2C_Master_Transmit(&hi2c1, addr, (uint8_t*)xfer->buf, xfer->len, 0);
        }
        else
        {
            retcode = HAL_I2C_Master_Receive(&hi2c1, addr, (uint8_t*)xfer->buf, xfer->len, 0);
        }

        if(retcode != 0)
            break;
    }

    return (retcode == 0) ? RC_OK : RC_BUS_ERROR;
}

static int i2c_xfer_async(i2c_xfer_list_t *xfers, int addr, i2c_xfer_cb_t cb, void* args)
{
    int retcode;
    int i;

    // When no callback is passed the transfer falls back 
    // into the synchronous way.
    if(!cb)
        return i2c_xfer(xfers, addr);

    // Save the informations that will be used when the 
    // bus operation is completed
    i2c_xfer_cb_data.cb = cb;
    i2c_xfer_cb_data.args = args;

    for(i = 0; i < xfers->xfer_num; ++i)
    {
        i2c_xfer_t* xfer = &xfers->xfers[i];

        if(xfer->direction == WRITE)
        {
            retcode = HAL_I2C_Master_Transmit_IT(&hi2c1, addr, (uint8_t*)xfer->buf, xfer->len);
        }
        else
        {
            retcode = HAL_I2C_Master_Receive_IT(&hi2c1, addr, (uint8_t*)xfer->buf, xfer->len);
        }

        if(retcode != 0)
            break;
    }

    return (retcode == 0) ? RC_OK : RC_BUS_ERROR;
}

void i2c_xfer_completed(int retcode)
{
    if(i2c_xfer_cb_data.cb)
        i2c_xfer_cb_data.cb(retcode, i2c_xfer_cb_data.args);
}

static int i2c_mux_select(i2c_mux_handle_t* mux, int channel, i2c_mux_callback_t callback)
{
    int result;
    int ret_code;

    // Call the I2C mux driver here
    result = i2c_mux_channel_select(mux, channel, callback);
    if(result == 0)
    {
        ret_code = RC_OK;
    }
    else
    {
        ret_code = RC_BUS_ERROR;
    }

    return ret_code;
}

static void i2c_mux_select_completed(int status)
{
    i2c_mux_semaphore = 1;
}

static int i2c_mux_select_wait(void)
{
    while(!i2c_mux_semaphore)
    {
        HAL_Delay(1);
    }

    i2c_mux_semaphore = 0;

    return RC_OK;
}

static int board_read_pressure(pressure_sensor_t* sensor, 
                               int16_t* pressure,
                               int16_t* temperature)
{
    int result;

    // Store the output data pointers
    board_fsm.pressure = pressure;
    board_fsm.temperature = temperature;

    // Read the sensor data
    result = hsc_read(&sensor->sensor, 
                      &board_fsm.sensor_pressure, 
                      &board_fsm.sensor_temperature, 
                      board_fsm_update);

    return (result == 0) ? RC_OK : RC_SENSOR_ERROR;
}

static int board_read_flow(zephyr_handle_t* sensor, uint16_t* flow)
{
    int result;

    result = zephyr_read(sensor, flow, board_fsm_update);

    return (result == 0) ? RC_OK : RC_SENSOR_ERROR;
}

static int board_pressure_data_received()
{
    *board_fsm.pressure    = (uint16_t)(board_fsm.sensor_pressure * 1000.0f);
    *board_fsm.temperature = (uint16_t)(board_fsm.sensor_temperature);

    return 0;
}


static void board_fsm_start(void)
{
    board_fsm.state = BOARD_STATE_CHANNEL1;
    board_fsm.result = 0;
    board_fsm_update(0);
}

static void board_fsm_clear(int ret_code)
{
    if(board_fsm.read_completed)
        board_fsm.read_completed(ret_code);

    board_fsm.state = BOARD_STATE_IDLE;
}

static void board_fsm_update(int status)
{
    int ret_code;

    // If there's any error abort sensor reading
    // Note: Whoever calls this function must correcly
    // recover from possible bus errors.
    if(status != 0)
    {
        board_fsm_clear(status);
        return;
    }

    switch(board_fsm.state)
    {
        case BOARD_STATE_CHANNEL1:
            ret_code = i2c_mux_select(&board_dev.i2c_mux, 0, board_fsm_update);
            break;

        case BOARD_STATE_PRESSURE1:
            // Trigger sensor 1 reading
            ret_code = board_read_pressure(&board_dev.pressure_sensor1, 
                                           &board_fsm.in_data->pressure1,
                                           &board_fsm.in_data->temperature1);
            break;

        case BOARD_STATE_FLOW1:
            // Process the pressure sensor 1 data  
            board_pressure_data_received();

            ret_code = board_read_flow(&board_dev.flow_sensor1,
                                       &board_fsm.in_data->flow1);
            break;


        case BOARD_STATE_CHANNEL2:
            ret_code = i2c_mux_select(&board_dev.i2c_mux, 1, board_fsm_update);
            break;

        case BOARD_STATE_PRESSURE2:
            // Trigger sensor 2 reading
            ret_code = board_read_pressure(&board_dev.pressure_sensor2, 
                                           &board_fsm.in_data->pressure2,
                                           &board_fsm.in_data->temperature2);
            break;

        case BOARD_STATE_FLOW2:
            // Process the pressure sensor 2 data  
            board_pressure_data_received();

            ret_code = board_read_flow(&board_dev.flow_sensor2,
                                       &board_fsm.in_data->flow2);
            break;

        case BOARD_STATE_CHANNEL3:
            ret_code = i2c_mux_select(&board_dev.i2c_mux, 2, board_fsm_update);
            break;

        case BOARD_STATE_PRESSURE3:
            // Trigger sensor 3 reading
            ret_code = board_read_pressure(&board_dev.pressure_sensor3, 
                                           &board_fsm.in_data->pressure3,
                                           &board_fsm.in_data->temperature3);
            break;

        case BOARD_STATE_COMPLETED:
            // Process the pressure sensor 2 data  
            ret_code = board_pressure_data_received();
            break;
    }

    if(ret_code == RC_OK)
    {
        if(board_fsm.state == BOARD_STATE_COMPLETED)
        {
            // If all the work is done call the completion callback
            board_fsm_clear(RC_OK);
        }
        else
        {
            // Go into the next state
            board_fsm.state++;
        }
    }
    else
    {
        board_fsm_clear(ret_code);
    }
}

static void board_read_completed_callback(int status)
{
    board_fsm.result = status;
}
