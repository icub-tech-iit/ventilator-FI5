#include "i2c_mux.h"

#include <errno.h>

#if defined(__ARMCOMPILER_VERSION)

    #if !defined(EPERM)
    #define EPERM 1
    #endif
      
    #if !defined(EINTR)
    #define EINTR 4
    #endif
      
    #if !defined(EBUSY)
    #define EBUSY 16
    #endif  
     
#endif

//#define CONFIG_READBACK

enum
{
    I2C_MUX_IDLE = 0,
    I2C_MUX_WRITE,
    I2C_MUX_CHECK
};

static int i2c_mux_config_write(i2c_mux_handle_t* dev, uint8_t config);
static void i2c_mux_callback(int status, void* args);

#ifdef CONFIG_READBACK
    static int i2c_mux_config_read(i2c_mux_handle_t* dev, uint8_t* config);
#endif // CONFIG_READBACK

int i2c_mux_init(i2c_mux_handle_t* dev, 
                i2c_xfer_fnct_t* xfer,
                int address)
{
    dev->i2c_xfer = xfer;
    dev->address  = address;
    dev->config   = 0x0;

    dev->state = I2C_MUX_IDLE;
    dev->completed_callback = 0x0;
    dev->required_config = 0x0;

    return 0;
}

int i2c_mux_channel_select(i2c_mux_handle_t* dev, 
                          int channel,
                          i2c_mux_callback_t callback)
{
    int result;
    uint8_t channel_mask;

    // The device handles up to 8 I2C channels
    if(channel >= 8)
    {
        return -EPERM;
    }

    // Check if any other transation is in progress
    if(dev->state != I2C_MUX_IDLE)
    {
        return -EBUSY;
    }

    // Start the device transaction setting its state
    // into the write state 
    dev->state = I2C_MUX_WRITE;

    // Compute the config register
    // Only the selected channel is activated.
    channel_mask = 1 << channel;

    // Store the transaction data
    dev->required_config = channel_mask;
    dev->completed_callback = callback;

    // Write the MUX control register
    result = i2c_mux_config_write(dev, channel_mask); 

    return result;
}

int i2c_mux_reset(i2c_mux_handle_t* dev)
{
    // Need to move the reset GPIO if connected.
    return 0;
}

static int i2c_mux_config_write(i2c_mux_handle_t* dev, uint8_t config)
{
    i2c_xfer_list_t xfer_list;

    // Setup the I2C transfer
    dev->xfer.direction = WRITE;
    dev->xfer.len = 1;

    // Use bounce buffer for async write
    dev->xfer.buf = dev->xfer_buf;
    dev->xfer_buf[0] = config;

    // I2C MUX switch command needs a single transfer
    xfer_list.xfer_num = 1;
    xfer_list.xfers    = &dev->xfer;

    return dev->i2c_xfer(&xfer_list, dev->address,
        i2c_mux_callback, (void*)dev);
}

static void i2c_mux_callback(int status, void* args)
{
    i2c_mux_handle_t* dev = (i2c_mux_handle_t*) args;
    int completed = 0;

    if(status == 0)
    {
        // Mux config register write completed
        if(dev->state == I2C_MUX_WRITE)
        {
#ifndef CONFIG_READBACK
            dev->config = dev->required_config;
            completed = 1;
#else
            dev->state = I2C_MUX_CHECK;
            status = i2c_mux_config_read(dev, &dev->config);
#endif // CONFIG_READBACK
        }

#ifdef CONFIG_READBACK
        // Mux config register readback completed
        else if(dev->state == I2C_MUX_CHECK)
        {
            if(dev->config != dev->required_config)
                status = -EIO;

            completed = 1;
        }
#endif // CONFIG_READBACK

        // Unattended interrupt
        else
        {
            status = -EINTR;
        }
    }

    // If the transaction is completed or there's any error
    // call the callback with the result code.
    if((completed) || (status != 0))
    {
        dev->completed_callback(status);
        dev->state = I2C_MUX_IDLE;
    }
}

#ifdef CONFIG_READBACK
    static int i2c_mux_config_read(i2c_mux_handle_t* dev, uint8_t* config)
    {
        i2c_xfer_list_t xfer_list;

        // Setup the I2C transfer
        dev->xfer.direction = READ;
        dev->xfer.buf       = (char*)config;

        // I2C MUX switch command needs a single transfer
        xfer_list.xfer_num = 1;
        xfer_list.xfers    = &dev->xfer;

        return dev->i2c_xfer(&xfer_list, dev->address, i2c_mux_callback, (void*)dev);
    }
#endif // CONFIG_READBACK
