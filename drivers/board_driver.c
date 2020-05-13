#include "board_driver.h"

#include "i2c_api.h"
#include "i2c_mux.h"
#include "hsc.h"
#include "pwm.h"
#include "zephyr.h"
#include "mcp23017.h"
#include "encoder.h"
#include "analog.h"
#include "button.h"
#include "buzzer.h"

#include "access_once.h"

#include <string.h>

#warning FIXME_PWM
/*
 * The correct PWM configuration can be achieved by disabling the following
 * PWM_QUIRK definition. Doing this brokes the higher level implementation
 * because it has been tuned when the PWM was wrongly configured. So we
 * need to remove PWM_QUIRK and fix higher level accordingly..
 *
 * Two things changes:
 * 1) when PWM_QUIRK is enabled the PWM input rage is 0 to 5000, when PWM_QUIRK
 *    is disabled the PWM input range is 0 to 24000; since the Vsupply is 24V,
 *    this cause the PWM input to correctly represent the equivalent PWM output
 *    voltage.
 * 2) when PWM_QUIRK is enabled, the PWM2 duty is clamped to 50%
 */
#define PWM_QUIRK

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
#define PRESSURE_SENSOR_4_CHANNEL       (0x03)

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
    pressure_sensor_t pressure_sensor4;
    zephyr_handle_t flow_sensor1;
    zephyr_handle_t flow_sensor2;
    i2c_mux_handle_t i2c_mux;
    mcp23017_handle_t gpio_expander;
    encoder_handle_t encoder;
    analog_handle_t analog;

    // Actuator devices
    pwm_handle_t pwm_valve1;
    pwm_handle_t pwm_valve2;

    // Buttons
    button_handle_t buttons[BOARD_BUTTON_NUMBER];

    // Board configuration
    board_config_t config;
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

    uint16_t gpio_value;
} board_fsm_t;

enum
{
    BOARD_STATE_IDLE = 0,
    BOARD_STATE_CHANNEL0,
    BOARD_STATE_PRESSURE1,
    BOARD_STATE_CHANNEL1,
    BOARD_STATE_PRESSURE2,
    BOARD_STATE_FLOW1,
    BOARD_STATE_CHANNEL2,
    BOARD_STATE_PRESSURE3,
    BOARD_STATE_FLOW2,
    BOARD_STATE_CHANNEL3,
    BOARD_STATE_PRESSURE4,
    BOARD_STATE_ANALOG,
    BOARD_STATE_GPIO_EXPANDER,
    BOARD_STATE_ENCODER,
    BOARD_STATE_BUTTONS,
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
                               int16_t* temperature,
	                       uint16_t* pressure_raw,
                               uint16_t* temperature_raw
	);

static int board_read_flow(zephyr_handle_t* sensor, uint16_t* flow, uint16_t *raw_flow);

static int board_read_gpio_expander(mcp23017_handle_t* gpio_dev, uint16_t* value);
static int board_read_gpio(uint32_t* value);

static int board_read_encoder(encoder_handle_t* encoder_dev,
                              int32_t* tick,
                              uint32_t* button_mask);
static int board_read_buttons(uint32_t* button_mask);
static int board_read_analog(analog_handle_t* analog_dev,
                             uint16_t* o2,
                             uint16_t* analog_input);

static int board_pressure_data_received(void);
static int board_gpio_data_received(void);

static void board_fsm_start(void);
static void board_fsm_clear(int ret_code);
static void board_fsm_update(int status);

static void board_read_completed_callback(int status);

static int board_init_all(void);
static int board_init_pressure_sensor(hsc_handle_t* sensor,
                                      device_config_t* config);
static int board_init_flow_sensor(zephyr_handle_t* sensor,
                                  device_config_t* config,
                                  int mux_channel);
static int board_init_gpio_expander(mcp23017_handle_t* dev,
                                    const device_config_t* config);

static int board_init_pressure_sensors(void);
static int board_init_flow_sensors(void);
static int board_init_pwms(void);
static int board_init_analog(void);
static int board_init_buttons(void);

