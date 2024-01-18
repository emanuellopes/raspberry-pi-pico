#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lcd.h"
#include "temperature.h"

int init();
void loop();
void showTemperature();