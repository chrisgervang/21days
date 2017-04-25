void weekendBorder() {
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
