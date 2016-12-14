// This #include statement was automatically added by the Particle IDE.
#include "EasingLibrary.h"
#include "Adafruit_Pixie.h"
#include "Pushbutton.h"
#include "neopixel.h"

#define NUMPIXELS 3 // Number of Pixies in the strip
// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D7
#define PIXEL_COUNT 192
#define PIXEL_TYPE WS2812B
Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &Serial1);
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int remoteCompleteHabit(String habit);

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
  bool lightsOut = false;
  long lastLightsOut = 0;
  long lastBorder = 0;
  /*Habit sleep;
  Habit time;
  Habit workout;*/
};

/*Color brightness(Color &color, int b) {
  Color newColor = {(color.r * b >> 8), (color.g * b >> 8), (color.b * b >> 8)};
  return newColor;
}*/

void completeHabit(state::HabitState &state, Habit &config) {
  Particle.publish("Habit Done!", String(config.sideLED));
  state.done = true;
  state.doneTime = Time.now();
  //Color color = brightness(config.color, 50);
  Color color = config.color;
  uint16_t b = 50;
  //turnOn(sweets.sideLED, 4, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
  //strip.setPixelColor(config.sideLED, strip.Color((color.r * b) >> 8, (color.g * b) >> 8, (color.b * b) >> 8));
  int rgb = strip.Color(color.r, color.g, color.b);
  strip.setPixelColor(config.sideLED, rgb);
  matrix.setPixelColor(map(config.sideLED + 1, 22), rgb);
  strip.setBrightness(50);
  strip.show();
  matrix.show();

  // 30% chance a sound clip will play.
  if(random(100) < 30) {
    digitalWrite(D6, LOW);
    delay(200);
    digitalWrite(D6, HIGH);
  }
}



void checkDone(state::HabitState &state, Habit &config) {
  if(!state.done && config.button.getSingleDebouncedPress()) {
    completeHabit(state, config);
  }
}

void checkLightsOn(state::HabitState &state, Habit &config) {
  //TODO: this isnt working as expected. throw a publish in here.
  if(state.done && state::lightsOut) {
    if(config.button.getSingleDebouncedPress()) {
      Color color = config.color;
      int rgb = strip.Color(color.r, color.g, color.b);
      strip.setPixelColor(config.sideLED, rgb);
      matrix.setPixelColor(map(config.sideLED + 1, 22), rgb);
      strip.setBrightness(50);
      strip.show();
      matrix.show();
      state::lastLightsOut = millis();
    } else if (state::lastLightsOut - millis() > 300000) {
      int rgb = strip.Color(0, 0, 0);
      strip.setPixelColor(config.sideLED, rgb);
      matrix.setPixelColor(map(config.sideLED + 1, 22), rgb);
      strip.show();
      matrix.show();
      state::lastLightsOut = millis();
    }
  }
}

void endOfDay(state::HabitState &state, Habit &config) {
  if((Time.day(state.doneTime) != Time.day()) && state.done) {
    Particle.publish("End of Day!", String(config.sideLED));
    state.done = false;
    state.doneTime = -1;
    strip.setPixelColor(config.sideLED, strip.Color(0, 0, 0));
    //strip.show();
  }
}

void morning() {
  if(state::lightsOut && Time.hour() > 8) {
    Particle.publish("Morning!");
    state::lightsOut = false;
  }
}

void setup() {
    /*Serial.begin(9600);
    while(!Serial.isConnected()) Particle.process();*/
    pinMode(D6, OUTPUT);
    digitalWrite(D6, HIGH);

    pinMode(A0, INPUT);

    Serial1.begin(115200);  // <- Alt. if using hardware serial port

    //strip.setPixelColor(sweets.sideLED, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
    strip.setBrightness(1);
    strip.show();

    matrix.begin();
    matrix.setBrightness(15);
    matrix.show(); // Initialize all pixels to 'off'
    Particle.function("habit", remoteCompleteHabit);
    Particle.publish("DSTTYFSWTYF", "Do something today that your future self will thank you for!");
}

