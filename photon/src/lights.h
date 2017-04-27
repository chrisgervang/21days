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
  void sideOff(Habit &config);
  void todayOff(Habit &config);
  void off(Habit &config);
  void sideOn(Habit &config);
  void matrixOn(int row, int col, uint32_t color);
  void todayOn(Habit &config);
  void historyOn(state::HabitState &state, Habit &config);
  void on(Habit &config);
}

#endif
