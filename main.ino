#include <Arduino.h>
#include <FastLED.h>
#include "src/LedMatrix/LedMatrix.h"

#define LED_NUM 35
#define LED_PIN 12
CRGB leds[LED_NUM];

LedMatrix ledMatrix;

void setup() {

  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);

  ledMatrix.getColorMatrix(leds);
}

void loop() {
  ledMatrix.loop();
  FastLED.show();
  delay(1000);
}
