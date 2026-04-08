#pragma once
#include <stdint.h>
#include "common.h"


void gpio_out_init(gpio_num_t gpio_num, uint32_t default_status);

void gpio_pullup_init(gpio_num_t gpio_num);
void gpio_pulldown_init(gpio_num_t gpio_num);