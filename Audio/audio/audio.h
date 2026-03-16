#ifndef AUDIO_H
#define AUDIO_H


#define ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define Fb3 185
#define D3 147
#define A3 220
#define B3 247
#define G3 196
#define E3 165
#define Cb4 277
#define D4 294
#define E4 330
#define D5 587
#define A4 440
#define A5 880
#define G5 784
#define Fb5 740
#define E5 659
#define Cb5 554
#define B5 988




void playSong(int pin, const int notes[][3], size_t len);

#endif