static void board_buzzer_ret_code(int ret_code);

static int board_read_check(board_sensor_data_t* in_data);

static void board_sample_encoder(bool *a, bool *b);
static int board_sample_encoder_button();

/****************************************************************/
/* Exported functions declarations.                             */
/****************************************************************/
// marco.accame: i remove it because it is already declared in board_driver.h
// void i2c_xfer_completed(int retcode);
void encoder_changed();

static const board_config_t s_board_config_default =
{
    .pressure_sensor1 =
    {
        .type = PRESSURE_SENSOR_TYPE_HSCDANN150PG2A5,
        .address = 0x28
    },
    .pressure_sensor2 =
    {
        .type = PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
        .address = 0x28
    },
    .pressure_sensor3 =
    {
        .type = PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
        .address = 0x28
    },
    .pressure_sensor4 =
    {
        .type = PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5,
        .address = 0x28
    },
    .flow_sensor1 =
    {
        .type = FLOW_SENSOR_TYPE_ZEPHYR,
        .address = 0x49
    },
    .flow_sensor2 =
    {
        .type = FLOW_SENSOR_TYPE_ZEPHYR,
        .address = 0x49
    },
    .o2_sensor =
    {
        .type = O2_SENSOR_TYPE_NONE,
        .address = 0x00
    },
    .gpio_expander =
    {
        .type = GPIO_EXPANDER_TYPE_MCP23017,
        .address = 0x20
    }
};

const board_config_t * p_board_config_default = &s_board_config_default;

/****************************************************************/
/* Local data.                                                  */
/****************************************************************/
static board_device_t board_dev;
static board_fsm_t board_fsm;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;
extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc3;

static mcp23017_cfg_t gpio_cfg = {
    .direction = 0xFFF0,  // 1b is input. 0b is output. lsb is mounted connector.
    .pup = 0xFFF0
};
#warning here is config of outputs. in mv.
static pwm_config_t pwm1_cfg = {
    .timer_frequency = 200000000,
    .pwm_frequency = 100000,
    .type = PWM_TYPE_NORMAL,
    .channel = 1,
    .n_channel = 0,
#ifdef PWM_QUIRK
    .input_voltage = 5000
#else
    .input_voltage = 24000
#endif
};

static pwm_config_t pwm2_cfg = {
    .timer_frequency = 200000000,
#ifdef PWM_QUIRK
    .pwm_frequency = 100000,
#else
    .pwm_frequency = 50000,
#endif
    .type = PWM_TYPE_COMPLEMENTARY,
    .channel = 2,
    .n_channel = 0,
#ifdef PWM_QUIRK
    .input_voltage = 5000
#else
    .input_voltage = 24000
#endif
};

static i2c_xfer_cb_data_t i2c_xfer_cb_data =
{
    .cb = 0,
    .args = 0,
    .index = 0
};

volatile static int i2c_mux_semaphore = 0;
volatile static int i2c_xfer_semaphore = 0;

/****************************************************************/
/* Exported APIs.                                               */
/****************************************************************/
int board_init(const board_config_t* config)
{
    int ret_code = RC_OK;

    if(NULL == config)
    {
        config = &s_board_config_default;
    }

    // Initialize the buzzer and enable it
    buzzer_init();

    // Store a copy of the board configuration
    memcpy(&board_dev.config, config, sizeof(board_config_t));

    // Initialize the HW
    ret_code = board_init_all();

    // Check the return code and play the buzzer
    board_buzzer_ret_code(ret_code);

    return ret_code;
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
    int ret_code;

    // Store the informations needed in the callback
    board_fsm.in_data = in_data;
    board_fsm.read_completed = read_completed;

    // Check the input parameters
    ret_code = board_read_check(in_data);
    if(ret_code != RC_OK)
        return ret_code;

    // Start the sensor reading FSM
    board_fsm_start();

    // The operation is started
    return RC_OK;
}

