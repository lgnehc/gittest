#include "common.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "driver/ledc.h"

/* ===================================================== */
void gpio_out_init(gpio_num_t gpio_num, uint32_t default_status)
{
    gpio_config_t gpio_out_config = {
        .pin_bit_mask = (1ULL << gpio_num),         // 引脚
        .mode = GPIO_MODE_OUTPUT,                   // 推挽输出,不需要上下拉
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE              // 禁止中断
    };
    myprintf("gpio_num = %d, gpio_out_init配置完成 \n\r",gpio_num);
    gpio_config(&gpio_out_config);
    gpio_set_level(gpio_num, 0);
}


/* ===================================================== */
void pwm_init(int gpio_num, ledc_channel_t  channel, 
    ledc_timer_bit_t  duty_resolution, ledc_mode_t  mode, uint32_t freq_hz)
{
        ledc_timer_config_t timer_cfg = {
        .speed_mode = mode,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = freq_hz,
        .duty_resolution = duty_resolution        // 0~1023
    };
    ledc_timer_config(&timer_cfg);

    //pwm配置
    ledc_channel_config_t channel_cfg = {
        .speed_mode = mode,
        .gpio_num = gpio_num,
        .timer_sel = LEDC_TIMER_0,
        .channel = channel,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&channel_cfg);
    ledc_fade_func_install(0);      // 打开硬件渐变功能
}