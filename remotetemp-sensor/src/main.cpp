#include <Arduino.h>
#include <TM1637Display.h>

#include <pinout.h>
#include <temperature.h>
#include <radio.h>

#define MEASUREMENT_INTERVAL_MS 5000

const uint8_t SEG_ERR[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G, // E
  SEG_E | SEG_G,                         // r
  SEG_E | SEG_G,                         // r
	0
	};

// Set up 7-segment driver
TM1637Display display(LED_CLK_PIN, LED_DAT_PIN);

// Global variables
unsigned long prevMillis = 0;
float temperature = 0.0;

void setup() {
  Serial.begin(9600);

  setupTemperature();
  setupRadio();

  display.setBrightness(2);
  display.clear();

  updateTemperature();
}



void loop() {
  if (millis() > prevMillis + MEASUREMENT_INTERVAL_MS) {
    prevMillis = millis();
    updateTemperature();

    if (temperature == -127) {
      display.setSegments(SEG_ERR);
    } else {
      display.showNumberDecEx(temperature * 100, 64);
    }
  }

  updateRadio();
}
