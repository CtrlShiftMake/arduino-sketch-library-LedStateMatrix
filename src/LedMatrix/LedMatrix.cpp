#include "Arduino.h"
#include "LedMatrix.h"

LedMatrix::LedMatrix()
{
    for (int i = 0; i < LED_NUM; i++)
    {
        led_state[i] = LED_INACTIVE;
    }

    setTrackColor(255, 255, 255);
    setMenuColor(0, 255, 255, 255);
    setMenuColor(1, 255, 255, 255);
    setMenuColor(2, 255, 255, 255);
    setMenuColor(3, 255, 255, 255);

    applyColorsToDisplayMatrix();

    time_current = millis();
    time_start = time_current;
}

void LedMatrix::loop()
{
    if ( !tickClock() )
        return;
    
    applyColorsToDisplayMatrix();
}

void LedMatrix::setStateByIndex(int index, int state)
{
    led_state[index] = state;
}

void LedMatrix::setTrackStates(int state)
{
    for (int i = 0; i < LED_NUM - 4; i++)
    {
        setStateByIndex(i, state);
    }
}

void LedMatrix::setMenuStates(int m0_state, int m1_state, int m2_state, int m3_state)
{
    setStateByIndex(31, m0_state);
    setStateByIndex(32, m1_state);
    setStateByIndex(33, m2_state);
    setStateByIndex(34, m3_state);
}

void LedMatrix::setMenuColor(int index, int r, int g, int b)
{
    Color color = {r, g, b};
    led_colormap[LED_NUM - (4 - index)] = calculateStateColors(color);
}

void LedMatrix::setTrackColor(int r, int g, int b)
{
    Color color = {r, g, b};
    ColorMap colorMap = calculateStateColors(color);

    for (int i = 0; i < LED_NUM - 4; i++)
    {
        led_colormap[i] = colorMap;
    }
}

ColorMap LedMatrix::calculateStateColors(Color color)
{
    ColorMap colorMap;
    colorMap.active = color * value_active;
    colorMap.inactive = color * value_inactive;
    colorMap.dimmed = color * value_dimmed;
    colorMap.pressed = color * value_pressed;

    return colorMap;
}

void LedMatrix::applyColorsToDisplayMatrix()
{
    for (int i = 0; i < LED_NUM; i++ )
    {
        switch (led_state[i])
        {
        case LED_ACTIVE:
            led_display[i] = led_colormap[i].active * global_brightness;
            break;

        case LED_INACTIVE:
            led_display[i] = led_colormap[i].inactive * global_brightness;
            break;
        
        case LED_DIMMED:
            led_display[i] = led_colormap[i].dimmed * global_brightness;
            break;
        
        case LED_PRESSED:
            led_display[i] = led_colormap[i].pressed * global_brightness;
            break;

        default:
            led_display[i] = led_colormap[i].inactive * global_brightness;
            break;
        }
    }
}

void LedMatrix::getColorMatrix(CRGB colorMatrix[LED_NUM])
{
    for (int i = 0; i < LED_NUM; i++)
    {
        colorMatrix[i] = CRGB(led_display[i].r, led_display[i].g, led_display[i].b);
    }
}

bool LedMatrix::tickClock() {
    time_current = millis();
    if (time_current - time_start >= time_interval)
    {
        time_start = time_current;
        return true;
    }
    return false;
}