
#include "pitches.h"

int red = 3;
int green = 5;
int blue = 6;

#define COMMON_ANODE

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
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT); 

  int notes = sizeof(melody) / sizeof(melody[0]);

  for (int thisNote = 0; thisNote < notes; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(2, melody[thisNote], noteDuration);
    if(melody[thisNote] == NOTE_A3){
      setColour(255, 0, 0);
    }
    else if(melody[thisNote] == NOTE_B3){
      setColour(255, 165, 0);
    }
    else if(melody[thisNote] == NOTE_C4){
      setColour(255, 255, 0);
    }
    else if(melody[thisNote] == NOTE_D4){
      setColour(0, 255, 0);
    }
    else if(melody[thisNote] == NOTE_E4){
      setColour(0, 255, 255);
    }
    else if(melody[thisNote] == NOTE_F4){
      setColour(0, 0, 255);
    }
    else if(melody[thisNote] == NOTE_G3){
      setColour(80, 0, 80);
    }
    else if(melody[thisNote] == NOTE_G4){
      setColour(255, 192, 203);
    }
    else;
    delay(noteDuration * 1.30);
    noTone(2);
  }
}

void loop() {

}

void setColour(int r, int g, int b){
  #ifdef COMMON_ANODE
    r = 255-r;
    g = 255-g;
    b = 255-b;
  #endif
  analogWrite(red, r);
  analogWrite(green, g);
  analogWrite(blue, b);
}

