# What CMake to start at
cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)

# Include the subsidiary .cmake file to get the SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(Temperature C CXX ASM)

# Link the Project to a source file (step 4.6)
add_executable(Temperature source.c)

# Example source

# Link the Project to an extra library (pico_stdlib)
target_link_libraries(Temperature
    pico_stdlib
    pico_cyw43_arch_none # enable picoW device
    hardware_adc # enable adc sensor
)

# Initalise the SDK
pico_sdk_init()

# enable uart/usb output, disable usb output
pico_enable_stdio_usb(Temperature 1) # add usb serial
pico_enable_stdio_uart(Temperature 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(Temperature)