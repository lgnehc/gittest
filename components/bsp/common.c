#include "common.h"
#include "driver/gpio.h"

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