int remoteCompleteHabit(String habit) {
  if(habit == "sweets") {
    completeHabit(state::sweets, sweets);
    return 1;
  } else if (habit == "murder") {
    completeHabit(state::murder, murder);
    return 1;
  } else if ( habit == "brush") {
    completeHabit(state::brush, brush);
    return 1;
  } else {
    return -1;
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<matrix.numPixels(); i++) {
      matrix.setPixelColor(i, Wheel((i+j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

int map(int row, int column) {
  //0-7, 64-71, 128-135
  int index = row * 8 + column; // 0 - 63
  // r: 1, c: 1. 9
  // r: 0, c: 7. 7
  // r: 3, c: 0. 24
  // r: 7, c: 7. 63
  if(column >= 8 && column <= 15) {
    index = index + 64 - 8;
    // r: 0 c: 8. 64
    // r: 1 c: 8. 73
    // r: 7 c: 16
  } else if(column >= 16 && column <= 23) {
    index = index + 2 * (64 - 8);
  }
  return index;
}

void loop() {

  checkDone(state::sweets, sweets);
  checkDone(state::murder, murder);
  checkDone(state::brush, brush);
  /*checkLightsOn(state::sweets, sweets);
  checkLightsOn(state::murder, murder);
  checkLightsOn(state::brush, brush);*/
  endOfDay(state::sweets, sweets);
  endOfDay(state::murder, murder);
  endOfDay(state::brush, brush);

  morning();

  /*matrix.setPixelColor(map(0, 0), 0, 0, 255); // Top Left board 1
  matrix.setPixelColor(map(0, 1), 0, 0, 255);
  matrix.setPixelColor(map(7, 7), 0, 0, 255);

  //matrix.setPixelColor(63, 0, 0, 255);
  matrix.setPixelColor(map(0, 8), 0, 255, 0); // Top Left board 2
  //matrix.setPixelColor(map(1, 9), 0, 255, 0);
  //matrix.setPixelColor(127, 0, 255, 0);
  matrix.setPixelColor(map(0, 16), 255, 0, 0); // Top Left board 3*/
  //matrix.setPixelColor(191, 255, 0, 0);

  /*if(millis() - state::lastBorder > 2000) {

    for(int col = 0; col < 24; col++) {
      matrix.setPixelColor(map(0, col), random(256), random(256), random(256)); // Top Left board 3
    }
    for (int col = 0; col < 24; col++) {
      matrix.setPixelColor(map(7, col), random(256), random(256), random(256)); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(map(row, 0), random(256), random(256), random(256)); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(map(row, 23), random(256), random(256), random(256)); // Top Left board 3
    }
    matrix.show();
    state::lastBorder = millis();
  }*/





  //rainbow(20);

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
    int light = analogRead(A0);
    //Particle.publish("light");

    if(light < 400 && !state::lightsOut && isAnyDone()) {
      state::lightsOut = true;
      Particle.publish("lights out!", String(light));
      int rgb = strip.Color(0, 0, 0);

      strip.setPixelColor(sweets.sideLED, rgb);
      strip.setPixelColor(murder.sideLED, rgb);
      strip.setPixelColor(brush.sideLED, rgb);
      matrix.setPixelColor(map(sweets.sideLED + 1, 22), rgb);
      matrix.setPixelColor(map(murder.sideLED + 1, 22), rgb);
      matrix.setPixelColor(map(brush.sideLED + 1, 22), rgb);

    }

    strip.show();
    //matrix.show();
    state::lastShow = millis();
  }
}

bool isAnyDone() {
  return state::sweets.done || state::murder.done || state::brush.done;
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

/*typedef struct {
    double r;       // percent
    double g;       // percent
    double b;       // percent
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0
            // s = 0, v is undefined
        out.s = 0.0;
        out.h = NAN;                            // its now undefined
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}*/

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
