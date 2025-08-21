#include "freertos/FreeRTOS.h"
#include "soil_moisture.h"
#include "water_pump.h"
#include "config.h"

#include <stdio.h>

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

        printf("Current moisture rate is: %d%%\n", soil_moisture_rate);

        if (soil_moisture_rate < 0) {
            printf("No contact with soil moisture sensor\n");
        }
        else if (soil_moisture_rate <= MIN_SOIL_MOISTURE) {
            printf("Pump should start now!\n");
            water_pump_pump_for(DEFAULT_PUMP_TIME_MS);
        }

        vTaskDelayUntil(&lastWakeTime, period);
    }
}