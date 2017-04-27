/*
 * Project 21 Days
 * Description:
 * Author: Chris Gervang
 * Date:
 */

// Depenency Order
#include "EasingLibrary.h"
#include "strip.h"
#include "matrix.h"
#include "color.h"
#include "sound.h"
#include "habit.h"
#include "lights.h"
#include "state.h"


#include "helpers.h"
#include "buttons.h"

#include "history.h"
#include "borders.h"
#include "actions.h"
#include "demo.h"

STARTUP(WiFi.selectAntenna(ANT_EXTERNAL));

int LEDinterval = 10;

/*Color brightness(Color &color, int b) {
 Color newColor = {(color.r * b >> 8), (color.g * b >> 8), (color.b * b >> 8)};
 return newColor;
}*/

int remoteCompleteHabit(String habit);

struct AnimationConfig{
  int duration;
  bool looping;
};

struct AnimationState {
  int lastFrame;
};

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
 for(unsigned int day = 0; day < DAYS - 1; day++) {
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
   borders::weekend();
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

int remoteCompleteHabit(String habit) {
 if(habit == "no sweets") {
   actions::completeHabit(state::sweets, sweets);
   return 1;
 } else if (habit == "dont murder") {
   actions::completeHabit(state::murder, murder);
   return 1;
 } else if ( habit == "brush twice") {
   actions::completeHabit(state::brush, brush);
   return 1;
 } else if (habit == "sleep by 12am") {
   actions::completeHabit(state::sleep, sleep);
   return 1;
 } else if (habit == "on time") {
   actions::completeHabit(state::onTime, onTime);
   return 1;
 } else if( habit == "workout") {
   actions::completeHabit(state::workout, workout);
   return 1;
 } else {
   return -1;
 }
}

void handleLightsOut(int light) {
  state::lightsOut = true;
  state::lastLightsOut = millis();
  Particle.publish("lights out!", String(light));
  borders::off();
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
  //pinMode(AUDIO_PIN, OUTPUT);
  //digitalWrite(AUDIO_PIN, HIGH);

  pinMode(A0, INPUT);

  Serial1.begin(115200);  // <- Alt. if using hardware serial port

  strip.setBrightness(1);
  strip.show();

  matrix.begin();
  matrix.setBrightness(100);
  Time.zone(-8);

  state::dayStamp = Time.now();
  //borders::sparkle();
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
  actions::checkDone(state::sweets, sweets);
  actions::checkDone(state::murder, murder);
  actions::checkDone(state::brush, brush);
  actions::checkDone(state::onTime, onTime);
  actions::checkDone(state::sleep, sleep);
  actions::checkDone(state::workout, workout);
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
        //allOn();
        delay(100);
      } else {
        state::introColumn += 1;
      }
      lights::show();
      state::lastIntro = millis();
    }

  }

  if(millis() - state::lastBorder > 2000) {
    /*if(!state::lightsOut) {
      borders::sparkle();
      lights::show();
    }*/

    /*lights::matrixOn(state::row, state::col, strip.Color(0, 255, 0));
    lights::show();
    if(state::col < 23) {
      state::col += 1;
    } else if(state::row < 7) {
      state::col = 0;
      state::row += 1;
    } else {
      state::col = 0;
      state::row = 0;
    }*/

    //Particle.publish("state", "sweets: " + String(state::sweets.done) + " " + String(sweets.sideLED) + " " + String(state::sweets.doneTime) + " murder: " + state::murder.done + " " + String(murder.sideLED) + " brush: " + String(state::brush.done) );
    state::lastBorder = millis();
  }

  /*lights::turnOn(0, 4, strip.Color(144, 0, 255));
  lights::turnOn(1, 4, strip.Color(0, 158, 255));
  lights::turnOn(2, 4, strip.Color(0, 255, 0));
  lights::turnOn(3, 4, strip.Color(255, 225, 0));
  lights::turnOn(0, 4, strip.Color(255, 158, 0));
  lights::turnOn(1, 4, strip.Color(255, 0, 37));*/

  if(millis() - state::lastShow > 1000) {
    int light = analogRead(A0);
    //Particle.publish("light", String(light));
    if(light < 100 && !state::lightsOut && isAnyDone()) {
      handleLightsOut(light);
    }

    strip.show();
    state::lastShow = millis();
  }

}
