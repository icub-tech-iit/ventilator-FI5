#ifndef __PWM_H__
#define __PWM_H__

#include <stdint.h>

#include "hal.h"

typedef struct 
{
    uint32_t timer_frequency;
    uint32_t pwm_frequency;
    uint32_t channel;
    uint32_t input_voltage;
} pwm_config_t;

typedef struct {
    void* hal_handle;
    pwm_config_t *config;
} pwm_handle_t;

int pwm_init(pwm_handle_t* dev, void* hal_handle, pwm_config_t* cfg);
int pwm_start(pwm_handle_t* dev);
int pwm_stop(pwm_handle_t* dev);
int pwm_set(pwm_handle_t* dev, uint32_t target_mV);


#endif // __PWM_H__