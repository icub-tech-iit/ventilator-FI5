#include "board_driver.h"

#include "i2c_api.h"
#include "i2c_mux.h"
#include "hsc.h"
#include "pwm.h"
#include "zephyr.h"
#include "mcp23017.h"

#define BREADBOARD

#ifdef DEBUG_HAL
    #include "board_sim.h"
#endif

#if defined(__ARMCOMPILER_VERSION)
#define ACCESS_ONCE(x) (x)
#else
#define ACCESS_ONCE(x) (*((volatile typeof(x) *)&(x)))
#endif

/****************************************************************/
/* Local definitions.                                           */
/****************************************************************/
#define I2C_MUX_ADDRESS                 (0x70)

#define PRESSURE_SENSOR_1_ADDRESS       (0x28)
#define PRESSURE_SENSOR_2_ADDRESS       (0x28)
#define PRESSURE_SENSOR_3_ADDRESS       (0x28)

#define PRESSURE_SENSOR_1_CHANNEL       (0x00)
#define PRESSURE_SENSOR_2_CHANNEL       (0x01)
#define PRESSURE_SENSOR_3_CHANNEL       (0x02)

#define FLOW_SENSOR_1_ADDRESS           (0x49)
#define FLOW_SENSOR_2_ADDRESS           (0x49)

#define FLOW_SENSOR_1_CHANNEL           (0x01)
#define FLOW_SENSOR_2_CHANNEL           (0x02)

#define GPIO_EXPANDER_ADDRESS           (0x20)

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
    i2c_xfer_list_t xfers;
    int addr;
    int index;
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
    mcp23017_handle_t gpio_expander;

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
    BOARD_STATE_CHANNEL2,
    BOARD_STATE_PRESSURE2,
    BOARD_STATE_FLOW1,
    BOARD_STATE_CHANNEL3,
    BOARD_STATE_PRESSURE3,
    BOARD_STATE_FLOW2,
    BOARD_STATE_GPIO,
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

static int board_read_gpio_expander(mcp23017_handle_t* gpio_dev, uint16_t* value);

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

static mcp23017_cfg_t gpio_cfg = {
    .direction = 0x00FF,
    .pup = 0x00FF
};

static pwm_config_t pwm1_cfg = {
    .timer_frequency = 200000000,
    .pwm_frequency = 10000,
    .channel = 1,
    .input_voltage = 5000
};

static pwm_config_t pwm2_cfg = {
    .timer_frequency = 100000000,
    .pwm_frequency = 10000,
    .channel = 1,
    .input_voltage = 5000
};

static i2c_xfer_cb_data_t i2c_xfer_cb_data = 
{
    .cb = 0,
    .args = 0,
    .index = 0
};

#if defined(__ARMCOMPILER_VERSION)
static volatile int i2c_mux_semaphore = 0;
static volatile int i2c_xfer_semaphore = 0;
#else
static int i2c_mux_semaphore = 0;
static int i2c_xfer_semaphore = 0;
#endif

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

#ifdef BREADBOARD
    // Initialize the sensor' properties.
    hsc_config_device(&board_dev.pressure_sensor1.sensor, &HSCSANN100PA2A5);
    hsc_config_device(&board_dev.pressure_sensor2.sensor, &HSCSANN100PA2A5);
#else
	hsc_config_device(&board_dev.pressure_sensor1.sensor, &HSCDANN150PG2A5);
	hsc_config_device(&board_dev.pressure_sensor2.sensor, &HSCMAND160MD2A5);
	hsc_config_device(&board_dev.pressure_sensor3.sensor, &HSCMAND160MD2A5);
#endif

    // Initialize the flow sensor
    i2c_mux_select(&board_dev.i2c_mux, FLOW_SENSOR_1_CHANNEL, i2c_mux_select_completed);
    i2c_mux_select_wait();
    zephyr_init(&board_dev.flow_sensor1, i2c_xfer_async, FLOW_SENSOR_1_ADDRESS);

#ifndef BREADBOARD
    i2c_mux_select(&board_dev.i2c_mux, FLOW_SENSOR_2_CHANNEL, i2c_mux_select_completed);
    i2c_mux_select_wait();
    zephyr_init(&board_dev.flow_sensor2, i2c_xfer_async, FLOW_SENSOR_2_ADDRESS);
