#include "water_pump.h"
#include "freertos/FreeRTOS.h"

#include <stdlib.h>

static int pump_pin = -1;

static TimerHandle_t pump_timer = NULL;

static void water_pump_stop() {
    if (pump_pin != -1) {
        gpio_set_level(pump_pin, 0);
    }
}

void water_pump_init(gpio_num_t pin) {
    if (pump_timer != NULL) {
        xTimerDelete(pump_timer, 0);
    }

    pump_pin = pin;

    gpio_config_t pump_config = {
        .pin_bit_mask = 1ULL << pin,    // ESP32 has up to 34 pins
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&pump_config);

    gpio_set_level(pump_pin, 0);

    pump_timer = xTimerCreate(
        "WaterPumpTimer",
        pdMS_TO_TICKS(1000),
        pdFALSE,
        NULL,
        water_pump_stop
    );
}

void water_pump_pump_for(int time_ms) {
    if (pump_pin < 0) {
        return;
    }

    gpio_set_level(pump_pin, 1);

    xTimerStop(pump_timer, 0);
    xTimerChangePeriod(pump_timer, pdMS_TO_TICKS(time_ms), 0);
    xTimerStart(pump_timer, 0);
}

void water_pump_deinit() {
    gpio_set_level(pump_pin, 0);

    pump_pin = -1;

    if (pump_timer) {
        xTimerDelete(pump_timer, 0);
        pump_timer = NULL;
    }
}