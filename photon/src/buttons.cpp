#include "buttons.h"

namespace buttons {
  bool isPressed(Habit &config) {
    return config.button.getSingleDebouncedPress();
  }

  bool isAnyPressed() {
    return buttons::isPressed(sweets) || buttons::isPressed(murder) || buttons::isPressed(brush) || buttons::isPressed(onTime) || buttons::isPressed(sleep) || buttons::isPressed(workout) ;
  }
};
