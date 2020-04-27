#include <stdio.h>
#include "drivers/board_driver.h"
#include "demo.h"

int cb_status, ready_flag = 0;
#define ACCESS_ONCE(x) (*((volatile typeof(x) *)&(x)))

int demo_init(void)
{
	int ret;

	printf("Cube init done\n");
	ret = board_init();
	printf("board driver init done, status %d\n", ret);
	return ret;
}

void async_cb(int status)
{
	ACCESS_ONCE(ready_flag) = 1;
	ACCESS_ONCE(cb_status) = status;
}

void demo_loop(void)
{
	board_sensor_data_t in_data;
	int ret;
	static board_actuation_data_t out_data;
	static int i = 0;


	in_data.read_mask = BOARD_PRESSURE_1 | BOARD_PRESSURE_2 |
		BOARD_FLOW_1 | BOARD_GPIO;
#ifdef READ_SYNC
	ret = board_read_sensors(&in_data);
	printf("read sensor status %d\n", ret);
#else

	ACCESS_ONCE(ready_flag) = 0;
	ret = board_read_sensors_async(&in_data, async_cb);
	while (!ACCESS_ONCE(ready_flag));
	printf("read sensor status %d %d\n", ret, ACCESS_ONCE(cb_status));
#endif

	printf("pressure %d %d %d\n",
	       in_data.pressure1, in_data.pressure2, in_data.pressure3);
	printf("temperature %d %d %d\n",
	       in_data.temperature1, in_data.temperature2, in_data.temperature3);

	printf("flow %d %d\n", in_data.flow1, in_data.flow2);
	printf("GPIO 0x%x\n", in_data.gpio);

	out_data.gpio = 1 << (i+8);
	ret = board_apply_actuation(&out_data);
	printf("actuation 0x%x ret %d\n\n", out_data.gpio, ret);
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
