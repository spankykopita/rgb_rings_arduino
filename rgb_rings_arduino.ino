#include <Arduino.h>
#include <FastLED.h>
#include <bluefairy.h>
#include "palettes.h"
#include "utils.h"
#include "audio.h"

#define NUM_LEDS   32
#define BRIGHTNESS  255
#define DISPLAY_HERTZ 90
#define ROTATION_HERTZ 1.0

uint8_t LEDS_PER_CIRCLE = NUM_LEDS / 2;

// Digital
#define LED_PIN 1

bluefairy::Scheduler scheduler;

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette = getRandomPalette();
CRGBPalette16 nextPalette = getRandomPalette();

// 256 fastLED index units per rotation * x rotations per second / y display refreshes per second
// = fastLED index units per display refresh
int rotationIncrement = 256 * ROTATION_HERTZ / DISPLAY_HERTZ;
uint8_t rotationOffset = 0;

uint8_t brightness = 0;

void setup() {
  random16_add_entropy(analogRead(MICROPHONE_PIN));
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
  
  scheduler.every(1000 / DISPLAY_HERTZ, [](){
    recordAmplitude();

    if (isStartOfPeak) {
      rotationIncrement *= -1;
      // currentPalette = getRandomPalette();
    }
    
    // rotationOffset += rotationIncrement;
    rotationOffset += rotationIncrement * (amplitudeRatio - 0.5) * 20.0;
    // if (amplitudeRatio < 0.5) rotationOffset = 0.0;

    // brightness = 255;
    // brightness = mapToByteRange(smoothedAmplitude, minAmplitude, maxAmplitude);
    // brightness = map(smoothedAmplitude, minAmplitude, maxAmplitude, 20, 255);
    brightness = isPeak ? 255 : brightness < 10 ? 0 : brightness - 5;

    for (int i = 0; i < LEDS_PER_CIRCLE; i++) {
      uint8_t colorIndex = mapToByteRange(i, 0, LEDS_PER_CIRCLE);
      CRGB color = ColorFromPalette(currentPalette, colorIndex + rotationOffset, brightness, LINEARBLEND);
      leds[i] = color;
      leds[i + LEDS_PER_CIRCLE] = color;
    }

    FastLED.show();
  });

  scheduler.every(1000, [](){
    nextPalette = getRandomPalette();
  });
  scheduler.every(10, [](){
    nblendPaletteTowardPalette(currentPalette, nextPalette, 80);
  });
}

void loop() {
  scheduler.loop();
  random16_add_entropy(analogRead(1));
}
