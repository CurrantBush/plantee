#pragma once

void soil_moisture_init();

int soil_moisture_read_raw();

int soil_moisture_percent(int raw);

void soil_moisture_deinit();