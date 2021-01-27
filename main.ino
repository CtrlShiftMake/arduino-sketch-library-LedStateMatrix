#include <Arduino.h>
#include <FastLED.h>
#include <Fsm.h>
#include <RTClib.h>
#include <SD.h>

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
DateTime now;
File file;

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
  Serial.begin(9600);

  // Init state transitions
  Serial.print("Initializing States...");
  setupStateMachine();
  Serial.print("done!\n");

  // Initializing RTC for time keeping
  Serial.print("Initializing RTC...");
  if (!rtc.begin())
    error("Couldn't find RTC");
  if (!rtc.initialized() || rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  rtc.start();
  Serial.println("done!");

  // Initialize SD card for storage
  Serial.print("Initializing SD Card...");
  pinMode(10, OUTPUT);
  if (!SD.begin(10)) {
    error("card failed, or not present");
  }
  Serial.println("done!");

  // Initialize the root directory
  Serial.print("Initializing Config File...");
  file = SD.open("config.txt");
  if (!file)
    error("problem loading file");
  Serial.println("done!");

  // Initializing LED matrix for display
  Serial.print("Initializing LED Matrix...");
  ledMatrix.init();
  ledMatrix.applyToCRGBArray(leds);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.show();
  Serial.println("done!");
}

void loop() { fsm.run_machine(); }

/////////// FINITE STATE MACHINE //////////////////

void setupStateMachine() {
  fsm.add_transition(&state_load, &state_track, TRACK_HABIT, NULL);
  fsm.add_transition(&state_track, &state_load, LOAD_HABIT, NULL);
}

// --  LOAD  --

void onEnterLoad() {
  now = rtc.now();
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" | ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
}
void onStateLoad() { fsm.trigger(TRACK_HABIT); }
void onExitLoad() {}

// --  TRACK --

void onEnterTrack() {}
void onStateTrack() {
  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_DIMMED);
    }
  }
  Serial.println("LEDS Dimmed");
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_ACTIVE);
    }
  }
  Serial.println("LEDS Active");
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_PRESSED);
    }
  }
  Serial.println("LEDS Pressed");
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  delay(600);
  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      ledMatrix.setState(x, y, LED_INACTIVE);
    }
  }
  Serial.println("LEDS INACTIVE");
  ledMatrix.applyToCRGBArray(leds);
  FastLED.show();

  int r = random(0, 255);
  int g = random(0, 255);
  int b = random(0, 255);

  ledMatrix.setInputsColor(r, g, b);
}
void onExitTrack() {}
