#include "src/LedMatrix/LedMatrix.h"

#include <Arduino.h>
#include <FastLED.h>
#include <RTClib.h>
#include <SD.h>
#include <Wire.h>

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};

const int chipSelect = 10;
File configFile;

CRGB leds[LED_NUM];
LedMatrix ledMatrix;

void error(char *str) {
  Serial.print("ERROR: ");
  Serial.print(str);
  while (1)
    ;
}

void setup() {
  Serial.begin(9600);
#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif

  // Initializing RTC for time keeping
  Serial.print("Initializing RTC...");
  if (!rtc.begin())
    error("Couldn't find RTC");
  if (!rtc.initialized() || rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();
  Serial.print("done!\n");

  // Initialize SD Card
  Serial.print("Initializing SD Card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    error("card failed, or not present");
  }
  Serial.print("done!\n");

  // Initializing Config File on SD Card
  Serial.print("Initializing config file...");
  configFile = SD.open("config.txt", FILE_WRITE);
  if (!configFile) {
    error("couldn't create file");
  }
  Serial.print("done!\n");

  // Initializing LED Matrix
  Serial.print("Initializing LED Matrix...");
  ledMatrix.init();
  ledMatrix.applyToCRGBArray(leds);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.show();
  Serial.print("done!\n");
}

void loop() {}

void DebugRTC() {
  DateTime now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}

void DebugLEDStates() {
  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_DIMMED);
    }
  }
  ledMatrix.applyToCRGBArray(leds);
  Serial.println("DIMMED");
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_ACTIVE);
    }
  }
  ledMatrix.applyToCRGBArray(leds);
  Serial.println("ACTIVE");
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_PRESSED);
    }
  }
  Serial.println("PRESSED");
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_INACTIVE);
    }
  }
  ledMatrix.applyToCRGBArray(leds);
  Serial.println("INACTIVE");
  FastLED.show();

  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);

  ledMatrix.setInputsColor(r, g, b);
}
