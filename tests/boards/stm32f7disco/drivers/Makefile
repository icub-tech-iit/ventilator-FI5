WARNFLAGS = -Werror=implicit-function-declaration -Werror=implicit-int \
            -Wuninitialized -Winit-self -Wreturn-type -Wsequence-point \
            -Wparentheses -Wcomment -Wno-packed -Wenum-compare -Werror=enum-compare -Wall

CFLAGS = -g -O0

all: hsc_test zephyr_test mcp23017_test

hsc_test:  hsc.c hsc_test.c test_utils.c
	gcc $(CFLAGS) $(WARNFLAGS) hsc.c hsc_test.c test_utils.c -o hsc_test

zephyr_test:  zephyr.c zephyr_test.c test_utils.c delay_mock.c
	gcc $(CFLAGS) $(WARNFLAGS) -DDEBUG_HAL delay_mock.c zephyr.c zephyr_test.c test_utils.c -o zephyr_test

mcp23017_test: mcp23017.c mcp23017_test.c
	gcc $(CFLAGS) $(WARNFLAGS) mcp23017.c mcp23017_test.c test_utils.c -o mcp23017_test

#encoder_test: encoder.c encoder_test.c
#	gcc $(CFLAGS) $(WARNFLAGS) encoder.c encoder_test.c test_utils.c -o encoder_test

directories:
	mkdir -p build

board_sim: directories
	gcc -O0 -g -DDEBUG_HAL -c hsc.c -o build/hsc.o
	gcc -O0 -g -DDEBUG_HAL -c i2c_mux.c -o build/i2c_mux.o
	gcc -O0 -g -DDEBUG_HAL -c pwm.c -o build/pwm.o
	gcc -O0 -g -DDEBUG_HAL -c zephyr.c -o build/zephyr.o
	gcc -O0 -g -DDEBUG_HAL -c mcp23017.c -o build/mcp23017.o
	gcc -O0 -g -DDEBUG_HAL -c encoder.c -o build/encoder.o
	gcc -O0 -g -DDEBUG_HAL -c button.c -o build/button.o
	gcc -O0 -g -DDEBUG_HAL -c analog.c -o build/analog.o
	gcc -O0 -g -DDEBUG_HAL -c buzzer.c -o build/buzzer.o
	gcc -O0 -g -DDEBUG_HAL -c board_driver.c -o build/board_driver.o
	gcc -O0 -g -DDEBUG_HAL -c hal.c -o build/hal.o
	gcc -O0 -g -DDEBUG_HAL -c delay_mock.c -o build/delay_mock.o
	gcc -O0 -g -DDEBUG_HAL -c board_sim.c -o build/board_sim.o
	gcc -O0 -g -DDEBUG_HAL -c board_test.c -o build/board_test.o
	gcc build/hsc.o build/i2c_mux.o build/pwm.o build/zephyr.o build/mcp23017.o \
		build/encoder.o build/button.o build/analog.o build/buzzer.o \
		build/board_driver.o build/hal.o \
		build/delay_mock.o build/board_sim.o \
		build/board_test.o -o board_sim

clean:
	rm -f board_test hsc_test zephyr_test mcp23017_test encoder_test
	rm -rf build
	rm -f board_sim
