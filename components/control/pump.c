#include "pump.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"

#include <stdlib.h>


#define HIGH_LEVEL  1
#define LOW_LEVEL   0

static int pump_pin = -1;

static TimerHandle_t pump_timer = NULL;

static void pump_stop() {
    if (pump_pin != -1) {
        gpio_set_level(pump_pin, LOW_LEVEL);
    }
}

void pump_setup(int pin) {
    pump_pin = pin;

    gpio_config_t pump_config = {
        .pin_bit_mask = 1ULL << pin,    // ESP32 has up to 34 pins
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&pump_config);

    gpio_set_level(pump_pin, LOW_LEVEL);

    pump_timer = xTimerCreate(
        "PumpTimer",
        pdMS_TO_TICKS(1000),
        pdFALSE,
        NULL,
        pump_stop
    );
}

void pump_for(int time_ms) {
    if (pump_pin < 0) {
        return;
    }

    gpio_set_level(pump_pin, HIGH_LEVEL);

    xTimerStop(pump_timer, 0);
    xTimerChangePeriod(pump_timer, pdMS_TO_TICKS(time_ms), 0);
    xTimerStart(pump_timer, 0);
}

void pump_free() {
    gpio_set_level(pump_pin, LOW_LEVEL);

    pump_pin = -1;

    if (pump_timer) {
        xTimerDelete(pump_timer, 0);
        pump_timer = NULL;
    }
}