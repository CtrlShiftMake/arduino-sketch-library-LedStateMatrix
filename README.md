# LedStateMatrix

Arduino C++ code for creating a matrix of LEDs with states that change the display brightness.

The library was created for a project where an array of arcade push buttons (5 x 7) would be illuminated from below by addressable LEDs, where the color of the button would indicate it's function and the brightness it's state:

- Inactive: `0.0f` the button is off, no interaction
- Active: `0.5f` the button is on, interaction possible
- Dimmed: `0.1f` the button is on, no interaction currently possible
- Pressed: `1.0f` the button is on and being pressed by the user

This library is just an example use-case and the functions available won't cover every use-case, but it should be a good starting point to adapt it to your own needs. Please be aware that `FastLED` is require for this example code to function.

## How to Use

You can view the example code in `main.ino` but here's a quick example usage which will set all of the buttons to a random color, then set {0, 3} to dimmed. It is possible to assign both unique colors and states to each LED in the matrix.

```
#include "src/LedStateMatrix/LedStateMatrix.h"
#include <FastLED.h>

CRGB leds[LED_NUM];
LedStateMatrix l_state;

void setup(){
    l_state.init();
    l_state.applyToCRGBArray(leds);
}

void loop(){
    int r = random(0, 255);
    int g = random(0, 255);
    int b = random(0, 255);

    l_state.setInputsColor(r, g, b);

    l_state.setState(0, 3, LED_DIMMED);
}
```