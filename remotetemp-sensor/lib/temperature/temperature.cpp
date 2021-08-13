#include <pinout.h>
#include <DallasTemperature.h>

#define TEMPERATURE_PRECISION 10

// Set up Dallas Temperature probe
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempAddress;

// Globals
extern float temperature;

void setupTemperature() {
  if (sensors.getAddress(tempAddress, 0)) {
    Serial.print("Found device with address: ");
    for (uint8_t i = 0; i < 8; i++) {
      if (tempAddress[i] < 16) Serial.print("0");
      Serial.println(tempAddress[i], HEX);
    }

    sensors.setResolution(tempAddress, TEMPERATURE_PRECISION);
    sensors.setWaitForConversion(true);
  } else {
    // Guess it can't find the sensor
    Serial.println("Cannot find temperature probe...");
  }
}

void updateTemperature() {
  sensors.requestTemperatures();
  temperature = sensors.getTempC(tempAddress);
  if (temperature == DEVICE_DISCONNECTED_C) {
    return;
  }

  Serial.println(temperature);
}
