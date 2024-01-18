#include "header/main.h"

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

    initTemperature();
    return 0;
}

void loop() {
    while(true) {
        showTemperature();
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // sleep_ms(2000);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(10000); // 10seconds
    }
}

void showTemperature() {
    float temperature = read_onboard_temperature();
    printf("Onboard temperature = %.01f %c\n", temperature, TEMPERATURE_UNITS);
}

