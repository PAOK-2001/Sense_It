#ifndef READFROMADC_H
#define READFROMADC_H

#include "hardware/adc.h"

/*
adc_read
Takes pin number and ADC channel and reads value, intended to be used in a loop.
*/
void init_adc(const int PIN, const int ADC_CHANNEL){
    adc_init();
    adc_gpio_init(PIN);
    adc_select_input(ADC_CHANNEL);

}

uint16_t raw_adc(const int CHANNEL){
    // CHANNEL ------ PORT
    //    0            26
    //    1            27
    //    2            28
    //    3            29
    //    4        ONBOARD TEMP
    adc_select_input(CHANNEL);
    return adc_read();

}


#endif 
