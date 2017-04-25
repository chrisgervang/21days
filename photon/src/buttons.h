#pragma once

#ifndef _BUTTONS
#define _BUTTONS

#include "habit.h"

namespace buttons {
  bool isPressed(struct Habit &config);
  bool isAnyPressed();
}

#endif
