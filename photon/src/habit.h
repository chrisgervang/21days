#pragma once

#ifndef _HABIT
#define _HABIT

#include "Pushbutton.h"
#include "color.h"
#include "sound.h"
#include "state.h"

struct Habit {
  Pushbutton button;
  int sideLED;
  int matrixRow;
  Color *color;
};

extern Habit sweets;

extern Habit workout;

extern Habit sleep;

extern Habit murder;

extern Habit onTime;

extern Habit brush;

#endif
