#include "LedStateMatrix.h"
#include "Arduino.h"

LedStateMatrix::LedStateMatrix() {}

void LedStateMatrix::init() {

  for (int x = 0; x < LED_ARRAY_X; x++) {
    for (int y = 0; y < LED_ARRAY_Y; y++) {
      setState(x, y, LED_INACTIVE);
    }
  }

  setInputsColor(255, 255, 255);
  setFunctionColor(FUNC_0, 255, 0, 0);
  setFunctionColor(FUNC_1, 0, 255, 0);
  setFunctionColor(FUNC_2, 0, 255, 0);
  setFunctionColor(FUNC_3, 255, 0, 0);
}

void LedStateMatrix::setInputsColor(int r, int g, int b) {
  generateStateMap(&colorInput, r, g, b);
}

void LedStateMatrix::setFunctionColor(int index, int r, int g, int b) {
  switch (index) {
  case FUNC_0:
    generateStateMap(&colorFunc0, r, g, b);
    break;

  case FUNC_1:
    generateStateMap(&colorFunc1, r, g, b);
    break;

  case FUNC_2:
    generateStateMap(&colorFunc2, r, g, b);
    break;

  case FUNC_3:
    generateStateMap(&colorFunc3, r, g, b);
    break;

  default:
    break;
  }
}

void LedStateMatrix::generateStateMap(CRGBStateMap *stateMap, int r, int g,
                                      int b) {
  stateMap->inactive =
      CRGB(r * VAL_INACTIVE, g * VAL_INACTIVE, b * VAL_INACTIVE);
  stateMap->active = CRGB(r * VAL_ACTIVE, g * VAL_ACTIVE, b * VAL_ACTIVE);
  stateMap->dimmed = CRGB(r * VAL_DIMMED, g * VAL_DIMMED, b * VAL_DIMMED);
  stateMap->pressed = CRGB(r * VAL_PRESSED, g * VAL_PRESSED, b * VAL_PRESSED);
}

void LedStateMatrix::setState(int x, int y, char state) {
  int index = x + (x * (LED_ARRAY_Y - 1)) + y;
  if (index >= 0 && index < LED_NUM) {
    led_state[index] = state;
  }
}

void LedStateMatrix::applyToCRGBArray(CRGB matrix[LED_NUM]) {
  for (int i = 0; i < LED_NUM; i++) {
    switch (i) {
    case FUNC_0:
      matrix[i] = getColorForState(i, colorFunc0);
      break;

    case FUNC_1:
      matrix[i] = getColorForState(i, colorFunc1);
      break;

    case FUNC_2:
      matrix[i] = getColorForState(i, colorFunc2);
      break;

    case FUNC_3:
      matrix[i] = getColorForState(i, colorFunc3);
      break;

    default:
      matrix[i] = getColorForState(i, colorInput);
      break;
    }
  }
}

CRGB LedStateMatrix::getColorForState(int i, CRGBStateMap stateMap) {
  switch (led_state[i]) {
  case LED_INACTIVE:
    return stateMap.inactive;
    break;

  case LED_ACTIVE:
    return stateMap.active;
    break;

  case LED_DIMMED:
    return stateMap.dimmed;
    break;

  case LED_PRESSED:
    return stateMap.pressed;
    break;

  default:
    return stateMap.inactive;
    break;
  }
}