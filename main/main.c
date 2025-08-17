#include "freertos/FreeRTOS.h"
#include "soil_moisture.h"
#include "water_pump.h"
#include <stdio.h>

#define PUMP_PIN 2

#define SLEEP_TIME_MS 1000L

static void app_setup() {
    soil_moisture_init();
    water_pump_init(PUMP_PIN);
}

void app_main() {
    app_setup();

    TickType_t lastWakeTime = xTaskGetTickCount();
    const TickType_t period = pdMS_TO_TICKS(SLEEP_TIME_MS);

    while (1) {
        int soil_moisture_raw_rate = soil_moisture_read_raw();
        int soil_moisture_rate = soil_moisture_percent(soil_moisture_raw_rate);

        if (soil_moisture_rate < 0) {
            printf("No contact with soil moisture sensor\n");
        }
        else {
            printf("Soil moisture rate: %3u%%\n", soil_moisture_rate);
        }

        water_pump_pump_for(500);

        vTaskDelayUntil(&lastWakeTime, period);
    }
}