int board_apply_actuation(board_actuation_data_t* out_data)
{
    int ret_code;

    // Set the valve 1 PWM duty cycle
    ret_code = pwm_set(&board_dev.pwm_valve1, out_data->valve1);
    if(ret_code != 0)
    {
        return RC_PWM_ERROR;
    }

    // Set the valve 2 PWM duty cycle
    ret_code = pwm_set(&board_dev.pwm_valve2, out_data->valve2);
    if(ret_code != 0)
    {
        return RC_PWM_ERROR;
    }

    // Enable or disable the buzzer
    if(out_data->buzzer)
    {
        ret_code = buzzer_on();
    }
    else
    {
        ret_code = buzzer_off();
    }

    if(ret_code != 0)
    {
        return RC_BUZZER_ERROR;
    }

    // Write the GPIO on the expander
    if(board_dev.config.gpio_expander.type != GPIO_EXPANDER_TYPE_NONE)
    {
        ret_code = mcp23017_write(&board_dev.gpio_expander, out_data->gpio, 0);
        if(ret_code != 0)
        {
            return RC_GPIO_ERROR;
        }
    }

    // Write the local GPIOs
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, (out_data->gpio & BOARD_LED_ALARM) ? GPIO_PIN_SET : GPIO_PIN_RESET);

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
    volatile int retcode = RC_OK;

    ACCESS_ONCE(i2c_xfer_semaphore) = 0;
    i2c_xfer_async(xfers, addr,  i2c_xfer_sync_cb, (int*)&retcode);
    while (!ACCESS_ONCE(i2c_xfer_semaphore))
    {
#ifdef DEBUG_HAL
        HAL_Delay(1);
#endif
    }

    return (ACCESS_ONCE(retcode) == 0) ? RC_OK : RC_BUS_ERROR;
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

    // TODO: error checking on the mux I2C transaction
    return RC_OK;
}

static int board_read_pressure(pressure_sensor_t* sensor,
                               int16_t* pressure,
                               int16_t* temperature,
                               uint16_t* pressure_raw,
                               uint16_t* temperature_raw)
{
    int result;

    // Store the output data pointers
    board_fsm.pressure = pressure;
    board_fsm.temperature = temperature;

    // Read the sensor data
    result = hsc_read(&sensor->sensor,
                      &board_fsm.sensor_pressure,
                      &board_fsm.sensor_temperature,
		      pressure_raw,
                      temperature_raw,
                      board_fsm_update);

    return (result == 0) ? RC_OK : RC_SENSOR_ERROR;
}

static int board_read_flow(zephyr_handle_t* sensor, uint16_t* flow, uint16_t *raw_flow)
{
    int result;

    result = zephyr_read(sensor, flow, raw_flow, board_fsm_update);

    return (result == 0) ? RC_OK : RC_SENSOR_ERROR;
}

static int board_read_gpio_expander(mcp23017_handle_t* gpio_dev, uint16_t* value)
{
    int result;

    result = mcp23017_read(gpio_dev, value, board_fsm_update);

    return (result == 0) ? RC_OK : RC_SENSOR_ERROR;
}

static int board_read_gpio(uint32_t* value)
{
    GPIO_PinState pin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);

    if(pin == GPIO_PIN_SET)
        *value |= BOARD_LED_ALARM;
    else
        *value &= ~BOARD_LED_ALARM;

    return RC_OK;
}

static int board_read_encoder(encoder_handle_t* encoder_dev,
                              int32_t* tick,
                              uint32_t* button_mask)
{
    int encoder_tick;

    encoder_get(encoder_dev, &encoder_tick);

    *tick = encoder_tick;

    return 0;
}

static int board_read_buttons(uint32_t* button_mask)
{
    int i;

    for(i = 0; i < BOARD_BUTTON_NUMBER; ++i)
    {
        bool pressed;
        uint32_t button_bit = (0x1 << i);

        button_out(&board_dev.buttons[i], &pressed);

        if(pressed)
            *button_mask = *button_mask | button_bit;
        else
            *button_mask = *button_mask & ~button_bit;
    }

    return 0;
}

