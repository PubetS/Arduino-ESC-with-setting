#include <Servo.h>
#include <EEPROM.h>
//#include "TimerOne.h"
Servo ESC;     // create servo object to control the ESC
int addr0 = 0;
int addr1 = 1;
int ledState = LOW;
int redled = 13;
//int greenled = 12;

#define interruptPin1 2
int inputPin = A0;
int sensorValue = 0;         // the sensor value
int presensorMin = 0;        // minimum sensor value
int presensorMax = 1023;           // maximum sensor value
int sensorMin = 0;        // minimum sensor value
int sensorMax = 0;           // maximum sensor value

void setup() {
 // Timer1.initialize(); 
  pinMode(redled, OUTPUT);
  //pinMode(greenled, OUTPUT);
  pinMode(interruptPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), calibration, LOW);
  // Attach the ESC on pin 9
  ESC.attach(9, 1000, 2000); // (pin, min pulse width, max pulse width in milliseconds)
  //Serial.begin(9600);

}

void loop() {
  digitalWrite(redled, HIGH);
   //digitalWrite(greenled, HIGH);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), calibration, LOW);
  sensorValue = analogRead(inputPin);



  sensorMin = EEPROM.read(addr0);
  sensorMax = EEPROM.read(addr1);

  int sensorMin2 = map(sensorMin, 0, 180, 0, 1023);
  int sensorMax2 = map(sensorMax, 0, 180, 0, 1023);

  //  Serial.print("Analog in = ");
  //  Serial.println(sensorValue);
  //  Serial.print("sensorMax = ");
  //  Serial.println(sensorMax);
  //  Serial.print("sensorMin = ");
  //  Serial.println(sensorMin);
  //  Serial.print("sensorMax2 = ");
  //  Serial.println(sensorMax2);
  //  Serial.print("sensorMin2 = ");
  //  Serial.println(sensorMin2);

  sensorValue = map(sensorValue, sensorMin2, sensorMax2, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)


  ESC.write(sensorValue);    // Send the signal to the ESC
  //delay(100);
}

void calibration() {

  int newsensorMin = 1023;        // minimum sensor value
  int newsensorMax = 0;           // maximum sensor value
  detachInterrupt(digitalPinToInterrupt(interruptPin1));
  while (digitalRead(2) == LOW) {
    ESC.write(0); 
    sensorValue = analogRead(inputPin);   // reads the value of the potentiometer (value between 0 and 1023)
    flash();
    //digitalWrite(redled, ledState);
    if (sensorValue > newsensorMax) {
      newsensorMax = sensorValue;
    }

    // record the minimum sensor value
    if (sensorValue < newsensorMin) {
      newsensorMin = sensorValue;
    }

    //    Serial.print("newsensorMax = ");
    //    Serial.println(newsensorMax);
    //    Serial.print("newsensorMin = ");
    //    Serial.println(newsensorMin);

  }
  sensorMax = newsensorMax;
  sensorMin = newsensorMin;
  sensorMax = map(sensorMax, 0, 1023, 0, 180);
  sensorMin = map(sensorMin, 0, 1023, 0, 180);
  EEPROM.write(addr0, sensorMin);
  EEPROM.write(addr1, sensorMax);

}

void flash() {
  ledState = !ledState;
  for(int i=0;i<=8000;i++){
  
  digitalWrite(redled, ledState);
//digitalWrite(redled, digitalRead(ledPin) ^ 1);
  }
}
