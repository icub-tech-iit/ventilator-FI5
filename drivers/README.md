Low level drivers
=================

Honeywell HSC I2C driver
------------------------

This driver reads pressure and temperature from a honeywell pressure sensor. It is designed to be agnostic wrt the I2C host controller, board and so on.

It supports HSCDANN150PG2A5 and HSCMAND160MD2A5 sensors, but it can be easily extended to all HSC-family I2C sensor just by defining proper configuration structures.

It is composted by `hsc.c` and `hsc.h` files.

API:

Alle the API function takes an opaque handle as first parameter. It's allocated by the caller.

`int hsc_init(hsc_handle_t *h, int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr), int addr)`: initialize the driver SW module; the parameters are the I2C xfer function and the I2C address of the device.

`int hsc_config_device(hsc_handle_t *h, hsc_sensor_t *cfg)`: this is needed to configure the driver for the proper sensor.

Right now the driver has two predefined sensor configurations: `HSCDANN150PG2A5` and `HSCMAND160MD2A5`.

`int hsc_read(hsc_handle_t *h, float *pressure_mbar, float *temperature_c)`: this functions reads the data from the sensor and returns it

The driver also come with a mock that emulates the I2C host bus and an attacched sensor, and it runs some basic tests.

Here an example of output


```
drivers$ ./hsc_test
PASS: generic read
PASS: xfer failure check
PASS: normal stale read
PASS: stale read /w wrong temp data
PASS: normal read
PASS: stale read /w wrong pres data
PASS: pressure lower bound
PASS: pressure higher bound
PASS: unexpected failure
PASS: temperature decode 92.256958
PASS: unexpected failure
PASS: pressure decode -0.997101

all test passed

```

Currently just a buch of very basic tests are included.