#endif
    // Initialize the I2C GPIO expander
    mcp23017_init(&board_dev.gpio_expander, i2c_xfer_async, GPIO_EXPANDER_ADDRESS);

    mcp23017_config_device(&board_dev.gpio_expander, &gpio_cfg);

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

    ret_code = mcp23017_write(&board_dev.gpio_expander, out_data->gpio, 0);

    return RC_OK;
}

/****************************************************************/
/* Local functions definitions.                                 */
/****************************************************************/

static int i2c_xfer_segment(i2c_xfer_list_t *xfers, int addr, int index)
{
    int flag = I2C_NEXT_FRAME;
    i2c_xfer_t* xfer = &xfers->xfers[index];

    if(xfers->xfer_num == 1)
        flag = I2C_FIRST_AND_LAST_FRAME;
    else if (index == 0)
        flag = I2C_FIRST_FRAME;
    else if (index == (xfers->xfer_num - 1))
        flag = I2C_LAST_FRAME;
    else
        flag = I2C_NEXT_FRAME;

    addr = addr << 1;
    if(xfer->direction == WRITE)
    {
        return HAL_I2C_Master_Sequential_Transmit_IT(&hi2c1, addr, (uint8_t*)xfer->buf, xfer->len, flag);
    }
    else
    {
        return HAL_I2C_Master_Sequential_Receive_IT(&hi2c1, addr, (uint8_t*)xfer->buf, xfer->len, flag);
    }
}

static void i2c_xfer_sync_cb(int status, void *arg)
{
    int *retcode = (int *)arg;
    *retcode = status;
    ACCESS_ONCE(i2c_xfer_semaphore) = 1;
}

static int i2c_xfer(i2c_xfer_list_t *xfers, int addr)
{
#if defined(__ARMCOMPILER_VERSION)
    volatile int retcode = RC_OK;
    ACCESS_ONCE(i2c_xfer_semaphore) = 0;
    i2c_xfer_async(xfers, addr,  i2c_xfer_sync_cb, (int*)&retcode);
    while (!ACCESS_ONCE(i2c_xfer_semaphore));

    return (ACCESS_ONCE(retcode) == 0) ? RC_OK : RC_BUS_ERROR;  
#else
    int retcode;   
    ACCESS_ONCE(i2c_xfer_semaphore) = 0;
    i2c_xfer_async(xfers, addr,  i2c_xfer_sync_cb, &retcode);
    while (!ACCESS_ONCE(i2c_xfer_semaphore));

    return (ACCESS_ONCE(retcode) == 0) ? RC_OK : RC_BUS_ERROR;  
#endif    
}

static int i2c_xfer_async(i2c_xfer_list_t *xfers, int addr, i2c_xfer_cb_t cb, void* args)
{
    int retcode;

    // When no callback is passed the transfer falls back
    // into the synchronous way.
    if(!cb)
        return i2c_xfer(xfers, addr);

    // Save the informations that will be used when the
    // bus operation is completed
    i2c_xfer_cb_data.cb = cb;
    i2c_xfer_cb_data.args = args;
    i2c_xfer_cb_data.index = 0;
    i2c_xfer_cb_data.xfers = *xfers;
    i2c_xfer_cb_data.addr = addr;

    retcode = i2c_xfer_segment(xfers, addr, 0);

    return (retcode == 0) ? RC_OK : RC_BUS_ERROR;
}

void i2c_xfer_completed(int retcode)
{
    int last = 1;

    i2c_xfer_cb_data.index++;
    /* As long as we don't hit an error, and there are segments, keep on TXing */
    if((retcode == 0) && (i2c_xfer_cb_data.index < i2c_xfer_cb_data.xfers.xfer_num))
    {
        retcode = i2c_xfer_segment(&i2c_xfer_cb_data.xfers, i2c_xfer_cb_data.addr, i2c_xfer_cb_data.index);
        if (retcode == 0)
            last = 0; /* segment successfully enqueued, this is not the last CB invokation */
    }

    if(i2c_xfer_cb_data.cb && last)
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
    ACCESS_ONCE(i2c_mux_semaphore) = 1;
}

