// This #include statement was automatically added by the Particle IDE.
#include "21days.h"

#define NUMPIXELS 3 // Number of Pixies in the strip
// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D0
#define PIXEL_COUNT 192
#define PIXEL_TYPE WS2812B
Adafruit_Pixie strip = Adafruit_Pixie(NUMPIXELS, &Serial1);
Adafruit_NeoPixel matrix = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

int LEDinterval = 10;
Habit sweets = {
  Pushbutton(D1, false, 0),
  0,
  4,
  {0, 255, 0}
};

Habit workout = {
  Pushbutton(D2, false, 0),
  1,
  3,
  {255, 255, 0}
};

Habit sleep = {
  Pushbutton(D3, false, 0),
  2,
  5,
  {0, 158, 255}
};

Habit murder = {
  Pushbutton(D4, false, 0),
  3,
  1,
  {255, 0, 25}
};

Habit onTime = {
  Pushbutton(D5, false, 0),
  4,
  2,
  {255, 158, 0}
};

Habit brush = {
  Pushbutton(D6, false, 0),
  5,
  6,
  {144, 0, 255}
};


namespace state {
  typedef struct {
    long previousMillis;
    int brightness;
    uint8_t history [22];
    int doneTime;
  } HabitState;

  HabitState sweets = {
    0, 255, {0}, -1
  };

  HabitState murder = {
    0, 255, {0}, -1
  };

  HabitState brush = {
    0, 255, {0}, -1
  };

  HabitState sleep = {
    0, 255, {0}, -1
  };

  HabitState workout = {
    0, 255, {0}, -1
  };

  HabitState onTime = {
    0, 255, {0}, -1
  };

  long lastShow = 0;
  bool lightsOut = false;
  long lastLightsOut = 0;
  long lastBorder = 0;
  long lastNightLight = 0;
  bool nightLight = false;
  long dayStamp = 0;

  bool intro = false;
  long lastIntro = 0;
  int introColumn = 1;
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
    matrix.setPixelColor(map(config.matrixRow, 22), rgb);
  }

  void off(Habit &config) {
    sideOff(config);
    todayOff(config);
  }

  void sideOn(Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(color.r, color.g, color.b);
    strip.setPixelColor(config.sideLED, rgb);
    strip.setBrightness(127);
  }

  void todayOn(Habit &config) {
    Color color = config.color;
    int rgb = strip.Color(color.r, color.g, color.b);
    matrix.setPixelColor(map(config.matrixRow, 22), rgb);
  }

  void historyOn(state::HabitState &state, Habit &config) {
    Color c = config.color;
    // int rgb = strip.Color(color.r, color.g, color.b);
    for (unsigned int day = 0; day < sizeof(state.history); day++) {
      if(state.history[day] == 1) {
        if(day == sizeof(state.history) - 1) {
          matrix.setColorDimmed(map(config.matrixRow, day + 1), c.r, c.g, c.b, state.brightness);
        } else {
          matrix.setColorDimmed(map(config.matrixRow, day + 1), c.r, c.g, c.b, 180);
        }
      } else {
        matrix.setPixelColor(map(config.matrixRow, day + 1), 0, 0, 0);
      }
    }
  }

  void on(Habit &config) {
    sideOn(config);
    todayOn(config);
  }

  void randomBorder() {
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
  }
};

namespace buttons {
  bool isPressed(Habit &config) {
    return config.button.getSingleDebouncedPress();
  }

  bool isAnyPressed() {
    return buttons::isPressed(sweets) || buttons::isPressed(murder) || buttons::isPressed(brush) || buttons::isPressed(onTime) || buttons::isPressed(sleep) || buttons::isPressed(workout) ;
  }
};

