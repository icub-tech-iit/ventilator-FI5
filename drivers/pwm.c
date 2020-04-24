#include "pwm.h"

#include <stddef.h>

/****************************************************************/
/* Local functions declarations.                                */
/****************************************************************/
static int pwm_write_duty(pwm_handle_t* dev, uint32_t pulse_value);
static int pwm_timer_channel(pwm_handle_t* dev, uint32_t* channel);

/****************************************************************/
/* Exported APIs.                                               */
/****************************************************************/
int pwm_init(pwm_handle_t* dev, void* hal_handle, pwm_config_t* cfg)
{
    // There's no need to initialize the hardware here.
    // That should hav been done by CubeMX before. 
    dev->hal_handle = hal_handle;
    dev->config = cfg;
    return 0;
}

int pwm_start(pwm_handle_t* dev)
{
    int result;
    uint32_t hal_timer_channel;
     
    result = pwm_timer_channel(dev, &hal_timer_channel);
    if(result == 0)
    {
        // HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
        result = HAL_TIM_PWM_Start_IT(dev->hal_handle, hal_timer_channel);
    }

    return result;
}

int pwm_stop(pwm_handle_t* dev)
{
    int result;
    uint32_t hal_timer_channel;
     
    result = pwm_timer_channel(dev, &hal_timer_channel);
    if(result == 0)
    {
        // HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
        result = HAL_TIM_PWM_Stop(dev->hal_handle, hal_timer_channel);
    }

    return result;
}

int pwm_set(pwm_handle_t* dev, uint32_t target_mV)
{
    float duty;
    uint32_t arr_value;
    uint32_t pulse_value;
    
    // Compute the duty cycle as the ratio between the 
    // PWM driver input voltage and the desired voltage.
    duty = (float)target_mV / (float)dev->config->input_voltage;

    // Clamp the duty cycle to 100%
    if(duty > 1.0f)
        duty = 1.0f;

    // Compute the auto-reload timer register value
    // as the number of timer ticks that the PWM period takes
    // to complete.
    arr_value = dev->config->timer_frequency / dev->config->pwm_frequency;
    
    // Compute the pulse duration in timer ticks
    pulse_value = (uint32_t)(arr_value * duty);

    return pwm_write_duty(dev, pulse_value);
}


/****************************************************************/
/* Local functions definitions.                                 */
/****************************************************************/
static int pwm_write_duty(pwm_handle_t* dev, uint32_t pulse_value)
{
    int ret_code = 1;
    uint32_t* CCRx_ptr = 0;

    // Select the CCR register related to the timer output channel
    switch(dev->config->channel)
    {
        case 1:
            CCRx_ptr = &(((TIM_HandleTypeDef*)dev->hal_handle)->Instance->CCR1);
            break;

        case 2:
            CCRx_ptr = &(((TIM_HandleTypeDef*)dev->hal_handle)->Instance->CCR2);
            break;

        case 3:
            CCRx_ptr = &(((TIM_HandleTypeDef*)dev->hal_handle)->Instance->CCR3);
            break;

        case 4:
            CCRx_ptr = &(((TIM_HandleTypeDef*)dev->hal_handle)->Instance->CCR4);
            break;

        case 5:
            CCRx_ptr = &(((TIM_HandleTypeDef*)dev->hal_handle)->Instance->CCR5);
            break;

        case 6:
            CCRx_ptr = &(((TIM_HandleTypeDef*)dev->hal_handle)->Instance->CCR6);
            break;
        
        default:
            break;
    }

    // If a valid channel was configured write the new duty value 
    // into the CCRx register of the timer peripheral
    if(CCRx_ptr)
    {
        *CCRx_ptr = pulse_value;
        ret_code = 0;
    }

    return ret_code;
}

static int pwm_timer_channel(pwm_handle_t* dev, uint32_t* channel)
{
    int result;

    uint32_t channelIds[] = {
        TIM_CHANNEL_1,
        TIM_CHANNEL_2,
        TIM_CHANNEL_3,
        TIM_CHANNEL_4,
        TIM_CHANNEL_5,
        TIM_CHANNEL_6
    };

    if(dev->config->channel <= sizeof(channelIds) / sizeof(channelIds[0]))
    {
        *channel = channelIds[ dev->config->channel - 1 ];
        result = 0;
    }
    else
    {
        result = 1;
    }

    return result;
}
