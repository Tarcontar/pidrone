#include <Arduino.h>
#include <SoftwareSerial.h>

#define BLINK_DELAY_MS 500

void setup() {
  pinMode(13,OUTPUT);
}

void loop() {
  //_delay_ms(BLINK_DELAY_MS);   // delay  
  digitalWrite(13,HIGH);
  delay(1000);
  digitalWrite(13,LOW);
  delay(1000);
}

int main(void) {
  setup();
  while(true)
    loop();
}