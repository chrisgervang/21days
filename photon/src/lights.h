#ifndef _LIGHTS
#define _LIGHTS

#include "color.h"
#include "strip.h"
#include "matrix.h"

namespace lights {
  int map(int row, int column);
  void show();
  void sideOff(Habit &config);
  void todayOff(Habit &config);
  void off(Habit &config);
  void sideOn(Habit &config);
  void todayOn(Habit &config);
  void historyOn(state::HabitState &state, Habit &config);
  void on(Habit &config);

  void randomBorder();
  void borderOff();
}

#endif
