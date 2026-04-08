#pragma once
#include <stdint.h>
#include "soc/gpio_num.h"

#define myprintf(fmt, ...) \
    printf("[%s | %s | %d ] : " fmt "", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define Task_Context_Base_Size 1024