bool isAnyDone() {
  return state::sweets.history[21] || state::murder.history[21] || state::brush.history[21] || state::onTime.history[21] || state::sleep.history[21] || state::workout.history[21];
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
  Particle.publish("Habit Done!", String(config.matrixRow));
  state.history[21] = true;
  state.doneTime = Time.now();
  Color color = config.color;
  //turnOn(sweets.sideLED, 4, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
  //strip.setPixelColor(config.sideLED + 1, strip.Color((color.r * b) >> 8, (color.g * b) >> 8, (color.b * b) >> 8));
  if(!state::lightsOut) {
    lights::on(config);
    lights::show();
    // 15% chance a sound clip will play.
    if(random(100) < 15) {
      playRandomSound();
    }
  } else {
    lights::todayOn(config);
    lights::show();
  }

}
bool up = false;
void checkDone(state::HabitState &state, Habit &config) {
  if(!state.history[21] && buttons::isPressed(config)) {
    completeHabit(state, config);
    state.previousMillis = millis();

  }
  // else if (state.history[21] && millis() - state.previousMillis > 50) {
  //   if (state.brightness < 255 && up) {
  //     state.brightness += 1;
  //     if(state.brightness == 255) {
  //       up = false;
  //     }
  //   } else if (state.brightness > 200 && !up) {
  //     state.brightness -= 1;
  //     if(state.brightness == 200) {
  //       up = true;
  //     }
  //   }
  //   matrix.setColorDimmed(lights::map(config.matrixRow, 22), config.color.r, config.color.g, config.color.b, state.brightness);
  //   state.previousMillis = millis();
  //
  //   matrix.show();
  //
  // }

}

void nightLight() {
  //TODO: this isnt working as expected. throw a publish in here.
  if(isAnyDone() && state::lightsOut) {
    if(!state::nightLight && buttons::isAnyPressed()) {
      Particle.publish("ON Lights At Night");
      if(state::sweets.history[21]) {
        lights::todayOn(sweets);
      }
      if(state::murder.history[21]) {
        lights::todayOn(murder);
      }
      if(state::brush.history[21]) {
        lights::todayOn(brush);
      }
      if(state::onTime.history[21]) {
        lights::todayOn(onTime);
      }
      if(state::sleep.history[21]) {
        lights::todayOn(sleep);
      }
      if(state::workout.history[21]) {
        lights::todayOn(workout);
      }
      lights::show();
      state::lastNightLight = millis();
      state::nightLight = true;
    } else if (state::nightLight && millis() - state::lastNightLight > 60000) {
      Particle.publish("OFF Lights At Night");
      lights::todayOff(sweets);
      lights::todayOff(murder);
      lights::todayOff(brush);
      lights::todayOff(workout);
      lights::todayOff(onTime);
      lights::todayOff(sleep);
      lights::show();
      state::nightLight = false;
    }
  }
}

void processEndOfDay(state::HabitState &state, Habit &config) {
  for(unsigned int day = 0; day < sizeof(state.history) - 1; day++) {
    state.history[day] = state.history[day+1];
  }

  if(state.history[21] && state.doneTime != -1) {
    state.history[21] = false;
    state.doneTime = -1;
    //state.history[sizeof(state.history) - 2] = true;
  } else {
    //state.history[sizeof(state.history) - 2] = false;
  }

  lights::historyOn(state, config);
  lights::off(config);
}

void weekendBorder() {
  long currentDay = state::dayStamp;
  //int rgb = strip.Color(255, 255, 255);

  for(unsigned int day = 22; day > 0; day--) {
    int weekday = Time.weekday(currentDay);
    if(weekday == 1 || weekday == 7) {
      matrix.setColorDimmed(lights::map(7, day), 255, 255, 255, 100);
      matrix.setColorDimmed(lights::map(0, day), 255, 255, 255, 100);
    } else {
      matrix.setPixelColor(lights::map(7, day), 0, 0, 0);
      matrix.setPixelColor(lights::map(0, day), 0, 0, 0);
    }
    currentDay = currentDay - 86400;
  }
}

