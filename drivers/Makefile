all: board_test hsc_test zephyr_test

board_test: hsc.c zephyr.c board_driver.c board_test.c delay_mock.c
	gcc -O0 -g delay_mock.c hsc.c zephyr.c board_driver.c board_test.c -o board_test

hsc_test:  hsc.c hsc_test.c test_utils.c
	gcc -O0 -g hsc.c hsc_test.c test_utils.c -o hsc_test

zephyr_test:  zephyr.c zephyr_test.c test_utils.c delay_mock.c
	gcc -O0 -g delay_mock.c zephyr.c zephyr_test.c test_utils.c -o zephyr_test

clean:
	rm board_test hsc_test zephyr_test
