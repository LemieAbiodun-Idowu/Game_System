// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define G3 196
#define E3 165
#define Fb3 185
#define A3 220
#define D3 147
#define C3 131

const int midi1[84][3]PROGMEM = {
 {G3, 743, 1},
 {E3, 743, 1},
 {Fb3, 495, 1},
 {G3, 743, 1},
 {A3, 743, 1},
 {G3, 495, 1},
 {E3, 743, 1},
 {Fb3, 743, 1},
 {E3, 495, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {D3, 495, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {Fb3, 495, 1},
 {G3, 743, 1},
 {A3, 743, 1},
 {G3, 495, 1},
 {E3, 743, 1},
 {Fb3, 743, 1},
 {A3, 495, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {D3, 495, 1},
 {C3, 991, 1},
 {D3, 991, 1},
 {G3, 991, 1},
 {E3, 495, 1},
 {D3, 495, 1},
 {C3, 991, 1},
 {D3, 991, 1},
 {G3, 991, 1},
 {A3, 495, 1},
 {G3, 495, 1},
 {E3, 991, 1},
 {A3, 991, 1},
 {E3, 991, 1},
 {G3, 991, 1},
 {Fb3, 991, 1},
 {E3, 991, 1},
 {E3, 991, 1},
 {Fb3, 991, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {Fb3, 495, 1},
 {G3, 743, 1},
 {A3, 743, 1},
 {G3, 495, 1},
 {E3, 743, 1},
 {Fb3, 743, 1},
 {E3, 495, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {D3, 495, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {Fb3, 495, 1},
 {G3, 743, 1},
 {A3, 743, 1},
 {G3, 495, 1},
 {E3, 743, 1},
 {Fb3, 743, 1},
 {A3, 495, 1},
 {G3, 743, 1},
 {E3, 743, 1},
 {D3, 495, 1},
 {C3, 991, 1},
 {D3, 991, 1},
 {G3, 991, 1},
 {E3, 495, 1},
 {D3, 495, 1},
 {C3, 991, 1},
 {D3, 991, 1},
 {G3, 991, 1},
 {A3, 495, 1},
 {G3, 495, 1},
 {E3, 991, 1},
 {A3, 991, 1},
 {E3, 991, 1},
 {G3, 991, 1},
 {Fb3, 991, 1},
 {E3, 991, 1},
 {E3, 991, 1},
 {Fb3, 991, 0},
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
