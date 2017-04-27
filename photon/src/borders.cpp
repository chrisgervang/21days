#include "borders.h"

namespace borders {
  void weekend() {
    long currentDay = state::dayStamp;
    //int rgb = strip.Color(255, 255, 255);

    for(unsigned int day = 22; day > 0; day--) {
      int weekday = Time.weekday(currentDay);
      if(weekday == 1 || weekday == 7) {
        matrix.setColorDimmed(lights::map(7, day), 255, 255, 255, 100);
        matrix.setColorDimmed(lights::map(0, day), 255, 255, 255, 100);
      } else {
        matrix.setPixelColor(lights::map(7, day), 0, 0, 0);
        matrix.setPixelColor(lights::map(0, day), 0, 0, 0);
      }
      currentDay = currentDay - 86400;
    }
  }

  void sparkle() {
    for(int col = 0; col < 24; col++) {
      matrix.setPixelColor(lights::map(0, col), random(256), random(256), random(256)); // Top Left board 3
    }
    for (int col = 0; col < 24; col++) {
      matrix.setPixelColor(lights::map(7, col), random(256), random(256), random(256)); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(lights::map(row, 0), random(256), random(256), random(256)); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(lights::map(row, 23), random(256), random(256), random(256)); // Top Left board 3
    }
  }

  void off() {
    for(int col = 0; col < 24; col++) {
      matrix.setPixelColor(lights::map(0, col), 0,0,0); // Top Left board 3
    }
    for (int col = 0; col < 24; col++) {
      matrix.setPixelColor(lights::map(7, col), 0,0,0); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(lights::map(row, 0), 0,0,0); // Top Left board 3
    }
    for (int row = 1; row < 7; row++) {
      matrix.setPixelColor(lights::map(row, 23), 0,0,0); // Top Left board 3
    }
  }
}
