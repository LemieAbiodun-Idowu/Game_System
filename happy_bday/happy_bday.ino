/*
  Melody

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 8

  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/digital/toneMelody/
*/

#include "pitches.h"

int melody[] = {
  NOTE_G3, NOTE_G3,
  NOTE_A3, NOTE_G3, NOTE_C4, NOTE_B3,

  NOTE_G3, NOTE_G3,
  NOTE_A3, NOTE_G3, NOTE_D4, NOTE_C4,

  NOTE_G3, NOTE_G3,
  NOTE_G4, NOTE_E4, NOTE_C4, NOTE_B3, NOTE_A3,

  NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_C4, NOTE_D4, NOTE_C4
};

// note durations: 4 = quarter, 8 = eighth, etc.
int noteDurations[] = {
  4, 8,
  4, 4, 4, 2,

  4, 8,
  4, 4, 4, 2,

  4, 8,
  4, 4, 4, 4, 2,

  4, 8,
  4, 4, 4, 2
};

void setup() {
  int notes = sizeof(melody) / sizeof(melody[0]);

  for (int thisNote = 0; thisNote < notes; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(2, melody[thisNote], noteDuration);

    delay(noteDuration * 1.30);
    noTone(2);
  }
}

void loop() {

}
