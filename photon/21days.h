#include "EasingLibrary.h"
#include "Adafruit_Pixie.h"
#include "Pushbutton.h"
#include "neopixel.h"

int remoteCompleteHabit(String habit);

typedef struct {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} Color;

typedef struct {
  Pushbutton button;
  int sideLED;
  Color color;
} Habit;

namespace lights {
  int map(int row, int column);
  void off(Habit &config);
  void on(Habit &config);
  void border();
}

namespace buttons {
  bool isPressed(Habit &config);
  bool isAnyPressed();
}
