#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "soil_moisture.h"
#include <stdio.h>

#define SLEEP_TIME_MS 5000L

static void setup() {
    soil_moisture_sensor_setup();
}

void app_main() {
    setup();

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(SLEEP_TIME_MS);

    while (1) {
        int soil_moisture_raw_rate = get_raw_soil_moisture();
        unsigned int soil_moisture_rate = calculate_moisture_percents(soil_moisture_raw_rate);

        printf("Soil moisture rate: %3u%%\n", soil_moisture_rate);

        vTaskDelayUntil(&lastWakeTime, period);
    }
}