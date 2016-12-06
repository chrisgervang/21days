// This #include statement was automatically added by the Particle IDE.
#include "EasingLibrary.h"
#include "Adafruit_Pixie.h"
#include "Pushbutton.h"

#define NUMPIXELS 3 // Number of Pixies in the strip

Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &Serial1);

typedef struct {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} Color;

typedef struct {
  Pushbutton button;
  int sideLED;
  Color color;
} Habit;

Habit sweets = {
  Pushbutton(D0, false, 0),
  0,
  {0, 255, 0}
};

Habit murder = {
  Pushbutton(D1, false, 0),
  1,
  {255, 0, 25}
};

Habit brush = {
  Pushbutton(D2, false, 0),
  2,
  {0, 158, 255}
};

int LEDinterval = 10;

namespace state {
  typedef struct {
    long previousMillis;
    int brightness;
    bool done;
    int doneTime;
  } HabitState;

  HabitState sweets = {
    0, 0, 0, -1
  };

  HabitState murder = {
    0, 0, 0, -1
  };

  HabitState brush = {
    0, 0, 0, -1
  };

  long lastShow = 0;
  /*Habit sleep;
  Habit time;
  Habit workout;*/
};

/*Color brightness(Color &color, int b) {
  Color newColor = {(color.r * b >> 8), (color.g * b >> 8), (color.b * b >> 8)};
  return newColor;
}*/

void checkIfDone(state::HabitState &state, Habit &config) {
  if(config.button.getSingleDebouncedPress() && !state.done) {
    Particle.publish("Habit Done!", String(config.sideLED));
    state.done = true;
    state.doneTime = Time.now();
    //Color color = brightness(config.color, 50);
    Color color = config.color;
    uint16_t b = 50;
    //turnOn(sweets.sideLED, 4, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
    //strip.setPixelColor(config.sideLED, strip.Color((color.r * b) >> 8, (color.g * b) >> 8, (color.b * b) >> 8));
    strip.setPixelColor(config.sideLED, strip.Color(color.r, color.g, color.b));
    strip.setBrightness(50);
    strip.show();
  }
}

void endOfDay(state::HabitState &state, Habit &config) {
  if((Time.minute(state.doneTime) != Time.minute()) && state.done) {
    Particle.publish("End of Day!", String(config.sideLED));
    state.done = false;
    state.doneTime = -1;
    strip.setPixelColor(config.sideLED, strip.Color(0, 0, 0));
    //strip.show();
  }
}

void setup() {
    /*Serial.begin(9600);
    while(!Serial.isConnected()) Particle.process();*/

    Serial1.begin(115200);  // <- Alt. if using hardware serial port

    //strip.setPixelColor(sweets.sideLED, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
    strip.setBrightness(1);
    strip.show();
    Particle.publish("DSTTYFSWTYF", "Do something today that your future self will thank you for!");
}

void loop() {

  checkIfDone(state::sweets, sweets);
  checkIfDone(state::murder, murder);
  checkIfDone(state::brush, brush);
  endOfDay(state::sweets, sweets);
  endOfDay(state::murder, murder);
  endOfDay(state::brush, brush);
    //uint16_t i;
    //String str = "";

    /*turnOn(0, 4, strip.Color(144, 0, 255));
    turnOn(1, 4, strip.Color(0, 158, 255));
    turnOn(2, 4, strip.Color(0, 255, 0));
    turnOn(3, 4, strip.Color(255, 225, 0));
    turnOn(0, 4, strip.Color(255, 158, 0));
    turnOn(1, 4, strip.Color(255, 0, 37));*/

    //turnOn(1, 4, strip.Color(255, 0, 0));

    // for(j=1; j<256; j++) { // 5 cycles of all colors on wheel
    //     strip.setBrightness(j);
    //     strip.show();
    //     delay(10);
    // }
    // delay(100);
    // for(i=255; i>0; i--) { // 5 cycles of all colors on wheel
    //     strip.setBrightness(i);
    //     strip.show();
    //     delay(10);
    // }
    /*uint32_t c = strip.getPixelColor(0);
    uint8_t r = (uint8_t)(c >> 16),
            g = (uint8_t)(c >>  8),
            b = (uint8_t)c;
    str = String((int)r) + String((int)g) + String((int)b) + " " + String((int)strip.getPixelColor(0));*/
    /*Particle.publish("rgbb", String((int)strip.getBrightness()));
    delay(1000);*/
    //   rainbowCycle(10);
  if(millis() - state::lastShow > 1000) {
    strip.show();
    state::lastShow = millis();
  }
}




void turnOn(uint8_t pixel, uint8_t duration, uint32_t color) {
    CubicEase back;
    back.setDuration(4);  // 2 units, which we will interpret as seconds
    back.setTotalChangeInPosition(25);
    uint16_t i;

    double easedPosition,t=0;
    for(int i=0;i<=duration*100;i++) {
        easedPosition=back.easeIn(t);
        t+=0.01;
        strip.setPixelColor(pixel, color);
        strip.setBrightness(easedPosition);
        strip.show();
        delay(10);
    }
}

// // Slightly different, this makes the rainbow equally distributed throughout
// void rainbowCycle(uint8_t wait) {
//     uint16_t i, j;

//     for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//         for(i=0; i< NUMPIXELS; i++) {
//             strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//         }
//         strip.show();
//         delay(wait);
//     }
// }


// // Input a value 0 to 255 to get a color value.
// // The colours are a transition r - g - b - back to r.
// uint32_t Wheel(byte WheelPos) {
//     if(WheelPos < 85) {
//         return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//     } else if(WheelPos < 170) {
//         WheelPos -= 85;
//         return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//     } else {
//         WheelPos -= 170;
//         return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//     }
// }
