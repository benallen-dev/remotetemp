#include <pinout.h>
#include <FastLED.h>

#define NUM_LEDS 8
#define NEOPIXEL_BRIGHTNESS 20

CRGB leds[NUM_LEDS];
uint8_t hue;

void setupNeopixels() {
  FastLED.addLeds<NEOPIXEL, NEOPIXEL_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(NEOPIXEL_BRIGHTNESS);
}

void updateNeopixels() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setHue(hue + ((255 / 8) * i));
  }

  FastLED.show();

  hue++;
}
