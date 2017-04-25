#pragma once

#ifndef _LIGHTS
#define _LIGHTS

#include "habit.h"
#include "color.h"
#include "strip.h"
#include "matrix.h"
#include "state.h"

namespace lights {
  int map(int row, int column);
  void show();
  void sideOff(struct Habit &config);
  void todayOff(struct Habit &config);
  void off(struct Habit &config);
  void sideOn(struct Habit &config);
  void todayOn(struct Habit &config);
  void historyOn(state::HabitState &state, struct Habit &config);
  void on(struct Habit &config);

  void randomBorder();
  void borderOff();
}

#endif
