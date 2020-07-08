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

`int hsc_read(hsc_handle_t *h, float *pressure_bar, float *temperature_c, void (*read_cb)(int status))` this function trigger a data read operation from the sensor and if will invoke the `read_cb()` callback once done.

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

Honeywell Zephyr I2C driver
---------------------------

This driver reads the flux informationd from a honeywell Zephyr sensor. It is designed to be agnostic wrt the I2C host controller, board and so on.

It should support HAFUHT0050L4AXT and similar Honeywell devices.


API:

Alle the API function takes an opaque handle as first parameter. It's allocated by the caller.

`int zephyr_init(zephyr_handle_t *h, int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr), int addr)`: initialize the driver SW module; the parameters are the I2C xfer function and the I2C address of the device.

`int zephyr_get_sn(hsc_handle_t *h, uint32_t *sn)`: this function fills its `sn` parameters with the serial numeber read from the device.

`int zephyr_read(zephyr_handle_t *h, uint16_t *flow, void (*read_cb)(int status))`: this functions triggers a data read operation from the sensor that will end up in fills its `flux` parameter. Once the operation is completed `read_cb()` will be called.

The driver also come with a mock that emulates the I2C host bus and an attacched sensor, and it runs some basic tests.

Here an example of output


```
drivers$ ./zephyr_test
PASS: zephyr init
PASS: get SN operation status
PASS: read SN 0xf0cacc1a
PASS: read operation status
PASS: read data 0x321
PASS: recognize invalid read data
PASS: csum check
PASS: SN check
PASS: Init twice
PASS: xfer failure check when init (0)
PASS: xfer failure check when init (2)
PASS: xfer failure check when init (3)
PASS: xfer failure check when init (5)
PASS: xfer failure check when init (6)
PASS: xfer failure check when init (7)
PASS: xfer failure check when init (8)
PASS: unexpected failure
PASS: xfer failure check when reading (4)

All test passed!

```

Microchip MCP23017 driver
---------------------------

This driver allows for configuring, reading, and writing a MCP23017 GPIO-expander chip.

API:

Alle the API function takes an opaque handle as first parameter. It's allocated by the caller.

`int mcp23017_init(mcp23017_handle_t *h, int(*i2c_xfer)(i2c_xfer_list_t *xfers, int addr), int addr)`: initialize the driver SW module; the parameters are the I2C xfer function and the I2C address of the device.


`int mcp23017_config_device(mcp23017_handle_t *h, mcp23017_cfg_t *cfg)`: configures the chip with the supplied configuration. Pull-ups and pins direction can be choosen (via bitfields).

`int mcp23017_read(mcp23017_handle_t *h, uint16_t *port, void (*read_cb)(int status))`: this function triggers a read from the GPIO chip port; the `port` argument will be filled with the current status of all GPIOs. When the operation is complete the `read_cb()` callback will be invoked.

`int mcp23017_write(mcp23017_handle_t *h, uint16_t port, void (*write_cb)(int status))`: this function triggers a write to the GPIO chip port; the pins of the gpio chip will be set as per the `port` bitfield argument. When the operation is complete the `write_cb()` callback will be invoked. **NOTE:** pins might take time to shift to the required electrical value, even after the callback has been invoked.

The driver also come with a mock that emulates the I2C host bus and an attacched sensor, and it runs some basic tests.

Here an example of output

```
drivers$ ./mcp23017_test
PASS: mcp init
PASS: mcp config 0x0 0x55aa
PASS: mcp config 0xcafe 0xffff
PASS: mcp wrong config 0xcafe 0xffff
PASS: port read 0xbeef. (ret:0 cb_status:0)
PASS: port read 0x7dde. (ret:0 cb_status:0)
PASS: port read 0xfbbc. (ret:0 cb_status:0)
PASS: port read 0xf778. (ret:0 cb_status:0)
PASS: port read 0xeef0. (ret:0 cb_status:0)
PASS: port write 0xa1b2. (ret:0 cb_status:0)
PASS: port write 0x4364. (ret:0 cb_status:0)
PASS: port write 0x86c8. (ret:0 cb_status:0)
PASS: port write 0xd90. (ret:0 cb_status:0)
PASS: port write 0x1b20. (ret:0 cb_status:0)
PASS: port write for read. (ret:0 cb_status:0)
PASS: port rw 0x5678. (ret:0 cb_status:0)

All test passed!

```