static int i2c_mux_select_wait(void)
{
    while(!ACCESS_ONCE(i2c_mux_semaphore))
    {
        HAL_Delay(1);
    }

    ACCESS_ONCE(i2c_mux_semaphore) = 0;

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

static int board_read_gpio_expander(mcp23017_handle_t* gpio_dev, uint16_t* value)
{
    int result;

    result = mcp23017_read(gpio_dev, value, board_fsm_update);

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
    int ret_code = RC_OK;
    int skip_state = 0;

    // If there's any error abort sensor reading
    // Note: Whoever calls this function must correcly
    // recover from possible bus errors.
    if(status != 0)
    {
        board_fsm_clear(status);
        return;
    }

    // The FSM update function was called due to
    // an event raising. The exit function of the
    // FSM old state must be called before switch to the
    // new state.
    switch(board_fsm.state)
    {
        case BOARD_STATE_CHANNEL2:
            // Process the pressure sensor 1 data
            ret_code = board_pressure_data_received();
            break;

        case BOARD_STATE_FLOW1:
            // Process the pressure sensor 2 data
            ret_code = board_pressure_data_received();
            break;

        case BOARD_STATE_FLOW2:
            // Process the pressure sensor 2 data
            ret_code = board_pressure_data_received();
            break;

        default:
            break;
    }

    // If any error happened reset the FSM
    if(ret_code != RC_OK)
    {
        board_fsm_clear(ret_code);
        return;
    }

    // Loop over the FSM states until there's something usefull to do.
    // The read mask inside the in_sensor data structure defines
    // which sensors are to be read. When the FSM is in a state responsible to
    // the read action of a sensor which is not needed that state is skipped.
    // The FSM evolve immediately into the next one until it finds some
    // needed sensor reading or the complete state.
    do
    {
        // Reset the skip flag. We assume that the state will be skipped.
        // Only when an action is executed the skip flag is reset.
        skip_state = 1;

        switch(board_fsm.state)
        {
            case BOARD_STATE_CHANNEL1:
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_1 | BOARD_TEMPERATURE_1)) != 0)
                {
                    ret_code = i2c_mux_select(&board_dev.i2c_mux, 0, board_fsm_update);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_PRESSURE1:
                // Trigger sensor 1 reading
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_1 | BOARD_TEMPERATURE_1)) != 0)
                {
                    ret_code = board_read_pressure(&board_dev.pressure_sensor1,
                                                &board_fsm.in_data->pressure1,
                                                &board_fsm.in_data->temperature1);
                    skip_state = 0;
                }
                break;


            case BOARD_STATE_CHANNEL2:
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_2 | BOARD_TEMPERATURE_2 | BOARD_FLOW_1)) != 0)
                {
                    ret_code = i2c_mux_select(&board_dev.i2c_mux, 1, board_fsm_update);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_PRESSURE2:
                // Trigger sensor 2 reading
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_2 | BOARD_TEMPERATURE_2)) != 0)
                {
                    ret_code = board_read_pressure(&board_dev.pressure_sensor2,
                                                &board_fsm.in_data->pressure2,
                                                &board_fsm.in_data->temperature2);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_FLOW1:
                if((board_fsm.in_data->read_mask & BOARD_FLOW_1) != 0)
                {
                    ret_code = board_read_flow(&board_dev.flow_sensor1,
                                               &board_fsm.in_data->flow1);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_CHANNEL3:
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_3 | BOARD_TEMPERATURE_3 | BOARD_FLOW_2)) != 0)
                {
                    ret_code = i2c_mux_select(&board_dev.i2c_mux, 2, board_fsm_update);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_PRESSURE3:
                // Trigger sensor 3 reading
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_3 | BOARD_TEMPERATURE_3)) != 0)
                {
                    ret_code = board_read_pressure(&board_dev.pressure_sensor3,
                                                &board_fsm.in_data->pressure3,
                                                &board_fsm.in_data->temperature3);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_FLOW2:
                if((board_fsm.in_data->read_mask & BOARD_FLOW_2) != 0)
                {
                    ret_code = board_read_flow(&board_dev.flow_sensor2,
                                               &board_fsm.in_data->flow2);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_GPIO:
                // Trigger sensor 3 reading
                if((board_fsm.in_data->read_mask & BOARD_GPIO) != 0)
                {
                    ret_code = board_read_gpio_expander(&board_dev.gpio_expander,
                                                &board_fsm.in_data->gpio);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_COMPLETED:
                // Never skip the final state
                skip_state = 0;
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
            break;
        }
    } while(skip_state != 0);
}

static void board_read_completed_callback(int status)
{
    board_fsm.result = status;
}
