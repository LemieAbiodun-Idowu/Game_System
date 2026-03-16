// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define A2 110
#define G2 98
#define F2 87
#define E2 82
#define D2 73
#define Ab3 233
#define B3 247
#define B2 123
#define C3 131
#define D3 147
#define E3 165
#define F3 175
#define G3 196

const int midi1[118][3]PROGMEM = {
 {A2, 1583, 1417},
 {G2, 158, 9},
 {F2, 1741, 1426},
 {G2, 158, 9},
 {A2, 1741, 1426},
 {G2, 158, 9},
 {F2, 1741, 1426},
 {E2, 158, 9},
 {D2, 1741, 1426},
 {D2, 158, 9},
 {G2, 1741, 592},
 {Ab3, 158, 9},
 {B3, 158, 176},
 {Ab3, 158, 9},
 {B3, 158, 176},
 {A2, 1424, 242},
 {B2, 158, 9},
 {C3, 158, 9},
 {D3, 158, 9},
 {C3, 158, 9},
 {B2, 158, 9},
 {A2, 158, 9},
 {G2, 158, 9},
 {A2, 158, 9},
 {G2, 158, 9},
 {E2, 158, 9},
 {F2, 1424, 242},
 {C3, 158, 9},
 {D3, 158, 9},
 {C3, 158, 9},
 {B2, 158, 9},
 {C3, 158, 9},
 {D3, 158, 9},
 {E3, 158, 9},
 {F3, 158, 9},
 {G3, 158, 9},
 {G2, 158, 9},
 {A2, 791, 209},
 {E3, 158, 9},
 {F3, 158, 9},
 {E3, 158, 9},
 {D3, 78, 5},
 {E3, 78, 5},
 {D3, 78, 5},
 {C3, 78, 5},
 {B2, 158, 9},
 {A2, 158, 9},
 {G2, 158, 9},
 {A2, 158, 9},
 {B2, 158, 9},
 {C3, 158, 9},
 {D3, 158, 9},
 {E3, 158, 9},
 {G2, 158, 9},
 {F2, 791, 209},
 {C3, 158, 9},
 {B2, 158, 9},
 {A2, 158, 9},
 {C3, 158, 9},
 {B2, 158, 9},
 {A2, 158, 9},
 {C3, 158, 9},
 {B2, 158, 9},
 {A2, 158, 9},
 {C3, 158, 9},
 {B2, 158, 9},
 {A2, 316, 17},
 {E2, 158, 9},
 {D2, 791, 209},
 {F2, 158, 9},
 {E2, 158, 9},
 {D2, 158, 9},
 {E2, 158, 9},
 {F2, 158, 9},
 {G2, 158, 9},
 {A2, 158, 9},
 {B2, 158, 9},
 {C3, 158, 9},
 {B2, 158, 9},
 {A2, 158, 9},
 {G2, 158, 9},
 {A2, 158, 9},
 {D2, 158, 9},
 {E2, 1108, 226},
 {E2, 2058, 109},
 {B2, 633, 2701},
 {A2, 474, 26},
 {E2, 316, 184},
 {G2, 316, 184},
 {A2, 474, 26},
 {E2, 316, 17},
 {G2, 316, 17},
 {A2, 316, 17},
 {C3, 316, 17},
 {A2, 474, 26},
 {E2, 316, 184},
 {G2, 316, 184},
 {A2, 474, 26},
 {E2, 316, 17},
 {G2, 316, 17},
 {A2, 316, 17},
 {D3, 158, 9},
 {C3, 158, 9},
 {A2, 474, 26},
 {E2, 316, 184},
 {G2, 316, 184},
 {A2, 474, 26},
 {E2, 316, 17},
 {G2, 316, 17},
 {A2, 316, 17},
 {C3, 316, 17},
 {A2, 474, 26},
 {E2, 316, 184},
 {G2, 316, 184},
 {A2, 474, 26},
 {E2, 316, 17},
 {G2, 316, 17},
 {E2, 316, 0},
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

