#include "board_sim.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>

struct pressure_sensor_data_t
{
    float pressure;
    float temperature;
    uint8_t raw_data[4];
};

struct bus_mux_data_t
{
    struct bus_t out_port_list[8];
    uint8_t status;
};

struct flow_sensor_data_t
{
    uint8_t boot[2];
    uint8_t serial[4];
    uint8_t checksum_ack[2];
    uint8_t raw[2];
    uint8_t* data_ptr;
    int state;
};

struct gpio_exp_data_t
{
    uint16_t direction;
    uint16_t pullup;
    uint16_t gpio;
};

static void connect_remote_device(struct bus_t* bus, struct remote_device_t* dev);
static int get_remote_device(struct bus_t* bus, int address, struct remote_device_t** slave);

static int execute_bus_read(struct bus_t* bus);
static int execute_bus_write(struct bus_t* bus);

static struct bus_t* get_mux_line(struct remote_device_t* dev, int line);
static void mux_route(struct remote_device_t* dev);
static void bus_reset(struct bus_t* bus);

static int pressure_write(struct remote_device_t* dev, void* data, int size);
static int pressure_read(struct remote_device_t* dev, void* data, int size);
static int flow_write(struct remote_device_t* dev, void* data, int size);
static int flow_read(struct remote_device_t* dev, void* data, int size);
static int mux_write(struct remote_device_t* dev, void* data, int size);
static int mux_read(struct remote_device_t* dev, void* data, int size);
static int gpio_exp_write(struct remote_device_t* dev, void* data, int size);
static int gpio_exp_read(struct remote_device_t* dev, void* data, int size);

static struct ventilator_dev device;
static struct pressure_sensor_data_t pressure_sensor_data[] = 
{
    {
        .pressure = 1.0f,
        .temperature = 20.0f,
        .raw_data = { 0x10, 0x10, 0x10, 0x10 }
    },
    {
        .pressure = 1.2f,
        .temperature = 30.0f,
        .raw_data = { 0x20, 0x20, 0x20, 0x20 }
    },
    {
        .pressure = 0.8f,
        .temperature = 50.0f,
        .raw_data = { 0x30, 0x30, 0x30, 0x30 }
    },
    {
        .pressure = 0.9f,
        .temperature = 40.0f,
        .raw_data = { 0x25, 0x25, 0x25, 0x25 }
    }
};

static struct bus_mux_data_t mux_data[] = 
{
    {
        .out_port_list = {{0}},
        .status = 0x0
    }
};

static struct flow_sensor_data_t flow_sensor_data[] =
{
    {
        .boot = { 0x00, 0x00 },
        .serial = { 0x11, 0x22, 0x33, 0x44 },
        .checksum_ack = { 0xCC, 0xA5 },
        .raw = { 0x12, 0x34 },
        .data_ptr = flow_sensor_data[0].boot,
        .state = 0
    },
    {
        .boot = { 0x00, 0x00 },
        .serial = { 0xAA, 0x22, 0x33, 0x44 },
        .checksum_ack = { 0xCC, 0xA5 },
        .raw = { 0x16, 0x78 },
        .data_ptr = flow_sensor_data[1].boot,
        .state = 0
    }
};

static struct gpio_exp_data_t gpio_exp_data[] = 
{
    {
        .direction = 0x0,
        .pullup = 0x0,
        .gpio = 0xabcd
    }
};


