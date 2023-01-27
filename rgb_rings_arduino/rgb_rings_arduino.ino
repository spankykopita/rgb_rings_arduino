#include "FastLED.h"
#include <bluefairy.h>

bluefairy::Scheduler scheduler;

CRGB leds[1];
boolean showWhite = 1;

void setup() {
  FastLED.addLeds<NEOPIXEL, 1>(leds, 1);
  scheduler.every(500, [](){
    showWhite = !showWhite;
    leds[0] = showWhite ? CRGB::White : CRGB::Black;
    FastLED.show();
  });
}

void loop() {
  scheduler.loop();
}