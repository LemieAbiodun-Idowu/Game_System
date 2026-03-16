// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define F5 698
#define Fb5 740
#define G5 784
#define C5 523
#define D5 587
#define Db5 622
#define A5 880
#define Gb5 831
#define E5 659
#define B4 494
#define Cb5 554
#define Ab5 932
#define B5 988
#define C6 1047

const int midi1[126][3]PROGMEM = {
 {F5, 66, 1},
 {Fb5, 66, 1},
 {G5, 266, 1},
 {G5, 99, 101},
 {C5, 1799, 1},
 {C5, 199, 1},
 {D5, 99, 101},
 {Db5, 99, 101},
 {F5, 99, 101},
 {F5, 66, 1},
 {Fb5, 66, 1},
 {G5, 266, 1},
 {G5, 99, 101},
 {C5, 1799, 1},
 {C5, 199, 1},
 {D5, 99, 101},
 {Db5, 99, 101},
 {F5, 99, 101},
 {Fb5, 599, 1},
 {A5, 999, 1},
 {Gb5, 66, 1},
 {A5, 133, 1},
 {E5, 99, 101},
 {Fb5, 99, 101},
 {D5, 1599, 1},
 {D5, 99, 1},
 {E5, 99, 1},
 {Fb5, 599, 1},
 {B4, 99, 1},
 {Cb5, 99, 1},
 {D5, 1599, 1},
 {Db5, 66, 1},
 {E5, 533, 1},
 {G5, 599, 201},
 {G5, 99, 1},
 {A5, 99, 1},
 {Fb5, 599, 1},
 {A5, 49, 1},
 {Ab5, 49, 1},
 {B5, 499, 1},
 {A5, 99, 101},
 {G5, 1599, 1},
 {Fb5, 99, 1},
 {G5, 99, 1},
 {Fb5, 199, 1},
 {D5, 99, 101},
 {E5, 1199, 1},
 {F5, 599, 1},
 {Gb5, 599, 201},
 {Gb5, 99, 1},
 {Ab5, 99, 1},
 {G5, 599, 1},
 {Db5, 599, 1},
 {C5, 99, 101},
 {F5, 1599, 1},
 {F5, 99, 1},
 {Gb5, 99, 1},
 {C6, 99, 1},
 {Ab5, 99, 1},
 {Gb5, 1399, 1},
 {G5, 99, 1},
 {Gb5, 99, 1},
 {G5, 2999, 3201},
 {F5, 66, 1},
 {Fb5, 66, 1},
 {G5, 266, 1},
 {G5, 99, 101},
 {C5, 1799, 1},
 {C5, 199, 1},
 {D5, 99, 101},
 {Db5, 99, 101},
 {F5, 99, 101},
 {F5, 66, 1},
 {Fb5, 66, 1},
 {G5, 266, 1},
 {G5, 99, 101},
 {C5, 1799, 1},
 {C5, 199, 1},
 {D5, 99, 101},
 {Db5, 99, 101},
 {F5, 99, 101},
 {Fb5, 599, 1},
 {A5, 999, 1},
 {Gb5, 66, 1},
 {A5, 133, 1},
 {E5, 99, 101},
 {Fb5, 99, 101},
 {D5, 1599, 1},
 {D5, 99, 1},
 {E5, 99, 1},
 {Fb5, 599, 1},
 {B4, 99, 1},
 {Cb5, 99, 1},
 {D5, 1599, 1},
 {Db5, 66, 1},
 {E5, 533, 1},
 {G5, 599, 201},
 {G5, 99, 1},
 {A5, 99, 1},
 {Fb5, 599, 1},
 {A5, 49, 1},
 {Ab5, 49, 1},
 {B5, 499, 1},
 {A5, 99, 101},
 {G5, 1599, 1},
 {Fb5, 99, 1},
 {G5, 99, 1},
 {Fb5, 199, 1},
 {D5, 99, 101},
 {E5, 1199, 1},
 {F5, 599, 1},
 {Gb5, 599, 201},
 {Gb5, 99, 1},
 {Ab5, 99, 1},
 {G5, 599, 1},
 {Db5, 599, 1},
 {C5, 99, 101},
 {F5, 1599, 1},
 {F5, 99, 1},
 {Gb5, 99, 1},
 {C6, 99, 1},
 {Ab5, 99, 1},
 {Gb5, 1399, 1},
 {G5, 99, 1},
 {Gb5, 99, 1},
 {G5, 2999, 0},
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