void endOfDay() {
  if(Time.day(state::dayStamp) != Time.day()) {
    Particle.publish("End of Day!", String(state::dayStamp));

    processEndOfDay(state::sweets, sweets);
    processEndOfDay(state::murder, murder);
    processEndOfDay(state::brush, brush);
    processEndOfDay(state::onTime, onTime);
    processEndOfDay(state::workout, workout);
    processEndOfDay(state::sleep, sleep);
    weekendBorder();
    lights::show();
    state::dayStamp = Time.now();

  }
}

void morning() {
  if(state::lightsOut && Time.hour() == 8) {
    Particle.publish("Morning!");
    state::lightsOut = false;
  }
}

// void gradientHistory() {
//   for (unsigned int day = 0; day < sizeof(state::sweets.history); day++) {
//     Color c1 = hsv2rgb({120, 100, map(day, 0, 20, 0, 100)});
//     matrix.setPixelColor(lights::map(sweets.matrixRow, day + 1), c1.r, c1.g, c1.b);
//     Color c2 = hsv2rgb({350, 100, map(day, 0, 20, 0, 100)});
//     matrix.setPixelColor(lights::map(murder.matrixRow, day + 1), c2.r, c2.g, c2.b);
//     Color c3 = hsv2rgb({273, 100, map(day, 0, 20, 0, 100)});
//     matrix.setPixelColor(lights::map(brush.matrixRow, day + 1), c3.r, c3.g, c3.b);
//     Color c4 = hsv2rgb({201, 100, map(day, 0, 20, 0, 100)});
//     matrix.setPixelColor(lights::map(sleep.matrixRow, day + 1), c4.r, c4.g, c4.b);
//     Color c5 = hsv2rgb({36, 100, map(day, 0, 20, 0, 100)});
//     matrix.setPixelColor(lights::map(workout.matrixRow, day + 1), c5.r, c5.g, c5.b);
//     Color c6 = hsv2rgb({52, 100, map(day, 0, 20, 0, 100)});
//     matrix.setPixelColor(lights::map(onTime.matrixRow, day + 1), c6.r, c6.g, c6.b);
//   }
// }

void handleHistory(const char *event, const char *data) {
  // Handle the webhook response
  int length = strlen(data) + 1;
  const int brush_s = 0;
  const int murder_s = 23;
  const int sweets_s = 46;
  const int workout_s = 69;
  const int sleep_s = 92;
  const int ontime_s = 115;

  char result[length];
  strcpy(result, data);

  for (int index = 0; index < length; index++) {
    if(index >= brush_s && index < brush_s + 22) {
      //"brush twice"
      state::brush.history[index - brush_s] = result[index] - '0';
    } else if(index >= murder_s && index < murder_s + 22) {
      //"dont murder"
      state::murder.history[index - murder_s] = result[index] - '0';
    } else if(index >= sweets_s && index < sweets_s + 22) {
      //"no sweets"
      state::sweets.history[index - sweets_s] = result[index] - '0';
    } else if(index >= workout_s && index < workout_s + 22) {
      //"workout"
      state::workout.history[index - workout_s] = result[index] - '0';
    } else if(index >= sleep_s && index < sleep_s + 22) {
      //"sleep by 12am"
      state::sleep.history[index - sleep_s] = result[index] - '0';
    } else if(index >= ontime_s && index < ontime_s + 22) {
      //"on time"
      state::onTime.history[index - ontime_s] = result[index] - '0';
    }
  }

  lights::historyOn(state::brush, brush);
  lights::historyOn(state::murder, murder);
  lights::historyOn(state::sweets, sweets);
  lights::historyOn(state::workout, workout);
  lights::historyOn(state::sleep, sleep);
  lights::historyOn(state::onTime, onTime);

  lights::show();
  // char send[length];
  // strcpy(send, data);
  // const int buff = JSON_ARRAY_SIZE(6) + JSON_OBJECT_SIZE(2) + 238;
  //
  // Particle.publish("Lengths", String(length) + " " + String(strlen(send) + 1) + " " + String(System.freeMemory()) + " " + String(buff));
  // Particle.publish("result", String(send));
  // delay(1000);

  // char json[length];
  // strcpy(json, data);
  //
  // StaticJsonBuffer<1000> jsonBuffer;
  //
  // JsonObject& root = jsonBuffer.parseObject(json);
  //
  // if (!root.success())
  // {
  //   Particle.publish("parseObject() failed", String(System.freeMemory()));
  // } else {
  //   Particle.publish("parseObject() success!", String(System.freeMemory()));

    // const char* history = root["history"];
    //
    // Particle.publish("HISTORY", String(history));

    // JsonArray& order = root["order"];
    // char arr[6];
    // order.printTo(arr, buff);
    // Particle.publish("ORDER", String(arr) + " " + String(order.size()));
    // for(int index = 0; index < brushTwice.size(); index++) {
    //   uint8_t isDone = (uint8_t)brushTwice[index];
    //   state::brush.history[index] = isDone;
    // }
    // Particle.publish("bt", String(state::brush.history[0]) + String(state::brush.history[20]));
    //
    // lights::historyOn(state::brush, brush);
    // lights::show();

  // }



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
    matrix.setBrightness(100);
    Time.zone(-8);

    state::dayStamp = Time.now();
    weekendBorder();
    //gradientHistory();
    delay(100);
    matrix.show(); // Initialize all pixels to 'off'

    // hsv test = {350, 100, 50};
    // Color t2 = hsv2rgb(test);
    // Particle.publish("test", String(t2.r) + String(t2.b) + String(t2.g));
    Particle.function("habit", remoteCompleteHabit);
    Particle.publish("DSTTYFSWTYF", "Do something today that your future self will thank you for!");
    Particle.publish("Time", Time.format(state::dayStamp, TIME_FORMAT_ISO8601_FULL));
    Particle.publish("history", "", PRIVATE);
    Particle.subscribe("hook-response/history", handleHistory, MY_DEVICES);
}

