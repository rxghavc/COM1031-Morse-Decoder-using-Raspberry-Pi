# COM1031 — Morse Decoder using Raspberry Pi

This embedded systems project implements a fully functional Morse Code decoder using a Raspberry Pi Pico and various hardware components including a 7-segment display, buzzer, RGB LED, button, and potentiometer. The system interprets Morse inputs and outputs corresponding characters in real time.

It was a group project consisting of 4 people: Abdul, Jacob, Hassan, and  myself.

---

## Coursework Requirements Achieved (100%)

This project fully satisfies all criteria defined in the COM1031 specification:

### Startup Functionality
- Displays `8` on the 7-segment display as a test, then turns off all LEDs.
- Outputs a welcome message to the serial console on program start.

### Hardware Integration
- Correct circuit connections (button, 7-segment display, buzzer, RGB LED, and potentiometer) based on the lab template.

### Morse Input Recognition
- Button press durations interpreted correctly:
  - dot < 250ms
  - dash ≥ 250ms
- Gap durations between signals and letters handled precisely:
  - Inter-signal gap < 400ms
  - Inter-letter gap ≥ 400ms

### Character Recognition & Display
- Correctly decodes and displays all alphanumeric characters ranging from A-Z and 0-9
- Displays decoded characters on:
  - 7-segment display
  - Serial terminal

### Error Handling
- Inputs longer than 700ms or invalid Morse combinations trigger:
  - Error message on serial console
  - Display of `8` on the 7-segment display

### Buzzer Feedback
- Short beep for dot, long beep for dash
- Distinct negative sound if:
  - Input time is exceeded
  - Inputs cannot be decoded

### RGB LED Feedback
- Green: valid character decoded
- Red: invalid or unrecognisable input

### Time Limit Configuration
- Potentiometer allows real-time adjustment of the time limit per letter (default: 4s)
- Serial terminal displays the configured time limit
- System handles both increase and decrease in input time window

### Completion Logic
- After 4 valid characters:
  - Displays decoded message in terminal
  - Plays celebratory buzzer tune

### User Continuation Option
- Prompt shown:
  - Left button: Continue (reset components, restart input)
  - Right button: Exit (shutdown components)
- RGB LED indicates:
  - Green: Continue
  - Red: Terminate

---

## Project Structure

```bash
├── main.c               # Main implementation of Morse decoder logic
├── hardware_setup.c     # GPIO, timers, and peripheral initialisation
├── display.c            # 7-segment display encoding logic
├── buzzer.c             # Buzzer control logic
├── rgb_led.c            # RGB LED control logic
├── morse_utils.c        # Morse translation, mapping, and error handling
├── potentiometer.c      # Time control setup and tuning
├── README.md            # Project documentation
