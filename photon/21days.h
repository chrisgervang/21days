#include "EasingLibrary.h"
#include "Adafruit_Pixie.h"
#include "Pushbutton.h"
#include "neopixel.h"
#include "SparkJson.h"

int remoteCompleteHabit(String habit);
void handleHistory(const char *event, const char *data);

void setup();

void loop();

typedef struct {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} Color;

// typedef struct {
//     double h;       // angle in degrees
//     double s;       // percent
//     double v;       // percent
// } hsv;

typedef struct {
  Pushbutton button;
  int sideLED;
  int matrixRow;
  Color color;
} Habit;

typedef struct {
  int duration;
  bool looping;
} AnimationConfig;

typedef struct {
  int lastFrame;
} AnimationState;

// hsv rgb2hsv(Color in);
// Color hsv2rgb(hsv in);

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
