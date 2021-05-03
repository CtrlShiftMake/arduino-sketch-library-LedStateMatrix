#include <Arduino.h>
#include <FastLED.h>

#include "src/LedStateMatrix/LedStateMatrix.h"

CRGB leds[LED_NUM];
LedStateMatrix l_state;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing LED Matrix...");
  l_state.init();
  l_state.applyToCRGBArray(leds);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.show();
  Serial.println("done!");
}

void loop() {
  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      l_state.setState(x, y, LED_DIMMED);
    }
  }
  Serial.println("LEDS Dimmed");
  l_state.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      l_state.setState(x, y, LED_ACTIVE);
    }
  }
  Serial.println("LEDS Active");
  l_state.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      l_state.setState(x, y, LED_PRESSED);
    }
  }
  Serial.println("LEDS Pressed");
  l_state.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      l_state.setState(x, y, LED_INACTIVE);
    }
  }
  Serial.println("LEDS INACTIVE");
  l_state.applyToCRGBArray(leds);
  FastLED.show();

  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);

  l_state.setInputsColor(r, g, b);
}
