#pragma once
#include "common.h"
#include "driver/ledc.h"


typedef struct PWM_Config {
    ledc_timer_t timer_num;
    gpio_num_t gpio_num;
    ledc_channel_t  channel; 
    ledc_mode_t  speed_mode;
    uint32_t freq_hz;
    ledc_timer_bit_t  duty_resolution;
}PWM_Config;


esp_err_t pwm_init(PWM_Config *pwm_cfg);
void set_pwm_duty(struct PWM_Config *pwm_cfg, uint8_t duty_percent);
void pwm_set_fade(PWM_Config *pwm_cfg, uint8_t target_duty_percent, uint32_t time_ms);