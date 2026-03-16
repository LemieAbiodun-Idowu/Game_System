#include "audio.h"
#include "pokemon_center_melody.h"
#include "pokemon_center_accompaniment"

void playSong(int pin, const int notes[][3], size_t len){
  for (int i = 0; i < len; i++) {
    int note = pgm_read_word(&notes[i][0]);
    int duration = pgm_read_word(&notes[i][1]);
    int pause = pgm_read_word(&notes[i][2]);

    tone(pin, note, duration);
    delay(duration);
    noTone(pin);
    delay(pause);
  }

}

void setup() {
}

void loop(){
  playSong(pin, Songname, Songlength);
  delay(2000);
}