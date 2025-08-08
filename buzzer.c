#include "buzzer.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define BUZZER_PIN 17 // Define the GPIO pin number for the buzzer
#define F1 44
#define DS6 1245
#define G7 3136

void beep_on(int freq) {
    // Set the frequency and duty cycle for the specified pin to the specified frequency.
    pwm_set_freq_duty(pwm_gpio_to_slice_num(BUZZER_PIN), pwm_gpio_to_channel(BUZZER_PIN), freq, 0.1);
    pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER_PIN), true);
}

void beep_off() {
    pwm_set_enabled(pwm_gpio_to_slice_num(BUZZER_PIN), false);
}

// this function will provide audio feedback for a dot (short beep)
void beep_dot() {
    beep_init();
    beep_on(DS6);
    sleep_ms(100);
    beep_off();
    beep_deinit();
}

// this function provides audio feedback for a dash (long beep)
void beep_dash() {
    beep_init();
    beep_on(F1); 
    sleep_ms(300);
    beep_off();
    beep_deinit();
}

// this function provides negative feedback beep if there is an error
void beep_negative_feedback() {
    beep_init();
    beep_on(DS6);
    sleep_ms(250);
    beep_off();      // Long beep for error
    sleep_ms(100);  // Short pause
    beep_on(DS6);
    sleep_ms(250);      // Long beep again for error
    beep_off();
    beep_deinit();
}

void play_song() {
    beep_init();
    for (int i = 0; i < 3; i++) {
        beep_on(F1);
        sleep_ms(100);
        beep_on(DS6);
        sleep_ms(100);
        beep_on(G7);
        sleep_ms(100);
    }
    beep_off();
    beep_deinit();
}

void beep_init() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
}

void beep_deinit() {
    gpio_deinit(BUZZER_PIN);
}

unsigned int pwm_set_freq_duty(unsigned int slice, unsigned int channel, unsigned int frequency, double duty) {
    // The Raspberry Pi Pico is clocked at 125Mhz.
    unsigned int clock = 125000000;
    unsigned int divider16 = ceil(clock / (frequency * (double) 4096));
    if (divider16 < 16) divider16 = 16;
    unsigned int wrap = (clock * 16 / divider16 / frequency) - 1;
    pwm_set_clkdiv_int_frac(slice, divider16 / 16, divider16 & 0xF);
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, channel, wrap * duty);
    return wrap;
}