#ifndef _IIT_BOARD_SIM_H_
#define _IIT_BOARD_SIM_H_

#include <stdint.h>

// Forward declarations
struct bus_t;
struct remote_device_t;

enum
{
    BUS_IDLE = 0,
    BUS_READ,
    BUS_WRITE,
    BUS_ERROR
};

struct bus_operation_t
{
    int address;
    void* data;
    int size;
};

struct bus_t
{
    int id;
    struct remote_device_t* dev_list[16];
    int state;
    struct bus_operation_t operation;
    void (*irq_handler)(int status);
};

struct remote_device_ops_t
{
    int (*write)(struct remote_device_t* dev, void* data, int size);
    int (*read)(struct remote_device_t* dev, void* data, int size);
};

struct remote_device_t
{
    struct bus_t* bus;
    int address;
    
    struct remote_device_ops_t ops;
    void* data;
};

struct ventilator_dev
{
    struct bus_t i2c_1;

    struct remote_device_t pressure1;
    struct remote_device_t pressure2;
    struct remote_device_t pressure3;

    struct remote_device_t flow1;
    struct remote_device_t flow2;

    struct remote_device_t mux;
};

int board_sim_init(void);
int board_sim_bus_read(int busId, int address, void* data, int size);
int board_sim_bus_write(int busId, int address, void* data, int size);
int board_sim_bus_irq(int busId);
int board_sim_bus_poll(int busId);
int board_sim_bus_irq_handler(int busId, void (*handler)(int status));

#endif // _IIT_BOARD_SIM_H_