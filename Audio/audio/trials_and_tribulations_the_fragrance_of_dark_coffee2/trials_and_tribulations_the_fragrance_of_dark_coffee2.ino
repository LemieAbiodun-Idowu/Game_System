// Can be moved in header file i.e notes.h
#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define A3 220
#define D3 147
#define E3 165
#define F_Sh_3 185
#define A_Sh_3 233
#define G3 196
#define C3 131
#define C_Sh_4 277

const int midi1[267][3]PROGMEM = {
  {0, 0, 1232},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {A3, 1753, 94},
 {F_Sh_3, 1753, 94},
 {A_Sh_3, 1753, 94},
 {E3, 1753, 94},
 {G3, 1753, 94},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {A3, 1753, 94},
 {F_Sh_3, 1753, 94},
 {A_Sh_3, 1753, 94},
 {E3, 3506, 186},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {A3, 1753, 94},
 {C3, 1753, 94},
 {D3, 1753, 94},
 {E3, 3506, 186},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {C_Sh_4, 1753, 94},
 {F_Sh_3, 1753, 94},
 {A_Sh_3, 1753, 94},
 {E3, 3506, 186},
 {F_Sh_3, 876, 47},
 {C3, 876, 47},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {G3, 1753, 94},
 {E3, 3506, 186},
 {D3, 3506, 186},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {A3, 1753, 94},
 {F_Sh_3, 1753, 94},
 {A_Sh_3, 1753, 94},
 {E3, 1753, 94},
 {G3, 1753, 94},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {A3, 1753, 94},
 {F_Sh_3, 1753, 94},
 {A_Sh_3, 1753, 94},
 {E3, 3506, 186},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {A3, 1753, 94},
 {C3, 1753, 94},
 {D3, 1753, 94},
 {E3, 3506, 186},
 {A3, 1753, 94},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {C_Sh_4, 1753, 94},
 {F_Sh_3, 1753, 94},
 {A_Sh_3, 1753, 94},
 {E3, 3506, 186},
 {F_Sh_3, 876, 47},
 {C3, 876, 47},
 {D3, 1753, 94},
 {E3, 1753, 94},
 {G3, 1753, 94},
 {E3, 3506, 186},
 {D3, 3506, 186},
 {A3, 1753, 0},
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
