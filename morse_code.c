#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "includes/seven_segment.h"
#include "includes/potentiometer.h"
#include "includes/buzzer.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <math.h>

// Pin Definitions 
#define BUTTON_PIN 16         // Button GPIO pin 
#define BUTTON_TWO_PIN 22
#define LED_GREEN_PIN 25      // Green LED GPIO pin 
#define LED_RED_PIN 15        // Red LED GPIO pin 

// #define SEVEN_SEGMENT_PINS 19 // Starting pin for 7-segment display 

// Morse Timing Definitions 
#define DOT_DURATION 250      // Duration of a dot (ms) 
#define INTER_LETTER_GAP 700     // Duration of a dash (ms) 
#define INTER_SIGNAL_GAP 400  // Gap between dots/dashes within a letter (ms)


unsigned int TIME_LIMIT = 4000;

typedef struct {
    const char *morse;
    char letter;
} MorseCode;

MorseCode morse_code_dict[] = {
    {".-", 'A'}, 
    {"-...", 'B'}, 
    {"-.-.", 'C'}, 
    {"-..", 'D'},
    {".", 'E'}, 
    {"..-.", 'F'}, 
    {"--.", 'G'}, 
    {"....", 'H'},
    {"..", 'I'}, 
    {".---", 'J'}, 
    {"-.-", 'K'}, 
    {".-..", 'L'},
    {"--", 'M'}, 
    {"-.", 'N'}, 
    {"---", 'O'}, 
    {".--.", 'P'},
    {"--.-", 'Q'}, 
    {".-.", 'R'}, 
    {"...", 'S'}, 
    {"-", 'T'},
    {"..-", 'U'}, 
    {"...-", 'V'}, 
    {".--", 'W'}, 
    {"-..-", 'X'},
    {"-.--", 'Y'}, 
    {"--..", 'Z'}
};
const int morse_code_count = sizeof(morse_code_dict) / sizeof(morse_code_dict[0]);



// Function Prototypes
void flash_delay(char signal);
uint32_t measure_button_press_duration(); 
char interpret_morse(uint32_t duration); 
char decode_morse_sequence(char *sequence); 


// Main Function 

