
#include "pins_arduino.h"
#include <Servo.h>

volatile byte operation;
volatile boolean process_it;
Servo motor_x, motor_y, motor_z;
int counter = 0;

bool shouldStop = false;

void setup() {
  pinMode(MISO, OUTPUT);
  
  // turn on SPI in slave mode
  SPCR |= _BV(SPE);
  
  // turn on interrupts
  SPCR |= _BV(SPIE);
  
  operation = 0;
  process_it = false;
  motor_x.attach(3);
  motor_y.attach(4);
  motor_z.attach(5);
  Serial.begin(9600);
}

ISR (SPI_STC_vect)
{
  operation = SPDR;
  Serial.println("Received something...");
  process_it = true;
}

void pick_four(int multiplier){
  motor_x.write(55);
  delay(2000);
  motor_z.write(60 * multiplier);
  delay(2000);
  motor_z.write(0);
  delay(2000);
  motor_x.write(0);
  delay(2000);
}


void pick_five(int multiplier){
  motor_x.write(30);
  delay(2000);
  motor_z.write(40 * multiplier);
  delay(2000);
  motor_z.write(0);
  delay(2000);
  motor_x.write(0);
  delay(2000);
  
}


void pick_six(int multiplier){
  //motor_y.write(20);
  //delay(2000);
  motor_z.write(40);
  delay(2000);
  motor_y.write(0);
  delay(2000);
  motor_z.write(0);
  delay(2000);
}


void loop() {
  motor_x.write(0);
  motor_y.write(0);
  motor_z.write(0);
  delay(2000);
  
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
        pick_four(1);
        break;
      case 5:
        Serial.println("Press 5");
        pick_five(1);
        break;
      case 6:
        Serial.println("Press 6");
        pick_six(1);
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
 

}
