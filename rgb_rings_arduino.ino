#include <FastLED.h>
#include <bluefairy.h>
#include "utils.h"

#define NUM_LEDS    32
#define BRIGHTNESS  40
#define DISPLAY_HERTZ 60
#define ROTATION_HERTZ 0.3

bluefairy::Scheduler scheduler;

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette = RainbowColors_p;
CRGBPalette16 nextPalette = getRandomPalette();

// 256 fastLED index units per rotation * x rotations per second / y display refreshes per second
// = fastLED index units per display refresh
uint8_t rotationIncrement = 256 * ROTATION_HERTZ / DISPLAY_HERTZ;
uint8_t rotationOffset = 0;

void setup() {
  random16_add_entropy(analogRead(0));
  FastLED.addLeds<NEOPIXEL, 1>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  
  scheduler.every(1000 / DISPLAY_HERTZ, [](){
    rotationOffset += rotationIncrement;

    for (int i = 0; i < NUM_LEDS; i++) {
      uint8_t colorIndex = mapToByteRange(i, 0, NUM_LEDS);
      leds[i] = ColorFromPalette(currentPalette, colorIndex + rotationOffset, 255, LINEARBLEND);
    }

    FastLED.show();
  });

  scheduler.every(10000, [](){
    nextPalette = getRandomPalette();
  });
  scheduler.every(100, [](){
    nblendPaletteTowardPalette(currentPalette, nextPalette, 80);
  });
}

void loop() {
  scheduler.loop();
  random16_add_entropy(1);
}

