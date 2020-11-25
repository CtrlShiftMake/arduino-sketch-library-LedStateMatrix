#include <Arduino.h>
#include <FastLED.h>
#include "src/LedMatrix/LedMatrix.h"

CRGB leds[LED_NUM];

LedMatrix ledMatrix;

void setup() {
  Serial.begin(9600);
  ledMatrix.init();
  ledMatrix.applyToCRGBArray(leds);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.show();
}

void loop() {

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++)
  {
      for (int y = 0; y < LED_ARRAY_Y; y++)
      {
          ledMatrix.setState(x, y, LED_DIMMED);
      }
  }
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++)
  {
      for (int y = 0; y < LED_ARRAY_Y; y++)
      {
          ledMatrix.setState(x, y, LED_ACTIVE);
      }
  }
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++)
  {
      for (int y = 0; y < LED_ARRAY_Y; y++)
      {
          ledMatrix.setState(x, y, LED_PRESSED);
      }
  }
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++)
  {
      for (int y = 0; y < LED_ARRAY_Y; y++)
      {
          ledMatrix.setState(x, y, LED_INACTIVE);
      }
  }
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);

  ledMatrix.setInputsColor(r, g, b);
  
}