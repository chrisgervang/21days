#include "history.h"

void handleHistory(const char *event, const char *data) {
  // Handle the webhook response
  int length = strlen(data) + 1;
  const int brush_s = 0;
  const int murder_s = 23;
  const int sweets_s = 46;
  const int workout_s = 69;
  const int sleep_s = 92;
  const int ontime_s = 115;

  char result[length];
  strcpy(result, data);

  for (int index = 0; index < length; index++) {
    if(index >= brush_s && index < brush_s + 22) {
      //"brush twice"
      state::brush.history[index - brush_s] = result[index] - '0';
    } else if(index >= murder_s && index < murder_s + 22) {
      //"dont murder"
      state::murder.history[index - murder_s] = result[index] - '0';
    } else if(index >= sweets_s && index < sweets_s + 22) {
      //"no sweets"
      state::sweets.history[index - sweets_s] = result[index] - '0';
    } else if(index >= workout_s && index < workout_s + 22) {
      //"workout"
      state::workout.history[index - workout_s] = result[index] - '0';
    } else if(index >= sleep_s && index < sleep_s + 22) {
      //"sleep by 12am"
      state::sleep.history[index - sleep_s] = result[index] - '0';
    } else if(index >= ontime_s && index < ontime_s + 22) {
      //"on time"
      state::onTime.history[index - ontime_s] = result[index] - '0';
    }
  }

  lights::historyOn(state::brush, brush);
  lights::historyOn(state::murder, murder);
  lights::historyOn(state::sweets, sweets);
  lights::historyOn(state::workout, workout);
  lights::historyOn(state::sleep, sleep);
  lights::historyOn(state::onTime, onTime);
  delay(100);
  lights::show();
}

void allOn() {
  for (unsigned int day = 0; day < DAYS; day++) {
    //Color c1 = hsv2rgb({120, 100, map(day, 0, 20, 0, 100)});
    matrix.setPixelColor(lights::map(sweets.matrixRow, day + 1), sweets.color->r, sweets.color->g, sweets.color->b);
    //Color c2 = hsv2rgb({350, 100, map(day, 0, 20, 0, 100)});
    matrix.setPixelColor(lights::map(murder.matrixRow, day + 1), murder.color->r, murder.color->g, murder.color->b);
    //Color c3 = hsv2rgb({273, 100, map(day, 0, 20, 0, 100)});
    matrix.setPixelColor(lights::map(brush.matrixRow, day + 1), brush.color->r, brush.color->g, brush.color->b);
    //Color c4 = hsv2rgb({201, 100, map(day, 0, 20, 0, 100)});
    matrix.setPixelColor(lights::map(sleep.matrixRow, day + 1), sleep.color->r, sleep.color->g, sleep.color->b);
    //Color c5 = hsv2rgb({36, 100, map(day, 0, 20, 0, 100)});
    matrix.setPixelColor(lights::map(workout.matrixRow, day + 1), workout.color->r, workout.color->g, workout.color->b);
    //Color c6 = hsv2rgb({52, 100, map(day, 0, 20, 0, 100)});
    matrix.setPixelColor(lights::map(onTime.matrixRow, day + 1), onTime.color->r, onTime.color->g, onTime.color->b);
  }
}
