#pragma once
#include <stdint.h>
#include "soc/gpio_num.h"
#include "driver/ledc.h"


#define myprintf(fmt, ...) \
    printf("[%s | %s | %d ] : " fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define Task_Context_Base_Size 1024

void gpio_out_init(gpio_num_t gpio_num, uint32_t default_status);
void pwm_init(int gpio_num, ledc_channel_t  channel, 
    ledc_timer_bit_t  duty_resolution, ledc_mode_t  mode, uint32_t freq_hz);