void randomHistory() {
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
  } else if (habit == "sleep") {
    completeHabit(state::sleep, sleep);
    return 1;
  } else if (habit == "onTime") {
    completeHabit(state::onTime, onTime);
    return 1;
  } else if( habit == "workout") {
    completeHabit(state::workout, workout);
    return 1;
  } else {
    return -1;
  }
}

// // Input a value 0 to 255 to get a color value.
// // The colours are a transition r - g - b - back to r.
// uint32_t Wheel(byte WheelPos) {
//   if(WheelPos < 85) {
//    return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//   } else if(WheelPos < 170) {
//    WheelPos -= 85;
//    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//   } else {
//    WheelPos -= 170;
//    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//   }
// }
//
// void rainbow(uint8_t wait) {
//   uint16_t i, j;
//
//   for(j=0; j<256; j++) {
//     for(i=0; i<matrix.numPixels(); i++) {
//       matrix.setPixelColor(i, Wheel((i+j) & 255));
//     }
//     matrix.show();
//     delay(wait);
//   }
// }

void handleLightsOut(int light) {
  state::lightsOut = true;
  state::lastLightsOut = millis();
  Particle.publish("lights out!", String(light));
  lights::borderOff();
  lights::off(sweets);
  lights::off(murder);
  lights::off(brush);
  lights::show();
}

