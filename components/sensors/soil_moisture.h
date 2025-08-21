#pragma once

#include "driver/gpio.h"

void soil_moisture_init(gpio_num_t pin);

int soil_moisture_read_raw();

int soil_moisture_percent(int raw);

void soil_moisture_deinit();