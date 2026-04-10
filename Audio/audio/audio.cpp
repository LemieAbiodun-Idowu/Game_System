#include "audio.h"
#include "notemap.h"
#include "songs/songs.h"
#include <Arduino.h>
#define CH1 1
#define CH2 4
#define CH3 7
#define BUZ1_PIN 27
#define BUZ2_PIN 1
#define BUZ3_PIN 3



// const uint16_t midi1[3][3]PROGMEM = {
//  {NOTE_F4, 77, 5},
//  {NOTE_F_Sh_4, 77, 5},
//  {NOTE_G6, 77, 5},
// };
uint16_t volume = 127;
Buzzer buzzer1 = { BUZ1_PIN, CH1, false, 0, 0 };
Buzzer buzzer2 = { BUZ2_PIN, CH2, false, 0, 0 };
Buzzer buzzer3 = { BUZ3_PIN, CH3, false, 0, 0 };

void updateTrack(Buzzer& buzzer, const uint16_t track[][3], const size_t trackLength, const unsigned long currentTime) {
  if (buzzer.index >= trackLength) return;
  uint16_t note = track[buzzer.index][0];
  if (note != 0) {
    note = frequencies[note];
  }
  uint16_t duration = track[buzzer.index][1];
  uint16_t pause = track[buzzer.index][2];
  if (!buzzer.isPlaying) {
    if (currentTime >= buzzer.nextEvent) {

      ledcWriteTone(buzzer.channel, note);
      // ledcWrite(buzzer.channel, volume);
      buzzer.nextEvent = buzzer.nextEvent + duration;
      buzzer.isPlaying = true;
    }

  } else if (currentTime >= buzzer.nextEvent) {
    ledcWriteTone(buzzer.channel, 0);
    buzzer.nextEvent = buzzer.nextEvent + pause;
    buzzer.isPlaying = false;
    buzzer.index++;
  }
  //ledcWrite(channel, dutyCycle); where dutyCycle = 0-255
}

void buzzerSetup(Buzzer& buzzer) {
  ledcSetup(buzzer.channel, 2000, 8);
  ledcAttachPin(buzzer.buzzerPin, buzzer.channel);
}

// void setup(){
//   buzzerSetup(buzzer1);
//   buzzerSetup(buzzer2);
//   buzzerSetup(buzzer3);

// }
// void loop() {
//   unsigned long currentTime = millis();
//   updateTrack(buzzer1, midi1, ARRAY_LEN(midi1), currentTime);
//   updateTrack(buzzer2, midi1, ARRAY_LEN(midi1), currentTime);
//   updateTrack(buzzer3, midi1, ARRAY_LEN(midi1), currentTime);
// }
uint8_t currentSongIndex = 0;
bool songPlaying = false;

void startSong(uint8_t index) {
  currentSongIndex = index;
  songPlaying = true;
  unsigned long present = millis();
  // Reset all buzzers
  buzzer1.index = 0;
  buzzer1.isPlaying = false;
  buzzer1.nextEvent = present;
  buzzer2.index = 0;
  buzzer2.isPlaying = false;
  buzzer2.nextEvent = present;
  buzzer3.index = 0;
  buzzer3.isPlaying = false;
  buzzer3.nextEvent = present;

  ledcWriteTone(buzzer1.channel, 0);
  ledcWriteTone(buzzer2.channel, 0);
  ledcWriteTone(buzzer3.channel, 0);
}

void stopSong() {
  songPlaying = false;
  ledcWriteTone(buzzer1.channel, 0);
  ledcWriteTone(buzzer2.channel, 0);
  ledcWriteTone(buzzer3.channel, 0);
}

void playNextSong() {
  currentSongIndex = (currentSongIndex + 1) % (sizeof(songList) / sizeof(songList[0]));
  startSong(currentSongIndex);
}

bool isSongFinished() {
  const Song& s = songList[currentSongIndex];
  return buzzer1.index >= s.ch1_len && buzzer2.index >= s.ch2_len && (s.ch3_len == 0 || buzzer3.index >= s.ch3_len);
}

void updateSong() {
  if (!songPlaying) return;

  // Auto-loop when finished
  if (isSongFinished()) {
    startSong(currentSongIndex);
    return;
  }

  const Song& s = songList[currentSongIndex];
  unsigned long now = millis();

  if (s.ch1_len > 0) updateTrack(buzzer1, s.ch1, s.ch1_len, now);
  if (s.ch2_len > 0) updateTrack(buzzer2, s.ch2, s.ch2_len, now);
  if (s.ch3_len > 0) updateTrack(buzzer3, s.ch3, s.ch3_len, now);
}

const char* getCurrentSongName() {
  return songList[currentSongIndex].name;
}

void buzzersSetup() {
  buzzerSetup(buzzer1);
  buzzerSetup(buzzer2);
  buzzerSetup(buzzer3);
}
