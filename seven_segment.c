#include "seven_segment.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Define the segmentArr array
const uint8_t segmentArr[] = {
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b01111110, // G
    0b01110110, // H
    0b00110000, // I
    0b00011110, // J
    0b01110101, // K
    0b00111000, // L
    0b00010101, // M
    0b01010100, // N
    0b01011100, // O
    0b01110011, // P
    0b01100111, // Q
    0b01010000, // R
    0b01101101, // S
    0b01111000, // T
    0b00111110, // U
    0b00111110, // V
    0b00101010, // W
    0b01110110, // X
    0b01101110, // Y
    0b01011011, // Z
    0b00000000  // Space
};

void display_on_seven_segment(char character) { 
   uint8_t display_value; 
   switch (character) { 
       case '8': display_value = 0; break; 
       case 'A': display_value = 1; break; 
       case 'B': display_value = 2; break; 
       case 'C': display_value = 3; break; 
       case 'D': display_value = 4; break; 
       case 'E': display_value = 5; break; 
       case 'F': display_value = 6; break; 
       case 'G': display_value = 7; break; 
       case 'H': display_value = 8; break; 
       case 'I': display_value = 9; break; 
       case 'J': display_value = 10; break; 
       case 'K': display_value = 11; break; 
       case 'L': display_value = 12; break; 
       case 'M': display_value = 13; break; 
       case 'N': display_value = 14; break; 
       case 'O': display_value = 15; break; 
       case 'P': display_value = 16; break; 
       case 'Q': display_value = 17; break; 
       case 'R': display_value = 18; break; 
       case 'S': display_value = 19; break; 
       case 'T': display_value = 20; break; 
       case 'U': display_value = 21; break; 
       case 'V': display_value = 22; break; 
       case 'W': display_value = 23; break; 
       case 'X': display_value = 24; break; 
       case 'Y': display_value = 25; break; 
       case 'Z': display_value = 26; break;
       case ' ': display_value = 27; break;

       default: display_value = 0; break; // Error case 
   } 

   // Send the value to the GPIO pins connected to the 7-segment display 
   for (unsigned int i = 0; i < 8; i++) {
       unsigned int segmentBit = 1 << (7 - i);
       bool illuminateSegment = (segmentBit & segmentArr[display_value]);
       gpio_put(ALL_SEGMENTS[i], !illuminateSegment); // Adjust the mask and pins as necessary 
   }
} 

void seven_segment_init() {
    for (unsigned int i = 0; i < 8; i++) {
        gpio_init(ALL_SEGMENTS[i]);
        gpio_set_dir(ALL_SEGMENTS[i], GPIO_OUT);
    }
}

void seven_segment_off() {
    for (unsigned int i = 0; i < 8; i++) {
        gpio_put(ALL_SEGMENTS[i], true);
    }
}

void seven_segment_on() {
    for (unsigned int i = 0; i < 8; i++) {
        gpio_put(ALL_SEGMENTS[i], false);
    }
}