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

static adc_oneshot_unit_handle_t soil_moisture_adc_handle;

unsigned int calculate_moisture_percents(int raw) {
    float f_raw = raw / 100.f;
    return (27.f - f_raw) / 16.f * 100;
}

int get_raw_soil_moisture() {
    int raw_data;

    adc_oneshot_read(soil_moisture_adc_handle, ADC_CHANNEL_6, &raw_data);

    if (raw_data == 0) {
        return 2700;
    }

    return raw_data;
}

void soil_moisture_sensor_setup() {
    adc_oneshot_unit_init_cfg_t soil_moisture_init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };

    adc_oneshot_new_unit(&soil_moisture_init_config, &soil_moisture_adc_handle);

    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12,
    };

    adc_oneshot_config_channel(soil_moisture_adc_handle, ADC_CHANNEL_6, &config);

    atexit(soil_moisture_sensor_free);
}

void soil_moisture_sensor_free() {
    adc_oneshot_del_unit(soil_moisture_adc_handle);
}
