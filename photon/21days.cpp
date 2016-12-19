// This #include statement was automatically added by the Particle IDE.
#include "21days.h"

#define NUMPIXELS 3 // Number of Pixies in the strip
// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D7
#define PIXEL_COUNT 192
#define PIXEL_TYPE WS2812B
Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &Serial1);
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int LEDinterval = 10;
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
  {144, 0, 255}
};
Habit sleep = {
  Pushbutton(D3, false, 0),
  3,
  {0, 158, 255}
};

Habit workout = {
  Pushbutton(D4, false, 0),
  4,
  {255, 255, 0}
};

Habit onTime = {
  Pushbutton(D5, false, 0),
  5,
  {255, 158, 0}
};

namespace state {
  typedef struct {
    long previousMillis;
    int brightness;
    bool done;
    bool history [21];
    int doneTime;
  } HabitState;

  HabitState sweets = {
    0, 0, 0, {false}, -1
  };

  HabitState murder = {
    0, 0, 0, {false}, -1
  };

  HabitState brush = {
    0, 0, 0, {false}, -1
  };

  HabitState sleep = {
    0, 0, 0, {false}, -1
  };

  HabitState workout = {
    0, 0, 0, {false}, -1
  };

  HabitState onTime = {
    0, 0, 0, {false}, -1
  };

  long lastShow = 0;
  bool lightsOut = false;
  long lastLightsOut = 0;
  long lastBorder = 0;
  long lastNightLight = 0;
  bool nightLight = false;
};


namespace lights {
  int map(int row, int column) {
    //0-7, 64-71, 128-135
    int LEDsPerBoard = 64;
    int boardWidth = 8;
    int index = row * boardWidth + column; // 0 - 63
    // r: 1, c: 1. 9
    // r: 0, c: 7. 7
    // r: 3, c: 0. 24
    // r: 7, c: 7. 63
    if(column >= 8 && column <= 15) {
      index = index + LEDsPerBoard - boardWidth;
      // r: 0 c: 8. 64
      // r: 1 c: 8. 73
      // r: 7 c: 16
    } else if(column >= 16 && column <= 23) {
      index = index + 2 * (LEDsPerBoard - boardWidth);
    }
    return index;
  }

  void show() {
    strip.show();
    delay(100);
    matrix.show();
  }

  void sideOff(Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(0, 0, 0);
    strip.setPixelColor(config.sideLED, rgb);
  }

  void todayOff(Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(0, 0, 0);
    matrix.setPixelColor(map(config.sideLED + 1, 22), rgb);
  }

  void off(Habit &config) {
    sideOff(config);
    todayOff(config);
    show();
  }

  void sideOn(Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(color.r, color.g, color.b);
    strip.setPixelColor(config.sideLED, rgb);
    strip.setBrightness(50);
  }

  void todayOn(Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(color.r, color.g, color.b);
    matrix.setPixelColor(map(config.sideLED + 1, 22), rgb);
  }

  void historyOn(state::HabitState &state, Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(color.r, color.g, color.b);
    for (unsigned int day = 0; day < sizeof(state.history); day++) {
      if(state.history[day]) {
        matrix.setPixelColor(map(config.sideLED + 1, day + 1), rgb);
      }
    }
  }

  void on(Habit &config) {
    sideOn(config);
    todayOn(config);
    show();
  }

  void borderOn() {
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
  }

  void borderOff() {
    for(int col = 0; col < 24; col++) {
      matrix.setPixelColor(map(0, col), 0,0,0); // Top Left board 3
    }
    for (int col = 0; col < 24; col++) {
      matrix.setPixelColor(map(7, col), 0,0,0); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(map(row, 0), 0,0,0); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(map(row, 23), 0,0,0); // Top Left board 3
    }
    matrix.show();
  }
};

namespace buttons {
  bool isPressed(Habit &config) {
    return config.button.getSingleDebouncedPress();
  }

  bool isAnyPressed() {
    return buttons::isPressed(sweets) || buttons::isPressed(murder) || buttons::isPressed(brush);
  }
};

bool isAnyDone() {
  return state::sweets.done || state::murder.done || state::brush.done;
}

void playRandomSound() {
  digitalWrite(D6, LOW);
  delay(200);
  digitalWrite(D6, HIGH);
}

/*Color brightness(Color &color, int b) {
  Color newColor = {(color.r * b >> 8), (color.g * b >> 8), (color.b * b >> 8)};
  return newColor;
}*/

