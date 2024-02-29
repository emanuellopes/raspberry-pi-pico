#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main()
{
    stdio_init_all();
    if (cyw43_arch_init())
    {
        printf("cyw43_init_error\n");
        return 0;
    }

    return 0;
}