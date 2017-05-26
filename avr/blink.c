#include <avr/io.h>
#include <util/delay.h>

// Forward Declarations
void setup(void);
void loop(void);

#define BLINK_DELAY_MS 500

void setup() {
  sbi(DDRB, DDB5);                // LED: PORTB5 out 
}

void loop() {
  sbi(PINB, PINB5);            // toggle LED
  _delay_ms(BLINK_DELAY_MS);   // delay  
}

#ifndef ARDUINO
int main(void) {
  setup();
  while(1)
    loop();
}
#endif