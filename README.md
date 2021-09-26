# binaryClock

binaryClock is a script providing a binary clock on the digital pins 3 to 7 for the hours and 8 to 13 for the minutes on the Arduino Nano (or probably more general the ATmega 328). I wrote this project because I wanted a binary clock and was curious how the timer counter on the ATmega328 works.

## Installation / Usage

Clone the repo 
```bash
git clone https://github.com/alexschu98/binaryClock.git
```

and use the [Arduino IDE](https://www.arduino.cc/en/software) to flash it to your ATmega328 Arduino.

Please note that this project uses the internal TimerCounter1 of the ATmega328 which is why it only runs on this specific microcontroller and e. g. not on an Arduino Due. Although not tested I would expect it to work on the ATmega168 as well because to my knowledge it has the same registers but less memory which should not be a problem here.

## Setting the clock

To set the clock look for the lines 34 to 37 in the `binaryClock.ino` file:
```c
// global variables
uint8_t g_counterHours = 23;            ///< counter for passed hours
uint8_t g_counterMinutes = 13;          ///< counter for passed minutes
volatile uint8_t g_counterSeconds = 0;  ///< counter for passed seconds
```

Here you can set the time before you upload the sketch to the Arduino.