#include <Arduino.h>
//#include <ArduinoJson.h>
#include <FastLED.h>
#include <Fsm.h>
#include <RTClib.h>
#include <SD.h>
#include <Wire.h>

#include "src/LedMatrix/LedMatrix.h"

// Events to trigger state change
#define LOAD_HABIT 900
#define TRACK_HABIT 901

// Finite state machine
State state_load(&onEnterLoad, &onStateLoad, &onExitLoad);
State state_track(&onEnterTrack, &onStateTrack, &onExitTrack);
Fsm fsm(&state_load);

// Data logger shield setup
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};
const int chipSelect = 10;
File config;
DateTime now;

// LED strip
CRGB leds[LED_NUM];
LedMatrix ledMatrix;

void error(char *str) {
  Serial.print("ERROR: ");
  Serial.print(str);
  while (1)
    ;
}

void setup() {
  Serial.begin(115200);
#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif
  Serial.println(FreeRam());

  // Init state transitions
  Serial.print("Initializing States...");
  setupStateMachine();
  Serial.print("done!\n");

  Serial.println(FreeRam());

  // Initializing RTC for time keeping
  Serial.print("Initializing RTC...");
  if (!rtc.begin())
    error("Couldn't find RTC");
  if (!rtc.initialized() || rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();
  Serial.println("done!");
  Serial.println(FreeRam());

  // Initialize SD card for storage
  Serial.print("Initializing SD Card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    error("card failed, or not present");
  }
  Serial.println("done!");
  Serial.println(FreeRam());

  // Initialize the root directory
  Serial.print("Initializing Root Dir...");
  config = SD.open("config.txt");
  if (!config)
    error("problem loading config file");
  Serial.println("done!");
  Serial.println(FreeRam());

  // Initializing LED matrix for display
  Serial.print("Initializing LED Matrix...");
  ledMatrix.init();
  ledMatrix.applyToCRGBArray(leds);
  /*
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.show();
  */
  Serial.println("done!");
  Serial.println(FreeRam());
}

void loop() { fsm.run_machine(); }

/////////// FINITE STATE MACHINE //////////////////

void setupStateMachine() {
  fsm.add_transition(&state_load, &state_track, TRACK_HABIT, NULL);
  fsm.add_transition(&state_track, &state_load, LOAD_HABIT, NULL);
}

// --  LOAD  --

void onEnterLoad() {
  Serial.println("Enter LOADING");

  now = rtc.now();
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
void onStateLoad() {
  Serial.println("Loading...");
  fsm.trigger(TRACK_HABIT);
}
void onExitLoad() { Serial.println("Exit LOADING"); }

// --  TRACK --

void onEnterTrack() { Serial.println("Enter TRACKING"); }
void onStateTrack() {
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
void onExitTrack() {}
