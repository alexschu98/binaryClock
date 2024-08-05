/*
  binaryClock
  
  This sketch provides a binary clock using a set of LEDs connected to the
  digital pins 3-7 for the hour part where the LED connected to PIN2 to is the
  highest bit and the LED connected to PIN6 is the lowest bit. Same goes for the
  minutes, digital pins 8-13 are used here and again PIN8 is the highest bit and
  PIN13 is the lowest bit.

  The circuit:
  * DIGITAL PIN 03 - LED in color A
  * DIGITAL PIN 04 - LED in color A
  * DIGITAL PIN 05 - LED in color A
  * DIGITAL PIN 06 - LED in color A
  * DIGITAL PIN 07 - LED in color A
  * DIGITAL PIN 08 - LED in color B
  * DIGITAL PIN 09 - LED in color B
  * DIGITAL PIN 10 - LED in color B
  * DIGITAL PIN 11 - LED in color B
  * DIGITAL PIN 12 - LED in color B
  * DIGITAL PIN 13 - LED in color B
  
  I use blue for color A and yellow for color B but of course you are free to
  use whatever you want.

	Created 09/26/2021
	By Alexander Schünemann
	Modified 09/26/2021
	By Alexander Schünemann
*/

#include <stdint.h>

// global variables
uint8_t g_counterHours = 23;            ///< counter for passed hours
uint8_t g_counterMinutes = 13;          ///< counter for passed minutes
volatile uint8_t g_counterSeconds = 0;  ///< counter for passed seconds

/**
 * @brief Setup function. Initializes Timer Counter 1 so that every second an 
 * interrupt is executed which increases g_counterSeconds by one.
 */
void setup() {

  // clear global interrupt flag to edit TC1 registers
  cli();

  TCNT1 = 0; // TC1
  TCCR1A = 0; // TC control register A
  TCCR1B = 0; // TC control register B
  
  // TC output compare register: cpu_freq / (prescaler * interrupt_freq) - 1
  OCR1A = 15624;
  // clock select bit 10/12 (prescaler 1024)
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // CTC mode on (clear timer and compare)
  TCCR1B |= (1 << WGM12);
  // interrupt mask register (enable interrupt on match with value in OCR1A)
  TIMSK1 |= (1 << OCIE1A);

  // set global interrupt flag so the clock can start counting
  sei(); 
}

/**
 * @brief Loop / Main function that updates the timers and calls the methods
 * to enable/disable the LEDs.
 */
void loop() {
  if (g_counterSeconds == 60) {
    g_counterSeconds = 0;
    g_counterMinutes++;  
  }

  if (g_counterMinutes == 60) {
    g_counterMinutes = 0;
    g_counterHours++;
  }

  if (g_counterHours == 24) {
    g_counterHours = 0;  
  }

  writeLedHours();
  writeLedMinutes();
}

/**
 * @brief Reverses byte order of a given uint8_t integer c.
 * 
 * @param c The integer to reverse.
 * @return uint8_t The reversed integer.
 */
uint8_t reverseByte(uint8_t c) {
  c = ((c >> 1) & 0x55) | ((c << 1) & 0xAA);
  c = ((c >> 2) & 0x33) | ((c << 2) & 0xCC);
  c = (c >> 4) | (c << 4) ;
  return c;
}

/**
 * @brief Writes value in g_counterHours to the Pins connected to the hour Leds.
 */
void writeLedHours() {
  // PORTD maps to pins 0 to 7. The Leds are connected in reverse so the counter
  // g_counterHours has to be reversed first.
  PORTD = reverseByte(g_counterHours);
}

/**
 * @brief Writes value in g_counterMinutes to the Pins connected to the minute 
 * Leds.
 */
void writeLedMinutes() {
  // PORTB maps to pins 8 to 13. The Leds are connected in reverse so the couter
  // g_counterMinutes has to be reversed first. Also the highest bits map to the 
  // crystal pins and aren't usable therefore a bitshift two bits to the right 
  // is required.
  PORTB = reverseByte(g_counterMinutes) >> 2;
}

/**
 * @brief TC1 interrupt service routine increasing the seconds counter 
 * g_counterSeconds.
 */
ISR(TIMER1_COMPA_vect) {
  g_counterSeconds++;
}