int board_sim_init(void)
{
    // Set the whole structure to zero
    memset(&device, 0x0, sizeof(struct ventilator_dev));
    memset(&mux_data, 0x0, sizeof(struct bus_mux_data_t));
    
    // I2C 1 initialization
    device.i2c_1.id = 1;
    device.i2c_1.state = BUS_IDLE;
    device.i2c_1.operation.address = 0x0;
    device.i2c_1.operation.data = 0x0;
    device.i2c_1.operation.size = 0x0;
    device.i2c_1.irq_handler = 0;

    // Mux initialization
    device.mux.address = 0x70;
    device.mux.ops.write = mux_write;
    device.mux.ops.read = mux_read;
    device.mux.data = (void*)(&mux_data[0]);

    // Pressure sensor 1
    device.pressure1.address = 0x28;
    device.pressure1.ops.write = pressure_write;
    device.pressure1.ops.read = pressure_read;
    device.pressure1.data = (void*)(&pressure_sensor_data[0]);

    // Pressure sensor 2
    device.pressure2.address = 0x28;
    device.pressure2.ops.write = pressure_write;
    device.pressure2.ops.read = pressure_read;
    device.pressure2.data = (void*)(&pressure_sensor_data[1]);

    // Pressure sensor 3
    device.pressure3.address = 0x28;
    device.pressure3.ops.write = pressure_write;
    device.pressure3.ops.read = pressure_read;
    device.pressure3.data = (void*)(&pressure_sensor_data[2]);

    // Pressure sensor 4
    device.pressure4.address = 0x28;
    device.pressure4.ops.write = pressure_write;
    device.pressure4.ops.read = pressure_read;
    device.pressure4.data = (void*)(&pressure_sensor_data[3]);

    // Flow sensor 1
    device.flow1.address = 0x49;
    device.flow1.ops.write = flow_write;
    device.flow1.ops.read = flow_read;
    device.flow1.data = (void*)(&flow_sensor_data[0]);

    // Flow sensor 2
    device.flow2.address = 0x49;
    device.flow2.ops.write = flow_write;
    device.flow2.ops.read = flow_read;
    device.flow2.data = (void*)(&flow_sensor_data[1]);

    // GPIO expander
    device.gpio.address = 0x20; // Check pins A0, A1 and A2
    device.gpio.ops.write = gpio_exp_write;
    device.gpio.ops.read = gpio_exp_read;
    device.gpio.data = (void*)(&gpio_exp_data[0]);

    // Bus connections
    connect_remote_device(&device.i2c_1, &device.mux);
    connect_remote_device(&device.i2c_1, &device.gpio);

    connect_remote_device(get_mux_line(&device.mux, 0), &device.pressure1);
    connect_remote_device(get_mux_line(&device.mux, 1), &device.pressure2);
    connect_remote_device(get_mux_line(&device.mux, 2), &device.pressure3);
    connect_remote_device(get_mux_line(&device.mux, 3), &device.pressure4);

    connect_remote_device(get_mux_line(&device.mux, 1), &device.flow1);
    connect_remote_device(get_mux_line(&device.mux, 2), &device.flow2);

    return 0;
}

int board_sim_bus_read(int busId, int address, void* data, int size)
{
    // There's only one I2C bus
    if(busId != 1)
        return -EBADF;

    struct bus_t* bus = &device.i2c_1;

    // If another operation is in progress 
    // return the busy error code.
    if(bus->state != BUS_IDLE)
        return -EBUSY;

    // Configure the transaction informations
    bus->state = BUS_READ;
    bus->operation.address = address;
    bus->operation.data = data;
    bus->operation.size = size;

    return 0;
}

int board_sim_bus_write(int busId, int address, void* data, int size)
{
    // There's only one I2C bus
    if(busId != 1)
        return -EBADF;

    struct bus_t* bus = &device.i2c_1;

    // If another operation is in progress 
    // return the busy error code.
    if(bus->state != BUS_IDLE)
        return -EBUSY;

    // Configure the transaction informations
    bus->state = BUS_WRITE;
    bus->operation.address = address;
    bus->operation.data = data;
    bus->operation.size = size;

    return 0;
}

int board_sim_bus_irq(int busId)
{
    // There's only one I2C bus
    if(busId != 1)
        return -EBADF;

    struct bus_t* bus = &device.i2c_1;
    int result;

    if(bus->state == BUS_READ)
    {
        result = execute_bus_read(bus);
    }
    else if(bus->state == BUS_WRITE)
    {
        result = execute_bus_write(bus);
    }
    else
    {
        printf("Spourious IRQ? %d\n", bus->state);
        result = -EIO;
    }

    // Clear the bus state
    bus->state = BUS_IDLE;

    if(bus->irq_handler)
        bus->irq_handler(result);

    return result;
}

