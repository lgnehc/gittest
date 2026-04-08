#include <stdio.h>
#include "pwm.h"
#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"


static bool timer_init_flag[4] = {false};
static bool fade_func_installed = false;
static SemaphoreHandle_t pwm_mutex = NULL;



/* 这个esp32有多个定时器,每个定时器有多个channel,同个定时器的这个freq_hz和分辨率 */
esp_err_t pwm_init(PWM_Config *pwm_cfg)
{
    if (pwm_mutex == NULL) {
        pwm_mutex = xSemaphoreCreateMutex();
    }
    xSemaphoreTake(pwm_mutex, portMAX_DELAY);

    // 定时器配置
    if (!timer_init_flag[pwm_cfg->timer_num]) {
        ledc_timer_config_t timer_cfg = {
            .speed_mode = pwm_cfg->speed_mode,
            .timer_num = pwm_cfg->timer_num,
            .freq_hz = pwm_cfg->freq_hz,
            .duty_resolution = pwm_cfg->duty_resolution,
            .clk_cfg = LEDC_AUTO_CLK,
        };
        esp_err_t ret = ledc_timer_config(&timer_cfg);
        if (ret != ESP_OK) {
            xSemaphoreGive(pwm_mutex);
            return ret;
        }
        timer_init_flag[pwm_cfg->timer_num] = true;
    }

    // 通道配置
    ledc_channel_config_t channel_cfg = {
        .speed_mode = pwm_cfg->speed_mode,
        .gpio_num = pwm_cfg->gpio_num,
        .channel = pwm_cfg->channel,
        .timer_sel = pwm_cfg->timer_num,
        .duty = 0,
        .hpoint = 0,
    };
    esp_err_t ret = ledc_channel_config(&channel_cfg);
    if (ret != ESP_OK) {
        xSemaphoreGive(pwm_mutex);
        return ret;
    }

    // 安装渐变功能
    if (!fade_func_installed) {
        ledc_fade_func_install(0);
        fade_func_installed = true;
    }

    xSemaphoreGive(pwm_mutex);
    return ESP_OK;
}

// 设置固定pwm
void set_pwm_duty(struct PWM_Config *pwm_cfg, uint8_t duty_percent)
{   
    xSemaphoreTake(pwm_mutex, portMAX_DELAY);

    if (duty_percent > 100) {
        duty_percent = 100;
    }
    // 计算最大占空比和目标占空比
    uint32_t max_duty = (1UL << pwm_cfg->duty_resolution) - 1;
    uint32_t target_duty = (max_duty * duty_percent) / 100;

    // 设置并更新占空比
    ledc_set_duty(pwm_cfg->speed_mode, pwm_cfg->channel, target_duty);
    ledc_update_duty(pwm_cfg->speed_mode, pwm_cfg->channel);

    xSemaphoreGive(pwm_mutex);
}



void pwm_set_fade(PWM_Config *pwm_cfg, uint8_t target_duty_percent, uint32_t time_ms)
{
    xSemaphoreTake(pwm_mutex, portMAX_DELAY);

    // 限制百分比范围 (0~100)
    if (target_duty_percent > 100) {
        target_duty_percent = 100;
    }

    // 计算最大占空比和目标占空比
    uint32_t max_duty = (1UL << pwm_cfg->duty_resolution) - 1;
    uint32_t target_duty = (max_duty * target_duty_percent) / 100;

    // 设置渐变并启动
    ledc_set_fade_with_time(pwm_cfg->speed_mode, pwm_cfg->channel, target_duty, time_ms);
    ledc_fade_start(pwm_cfg->speed_mode, pwm_cfg->channel, LEDC_FADE_NO_WAIT);

    xSemaphoreGive(pwm_mutex);
}