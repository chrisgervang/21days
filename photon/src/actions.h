#include "habit.h"
#include "lights.h"
#include "buttons.h"

namespace actions {
  void completeHabit(state::HabitState &state, Habit &config);
  
  extern bool up;
  void checkDone(state::HabitState &state, Habit &config);
};
