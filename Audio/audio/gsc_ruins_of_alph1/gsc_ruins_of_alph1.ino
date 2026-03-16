// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define C5 523
#define Ab4 466
#define Cb5 554
#define Db5 622
#define F5 698
#define E5 659
#define B4 494
#define D5 587
#define Fb5 740
#define Fb4 370
#define E4 330
#define A4 440

const int midi1[184][3]PROGMEM = {
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 249, 1},
 {Db5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 999, 1},
 {Ab4, 499, 1},
 {Cb5, 499, 1},
 {F5, 249, 1},
 {E5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 999, 1},
 {Cb5, 499, 1},
 {C5, 499, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 249, 1},
 {Db5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 999, 1},
 {Ab4, 499, 1},
 {Cb5, 499, 1},
 {F5, 249, 1},
 {E5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 999, 1},
 {Cb5, 499, 1},
 {C5, 499, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 249, 1},
 {Db5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 999, 1},
 {Ab4, 499, 1},
 {Cb5, 499, 1},
 {F5, 249, 1},
 {E5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 999, 1},
 {Cb5, 499, 1},
 {C5, 499, 1},
 {Cb5, 499, 1},
 {B4, 499, 1},
 {D5, 249, 1},
 {E5, 249, 1},
 {Cb5, 249, 1},
 {B4, 249, 1},
 {Cb5, 499, 1},
 {B4, 499, 1},
 {D5, 999, 1},
 {B4, 499, 1},
 {D5, 499, 1},
 {Fb5, 249, 1},
 {F5, 249, 1},
 {Cb5, 249, 1},
 {B4, 249, 1},
 {Cb5, 999, 1},
 {D5, 499, 1},
 {Cb5, 499, 501},
 {C5, 1999, 1},
 {Cb5, 1999, 1},
 {D5, 1999, 1},
 {Cb5, 1999, 1},
 {Fb4, 999, 1},
 {E4, 999, 1},
 {Ab4, 999, 1},
 {A4, 999, 8001},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 249, 1},
 {Db5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 999, 1},
 {Ab4, 499, 1},
 {Cb5, 499, 1},
 {F5, 249, 1},
 {E5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 999, 1},
 {Cb5, 499, 1},
 {C5, 499, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 249, 1},
 {Db5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 999, 1},
 {Ab4, 499, 1},
 {Cb5, 499, 1},
 {F5, 249, 1},
 {E5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 999, 1},
 {Cb5, 499, 1},
 {C5, 499, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 249, 1},
 {Db5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 499, 1},
 {Ab4, 499, 1},
 {Cb5, 999, 1},
 {Ab4, 499, 1},
 {Cb5, 499, 1},
 {F5, 249, 1},
 {E5, 249, 1},
 {C5, 249, 1},
 {Ab4, 249, 1},
 {C5, 999, 1},
 {Cb5, 499, 1},
 {C5, 499, 1},
 {Cb5, 499, 1},
 {B4, 499, 1},
 {D5, 249, 1},
 {E5, 249, 1},
 {Cb5, 249, 1},
 {B4, 249, 1},
 {Cb5, 499, 1},
 {B4, 499, 1},
 {D5, 999, 1},
 {B4, 499, 1},
 {D5, 499, 1},
 {Fb5, 249, 1},
 {F5, 249, 1},
 {Cb5, 249, 1},
 {B4, 249, 1},
 {Cb5, 999, 1},
 {D5, 499, 1},
 {Cb5, 499, 501},
 {C5, 1999, 1},
 {Cb5, 1999, 1},
 {D5, 1999, 1},
 {Cb5, 1999, 1},
 {Fb4, 999, 1},
 {E4, 999, 1},
 {Ab4, 999, 1},
 {A4, 999, 0},
};

void playMidi(int pin, const int notes[][3], size_t len){
 for (int i = 0; i < len; i++) {
    int note = pgm_read_word(&(notes[i][0]));
    int duration = pgm_read_word(&(notes[i][1]));
    int pause = pgm_read_word(&(notes[i][2]));

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
