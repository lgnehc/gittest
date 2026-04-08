#include <stdint.h>
#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "common.h"

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
void app_main(void)
{    
    xTaskCreatePinnedToCore( task_helloworld, "task_helloworld", Task_Context_Base_Size*2, NULL, 4, NULL, 1);

    vTaskDelete(NULL);
}