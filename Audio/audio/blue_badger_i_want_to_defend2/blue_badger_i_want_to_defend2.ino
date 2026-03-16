// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define A3 220
#define D3 147
#define D4 294
#define Cb4 277
#define B3 247
#define G3 196
#define Fb3 185
#define E3 165

const int midi1[64][3]PROGMEM = {
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {Cb4, 284, 16},
 {B3, 284, 16},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {G3, 299, 301},
 {D3, 299, 301},
 {G3, 299, 301},
 {D4, 299, 301},
 {Fb3, 299, 301},
 {E3, 299, 301},
 {D3, 299, 901},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {Cb4, 284, 16},
 {B3, 284, 16},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {A3, 299, 301},
 {D3, 299, 301},
 {D4, 299, 301},
 {D3, 299, 301},
 {G3, 299, 301},
 {D3, 299, 301},
 {G3, 299, 301},
 {D4, 299, 301},
 {Fb3, 299, 301},
 {E3, 299, 301},
 {D3, 299, 0},
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
