#pragma once

void soil_moisture_sensor_setup();

int get_raw_soil_moisture();

unsigned int calculate_moisture_percents(int raw);

void soil_moisture_sensor_free();