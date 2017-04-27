#include "habit.h"

Color green = {0, 255, 0};
Habit sweets = {
  Pushbutton(D0, false, 0),
  3,
  4,
  &green
};

Color orange = {255, 127, 0};
Habit workout = {
  Pushbutton(D1, false, 0),
  4,
  2,
  &orange
};

Color blue = {0, 158, 255};
Habit sleep = {
  Pushbutton(D2, false, 0),
  5,
  5,
  &blue
};

Color red = {255, 0, 25};
Habit murder = {
  Pushbutton(D3, false, 0),
  2,
  1,
  &red
};

Color yellow = { 255, 255, 0 };
Habit onTime = {
  Pushbutton(D4, false, 0),
  1,
  3,
  &yellow
};

Color purple = { 144, 0, 255 };
Habit brush = {
  Pushbutton(D5, false, 0),
  0,
  6,
  &purple
};
