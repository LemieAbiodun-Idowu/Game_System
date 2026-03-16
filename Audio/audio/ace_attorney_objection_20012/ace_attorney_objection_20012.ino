// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define Ab4 466
#define C5 523
#define F4 349
#define A4 440
#define G4 392

const int midi1[32][3]PROGMEM = {
 {Ab4, 1999, 1},
 {Ab4, 1999, 1},
 {Ab4, 1999, 1},
 {C5, 999, 1},
 {F4, 999, 1},
 {Ab4, 2999, 1},
 {A4, 999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {G4, 1999, 1},
 {F4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {G4, 1999, 1},
 {F4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {G4, 1999, 1},
 {F4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {G4, 1999, 1},
 {F4, 1999, 1},
 {Ab4, 1999, 1},
 {A4, 1999, 1},
 {Ab4, 1999, 0},
};

void playMidi(int pin, const int notes[][3], size_t len){
 for (int i = 0; i < len; i++) {
  int note = pgm_read_word(&(notes[i][0]));
  int duration = pgm_read_word(&notes[i][1]);
  int pause = pgm_read_word(&notes[i][2]);

    tone(pin, note);
    delay(duration);
    noTone(pin);
    delay(pause);
  }
}
// Generated using https://github.com/ShivamJoker/MIDI-to-Arduino

// main.ino or main.cpp
void setup() {
  // put your setup code here, to run once:
  // play midi by passing pin no., midi, midi len
  playMidi(11, midi1, ARRAY_LEN(midi1));
}

void loop() {
  // put your main code here, to run repeatedly:
}
