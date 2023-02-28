#ifndef PULSEPWM_H
#define PULSEPWM_H

#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"

int slice, channel;

void init_pwm(int PWM_PIN, int PWM_WRAP, int INITIAL_VALUE){
    slice   = pwm_gpio_to_slice_num(PWM_PIN);
    channel = pwm_gpio_to_channel(PWM_PIN);
    gpio_set_function(PWM_PIN, GPIO_FUNC_PWM); // Set the pin 0 to be PWM
    pwm_set_clkdiv(slice, 256.0f);    
    pwm_set_wrap(slice, PWM_WRAP);  // setting the Wrap time to 9764 (20 ms)
    pwm_set_enabled(slice, true);
    pwm_set_chan_level(slice, channel, INITIAL_VALUE);

}

void  pwm_write(uint period){
    pwm_set_chan_level(slice, channel, period);  
    
}


void pwm_sweep(int ITER, int lower_bound, int upper_bound){
    for (uint i = lower_bound; i < upper_bound; i+=ITER)
    {
        pwm_set_chan_level(slice, channel, i);  /// Setting the duty period (0.6 ms)
        sleep_ms(500);
    }
}
#endif 