int main() { 
   stdio_init_all();  
   // Initialize GPIO pins 
   gpio_init(BUTTON_PIN); 
   gpio_set_dir(BUTTON_PIN, GPIO_IN); 
   gpio_pull_down(BUTTON_PIN); // Enable pull-down for button  
   gpio_init(BUZZER_PIN); 
   gpio_set_dir(BUZZER_PIN, GPIO_OUT); 
   gpio_init(LED_GREEN_PIN); 
   gpio_set_dir(LED_GREEN_PIN, GPIO_OUT); 
   gpio_init(LED_RED_PIN); 
   gpio_set_dir(LED_RED_PIN, GPIO_OUT); 
   adc_init();
   adc_gpio_init(POTENTIOMETER_PIN);
   gpio_init(BUTTON_TWO_PIN);
   gpio_set_dir(BUTTON_TWO_PIN, GPIO_IN);
   gpio_pull_down(BUTTON_TWO_PIN);

   seven_segment_init();
   seven_segment_off();

   timer_hw->dbgpause = 0;
   unsigned int time_used = 0;

   // Startup sequence 
   printf("%s", "\nWelcome to the Morse Code Decoder!"); 
   display_on_seven_segment('8'); // Turn on all segments to show "8" 
   sleep_ms(500);                 // Wait for 500ms 
   display_on_seven_segment(' '); // Turn off all segments 


   char morse_sequence[10]; // Buffer to store Morse code sequence 
   uint8_t morse_index = 0; // Current index in the Morse sequence
   uint8_t successful_decodes = 0;
   char current_string[10] = "";

   printf("\nSet the time limit using the potentiometer");
   printf("\nConfirm the input by pressing the right button.");
   printf("\nThe current time limit is: %d ms", TIME_LIMIT);
   while (!gpio_get(BUTTON_TWO_PIN)) {
    unsigned int new_time_limit = potentiometer_read(1000, 8000);
    if (new_time_limit != TIME_LIMIT) {
        printf("\nThe potential new time limit is: %d ms", new_time_limit);
        TIME_LIMIT = new_time_limit;
        sleep_ms(100);
    }
   }
   printf("\nThe time limit is: %d ms", TIME_LIMIT);

    

   while (true) {
       if (time_used > TIME_LIMIT) {
        printf("\nYou did not complete in the allocated time.");
        beep_negative_feedback();
        return 0;
       } else {
        time_used++;
        sleep_ms(1);
       }
       // Measure the duration of the button press 
       //printf("%d\n", potentiometer_read(360));
       uint32_t duration = measure_button_press_duration();  
	   //printf("%d\n", duration);

       // Interpret the Morse signal 
       char signal = interpret_morse(duration); 

       // If the signal is invalid, skip further processing 
       if (signal == '8') {
           gpio_put(LED_RED_PIN, 1); // Light up red LED 
           flash_delay(signal);
           gpio_put(LED_RED_PIN, 0); // Turn off red LED 
           display_on_seven_segment('8'); // Display '8' for invalid input 
        //    beep_negative_feedback();
       } else if (signal != ' ') {
        // Provide feedback for valid input 
        gpio_put(LED_GREEN_PIN, 1); // Light up green LED 
        flash_delay(signal);
        gpio_put(LED_GREEN_PIN, 0); // Turn off green LED 
       }

       // Append the signal to the Morse sequence
       if (signal != ' ') {
        morse_sequence[morse_index++] = signal; 
        morse_sequence[morse_index] = '\0'; // Null-terminate the sequence 
       }


        if (signal != ' ') {
           // Measure the duration of the gap
           uint32_t gapDuration = 0;
           while (!gpio_get(BUTTON_PIN) && gapDuration < INTER_LETTER_GAP) {
               gapDuration++;
               sleep_ms(1);
           }


           // If the gap is longer than 400ms, it is an inter-letter gap
           if (gapDuration >= INTER_SIGNAL_GAP) {
               // Decode and display the letter
               char letter = decode_morse_sequence(morse_sequence);
               if (letter != '8') {
                strncat(current_string, &letter, 1);
                display_on_seven_segment(letter);
                printf("\nDecoded Letter: %c", letter);
                printf("\nCurrent String: %s", current_string);
                successful_decodes++;
                if (successful_decodes > 3) {
                    printf("\nFinal String: %s", current_string);
                    play_song();
                    printf("\nDo you want to create another set of characters?");
                    while (!gpio_get(BUTTON_PIN) || !gpio_get(BUTTON_TWO_PIN)) {
                        if (gpio_get(BUTTON_PIN)) {
                            gpio_put(LED_GREEN_PIN, 1);
                            sleep_ms(250);
                            gpio_put(LED_GREEN_PIN, 0);
                            seven_segment_off();
                            beep_off();
                            current_string[0] = '\0';
                            break;
                        }
                        else if (gpio_get(BUTTON_TWO_PIN)) {
                            gpio_put(LED_RED_PIN, 1);
                            sleep_ms(250);
                            gpio_put(LED_RED_PIN, 0);
                            seven_segment_off();
                            beep_off();
                            return 0;
                        }
                    }
                    successful_decodes = 0;
                    time_used = 0;
                }
               } else {
                display_on_seven_segment('8');
                gpio_put(LED_RED_PIN, 1); // Light up red LED 
                flash_delay('8');
                gpio_put(LED_RED_PIN, 0); // Turn off red LED 
                beep_negative_feedback();
                beep_off();
               }

               // Reset Morse sequence for the next letter
               morse_index = 0;
               morse_sequence[0] = '\0';
               gapDuration = 0;
           }
       }
   } 
   return 0; 
} 

// Function to measure button press duration 
uint32_t measure_button_press_duration() { 
	uint32_t pressedDuration = 0;
	while (gpio_get(BUTTON_PIN)) {
		pressedDuration++;
		sleep_ms(1);
	}
	return pressedDuration;
} 


// Function to interpret Morse signal based on duration 

char interpret_morse(uint32_t duration) { 
   if (duration > 0 && duration < DOT_DURATION) { 
    beep_dot();
    return '.'; // Dot 
   } else if (duration >= DOT_DURATION && duration <= INTER_LETTER_GAP) {
    beep_dash(); 
    beep_off();
    return '-'; // Dash 
   } else if (duration == 0) { 
    return ' '; // Invalid signal 
   } else {
    return '8';
   }
} 

// Function to provide delay for flashing LEDs
void flash_delay(char signal) {
    if (signal == '.') {
        sleep_ms(100);           // Long flash for dash
    } else {
        sleep_ms(300); // Short flash for a dot and an error
    }
}

// Function to decode Morse sequence into a letter or number 

char decode_morse_sequence(char *sequence) { 
    for (int i = 0; i < morse_code_count; i++) {
        if (strcmp(morse_code_dict[i].morse, sequence) == 0) {
            return morse_code_dict[i].letter;
        }
    }
    return '8';
} 


