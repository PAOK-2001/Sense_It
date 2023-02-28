#ifndef I2C_MAIN_H
#define I2C_MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/i2c.h"

#define I2C_BAUDRATE 100000

uint8_t rxdata[3];
uint8_t txdata[2];

bool isReserved(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

void setup_i2c(){
    const int I2C_SDA = 4;
    const int I2C_SCL = 5;
    gpio_init(I2C_SDA);
    gpio_init(I2C_SCL);

    i2c_init(i2c0, I2C_BAUDRATE);

    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void i2c_bus_scan(){
    printf("\nI2C Bus Scan\n");
    printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
 
    for (int addr = 0; addr < (1 << 7); ++addr) {
        if (addr % 16 == 0) {
            printf("%02x ", addr);
        }

        int ret;
        uint8_t rxdata;
        if (isReserved(addr))
            ret = PICO_ERROR_GENERIC;
        else
            ret = i2c_read_blocking(i2c0, addr, &rxdata, 1, false);
 
        printf(ret < 0 ? "." : "@");
        printf(addr % 16 == 15 ? "\n" : "  ");
    }
    printf("Done.\n");
}


#endif