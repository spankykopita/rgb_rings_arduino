#include <Arduino.h>
#include <FastLED.h>
#include <bluefairy.h>
#include "palettes.h"
#include "utils.h"
#include "audio.h"
#include "visualizations.h"

#define BRIGHTNESS  60

// Digital
#define LED_PIN 1

bluefairy::Scheduler scheduler;

void setup() {
  random16_add_entropy(analogRead(MICROPHONE_PIN));
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(9600);
  
  scheduler.every(1000 / DISPLAY_HERTZ, [](){
    recordAmplitude();

    if (isStartOfPeak) {
      // Switch direction and change colors
      rotationIncrement *= -1;
      currentPalette = getRandomPalette();
    }

    rotateColors();
  
    if (lengthOfPeakMillis > 300) {
      showSparkles();
    } else {
      showSpinnyRing();
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
  random16_add_entropy(analogRead(0) + millis());
}
