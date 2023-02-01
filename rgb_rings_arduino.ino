#include <Arduino.h>
#include <FastLED.h>
#include <bluefairy.h>
#include "palettes.h"
#include "utils.h"

#define NUM_LEDS   16 
#define BRIGHTNESS  40
#define DISPLAY_HERTZ 90
#define ROTATION_HERTZ 1.0

// Digital
#define LED_PIN 1
// Analog
#define MICROPHONE_PIN A2

#define SAMPLES_COUNT 100
#define SAMPLING_FREQUENCY 8000 //Hz, must be less than 10000 due to ADC

unsigned int samplingPeriodMicro = round(1000000*(1.0/SAMPLING_FREQUENCY));

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
  scheduler.every(37, [](){
    // Correct
    // int micSample = analogRead(MICROPHONE_PIN); // Same as analogRead(2)
    // Serial.print("Sample1:");
    // Serial.println(micSample);

    // Flatline? This is almost definitely the LED A0 pin
    // int micSample2 = analogRead(0);
    // Serial.print("Sample2:");
    // Serial.println(micSample2);

    // Reacts to A1 touch
    // int micSample3 = analogRead(1);
    // Serial.print("Sample3:");
    // Serial.println(micSample3);

    int min = 1024;
    int max = 0;
    for (int i = 0; i < SAMPLES_COUNT; i++) {
      unsigned long microseconds = micros();
      int sample = analogRead(MICROPHONE_PIN);
      if (sample > max) {
        max = sample;
      }
      if (sample < min) {
        min = sample;
      }
      while (micros() < (microseconds + samplingPeriodMicro)) { }
    }
    Serial.print("Amplitude:");
    Serial.println(max - min);
  });
}

void loop() {
  scheduler.loop();
  random16_add_entropy(1);
}
