#include "pins_arduino.h"

volatile byte operation;
volatile boolean process_it;

void setup (void)
{
  Serial.begin (9600);   // debugging

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // turn on interrupts
  SPCR |= _BV(SPIE);
  
  operation = 0;
  process_it = false;
}  // end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  operation = SPDR;
  Serial.println("Received something...");
  process_it = true;
}

// main loop - wait for flag set in interrupt routine
void loop (void)
{
  delay(1000);
  if(process_it) {
    switch (operation) {
      case 1:
        Serial.println("Press 1");
        break;
      case 2:
        Serial.println("Press 2");
        break;
      default:
        Serial.println("Non expected value");
        Serial.println(operation);
        break;
    }
  }
}  // end of loop