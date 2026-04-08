#include <stdint.h>
#include <stdio.h>
#include "common.h"
#include "gpios.h"
#include "pwm.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/gpio_num.h"
#include "driver/gpio.h"

/*===========================================================================================*/
/* hello world测试 */
void task_helloworld(void *params) 
{   
    myprintf("hello world !!!!! \n\r");
    myprintf("hello world !!!!! \n\r");
    myprintf("hello world !!!!! \n\r");
    myprintf("任务剩余栈大小: %u\n", uxTaskGetStackHighWaterMark(NULL));
    vTaskDelete(NULL);      // 这个不是死循环的必须自己结束
}

/*===========================================================================================*/
void task_led(void *params)
{
    gpio_num_t gpio_num = GPIO_NUM_1;
    gpio_out_init(gpio_num, 0);
    while (1) {
        gpio_set_level(GPIO_NUM_1, 0); // 点亮
        myprintf("LED ON \n\r");
        vTaskDelay(pdMS_TO_TICKS(2000));

        gpio_set_level(GPIO_NUM_1, 1); // 熄灭
        myprintf("LED OFF \n\r");
        vTaskDelay(pdMS_TO_TICKS(2000));
        // myprintf("任务剩余栈大小: %u\n", uxTaskGetStackHighWaterMark(NULL));
    }
    vTaskDelete(NULL);
}


void task_blinkled(void *params)
{
    struct PWM_Config pwm_cfg = {
        .timer_num = LEDC_TIMER_0,
        .gpio_num = GPIO_NUM_1,
        .channel = LEDC_CHANNEL_0,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .freq_hz = 10 * 1000,
        .duty_resolution = LEDC_TIMER_10_BIT
    };
    esp_err_t ret = pwm_init(&pwm_cfg);
    if (ret != ESP_OK) {
        printf("PWM 初始化失败！\n");
        return; 
    }
    
    // set_pwm_duty(&pwm_cfg,60);
    while (1) {
        pwm_set_fade(&pwm_cfg, 100, 1000);
        vTaskDelay(pdMS_TO_TICKS(50));
        pwm_set_fade(&pwm_cfg, 0, 2000);

        // myprintf("任务剩余栈大小: %u\n", uxTaskGetStackHighWaterMark(NULL));
    }
    vTaskDelete(NULL);
}


/*===========================================================================================*/
void task_keys(void *params)
{   
    // 初始化 key 引脚
    gpio_num_t key_num = GPIO_NUM_0;
    gpio_pullup_init(key_num);
    
    while (1) {
        // 按键按下值为0
        int key_value = gpio_get_level(key_num);
        myprintf("key_value = %d \n\r",key_value);
        vTaskDelay(pdMS_TO_TICKS(200));         
        // myprintf("任务剩余栈大小: %u\n", uxTaskGetStackHighWaterMark(NULL));
    }
    vTaskDelete(NULL);
}



/*===========================================================================================*/
void app_main(void)
{    
    xTaskCreatePinnedToCore( task_helloworld, "task_helloworld", Task_Context_Base_Size*2, NULL, 4, NULL, 1);
    // xTaskCreatePinnedToCore( task_led, "task_led", Task_Context_Base_Size*2, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore( task_blinkled, "task_blinkled", Task_Context_Base_Size*2, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore( task_keys, "task_keys", Task_Context_Base_Size*2, NULL, 4, NULL, 1);

    vTaskDelete(NULL);
}