static int board_read_analog(analog_handle_t* analog_dev,
                             uint16_t* o2,
                             uint16_t* analog_input)
{
    uint16_t buffer[5] = {0};

    analog_get_data(analog_dev, buffer);

    *o2 = buffer[4];
    memcpy(analog_input, &buffer[0], 4 * sizeof(uint16_t));

    return 0;
}

static int board_pressure_data_received()
{
    *board_fsm.pressure    = (uint16_t)(board_fsm.sensor_pressure * 1000.0f);
    *board_fsm.temperature = (uint16_t)(board_fsm.sensor_temperature);

    return 0;
}

static int board_gpio_data_received()
{
    board_fsm.in_data->gpio |= ((uint32_t)(board_fsm.gpio_value) & 0x0000FFFF);
    return 0;
}

static void board_fsm_start(void)
{
    board_fsm.state = BOARD_STATE_CHANNEL0;
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
        case BOARD_STATE_CHANNEL1:
            // Process the pressure sensor 1 data
            ret_code = board_pressure_data_received();
            break;

        case BOARD_STATE_FLOW1:
            // Process the pressure sensor 2 data
            ret_code = board_pressure_data_received();
            break;

        case BOARD_STATE_FLOW2:
            // Process the pressure sensor 3 data
            ret_code = board_pressure_data_received();
            break;

        case BOARD_STATE_ANALOG:
            // Process the pressure sensor 4 data
            ret_code = board_pressure_data_received();
            break;

        case BOARD_STATE_ENCODER:
            // Process the data of the I2C GPIO expander 16 pins
            ret_code = board_gpio_data_received();
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
            case BOARD_STATE_CHANNEL0:
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
						&board_fsm.in_data->temperature1,
						&board_fsm.in_data->pressure1_raw,
						&board_fsm.in_data->temperature1_raw);
                    skip_state = 0;
                }
                break;


            case BOARD_STATE_CHANNEL1:
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
						&board_fsm.in_data->temperature2,
						&board_fsm.in_data->pressure2_raw,
						&board_fsm.in_data->temperature2_raw);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_FLOW1:
                if((board_fsm.in_data->read_mask & BOARD_FLOW_1) != 0)
                {
                    ret_code = board_read_flow(&board_dev.flow_sensor1,
                                               &board_fsm.in_data->flow1,
					       &board_fsm.in_data->flow1_raw);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_CHANNEL2:
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
						&board_fsm.in_data->temperature3,
						&board_fsm.in_data->pressure3_raw,
                                                &board_fsm.in_data->temperature3_raw);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_FLOW2:
                if((board_fsm.in_data->read_mask & BOARD_FLOW_2) != 0)
                {
                    ret_code = board_read_flow(&board_dev.flow_sensor2,
                                               &board_fsm.in_data->flow2,
					       &board_fsm.in_data->flow2_raw);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_CHANNEL3:
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_4 | BOARD_TEMPERATURE_4)) != 0)
                {
                    ret_code = i2c_mux_select(&board_dev.i2c_mux, 3, board_fsm_update);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_PRESSURE4:
                // Trigger sensor 3 reading
                if((board_fsm.in_data->read_mask & (BOARD_PRESSURE_4 | BOARD_TEMPERATURE_4)) != 0)
                {
                    ret_code = board_read_pressure(&board_dev.pressure_sensor4,
                                                   &board_fsm.in_data->pressure4,
                                                   &board_fsm.in_data->temperature4,
                                                   &board_fsm.in_data->pressure4_raw,
                                                   &board_fsm.in_data->temperature4_raw);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_ANALOG:
                if((board_fsm.in_data->read_mask & (BOARD_ANALOG | BOARD_O2)) != 0)
                {
                    ret_code = board_read_analog(&board_dev.analog,
                                                 &board_fsm.in_data->o2,
                                                 board_fsm.in_data->analog_input);

                    // There's nothing to wait here so keep skipping
                    skip_state = 1;
                }
                break;

            case BOARD_STATE_GPIO_EXPANDER:
                if((board_fsm.in_data->read_mask & BOARD_GPIO) != 0)
                {
                    ret_code = board_read_gpio_expander(&board_dev.gpio_expander,
                                                        &board_fsm.gpio_value);
                    skip_state = 0;
                }
                break;

            case BOARD_STATE_ENCODER:
                if((board_fsm.in_data->read_mask & BOARD_ENCODER) != 0)
                {
                    // Read the encoder interface
                    ret_code = board_read_encoder(&board_dev.encoder,
                                                  &board_fsm.in_data->encoder,
                                                  &board_fsm.in_data->buttons);

                    // There's nothing to wait here so keep skipping
                    skip_state = 1;
                }
                break;

            case BOARD_STATE_BUTTONS:
                // Read the buttons always
                ret_code = board_read_buttons(&board_fsm.in_data->buttons);
                // There's nothing to wait here so keep skipping
                skip_state = 1;
                break;

            case BOARD_STATE_GPIO:
                // Read the local GPIO pins always
                ret_code = board_read_gpio(&board_fsm.in_data->gpio);
                // There's nothing to wait here so keep skipping
                skip_state = 1;
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

static int board_init_all(void)
{
    int ret_code;

    // Initialize the I2C mux device handles
    i2c_mux_init(&board_dev.i2c_mux, i2c_xfer_async, I2C_MUX_ADDRESS);

    // Initialize the pressure sensors
    ret_code = board_init_pressure_sensors();
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the flow sensors
    ret_code = board_init_flow_sensors();
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the I2C GPIO expander
    ret_code = board_init_gpio_expander(&board_dev.gpio_expander,
                                        &board_dev.config.gpio_expander);
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the encoder
    encoder_init(&board_dev.encoder,
                 board_sample_encoder);

    // Initialize the analogic acquisitions
    ret_code = board_init_analog();
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the PWM devices
    ret_code = board_init_pwms();
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize buttons
    ret_code = board_init_buttons();
    if(ret_code != RC_OK)
        return ret_code;

    return RC_OK;
}

static int board_init_pressure_sensor(hsc_handle_t* sensor,
                                      device_config_t* config)
{
    int ret_code = RC_OK;
    hsc_sensor_t* hsc_sensor_cfg = 0;

    int result;

    // Get the right configuration parameters
    // related to the installed sensor.
    // If no sensor is selected the function just
    // returns OK while if an unknown sensor identifier
    // is set a UNSUPPORTED_ERROR is raised.
    switch(config->type)
    {
        case PRESSURE_SENSOR_TYPE_NONE:
            break;

        case PRESSURE_SENSOR_TYPE_HSCDANN150PG2A5:
            hsc_sensor_cfg = &HSCDANN150PG2A5;
            break;

        case PRESSURE_SENSOR_TYPE_HSCMAND160MD2A5:
            hsc_sensor_cfg = &HSCMAND160MD2A5;
            break;

        case PRESSURE_SENSOR_TYPE_HSCSANN100PA2A5:
            hsc_sensor_cfg = &HSCSANN100PA2A5;
            break;

        default:
            ret_code = RC_UNSUPPORTED_ERROR;
            break;
    }

    // If a valid sensor is installed initialize it
    if((ret_code == RC_OK) && (hsc_sensor_cfg))
    {
        result = hsc_init(sensor, i2c_xfer_async, config->address);
        if(result != 0)
            return RC_ERROR;

        result = hsc_config_device(sensor, hsc_sensor_cfg);
        if(result != 0)
            return RC_ERROR;
    }

    return ret_code;
}

static int board_init_flow_sensor(zephyr_handle_t* sensor,
                                  device_config_t* config,
                                  int mux_channel)
{
    int ret_code = RC_OK;
    int result;

    if(config->type == FLOW_SENSOR_TYPE_ZEPHYR)
    {
        // Manually route the I2C mux to address the sensor
        result = i2c_mux_select(&board_dev.i2c_mux, mux_channel, i2c_mux_select_completed);
        if(result != 0)
            return RC_ERROR;

        // Wait for the mux to complete
        i2c_mux_select_wait();

        // Initialize the sensor
        result = zephyr_init(sensor, i2c_xfer_async, config->address);
        if(result != 0)
            return RC_ERROR;
    }

    return ret_code;
}

static int board_init_gpio_expander(mcp23017_handle_t* dev,
                                    const device_config_t* config)
{
    int ret_code = RC_OK;
    int result;

    if(config->type == GPIO_EXPANDER_TYPE_MCP23017)
    {
        // Initialize the I2C GPIO expander
        result = mcp23017_init(dev, i2c_xfer_async, config->address);
        if(result != 0)
            return RC_ERROR;

        result = mcp23017_config_device(dev, &gpio_cfg);
        if(result != 0)
            return RC_ERROR;
    }

    return ret_code;
}

static int board_init_pressure_sensors(void)
{
    int ret_code;

    // Initialize the I2C mux channel mapping
    board_dev.pressure_sensor1.i2c_mux = &board_dev.i2c_mux;
    board_dev.pressure_sensor2.i2c_mux = &board_dev.i2c_mux;
    board_dev.pressure_sensor3.i2c_mux = &board_dev.i2c_mux;
    board_dev.pressure_sensor4.i2c_mux = &board_dev.i2c_mux;

    board_dev.pressure_sensor1.i2c_channel = PRESSURE_SENSOR_1_CHANNEL;
    board_dev.pressure_sensor2.i2c_channel = PRESSURE_SENSOR_2_CHANNEL;
    board_dev.pressure_sensor3.i2c_channel = PRESSURE_SENSOR_3_CHANNEL;
    board_dev.pressure_sensor4.i2c_channel = PRESSURE_SENSOR_4_CHANNEL;

    // Initialize the pressure sensor 1
    ret_code = board_init_pressure_sensor(&board_dev.pressure_sensor1.sensor, &board_dev.config.pressure_sensor1);
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the pressure sensor 2
    ret_code = board_init_pressure_sensor(&board_dev.pressure_sensor2.sensor, &board_dev.config.pressure_sensor2);
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the pressure sensor 3
    ret_code = board_init_pressure_sensor(&board_dev.pressure_sensor3.sensor, &board_dev.config.pressure_sensor3);
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize the pressure sensor 4
    ret_code = board_init_pressure_sensor(&board_dev.pressure_sensor4.sensor, &board_dev.config.pressure_sensor4);
    if(ret_code != RC_OK)
        return ret_code;

    // All done, no error
    return RC_OK;
}

static int board_init_flow_sensors(void)
{
    int ret_code;

    // Initialize flow sensor 1
    ret_code = board_init_flow_sensor(&board_dev.flow_sensor1,
                                      &board_dev.config.flow_sensor1,
                                      FLOW_SENSOR_1_CHANNEL);
    if(ret_code != RC_OK)
        return ret_code;

    // Initialize flow sensor 2
    ret_code = board_init_flow_sensor(&board_dev.flow_sensor2,
                                      &board_dev.config.flow_sensor2,
                                      FLOW_SENSOR_2_CHANNEL);
    if(ret_code != RC_OK)
        return ret_code;

    // All done, no error
    return RC_OK;
}

static int board_init_pwms(void)
{
    // Initialize the PWM drivers
    if((pwm_init(&board_dev.pwm_valve1, (void*)&htim1, &pwm1_cfg) != 0) ||
       (pwm_init(&board_dev.pwm_valve2, (void*)&htim8, &pwm2_cfg) != 0))
    {
        return RC_PWM_ERROR;
    }

    // Set to zero the PWM output voltage
    if((pwm_set(&board_dev.pwm_valve1, 0) != 0) ||
       (pwm_set(&board_dev.pwm_valve2, 0) != 0))
    {
        return RC_PWM_ERROR;
    }

    // Start the PWM channels
    if((pwm_start(&board_dev.pwm_valve1) != 0) ||
       (pwm_start(&board_dev.pwm_valve2) != 0))
    {
        return RC_PWM_ERROR;
    }

    return RC_OK;
}

static int board_init_analog(void)
{
    int result;

    result = analog_init(&board_dev.analog, &hadc3, DMA2_Stream0_IRQn);
    if(result != 0)
        return RC_ERROR;

    result = analog_config(&board_dev.analog, 0x1F);
    if(result != 0)
        return RC_ERROR;

    return RC_OK;
}

static void board_sample_encoder(bool *a, bool *b)
{
		*a = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15));
		*b = (GPIO_PIN_SET == HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2));
}

