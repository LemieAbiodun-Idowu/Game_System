// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define A4 440
#define Gb4 415
#define Fb4 370
#define E4 330
#define E5 659
#define D5 587
#define B4 494
#define Cb5 554
#define Fb5 740
#define A5 880
#define B5 988
#define Db5 622
#define Gb5 831
#define Cb6 1109

const int midi1[119][3]PROGMEM = {
 {A4, 249, 1},
 {Gb4, 374, 1},
 {Fb4, 124, 1},
 {E4, 749, 1},
 {Gb4, 749, 1},
 {E5, 249, 1},
 {D5, 249, 1},
 {B4, 249, 1},
 {E5, 749, 1},
 {Cb5, 624, 1},
 {E5, 124, 1},
 {D5, 749, 1},
 {B4, 749, 1},
 {D5, 749, 1},
 {B4, 249, 1},
 {Fb5, 374, 1},
 {D5, 124, 1},
 {E5, 749, 1},
 {Cb5, 749, 1},
 {E5, 749, 1},
 {Cb5, 624, 1},
 {E5, 124, 1},
 {Fb5, 749, 1},
 {D5, 749, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Fb5, 374, 1},
 {E5, 124, 1},
 {A5, 749, 1},
 {A5, 749, 1},
 {B5, 249, 1},
 {A5, 374, 1},
 {D5, 124, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Fb5, 374, 1},
 {E5, 124, 1},
 {Cb5, 749, 1},
 {D5, 249, 1},
 {Cb5, 374, 1},
 {B4, 124, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Db5, 374, 1},
 {E5, 124, 1},
 {A5, 749, 1},
 {B5, 249, 1},
 {A5, 374, 1},
 {D5, 124, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Fb5, 374, 1},
 {Gb5, 124, 1},
 {A5, 749, 1},
 {Gb5, 249, 1},
 {Fb5, 374, 1},
 {E5, 124, 1},
 {Gb5, 749, 1},
 {A5, 249, 1},
 {Gb5, 374, 1},
 {A5, 124, 1},
 {Cb6, 749, 1},
 {E5, 749, 1},
 {Cb5, 624, 1},
 {E5, 124, 1},
 {D5, 749, 1},
 {B4, 749, 1},
 {D5, 749, 1},
 {B4, 249, 1},
 {Fb5, 374, 1},
 {D5, 124, 1},
 {E5, 749, 1},
 {Cb5, 749, 1},
 {E5, 749, 1},
 {Cb5, 624, 1},
 {E5, 124, 1},
 {Fb5, 749, 1},
 {D5, 749, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Fb5, 374, 1},
 {E5, 124, 1},
 {A5, 749, 1},
 {A5, 749, 1},
 {B5, 249, 1},
 {A5, 374, 1},
 {D5, 124, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Fb5, 374, 1},
 {E5, 124, 1},
 {Cb5, 749, 1},
 {D5, 249, 1},
 {Cb5, 374, 1},
 {B4, 124, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Db5, 374, 1},
 {E5, 124, 1},
 {A5, 749, 1},
 {B5, 249, 1},
 {A5, 374, 1},
 {D5, 124, 1},
 {Fb5, 749, 1},
 {E5, 249, 1},
 {Fb5, 374, 1},
 {Gb5, 124, 1},
 {A5, 749, 1},
 {Gb5, 249, 1},
 {Fb5, 374, 1},
 {E5, 124, 1},
 {Gb5, 749, 1},
 {A5, 249, 1},
 {Gb5, 374, 1},
 {A5, 124, 1},
 {Cb6, 749, 1},
 {A5, 1499, 0},
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
