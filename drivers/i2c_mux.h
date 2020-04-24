#ifndef __I2C_MUX_H__
#define __I2C_MUX_H__

#include <stdint.h>

#include "i2c_api.h"

typedef int (i2c_xfer_fnct_t)(i2c_xfer_list_t *xfers, int addr, i2c_xfer_cb_t cb, void* args);
typedef void (*i2c_mux_callback_t)(int result);

typedef struct
{
    i2c_xfer_fnct_t* i2c_xfer;
    int address;
    uint8_t config;

    int state;
    i2c_mux_callback_t completed_callback;
    uint8_t required_config;
    i2c_xfer_t xfer;
    uint8_t xfer_buf[1];
} i2c_mux_handle_t;

int i2c_mux_init(i2c_mux_handle_t* dev, 
             i2c_xfer_fnct_t* xfer,
             int address);

int i2c_mux_channel_select(i2c_mux_handle_t* dev, int channel, i2c_mux_callback_t callback);
int i2c_mux_reset(i2c_mux_handle_t* dev);

#endif // __I2C_MUX_H__