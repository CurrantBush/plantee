#include "soil_moisture.h"

#include "hal/adc_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_check.h"

#include <stdlib.h>

#define SOIL_MOISTURE_RAW_MAX 2700
#define SOIL_MOISTURE_RAW_MIN 1600

static adc_oneshot_unit_handle_t soil_moisture_adc_handle;

int soil_moisture_percent(int raw) {
    if (raw < 0) {
        return -1;
    }

    raw = (raw < SOIL_MOISTURE_RAW_MIN) ? SOIL_MOISTURE_RAW_MIN : raw;
    raw = (raw > SOIL_MOISTURE_RAW_MAX) ? SOIL_MOISTURE_RAW_MAX : raw;

    float f_raw = (float) raw;
    float moisture = (SOIL_MOISTURE_RAW_MAX - f_raw) / (SOIL_MOISTURE_RAW_MAX - SOIL_MOISTURE_RAW_MIN) * 100.0f;

    return (unsigned int) moisture;
}

int soil_moisture_read_raw() {
    int raw_data;

    adc_oneshot_read(soil_moisture_adc_handle, ADC_CHANNEL_6, &raw_data);

    // There is no contact with the sensor
    if (raw_data == 0) {
        return -1;
    }

    return raw_data;
}

void soil_moisture_init() {
    adc_oneshot_unit_init_cfg_t soil_moisture_init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&soil_moisture_init_config, &soil_moisture_adc_handle));

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };

    ESP_ERROR_CHECK(adc_oneshot_config_channel(soil_moisture_adc_handle, ADC_CHANNEL_6, &config));

    atexit(soil_moisture_deinit);
}

void soil_moisture_deinit() {
    adc_oneshot_del_unit(soil_moisture_adc_handle);
}
