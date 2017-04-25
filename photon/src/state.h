#pragma once

#ifndef _STATE
#define _STATE

namespace state {
  struct HabitState {
    long previousMillis;
    int brightness;
    int history [22];
    int doneTime;
  };

  extern HabitState sweets;
  extern HabitState murder;
  extern HabitState brush;
  extern HabitState sleep;
  extern HabitState workout;
  extern HabitState onTime;

  extern long lastShow;
  extern bool lightsOut;
  extern long lastLightsOut;
  extern long lastBorder;
  extern long lastNightLight;
  extern bool nightLight;
  extern long dayStamp;

  extern bool intro;
  extern long lastIntro;
  extern int introColumn;
};

#endif
