#include "demo.h"

void randomHistory() {
  for (unsigned int day = 0; day < sizeof(state::sweets.history); day++) {
    state::sweets.history[day] = (random(100) < 49);
    state::murder.history[day] = (random(100) < 49);
    state::brush.history[day] = (random(100) < 49);
    state::sleep.history[day] = (random(100) < 49);
    state::workout.history[day] = (random(100) < 49);
    state::onTime.history[day] = (random(100) < 49);
  }
  lights::historyOn(state::sweets, sweets);
  lights::historyOn(state::brush, brush);
  lights::historyOn(state::murder, murder);
  lights::historyOn(state::sleep, sleep);
  lights::historyOn(state::workout, workout);
  lights::historyOn(state::onTime, onTime);
}
