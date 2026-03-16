// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define Ab3 233
#define Fb3 185
#define Cb4 277
#define A3 220
#define C4 262
#define Db3 156
#define Gb3 208
#define F3 175
#define G3 196
#define E3 165
#define D3 147
#define Cb3 139

const int midi1[96][3]PROGMEM = {
 {Ab3, 849, 47},
 {Fb3, 849, 47},
 {Cb4, 849, 3629},
 {A3, 849, 47},
 {Fb3, 849, 47},
 {C4, 849, 3629},
 {Ab3, 849, 47},
 {Fb3, 849, 47},
 {Cb4, 849, 3629},
 {Db3, 1700, 91},
 {Fb3, 1700, 91},
 {A3, 3401, 181},
 {Gb3, 3401, 181},
 {F3, 3401, 181},
 {G3, 3401, 181},
 {E3, 3401, 181},
 {Fb3, 3401, 181},
 {Db3, 3401, 181},
 {D3, 3401, 1076},
 {Fb3, 849, 47},
 {F3, 849, 47},
 {Db3, 849, 47},
 {Cb3, 849, 2733},
 {Ab3, 849, 47},
 {Fb3, 849, 47},
 {Cb4, 849, 3629},
 {A3, 849, 47},
 {Fb3, 849, 47},
 {C4, 849, 3629},
 {Ab3, 849, 47},
 {Fb3, 849, 47},
 {Cb4, 849, 3629},
 {Db3, 1700, 91},
 {Fb3, 1700, 91},
 {A3, 3401, 181},
 {Gb3, 3401, 181},
 {F3, 3401, 181},
 {G3, 3401, 181},
 {E3, 3401, 181},
 {Fb3, 3401, 181},
 {Db3, 3401, 181},
 {D3, 3401, 1076},
 {Fb3, 849, 47},
 {F3, 849, 47},
 {Db3, 849, 47},
 {Cb3, 849, 0},
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
