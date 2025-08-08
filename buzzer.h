#pragma once

void beep_on(int freq);
void beep_off();
void beep_dot();
void beep_dash();
void beep_negative_feedback();
void play_song();
void beep_init();
void beep_deinit();
unsigned int pwm_set_freq_duty(unsigned int slice, unsigned int channel, unsigned int frequency, double duty);