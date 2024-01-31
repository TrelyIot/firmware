#include <Arduino.h>

volatile unsigned long long PULSE = 0;
const int debounceDelay = 100; // Adjust this debounce delay according to your needs
volatile unsigned long lastDebounceTime = 0;
const gpio_num_t pulsePin = GPIO_NUM_39;

void set_pulse() {
  PULSE++;
}

volatile unsigned long get_PULSE() {
  return PULSE;
}

void handlePulse() {
  // Get the current time
  unsigned long currentMillis = millis();

  // Check if the current time is greater than the debounce delay
  if (currentMillis - lastDebounceTime >= debounceDelay) {
    // Your code to handle the interrupt here
    set_pulse();
    // Update the last debounce time
    lastDebounceTime = currentMillis;
  }
}

void IRAM_ATTR handlePulse_FM() {
  // Interrupt service routine to count pulses
  set_pulse();
}

void pulse_begin(bool isPulse) {
  if(isPulse) attachInterrupt(digitalPinToInterrupt(pulsePin), handlePulse, CHANGE); // This is for GAS
  else attachInterrupt(digitalPinToInterrupt(pulsePin), handlePulse_FM, RISING); // this is for FM
}
