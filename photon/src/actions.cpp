#include "actions.h"

namespace actions {
  void completeHabit(state::HabitState &state, Habit &config) {
    Particle.publish("Habit Done!", String(config.matrixRow));
    state.history[21] = true;
    state.doneTime = Time.now();
    Color *color = config.color;
    //turnOn(sweets.sideLED, 4, strip.Color(sweets.color.r, sweets.color.g, sweets.color.b));
    //strip.setPixelColor(config.sideLED + 1, strip.Color((color.r * b) >> 8, (color.g * b) >> 8, (color.b * b) >> 8));
    if(!state::lightsOut) {
      lights::on(config);
      lights::show();
      // 15% chance a sound clip will play.
      if(random(100) < 15) {
        playRandomSound();
      }
    } else {
      lights::todayOn(config);
      lights::show();
    }

  };

  bool up = false;

  void checkDone(state::HabitState &state, Habit &config) {
    if(!state.history[21] && buttons::isPressed(config)) {
      completeHabit(state, config);
      state.previousMillis = millis();

    }
    // else if (state.history[21] && millis() - state.previousMillis > 50) {
    //   if (state.brightness < 255 && up) {
    //     state.brightness += 1;
    //     if(state.brightness == 255) {
    //       up = false;
    //     }
    //   } else if (state.brightness > 200 && !up) {
    //     state.brightness -= 1;
    //     if(state.brightness == 200) {
    //       up = true;
    //     }
    //   }
    //   matrix.setColorDimmed(lights::map(config.matrixRow, 22), config.color.r, config.color.g, config.color.b, state.brightness);
    //   state.previousMillis = millis();
    //
    //   matrix.show();
    //
    // }

  }
};
