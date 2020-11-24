#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    35

CRGB leds[NUM_LEDS];

int LED_BRIGHTNESS_DIR = 0;
int LED_BRIGHTNESS = 255;
int LED_BRIGHTNESS_FADE = 5;
int LED_BRIGHTNESS_LOW = 50;
int LED_BRIGHTNESS_HIGH = 255;
unsigned long interval = 10;
unsigned long currentMillis;
unsigned long previousMillis = 0;

void setup() {
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    ApplyLEDColor(CRGB(0, 0, 255));
}

void loop() {
    ModulateLEDBrightness();
}

void ApplyLEDColor(CRGB crgb){
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = crgb;
        FastLED.show();
    }
}

void ModulateLEDBrightness(){
    currentMillis = millis();
    if ( (currentMillis - previousMillis) >= interval ){
        previousMillis = previousMillis + interval;

        switch(LED_BRIGHTNESS_DIR){
            case 0:
                LED_BRIGHTNESS += LED_BRIGHTNESS_FADE;
                break;
            case 1:
                LED_BRIGHTNESS -= LED_BRIGHTNESS_FADE;
                break;
        }
        
        if (LED_BRIGHTNESS >= LED_BRIGHTNESS_HIGH) {
            LED_BRIGHTNESS_DIR = 1;
            LED_BRIGHTNESS = LED_BRIGHTNESS_HIGH;
        } else if (LED_BRIGHTNESS <= LED_BRIGHTNESS_LOW) {
            LED_BRIGHTNESS_DIR = 0;
            LED_BRIGHTNESS = LED_BRIGHTNESS_LOW;
        }
    }

    ApplyLEDColor(CRGB(0, 0, LED_BRIGHTNESS));
}