/*
 * Project 21 Days
 * Description:
 * Author: Chris Gervang
 * Date:
 */

#include "EasingLibrary.h"

#include "strip.h"
#include "matrix.h"

#include "helpers.h"

#include "buttons.h"
#include "color.h"
#include "habit.h"
#include "lights.h"
#include "state.h"
#include "sound.h"

#include "history.h"

#include "demo.h"

int LEDinterval = 10;

/*Color brightness(Color &color, int b) {
 Color newColor = {(color.r * b >> 8), (color.g * b >> 8), (color.b * b >> 8)};
 return newColor;
}*/

int remoteCompleteHabit(String habit);
void handleHistory(const char *event, const char *data);

typedef struct {
  int duration;
  bool looping;
} AnimationConfig;

typedef struct {
  int lastFrame;
} AnimationState;

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



int remoteCompleteHabit(String habit) {
 if(habit == "no sweets") {
   completeHabit(state::sweets, sweets);
   return 1;
 } else if (habit == "dont murder") {
   completeHabit(state::murder, murder);
   return 1;
 } else if ( habit == "brush twice") {
   completeHabit(state::brush, brush);
   return 1;
 } else if (habit == "sleep by 12am") {
   completeHabit(state::sleep, sleep);
   return 1;
 } else if (habit == "on time") {
   completeHabit(state::onTime, onTime);
   return 1;
 } else if( habit == "workout") {
   completeHabit(state::workout, workout);
   return 1;
 } else {
   return -1;
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
 lights::show();
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

// loop() runs over and over again, as quickly as it can execute.
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