void loop() {

  checkDone(state::sweets, sweets);
  checkDone(state::murder, murder);
  checkDone(state::brush, brush);
  checkDone(state::onTime, onTime);
  checkDone(state::sleep, sleep);
  checkDone(state::workout, workout);
  endOfDay();
  nightLight();
  morning();

  if(!state::intro) {
    if(millis() - state::lastIntro > 1) {
      for(int row = 1; row < 7; row++) {
        int col = state::introColumn;
        if(col < 23) {
          matrix.setColorDimmed(lights::map(row, 23 - state::introColumn), 255, 255, 255, 200);
        }
        if(col > 1 && col < 24) {
          matrix.setColorDimmed(lights::map(row, 24 - state::introColumn), 255, 255, 255, 100);
        }
        if(col > 2 && col < 25) {
          matrix.setColorDimmed(lights::map(row, 25 - state::introColumn), 255, 255, 255, 50);
        }
        if(col > 3 && col < 26) {
          matrix.setColorDimmed(lights::map(row, 26 - state::introColumn), 255, 255, 255, 0);
        }
      }

      if(state::introColumn == 25) {
        state::intro = true;

        lights::historyOn(state::brush, brush);
        lights::historyOn(state::murder, murder);
        lights::historyOn(state::sweets, sweets);
        lights::historyOn(state::workout, workout);
        lights::historyOn(state::sleep, sleep);
        lights::historyOn(state::onTime, onTime);

      } else {
        state::introColumn += 1;
      }
      lights::show();
      state::lastIntro = millis();
    }

  }

  // if(millis() - state::lastBorder > 2000) {
  //   if(!state::lightsOut) {
  //     lights::randomBorder();
  //   }
  //   //Particle.publish("state", "sweets: " + String(state::sweets.done) + " " + String(sweets.sideLED) + " " + String(state::sweets.doneTime) + " murder: " + state::murder.done + " " + String(murder.sideLED) + " brush: " + String(state::brush.done) );
  //   state::lastBorder = millis();
  // }

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

// hsv rgb2hsv(Color in)
// {
//     hsv         out;
//     double      min, max, delta;
//
//     min = in.r < in.g ? in.r : in.g;
//     min = min  < in.b ? min  : in.b;
//
//     max = in.r > in.g ? in.r : in.g;
//     max = max  > in.b ? max  : in.b;
//
//     out.v = max;                                // v
//     delta = max - min;
//     if (delta < 0.00001)
//     {
//         out.s = 0;
//         out.h = 0; // undefined, maybe nan?
//         return out;
//     }
//     if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
//         out.s = (delta / max);                  // s
//     } else {
//         // if max is 0, then r = g = b = 0
//             // s = 0, v is undefined
//         out.s = 0.0;
//         out.h = 0.0;                            // its now undefined
//         return out;
//     }
//     if( in.r >= max )                           // > is bogus, just keeps compilor happy
//         out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
//     else
//     if( in.g >= max )
//         out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
//     else
//         out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan
//
//     out.h *= 60.0;                              // degrees
//
//     if( out.h < 0.0 )
//         out.h += 360.0;
//
//     return out;
// }
//
//
// Color hsv2rgb(hsv in)
// {
//     double      hh, p, q, t, ff;
//     long        i;
//     Color       out;
//
//     if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
//         out.r = in.v;
//         out.g = in.v;
//         out.b = in.v;
//         return out;
//     }
//     hh = in.h;
//     if(hh >= 360.0) hh = 0.0;
//     hh /= 60.0;
//     i = (long)hh;
//     ff = hh - i;
//     p = in.v * (1.0 - in.s);
//     q = in.v * (1.0 - (in.s * ff));
//     t = in.v * (1.0 - (in.s * (1.0 - ff)));
//
//     switch(i) {
//     case 0:
//         out.r = in.v;
//         out.g = t;
//         out.b = p;
//         break;
//     case 1:
//         out.r = q;
//         out.g = in.v;
//         out.b = p;
//         break;
//     case 2:
//         out.r = p;
//         out.g = in.v;
//         out.b = t;
//         break;
//
//     case 3:
//         out.r = p;
//         out.g = q;
//         out.b = in.v;
//         break;
//     case 4:
//         out.r = t;
//         out.g = p;
//         out.b = in.v;
//         break;
//     case 5:
//     default:
//         out.r = in.v;
//         out.g = p;
//         out.b = q;
//         break;
//     }
//     out.r = map(out.r, 0, 100, 0, 255);
//     out.g = map(out.g, 0, 100, 0, 255);
//     out.b = map(out.b, 0, 100, 0, 255);
//
//     return out;
// }
