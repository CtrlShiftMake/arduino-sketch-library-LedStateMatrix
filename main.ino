#include <Arduino.h>
#include <FastLED.h>

// ~~ LED MATRIX ~~

#define LED_PIN 12
#define LED_NUM 35

#define LED_INACTIVE 0
#define LED_ACTIVE 1
#define LED_DIMMED 2
#define LED_PRESSED 3

const float value_inactive = 0.0;
const float value_active = 0.5;
const float value_dimmed = 0.1;
const float value_pressed = 1.0;

struct LEDColorMap {
  CRGB inactive;
  CRGB active;
  CRGB dimmed;
  CRGB pressed;
};

LEDColorMap menu0;
LEDColorMap menu1;
LEDColorMap menu2;
LEDColorMap menu3;
LEDColorMap track;
bool LED_state[LED_NUM];
CRGB LED_color[LED_NUM];

auto timer = timer_create_default();
unsigned long timer_interval = 60;

void set_track_state(int state){
  for (int i = 0; i < LED_NUM - 4; i++){
    LED_state[i] = state;
  }
}

void set_menu_states(int m0_state, int m1_state, int m2_state, int m3_state){
  LED_state[31] = m0_state;
  LED_state[32] = m1_state;
  LED_state[33] = m2_state;
  LED_state[34] = m3_state;
}

void generate_color_map(int color_map[4][3], int color[]){
  for (int i = 0; i < 4; i++ ){
    float value;
    
    switch (i)
    {
    case LED_INACTIVE:
      value = value_inactive;
      break;
    case LED_ACTIVE:
      value = value_active;
      break;
    case LED_DIMMED:
      value = value_dimmed;
      break;
    case LED_PRESSED:
      value = value_pressed;
      break;
    default:
      value = value_inactive;
      break;
    }

    color_map[i][0] = int(float(color[0]) * value);
    color_map[i][1] = int(float(color[1]) * value);
    color_map[i][2] = int(float(color[2]) * value);
  }
}

void set_color_map(struct LEDColorMap *cm, int color_map[4][3]){
  cm->inactive = CRGB(color_map[LED_INACTIVE][0], color_map[LED_INACTIVE][1], color_map[LED_INACTIVE][2]);
  cm->active = CRGB(color_map[LED_ACTIVE][0], color_map[LED_ACTIVE][1], color_map[LED_ACTIVE][2]);
  cm->dimmed = CRGB(color_map[LED_DIMMED][0], color_map[LED_DIMMED][1], color_map[LED_DIMMED][2]);
  cm->pressed = CRGB(color_map[LED_PRESSED][0], color_map[LED_PRESSED][1], color_map[LED_PRESSED][2]);
}

void calculate_led_colors(){
  
  LEDColorMap apply;

  for (int i = 0; i < LED_NUM; i++){

    switch (i)
    {
    case 31:
      apply = menu0;
      break;
    case 32:
      apply = menu1;
      break;
    case 33:
      apply = menu2;
      break;
    case 34:
      apply = menu3;
      break;
    default:
      apply = track;
      break;
    }

    switch (LED_state[i])
    {
    case LED_INACTIVE:
      LED_color[i] = apply.inactive;
      break;
    case LED_ACTIVE:
      LED_color[i] = apply.active;
      break;
    case LED_DIMMED:
      LED_color[i] = apply.dimmed;
      break;
    case LED_PRESSED:
      LED_color[i] = apply.pressed;
      break;
    default:
      LED_color[i] = apply.dimmed;
      break;
    }
  }
}

// ~~ BUTTON MATRIX ~~



// ~~ MAIN APPLICATION ~~

const int color_white[3] = {255, 255, 255};
const int color_red[3] = {240, 25, 0};
const int color_yellow[3] = {240, 240, 0};

void setup() {

  Serial.begin(9600);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(LED_color, LED_NUM);

  int color_map[4][3];

  set_track_state(LED_INACTIVE);
  generate_color_map(color_map, color_white);
  set_color_map(&track, color_map);

  set_menu_states(LED_INACTIVE, LED_INACTIVE, LED_INACTIVE, LED_INACTIVE);
  generate_color_map(color_map, color_red);
  set_color_map(&menu0, color_map);
  set_color_map(&menu3, color_map);
  generate_color_map(color_map, color_yellow);
  set_color_map(&menu1, color_map);
  set_color_map(&menu2, color_map);
}

void loop() {
  calculate_led_colors();
  FastLED.show();
  delay(1000);
}
