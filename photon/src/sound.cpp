#include "sound.h"

void playRandomSound() {
  digitalWrite(D6, LOW);
  delay(200);
  digitalWrite(D6, HIGH);
}
