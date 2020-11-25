#ifndef LedMatrix_h
#define LedMatrix_h

#include "Arduino.h"
#include "FastLED.h"

#define LED_INACTIVE 0
#define LED_ACTIVE 1
#define LED_DIMMED 2
#define LED_PRESSED 3

const int LED_NUM = 35;

struct Color {
    int r;
    int g;
    int b;

    Color& operator=(Color other){
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
    }

    Color& operator*(float mult){
        this->r = int((float(this->r) * mult));
        this->g = int((float(this->g) * mult));
        this->b = int((float(this->b) * mult));
    }
};

struct ColorMap {
    Color inactive;
    Color active;
    Color dimmed;
    Color pressed;
};

class LedMatrix
{
    const float value_inactive = 0.0;
    const float value_active = 0.5;
    const float value_dimmed = 0.1;
    const float value_pressed = 1.0;

    public : LedMatrix();
        void loop();
        void setStateByIndex(int index, int state);
        void setTrackStates(int state);
        void setMenuStates(int m0_state, int m1_state, int m2_state, int m3_state);
        void setTrackColor(int r, int g, int b);
        void setMenuColor(int index, int r, int g, int b);
        void getColorMatrix(CRGB colorMatrix[LED_NUM]);

    private:
        float global_brightness = 1.0;
        int led_state[LED_NUM];
        ColorMap led_colormap[LED_NUM];
        Color led_display[LED_NUM];
        unsigned long time_current;
        unsigned long time_start;
        unsigned long time_interval = 360;
        ColorMap calculateStateColors(Color color);
        void applyColorsToDisplayMatrix();
        bool tickClock();
};

#endif