# What CMake to start at
cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

# Include the subsidiary .cmake file to get the SDK
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)
include($ENV{PICO_SDK_PATH}/pico_sdk_version.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(Temperature C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Link the Project to a source file (step 4.6)
add_executable(Temperature)

target_sources(Temperature PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/temperature.c
        )

# Example source

# Link the Project to an extra library (pico_stdlib)
target_link_libraries(Temperature
    pico_stdlib
    pico_cyw43_arch_none # enable picoW device
    hardware_adc # enable adc sensor
    hardware_i2c
)

# Initalise the SDK
pico_sdk_init()

pico_set_program_name(Temperature Temperature)
pico_set_program_version(Temperature 1.0)

# enable uart/usb output, disable usb output
pico_enable_stdio_usb(Temperature 1) # add usb serial
pico_enable_stdio_uart(Temperature 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(Temperature)