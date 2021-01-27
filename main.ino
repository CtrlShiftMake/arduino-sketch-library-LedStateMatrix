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
State state_init(&onEnterInit, NULL, NULL);
State state_load(&onEnterLoad, &onStateLoad, &onExitLoad);
State state_track(&onEnterTrack, &onStateTrack, &onExitTrack);
Fsm fsm(&state_init);

// Data logger shield setup
RTC_PCF8523 rtc;
DateTime now;
File root;

// LED strip
CRGB leds[LED_NUM];
LedMatrix ledMatrix;

// Application Globals
int HABIT_COUNT = 0;
String habit_prefix = "HB_";

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
  Serial.print("Initializing Root...");
  root = SD.open("/");
  if (!root)
    error("problem loading root");
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
  fsm.add_transition(&state_init, &state_load, LOAD_HABIT, NULL);
  fsm.add_transition(&state_load, &state_track, TRACK_HABIT, NULL);
  fsm.add_transition(&state_track, &state_load, LOAD_HABIT, NULL);
}

// --  INIT  --

void onEnterInit() {

  now = rtc.now();

  Serial.println();
  Serial.print("Habit Tracker v0.0.0, ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" | ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.println();

  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break;
    }

    if (entry.isDirectory()) {
      String name = entry.name();
      if (!name.indexOf(habit_prefix)) {
        HABIT_COUNT += 1;
        Serial.println(name);
      }
    }
  }

  if (HABIT_COUNT == 0) {
    Serial.println("NO HABIT FOUND, Creating Default...");
    // !!! create a habit folder, config file and current month
  }

  fsm.trigger(LOAD_HABIT);
}

// --  LOAD  --

void onEnterLoad() {}
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
