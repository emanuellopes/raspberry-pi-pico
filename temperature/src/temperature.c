#include "header/temperature.h"

float read_onboard_temperature() {

    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}

void initTemperature() {
    adc_init(); // enable adc
    adc_set_temp_sensor_enabled(true); // Activate temp sensor
    adc_select_input(4); // Input 4 is the onboard temperature sensor.
}