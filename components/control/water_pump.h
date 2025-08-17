#pragma once

#include "driver/gpio.h"

void water_pump_init(gpio_num_t pump_pin);

void water_pump_pump_for(int time_ms);

void water_pump_deinit();