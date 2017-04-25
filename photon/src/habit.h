#pragma once

#ifndef _HABIT
#define _HABIT

#include "Pushbutton.h"
#include "color.h"
#include "buttons.h"
#include "sound.h"
#include "state.h"
#include "lights.h"

typedef struct Habit {
  Pushbutton button;
  int sideLED;
  int matrixRow;
  struct Color *color;
};

extern struct Habit sweets;

extern struct Habit workout;

extern struct Habit sleep;

extern struct Habit murder;

extern struct Habit onTime;

extern struct Habit brush;

namespace habit {
  namespace actions {
    void completeHabit(state::HabitState &state, struct Habit &config);
    extern bool up;
    void checkDone(state::HabitState &state, struct Habit &config);
  }
}

#endif
