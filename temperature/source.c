#include "source.h"
#include "hardware/adc.h"

/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Choose 'C' for Celsius or 'F' for Fahrenheit. */
#define TEMPERATURE_UNITS 'C'

int init();
void loop();
float read_onboard_temperature();

int main() {
    init();
    loop();
}

int init() {
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("WiFi init failed");
        return -1;
    }
    adc_init(); // enable adc
    adc_set_temp_sensor_enabled(true); // Activate temp sensor
    adc_select_input(4); // Input 4 is the onboard temperature sensor.
    return 0;
}

void loop() {
    while(true) {
        float temperature = read_onboard_temperature();
        printf("Onboard temperature = %.02f %c\n", temperature, TEMPERATURE_UNITS);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(1000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(1000);
    }
}

float read_onboard_temperature() {

    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float)adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    return tempC;
}