#include "lights.h"

namespace lights {
  int map(int row, int column) {
    //0-7, 64-71, 128-135
    int LEDsPerBoard = 64;
    int boardWidth = 8;
    int index = row * boardWidth + column; // 0 - 63
    // r: 1, c: 1. 9
    // r: 0, c: 7. 7
    // r: 3, c: 0. 24
    // r: 7, c: 7. 63
    if(column >= 8 && column <= 15) {
      index = index + LEDsPerBoard - boardWidth;
      // r: 0 c: 8. 64
      // r: 1 c: 8. 73
      // r: 7 c: 16
    } else if(column >= 16 && column <= 23) {
      index = index + 2 * (LEDsPerBoard - boardWidth);
    }
    return index;
  }

  void show() {
    strip.show();
    delay(100);
    matrix.show();
  }

  void sideOff(Habit &config) {
    Color *color = config.color;
    int rgb = strip.Color(0, 0, 0);
    strip.setPixelColor(config.sideLED, rgb);
  }

  void todayOff(Habit &config) {
    Color *color = config.color;
    int rgb = strip.Color(0, 0, 0);
    matrix.setPixelColor(map(config.matrixRow, 22), rgb);
  }

  void off(Habit &config) {
    sideOff(config);
    todayOff(config);
  }

  void sideOn(Habit &config) {
    Color *color = config.color;
    int rgb = strip.Color(color->r, color->g, color->b);
    strip.setPixelColor(config.sideLED, rgb);
    strip.setBrightness(127);
  }

  void matrixOn(int row, int col, uint32_t color) {
    if(row >= 0 && row <= 7 && col >= 0 && col <=23) {
      matrix.setPixelColor(map(row, col), color);
    }
  }

  void todayOn(Habit &config) {
    Color *color = config.color;
    int rgb = strip.Color(color->r, color->g, color->b);
    matrix.setPixelColor(map(config.matrixRow, 22), rgb);
  }

  void historyOn(state::HabitState &state, Habit &config) {
    Color *c = config.color;
    // int rgb = strip.Color(color.r, color.g, color.b);
    for (unsigned int day = 0; day < DAYS; day++) {
      if(state.history[day] == 1) {
        if(day == DAYS - 1) {
          matrix.setColorDimmed(map(config.matrixRow, day + 1), c->r, c->g, c->b, state.brightness);
        } else {
          matrix.setColorDimmed(map(config.matrixRow, day + 1), c->r, c->g, c->b, 180);
        }
      } else {
        matrix.setPixelColor(map(config.matrixRow, day + 1), 0, 0, 0);
      }
    }
  }

  void on(Habit &config) {
    sideOn(config);
    todayOn(config);
  }

};
