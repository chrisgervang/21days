#include "state.h"

namespace state {
  HabitState sweets = {
    0, 255, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, -1
  };

  HabitState murder = {
    0, 255, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, -1
  };

  HabitState brush = {
    0, 255, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, -1
  };

  HabitState sleep = {
    0, 255, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, -1
  };

  HabitState workout = {
    0, 255, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, -1
  };

  HabitState onTime = {
    0, 255, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, -1
  };

  long lastShow = 0;
  bool lightsOut = false;
  long lastLightsOut = 0;
  long lastBorder = 0;
  long lastNightLight = 0;
  bool nightLight = false;
  long dayStamp = 0;

  bool intro = false;
  long lastIntro = 0;
  int introColumn = 1;

  int row = 0;
  int col = 0;
};
