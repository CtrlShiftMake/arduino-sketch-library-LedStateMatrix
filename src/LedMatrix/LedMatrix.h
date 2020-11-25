#ifndef LedMatrix_h
#define LedMatrix_h

#include "Arduino.h"
#include "FastLED.h"

#define LED_PIN 12

#define LED_INACTIVE '0'
#define LED_ACTIVE '1'
#define LED_DIMMED '2'
#define LED_PRESSED '3'

#define VAL_INACTIVE 0.0f
#define VAL_ACTIVE 0.5f
#define VAL_DIMMED 0.1f
#define VAL_PRESSED 1.0f

#define FUNC_0 31
#define FUNC_1 32
#define FUNC_2 33
#define FUNC_3 34

#define LED_NUM 35
#define LED_ARRAY_X 5
#define LED_ARRAY_Y 7

struct CRGBStateMap {
    CRGB inactive;
    CRGB active;
    CRGB dimmed;
    CRGB pressed;
};

class LedMatrix
{
    public : LedMatrix();
        void init();
        bool tick();
        void setInputsColor(int r, int g, int b);
        void setFunctionColor(int index, int r, int g, int b);
        void setState(int x, int y, char state);
        void applyToCRGBArray(CRGB matrix[LED_NUM]);

    private:
        char led_state[LED_NUM];
        CRGBStateMap colorInput;
        CRGBStateMap colorFunc0;
        CRGBStateMap colorFunc1;
        CRGBStateMap colorFunc2;
        CRGBStateMap colorFunc3;
        void generateStateMap(CRGBStateMap *stateMap, int r, int g, int b);
        CRGB getColorForState(int i, CRGBStateMap stateMap);
};

#endif