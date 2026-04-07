// #include "notemap.h"
#pragma once
#include <cstddef>
#include <Arduino.h>

extern uint16_t volume;
extern bool songPlaying;
struct Buzzer {
  uint8_t buzzerPin;
  uint8_t channel;
  bool isPlaying;
  uint16_t index;
  unsigned long nextEvent;
};

#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
// void playSong(int pin, const int notes[][3], size_t len);
void updateTrack(Buzzer &buzzer, const uint16_t track[][3], const size_t trackLength, const unsigned long currentTime);
void buzzerSetup(Buzzer &buzzer);
void startSong(uint8_t index);
void stopSong();
void playNextSong();
bool isSongFinished();
void updateSong();

const char *getCurrentSongName();
void buzzersSetup();