static int board_sample_encoder_button(void)
{
    return (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_1));
}

static int board_sample_button_1(void)
{
    return GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_0);
}

static int board_sample_button_2(void)
{
    return GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_7);
}

static int board_sample_button_3(void)
{
    return GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
}

static int board_sample_button_4(void)
{
    return GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_6);
}

static int board_sample_button_5(void)
{
    return GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3);
}

static int board_init_buttons(void)
{
    int i;
    int(*cbs[])(void) = {
        board_sample_button_1,
        board_sample_button_2,
        board_sample_button_3,
        board_sample_button_4,
        board_sample_button_5,
        board_sample_encoder_button
    };

	for(i = 0; i < BOARD_BUTTON_NUMBER; i++)
    {
		button_init(&board_dev.buttons[i], cbs[i]);
    }

    return RC_OK;
}

static void board_buzzer_ret_code(int ret_code)
{
    // Check the return code and play the buzzer
    if(ret_code == RC_OK)
    {
        buzzer_on();
        HAL_Delay(200);
        buzzer_off();
    }
    else
    {
        for(int i = 0; i < 3; ++i)
        {
            buzzer_on();
            HAL_Delay(500);
            buzzer_off();
            HAL_Delay(500);
        }
    }
}

static int board_read_check(board_sensor_data_t* in_data)
{
    // Check the data pointer
    if(!in_data)
        return RC_ERROR;

    uint32_t read_mask = in_data->read_mask;

    // Pressure sensor 1
    if(((read_mask & BOARD_PRESSURE_1) != 0) &&
       (board_dev.config.pressure_sensor1.type == PRESSURE_SENSOR_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    // Pressure sensor 2
    if(((read_mask & BOARD_PRESSURE_2) != 0) &&
       (board_dev.config.pressure_sensor2.type == PRESSURE_SENSOR_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    // Pressure sensor 3
    if(((read_mask & BOARD_PRESSURE_3) != 0) &&
       (board_dev.config.pressure_sensor3.type == PRESSURE_SENSOR_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    // Pressure sensor 4
    if(((read_mask & BOARD_PRESSURE_4) != 0) &&
       (board_dev.config.pressure_sensor4.type == PRESSURE_SENSOR_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    // Flow sensor 1
    if(((read_mask & BOARD_FLOW_1) != 0) &&
       (board_dev.config.flow_sensor1.type == FLOW_SENSOR_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    // Flow sensor 2
    if(((read_mask & BOARD_FLOW_2) != 0) &&
       (board_dev.config.flow_sensor2.type == FLOW_SENSOR_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    // GPIO expander
    if(((read_mask & BOARD_GPIO) != 0) &&
       (board_dev.config.gpio_expander.type == GPIO_EXPANDER_TYPE_NONE))
    {
        return RC_NOT_INSTALLED_ERROR;
    }

    return RC_OK;
}

/****************************************************************/
/* Exported functions definitions.                              */
/****************************************************************/
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

void encoder_changed()
{
	encoder_decode(&board_dev.encoder);
}

void button_changed(int n)
{
	button_notify(&board_dev.buttons[n-1]);
}
