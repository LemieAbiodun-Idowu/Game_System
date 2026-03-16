// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define D4 294
#define E4 330
#define Fb4 370
#define G4 392
#define A4 440
#define D5 587
#define B4 494
#define Cb5 554

const int midi1[86][3]PROGMEM = {
 {D4, 284, 16},
 {E4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 31},
 {A4, 569, 31},
 {D5, 569, 31},
 {A4, 569, 31},
 {A4, 284, 16},
 {G4, 284, 16},
 {Fb4, 569, 31},
 {D4, 284, 16},
 {E4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 31},
 {A4, 569, 31},
 {D5, 284, 16},
 {A4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 631},
 {D4, 284, 16},
 {E4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 31},
 {A4, 569, 31},
 {D5, 569, 31},
 {A4, 569, 31},
 {A4, 284, 16},
 {G4, 284, 16},
 {Fb4, 569, 31},
 {G4, 569, 31},
 {G4, 284, 16},
 {A4, 284, 16},
 {B4, 569, 31},
 {B4, 284, 16},
 {Cb5, 284, 16},
 {D5, 284, 16},
 {Fb4, 284, 16},
 {A4, 569, 31},
 {D4, 569, 631},
 {D4, 284, 16},
 {E4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 31},
 {A4, 569, 31},
 {D5, 569, 31},
 {A4, 569, 31},
 {A4, 284, 16},
 {G4, 284, 16},
 {Fb4, 569, 31},
 {D4, 284, 16},
 {E4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 31},
 {A4, 569, 31},
 {D5, 284, 16},
 {A4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 631},
 {D4, 284, 16},
 {E4, 284, 16},
 {Fb4, 284, 16},
 {G4, 284, 16},
 {A4, 569, 31},
 {A4, 569, 31},
 {D5, 569, 31},
 {A4, 569, 31},
 {A4, 284, 16},
 {G4, 284, 16},
 {Fb4, 569, 31},
 {G4, 569, 31},
 {G4, 284, 16},
 {A4, 284, 16},
 {B4, 569, 31},
 {B4, 284, 16},
 {Cb5, 284, 16},
 {D5, 284, 16},
 {Fb4, 284, 16},
 {A4, 569, 31},
 {D4, 569, 0},
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
