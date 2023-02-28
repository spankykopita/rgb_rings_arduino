#include <Arduino.h>
#include <FastLED.h>

#define TOUCH_PIN A1

bool pressedLastCycle = false;
int threshold = analogRead(TOUCH_PIN) + 20;

uint8_t currentBrightnessIndex = 0;
uint8_t brightnessOptions[] = {
  30, 65, 120, 190, 255,
};

void setNextBrightness() {
  int arraySize = sizeof(brightnessOptions) / sizeof(brightnessOptions[0]);
  currentBrightnessIndex = currentBrightnessIndex++ % arraySize;

  Serial.print("Brightness:");
  Serial.println(brightnessOptions[currentBrightnessIndex]);
  FastLED.setBrightness(brightnessOptions[currentBrightnessIndex]);
}

void initializeSettings() {
  setNextBrightness();
}

void checkButtons() {
  bool buttonPressed = analogRead(TOUCH_PIN) > threshold;
  Serial.print("ButtonTouch:");
  Serial.println(buttonPressed);
  if (buttonPressed) {
    if (!pressedLastCycle) {
      setNextBrightness();
    } else {
      // LONG PRESS?
    }
  }
  pressedLastCycle = buttonPressed;
}