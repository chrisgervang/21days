#pragma once

#ifndef _HABIT
#define _HABIT

#include "Pushbutton.h"
#include "buttons.h"
#include "sound.h"
#include "color.h"
#include "state.h"
#include "lights.h"

typedef struct Habit {
  Pushbutton button;
  int sideLED;
  int matrixRow;
  Color color;
} Habit;

extern Habit sweets;

extern Habit workout;

extern Habit sleep;

extern Habit murder;

extern Habit onTime;

extern Habit brush;

namespace habit {
  namespace actions {
    void completeHabit(state::HabitState &state, Habit &config);
    extern bool up;
    void checkDone(state::HabitState &state, Habit &config);
  }
}

#endif
