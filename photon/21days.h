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
    double h;       // angle in degrees
    double s;       // percent
    double v;       // percent
} hsv;

typedef struct {
  Pushbutton button;
  int sideLED;
  Color color;
} Habit;

hsv rgb2hsv(Color in);
Color hsv2rgb(hsv in);

namespace lights {
  int map(int row, int column);
  void show();
  void sideOff(Habit &config);
  void todayOff(Habit &config);
  void off(Habit &config);
  void sideOn(Habit &config);
  void todayOn(Habit &config);
  void on(Habit &config);

  void border();
}

namespace buttons {
  bool isPressed(Habit &config);
  bool isAnyPressed();
}