int board_sim_bus_poll(int busId)
{
    // There's only one I2C bus
    if(busId != 1)
        return -EBADF;

    struct bus_t* bus = &device.i2c_1;
    int result;

    if(bus->state == BUS_READ)
    {
        result = execute_bus_read(bus);
    }
    else if(bus->state == BUS_WRITE)
    {
        result = execute_bus_write(bus);
    }
    else
    {
        printf("Bus idle %d\n", bus->state);
        result = -EIO;
    }

    // Clear the bus state
    bus->state = BUS_IDLE;

    return result;
}

int board_sim_bus_irq_handler(int busId, void (*handler)(int status))
{
    // There's only one I2C bus
    if(busId != 1)
        return -EBADF;

    struct bus_t* bus = &device.i2c_1;

    bus->irq_handler = handler;

    return 0;
}

// Bus interconnection utility function
static void connect_remote_device(struct bus_t* bus, struct remote_device_t* dev)
{
    int i;
    for(i = 0; i < 16; ++i)
    {
        if(!(bus->dev_list[i]))
        {
            bus->dev_list[i] = dev;
            dev->bus = bus;
            break;
        }
    }
}

static int get_remote_device(struct bus_t* bus, 
                             int address, 
                             struct remote_device_t** slave)
{
    int i;

    // Set the slave to null
    *slave = 0;

    // Loop over all the devices connected to the bus
    // If a devie with the required address is found 
    // store its reference.
    // If more than one device is found return an error. 
    for(i = 0; i < 16; ++i)
    {
        if(bus->dev_list[i])
        {
            if(bus->dev_list[i]->address == address)
            {
                if(!(*slave))
                {
                    *slave = bus->dev_list[i];
                }
                else
                {
                    return -EIO;
                }
            }
        }
    } 

    // Check if any slave was found
    if(!(*slave))
        return -ENODEV;

    return 0;
}

static int execute_bus_read(struct bus_t* bus)
{
    struct remote_device_t* slave = 0;
    int result;

    // Read the transaction details from bus data
    int address = bus->operation.address;
    void* data = bus->operation.data;
    int size = bus->operation.size;

    // Search the slave remote device
    result = get_remote_device(bus, address, &slave);
    if(result)
        return result;

    // Call the slave read operation
    if(slave->ops.read)
        result = slave->ops.read(slave, data, size);
    
    return result;
}

static int execute_bus_write(struct bus_t* bus)
{
    struct remote_device_t* slave = 0;
    int result;

    // Read the transaction details from bus data
    int address = bus->operation.address;
    void* data = bus->operation.data;
    int size = bus->operation.size;

    // Search the slave remote device
    result = get_remote_device(bus, address, &slave);
    if(result)
        return result;

    // Call the slave write operation
    if(slave->ops.write)
        result = slave->ops.write(slave, data, size);
    
    return result;
}

static struct bus_t* get_mux_line(struct remote_device_t* dev, int line)
{
    struct bus_t* bus = 0;
    struct bus_mux_data_t* bus_data;

    if(dev)
    {
        bus_data = (struct bus_mux_data_t*)(dev->data);
        if(bus_data)
        {
            bus = &bus_data->out_port_list[line];
        }
    }

    return bus;
}

static void mux_route(struct remote_device_t* dev)
{
    if(!dev)
        return;

    struct bus_mux_data_t* data = (struct bus_mux_data_t*)dev->data;

    // Reset the bus inter-connections
    bus_reset(dev->bus);

    if(data)
    {
        int i;
        uint8_t mapping = data->status;
        for(i = 0; i < 8; ++i)
        {
            uint8_t connected = mapping & 0x1;
            if(connected)
            {
                int j;
                for(j = 0; j < 16; ++j)
                {
                    struct remote_device_t* slave = data->out_port_list[i].dev_list[j];
                    if(slave)
                        connect_remote_device(dev->bus, slave);
                }
            }

            mapping = mapping >> 1;
        }
    }
}

