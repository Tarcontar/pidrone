#include <avr/io.h>
#include <util/delay.h>
#include "pins_arduino.h"

// Forward Declarations
void setup(void);
void loop(void);

#define BLINK_DELAY_MS 500

void setup() {
  digitalWrite(1,HIGH);
}

void loop() {
  _delay_ms(BLINK_DELAY_MS);   // delay  
}

#ifndef ARDUINO
int main(void) {
  setup();
  while(1)
    loop();
}
#endif