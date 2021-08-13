#include <Arduino.h>
#include <pinout.h>

#include <radio.h>
#include <lcd.h>

// global variables
float temperature;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Hello world!");

  setupRadio();
  setupLcd();
}

void loop() {
  updateRadio();
  updateLcd();
}