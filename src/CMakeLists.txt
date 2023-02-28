#Add executable
set(EXECUTABLE_NAME "sensor_testing")
set(HEADER_FILES headers/PulsePWM.h headers/ReadFromADC.h)

add_executable (${EXECUTABLE_NAME}
    main.c
    ${HEADER_FILES}
)

# Link required libraries
target_link_libraries(${EXECUTABLE_NAME} pico_stdlib hardware_gpio hardware_i2c hardware_adc hardware_pwm hardware_clocks)

# Enable serial communication
pico_enable_stdio_usb(${EXECUTABLE_NAME} 1)
pico_enable_stdio_uart(${EXECUTABLE_NAME} 1)

# Create map/bin/hex, etc.
pico_add_extra_outputs(${EXECUTABLE_NAME})