void completeHabit(state::HabitState &state, Habit &config) {
  Particle.publish("Habit Done!", String(config.sideLED));
  state.done = true;
  state.doneTime = Time.now();
  Color color = config.color;
  //turnOn(sweets.sideLED, 4, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
  //strip.setPixelColor(config.sideLED, strip.Color((color.r * b) >> 8, (color.g * b) >> 8, (color.b * b) >> 8));
  if(!state::lightsOut) {
    lights::on(config);
    // 30% chance a sound clip will play.
    if(random(100) < 30) {
      playRandomSound();
    }
  } else {
    lights::todayOn(config);
    lights::show();
  }

}

void checkDone(state::HabitState &state, Habit &config) {
  if(!state.done && buttons::isPressed(config)) {
    completeHabit(state, config);
  }
}

void nightLight() {
  //TODO: this isnt working as expected. throw a publish in here.
  if(isAnyDone() && state::lightsOut) {
    if(!state::nightLight && buttons::isAnyPressed()) {
      Particle.publish("ON Lights At Night");
      if(state::sweets.done) {
        lights::todayOn(sweets);
      }
      if(state::murder.done) {
        lights::todayOn(murder);
      }
      if(state::brush.done) {
        lights::todayOn(brush);
      }
      lights::show();
      state::lastNightLight = millis();
      state::nightLight = true;
    } else if (state::nightLight && millis() - state::lastNightLight > 60000) {
      Particle.publish("OFF Lights At Night");
      lights::todayOff(sweets);
      lights::todayOff(murder);
      lights::todayOff(brush);
      lights::show();
      state::nightLight = false;
    }
  }
}

void endOfDay(state::HabitState &state, Habit &config) {
  if(state.doneTime != -1 && (Time.day(state.doneTime) != Time.day()) && state.done) {
    Particle.publish("End of Day!", String(config.sideLED));
    state.done = false;
    state.doneTime = -1;
    lights::off(config);
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

    strip.setBrightness(1);
    strip.show();

    matrix.begin();
    matrix.setBrightness(15);
    //bool proto [] = {false, false, true, true, false, false, false, true, true, false, true, false, true, true, false, false, false, true, true, false, true};
    for (unsigned int day = 0; day < sizeof(state::sweets.history); day++) {
      state::sweets.history[day] = (random(100) < 49);
      state::murder.history[day] = (random(100) < 49);
      state::brush.history[day] = (random(100) < 49);
      state::sleep.history[day] = (random(100) < 49);
      state::workout.history[day] = (random(100) < 49);
      state::onTime.history[day] = (random(100) < 49);
    }
    lights::historyOn(state::sweets, sweets);
    lights::historyOn(state::brush, brush);
    lights::historyOn(state::murder, murder);
    lights::historyOn(state::sleep, sleep);
    lights::historyOn(state::workout, workout);
    lights::historyOn(state::onTime, onTime);

    delay(100);
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





void handleLightsOut(int light) {
  state::lightsOut = true;
  state::lastLightsOut = millis();
  Particle.publish("lights out!", String(light));
  lights::borderOff();
  lights::off(sweets);
  lights::off(murder);
  lights::off(brush);
}

void loop() {

  checkDone(state::sweets, sweets);
  checkDone(state::murder, murder);
  checkDone(state::brush, brush);
  nightLight();
  endOfDay(state::sweets, sweets);
  endOfDay(state::murder, murder);
  endOfDay(state::brush, brush);

  morning();

  /*matrix.setPixelColor(map(0, 0), 0, 0, 255); // Top Left board 1
  matrix.setPixelColor(map(0, 1), 0, 0, 255);
  matrix.setPixelColor(map(7, 7), 0, 0, 255);
  matrix.setPixelColor(map(0, 8), 0, 255, 0); // Top Left board 2
  matrix.setPixelColor(map(0, 16), 255, 0, 0); // Top Left board 3*/

  if(millis() - state::lastBorder > 2000) {
    if(!state::lightsOut) {
      lights::borderOn();
    }
    //Particle.publish("state", "sweets: " + String(state::sweets.done) + " " + String(sweets.sideLED) + " " + String(state::sweets.doneTime) + " murder: " + state::murder.done + " " + String(murder.sideLED) + " brush: " + String(state::brush.done) );
    state::lastBorder = millis();
  }

    /*turnOn(0, 4, strip.Color(144, 0, 255));
    turnOn(1, 4, strip.Color(0, 158, 255));
    turnOn(2, 4, strip.Color(0, 255, 0));
    turnOn(3, 4, strip.Color(255, 225, 0));
    turnOn(0, 4, strip.Color(255, 158, 0));
    turnOn(1, 4, strip.Color(255, 0, 37));*/

  if(millis() - state::lastShow > 1000) {
    int light = analogRead(A0);

    if(light < 200 && !state::lightsOut && isAnyDone()) {
      handleLightsOut(light);
    }

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
