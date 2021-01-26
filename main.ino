#include <Arduino.h>
#include <ArduinoJson.h>
#include <FastLED.h>
#include <RTClib.h>
#include <SD.h>
#include <Wire.h>
#include <YA_FSM.h>

#include "src/LedMatrix/LedMatrix.h"

// Finite State Machine
// YA_FSM fsm(2, 1);
YA_FSM fsm;
enum Input { xLoad, xTrack, xEdit };
enum State { LOAD, TRACK, EDIT };
const char *stateName[] = {"Loading", "Tracking", "Editing"};
Input input;
uint8_t currentState;

// Adafruit Data Logger Shield
RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};
const int chipSelect = 10;
File root;
DateTime now;

// Addressable LED Strip
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

  // Initializing State Machine
  Serial.print("Initializing States...");
  setupStateMachine();
  input = Input::xLoad;
  currentState = fsm.GetState();
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

  // Initialize SD Card
  Serial.print("Initializing SD Card...");
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    error("card failed, or not present");
  }
  Serial.println("done!");

  // Initializing Config File on SD Card
  Serial.print("Initializing Root Dir...");
  root = SD.open("/");
  if (!root.isDirectory())
    error("couldn't open root");
  Serial.println("done!");

  // Initializing LED Matrix
  Serial.print("Initializing LED Matrix...");
  ledMatrix.init();
  ledMatrix.applyToCRGBArray(leds);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.show();
  Serial.println("done!");

  // Start the Load state
  onEnterLoad();
}

void loop() {
  // Update State Machine	(true is state changed)
  if (fsm.Update()) {
    currentState = fsm.GetState();
    Serial.print(F("Active state: "));
    Serial.println(stateName[currentState]);
  }
}

/////////// FINITE STATE MACHINE //////////////////

void setupStateMachine() {

  fsm.AddState(stateName[LOAD], 0, onEnterLoad, onStateLoad, onExitLoad);
  fsm.AddState(stateName[TRACK], 0, onEnterTrack, onStateTrack, onExitTrack);
  fsm.AddState(stateName[EDIT], 0, onEnterEdit, onStateEdit, onExitEdit);

  fsm.AddTransition(LOAD, TRACK, []() { return input == Input::xTrack; });
  fsm.AddTransition(TRACK, LOAD, []() { return input == Input::xLoad; });
  fsm.AddTransition(TRACK, EDIT, []() { return input == Input::xEdit; });
  fsm.AddTransition(EDIT, TRACK, []() { return input == Input::xTrack; });
}

// --  LOAD  --

void onEnterLoad() {
  /*
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
  */
}
void onStateLoad() { input = Input::xTrack; }
void onExitLoad() {}

// --  TRACK --

void onEnterTrack() {}
void onStateTrack() {
  /*
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
  */
}
void onExitTrack() {}

// --  EDIT  --

void onEnterEdit() {}
void onStateEdit() {}
void onExitEdit() {}