static void bus_reset(struct bus_t* bus)
{
    if(!bus)
        return;

    // Clear all the connection informations
    memset(bus->dev_list, 0x0, sizeof(bus->dev_list));

    // Add to the bus the directly connected devices
    if(bus == &device.i2c_1)
    {
        connect_remote_device(&device.i2c_1, &device.mux);
        connect_remote_device(&device.i2c_1, &device.gpio);
    }
}

// Sensor simulation functions
static int pressure_write(struct remote_device_t* dev, void* data, int size)
{
    return 0;
}

static int pressure_read(struct remote_device_t* dev, void* data, int size)
{
    if((!dev) || (!data))
        return -EBADF;

    if(size > 4)
        size = 4;

    struct pressure_sensor_data_t* pressure_data = (struct pressure_sensor_data_t*)dev->data;

    if(pressure_data)
    {
        memcpy(data, pressure_data->raw_data, size);
    }
    
    return 0;
}

static int flow_write(struct remote_device_t* dev, void* data, int size)
{
    if(!dev)
        return -EBADF;

    if(size != 1)
        return -EFAULT;

    struct flow_sensor_data_t* sensor_data = (struct flow_sensor_data_t*)(dev->data);
    uint8_t cmd = *((uint8_t*)data);

    if(sensor_data)
    {
        switch(cmd)
        {
            // Read serial number command
            case 0x1:
                sensor_data->state = 1;
                sensor_data->data_ptr = sensor_data->serial;
                break;

            // Reset command
            case 0x2:
                sensor_data->state = 1;
                sensor_data->data_ptr = sensor_data->serial;
                break;

            // Verify checksum command
            case 0x3:
                sensor_data->state = 3;
                sensor_data->data_ptr = sensor_data->checksum_ack;
                break;
        }
    }

    return 0;
}

static int flow_read(struct remote_device_t* dev, void* data, int size)
{
    if(size != 2)
        return -EFAULT;
    
    struct flow_sensor_data_t* sensor_data = (struct flow_sensor_data_t*)(dev->data);

    memcpy(data, sensor_data->data_ptr, size);

    switch(sensor_data->state)
    {
        case 0:
            sensor_data->data_ptr = sensor_data->serial;
            sensor_data->state = 1;
            break;

        case 1:
            sensor_data->data_ptr = &sensor_data->serial[2];
            sensor_data->state = 2;
            break;

        case 2:
            sensor_data->data_ptr = sensor_data->raw;
            sensor_data->state = 4;
            break;

        case 3:
            sensor_data->data_ptr = sensor_data->raw;
            sensor_data->state = 4;
            break;

        default:
            break;
    }
    
    return 0;
}

static int mux_write(struct remote_device_t* dev, void* data, int size)
{
    if(!dev)
        return -EBADF;

    // I2C mux has only one-byte one status register
    // containing the mux lines configuration.
    // No register address must be passed.
    if(size != 1)
        return -EFAULT;

    struct bus_mux_data_t* mux_data = (struct bus_mux_data_t*)dev->data;

    if(mux_data)
    {
        mux_data->status = *((uint8_t*)data);
        mux_route(dev);
    }

    return 0;
}

static int mux_read(struct remote_device_t* dev, void* data, int size)
{
    if(!dev)
        return -EBADF;

    // I2C mux has only one-byte one status register
    // containing the mux lines configuration.
    // No register address must be passed.
    if(size != 1)
        return -EFAULT;

    struct bus_mux_data_t* mux_data = (struct bus_mux_data_t*)dev->data;

    if(mux_data)
    {
        memcpy(data, &mux_data->status, size);
    }
    
    return 0;
}

static int gpio_exp_write(struct remote_device_t* dev, void* data, int size)
{
    return 0;
}

static int gpio_exp_read(struct remote_device_t* dev, void* data, int size)
{
    return 0;
}
