#include "freertos/FreeRTOS.h"
#include "soil_moisture.h"
#include "pump.h"
#include <stdio.h>

#define PUMP_PIN 2

#define SLEEP_TIME_MS 5000L

static void app_setup() {
    soil_moisture_sensor_setup();
    pump_setup(PUMP_PIN);
}

void app_main() {
    app_setup();

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(SLEEP_TIME_MS);

    while (1) {
        int soil_moisture_raw_rate = get_raw_soil_moisture();
        unsigned int soil_moisture_rate = calculate_moisture_percents(soil_moisture_raw_rate);

        printf("Soil moisture rate: %3u%%\n", soil_moisture_rate);

        pump_for(3000);

        vTaskDelayUntil(&lastWakeTime, period);
    }
}