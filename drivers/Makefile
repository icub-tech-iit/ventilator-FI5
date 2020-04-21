WARNFLAGS = -Werror=implicit-function-declaration -Werror=implicit-int \
            -Wuninitialized -Winit-self -Wreturn-type -Wsequence-point \
            -Wparentheses -Wcomment -Wno-packed -Wenum-compare -Werror=enum-compare

CFLAGS = -g -O0

all: board_test hsc_test zephyr_test mcp23017_test

board_test: hsc.c zephyr.c board_driver.c board_test.c delay_mock.c i2c_mux.c pwm.c board_sim.c hal.c
	gcc $(CFLAGS) delay_mock.c hsc.c zephyr.c i2c_mux.c pwm.c board_driver.c board_sim.c hal.c board_test.c -o board_test

hsc_test:  hsc.c hsc_test.c test_utils.c
	gcc $(CFLAGS) $(WARNFLAGS) hsc.c hsc_test.c test_utils.c -o hsc_test

zephyr_test:  zephyr.c zephyr_test.c test_utils.c delay_mock.c
	gcc $(CFLAGS) $(WARNFLAGS) delay_mock.c zephyr.c zephyr_test.c test_utils.c -o zephyr_test

mcp23017_test: mcp23017.c mcp23017_test.c
	gcc $(CFLAGS) $(WARNFLAGS) mcp23017.c mcp23017_test.c test_utils.c -o mcp23017_test

directories:
	mkdir -p build

board_sim: directories
	gcc -O0 -g -DDEBUG_HAL -c hsc.c -o build/hsc.o
	gcc -O0 -g -DDEBUG_HAL -c i2c_mux.c -o build/i2c_mux.o
	gcc -O0 -g -DDEBUG_HAL -c pwm.c -o build/pwm.o
	gcc -O0 -g -DDEBUG_HAL -c zephyr.c -o build/zephyr.o
	gcc -O0 -g -DDEBUG_HAL -c board_driver.c -o build/board_driver.o
	gcc -O0 -g -DDEBUG_HAL -c hal.c -o build/hal.o
	gcc -O0 -g -DDEBUG_HAL -c delay_mock.c -o build/delay_mock.o
	gcc -O0 -g -DDEBUG_HAL -c board_sim.c -o build/board_sim.o
	gcc -O0 -g -DDEBUG_HAL -c board_test.c -o build/board_test.o
	gcc build/hsc.o build/i2c_mux.o build/pwm.o build/zephyr.o build/board_driver.o build/hal.o build/delay_mock.o build/board_sim.o build/board_test.o -o board_sim

clean:
	rm -f board_test hsc_test zephyr_test mcp23017_test
	rm -rf build
	rm -f board_sim
