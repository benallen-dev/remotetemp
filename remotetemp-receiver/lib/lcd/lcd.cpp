#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
#include <pinout.h>

#define LCD_ADDRESS

extern float temperature;

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display
unsigned long prevMillis;

void setupLcd() {
  int error = 0;

  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");
    lcd.begin(16, 2);  // initialize the lcd
    lcd.setBacklight(255);
    lcd.clear();
    lcd.home();
    lcd.print("Initialised!");
  } else {
    Serial.println(": LCD not found.");
  }  // if
};

void updateLcd() {
  if (millis() > prevMillis + 2000) {
    prevMillis = millis();

    lcd.home();
    lcd.print("Temperature:");
    lcd.setCursor(0, 1);
    lcd.print(temperature);
  }
};