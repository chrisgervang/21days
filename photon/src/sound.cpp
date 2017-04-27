#include "sound.h"

void playRandomSound() {
  digitalWrite(AUDIO_PIN, LOW);
  delay(200);
  digitalWrite(AUDIO_PIN, HIGH);
}

// iphone text sound
