#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "common.h"
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


void task_led(void *params)
{
    gpio_num_t gpio_num = GPIO_NUM_1;
    gpio_out_init(gpio_num, 0);
    while (1) {
        gpio_set_level(GPIO_NUM_1, 0); // 点亮
        myprintf("LED ON \n\r");
        vTaskDelay(pdMS_TO_TICKS(5000));

        gpio_set_level(GPIO_NUM_1, 1); // 熄灭
        myprintf("LED OFF \n\r");
        vTaskDelay(pdMS_TO_TICKS(2000));
        myprintf("任务剩余栈大小: %u\n", uxTaskGetStackHighWaterMark(NULL));
    }
    vTaskDelete(NULL);
}


/*===========================================================================================*/
void app_main(void)
{    
    xTaskCreatePinnedToCore( task_helloworld, "task_helloworld", Task_Context_Base_Size*2, NULL, 4, NULL, 1);
    xTaskCreatePinnedToCore( task_led, "task_led", Task_Context_Base_Size*2, NULL, 4, NULL, 1);

    vTaskDelete(NULL);
}