#include <stdio.h>
#include <stdbool.h>
#include "drivers/board_driver.h"
#include "drivers/access_once.h"
#include "demo.h"

volatile int cb_status, ready_flag = 0;

/*
 * MUX0, inBr1: HSCDANN150PG2A5
 * MUX1, inBr2: HSCMAND160MD2A5, flow
 * MUX2, esBr:  HSCMAND160MD2A5, flow
 * MUX3, mask:  HSCMAND160MD2A5
 */

static board_config_t board_config =
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

int demo_init(void)
{
	int ret;

	printf("Cube init done\r\n");
	ret = board_init(&board_config);
	printf("board driver init done, status %d\r\n", ret);

	return ret;
}

void async_cb(int status)
{
	ACCESS_ONCE(ready_flag) = 1;
	ACCESS_ONCE(cb_status) = status;
}

void demo_loop(void)
{
	board_sensor_data_t in_data = {0};
	int ret;
	bool button_state[4];
	bool encoder_button = 0;
	static int encoder_counter = 0;
	static board_actuation_data_t out_data;
	static int i = 0;

	in_data.read_mask =
        BOARD_PRESSURE_1 | BOARD_PRESSURE_2 |
        BOARD_PRESSURE_3 | BOARD_PRESSURE_4 |
		BOARD_FLOW_1 | BOARD_FLOW_2 |
        BOARD_ENCODER | BOARD_ANALOG | BOARD_GPIO;

#ifdef READ_SYNC
	ret = board_read_sensors(&in_data);
	printf("read sensor status %d\r\n", ret);
#else

	ACCESS_ONCE(ready_flag) = 0;
	ret = board_read_sensors_async(&in_data, async_cb);
    if(ret == RC_OK)
    {
	    while (!ACCESS_ONCE(ready_flag));
    }

	printf("read sensor status %d %d\r\n", ret, ACCESS_ONCE(cb_status));
#endif

	printf("pressure (mBar) %d %d %d %d (raw) 0x%x 0x%x 0x%x 0x%x\r\n",
	       in_data.pressure1, in_data.pressure2, in_data.pressure3, in_data.pressure4,
	       in_data.pressure1_raw, in_data.pressure2_raw, in_data.pressure3_raw, in_data.pressure4_raw);
	printf("temperature (C) %d %d %d %d (raw) 0x%x 0x%x 0x%x 0x%x\r\n",
	       in_data.temperature1, in_data.temperature2, in_data.temperature3, in_data.temperature4,
	       in_data.temperature1_raw, in_data.temperature2_raw,
	       in_data.temperature3_raw, in_data.temperature4_raw);

	printf("flow (mSLPM) %d %d (raw) 0x%x 0x%x\r\n",
	       in_data.flow1, in_data.flow2,
	       in_data.flow1_raw, in_data.flow2_raw);
	printf("GPIO 0x%x\r\n", in_data.gpio);

	encoder_counter += in_data.encoder;
	encoder_button = !!(in_data.buttons & BOARD_BUTTON_ENCODER);
    button_state[0] = !!(in_data.buttons & BOARD_BUTTON_1);
    button_state[1] = !!(in_data.buttons & BOARD_BUTTON_2);
    button_state[2] = !!(in_data.buttons & BOARD_BUTTON_3);
    button_state[3] = !!(in_data.buttons & BOARD_BUTTON_4);

	printf("Encoder %d(%d) %d\r\n", encoder_counter, (int)in_data.encoder, (int)encoder_button);

	printf("O2: %u\r\n", in_data.o2);
	printf("ADC: %u %u %u %u\r\n",
	       in_data.analog_input[0],
           in_data.analog_input[1],
           in_data.analog_input[2],
           in_data.analog_input[3]);

	printf("Buttons %d %d %d %d\r\n",
	       button_state[0],
	       button_state[1],
	       button_state[2],
	       button_state[3]);

	out_data.gpio = 1 << (i+8);
	out_data.valve1 = 1000;
	out_data.valve2 = 1500;
	out_data.buzzer = 2000;
	ret = board_apply_actuation(&out_data);
	printf("actuation 0x%x ret %d\r\n\r\n", out_data.gpio, ret);
	if (i == 7) {
		i = 0;
	} else {
		i++;
	}

	HAL_Delay(500);
}

extern UART_HandleTypeDef huart1;
/* printf hooked to dbg usart */
int _write(int file, char *data, int len)
{
   if ((file != 1) && (file != 2))
   {
      return -1;
   }

   // arbitrary timeout 1000
   HAL_StatusTypeDef status =
      HAL_UART_Transmit(&huart1, (uint8_t*)data, len, 1000);

   // return # of bytes written - as best we can tell
   return (status == HAL_OK ? len : 0);
}
