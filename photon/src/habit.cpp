#include "habit.h"

Color green = {0, 255, 0};
Habit sweets = {
  Pushbutton(D1, false, 0),
  0,
  4,
  &green
};

Color orange = {255, 127, 0};
Habit workout = {
  Pushbutton(D2, false, 0),
  1,
  2,
  &orange
};

Color blue = {0, 158, 255};
Habit sleep = {
  Pushbutton(D3, false, 0),
  2,
  5,
  &blue
};

Color red = {255, 0, 25};
Habit murder = {
  Pushbutton(D4, false, 0),
  3,
  1,
  &red
};

Color yellow = { 255, 255, 0 };
Habit onTime = {
  Pushbutton(D5, false, 0),
  4,
  3,
  &yellow
};

Color purple = { 144, 0, 255 };
Habit brush = {
  Pushbutton(D6, false, 0),
  5,
  6,
  &purple
};
