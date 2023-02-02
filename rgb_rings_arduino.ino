#include <Arduino.h>
#include <FastLED.h>
#include <bluefairy.h>
#include "palettes.h"
#include "utils.h"
#include "audio.h"

#define NUM_LEDS   16 
#define BRIGHTNESS  40
#define DISPLAY_HERTZ 90
#define ROTATION_HERTZ 1.0

// Digital
#define LED_PIN 1

bluefairy::Scheduler scheduler;

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette = getRandomPalette();
CRGBPalette16 nextPalette = getRandomPalette();

// 256 fastLED index units per rotation * x rotations per second / y display refreshes per second
// = fastLED index units per display refresh
uint8_t rotationIncrement = 256 * ROTATION_HERTZ / DISPLAY_HERTZ;
uint8_t rotationOffset = 0;

void setup() {
  random16_add_entropy(analogRead(MICROPHONE_PIN));
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
  // while(!Serial);
  
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
  scheduler.every(51, recordAmplitude);
}

void loop() {
  scheduler.loop();
  random16_add_entropy(analogRead(1));
}
