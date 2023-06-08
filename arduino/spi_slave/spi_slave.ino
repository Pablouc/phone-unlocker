#include "pins_arduino.h"
#include <Servo.h>

Servo motor_x, motor_y, motor_z;

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

  motor_x.attach(50);
  motor_y.attach(48);
  motor_z.attach(46);
  
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
  int data_to_respond = 10;
  if(process_it) {
    switch (operation) {
      case 0:
        Serial.println("Press 0");
        break;
      case 1:
        Serial.println("Press 1");
        break;
      case 2:
        Serial.println("Press 2");
        break;
      case 3:
        Serial.println("Press 3");
        break;
      case 4:
        Serial.println("Press 4");
          motor_x.write(55);
          delay(2000);
          motor_z.write(60 * multiplier);
          delay(2000);
          motor_z.write(0);
          delay(2000);
          motor_x.write(0);
          delay(2000);
        break;
      case 5:
        Serial.println("Press 5");
        motor_x.write(30);
        delay(2000);
        motor_z.write(40 * multiplier);
        delay(2000);
        motor_z.write(0);
        delay(2000);
        motor_x.write(0);
        delay(2000);
        break;
      case 6:
        Serial.println("Press 6");
        motor_z.write(40);
        delay(2000);
        motor_y.write(0);
        delay(2000);
        motor_z.write(0);
        delay(2000);
        break;
      case 7:
        Serial.println("Press 7");
        break;
      case 8:
        Serial.println("Press 8");
        break;
      case 9:
        Serial.println("Press 9");
        break;
      case 11:
        Serial.println("Change to screen 1");
        break;
      case 22:
        Serial.println("Change to screen 2");
        break;
      case 33:
        Serial.println("Change to screen 3");
        break;
      default:
        Serial.println("Non expected value");
        Serial.println(operation);
        SPDR = 1;
        break;
    }
    process_it = false;
    SPDR = data_to_respond;
  }
}  // end of loop
