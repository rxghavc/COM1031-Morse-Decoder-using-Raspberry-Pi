#define SEVEN_SEGMENT_H

#define SEGMENT_A 7  // Pin 10 (GPIO 07) - Top-center horizontal bar
#define SEGMENT_B 6  // Pin 09 (GPIO 06) - Top-right vertical bar
#define SEGMENT_C 20  // Pin 26 (GPIO 20) - Bottom-right vertical bar
#define SEGMENT_D 19  // Pin 25 (GPIO 19) - Bottom-center horizontal bar
#define SEGMENT_E 18  // Pin 24 (GPIO 18) - Bottom-left vertical bar
#define SEGMENT_F 8  // Pin 11 (GPIO 08) - Top-left vertical bar
#define SEGMENT_G 9  // Pin 12 (GPIO 09) - Center horizontal bar
#define SEGMENT_DP 21  // Pin 27 (GPIO 21) - Decimal-point

static const unsigned int ALL_SEGMENTS[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DP}; // Array of segment pins

extern const uint8_t segmentArr[];

void display_on_seven_segment(char character); 
void seven_segment_init();
void seven_segment_on();
void seven_segment_off();
