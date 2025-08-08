#pragma once

void potentiometer_init();
unsigned int potentiometer_read_raw();
int map(int value, int original_min, int original_max, int min, int max);
int clamp(int value, int min, int max);
unsigned int potentiometer_read(unsigned int min_limit, unsigned int max_limit);