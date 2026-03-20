#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include "pitches.h"

// int JOY_B = 2;  //Please explain what this used for
int B_1 = 3;  //rotate
int B_2 = 4;  //speed

int UP = 2;
int DOWN = 15;
int LEFT = 17;
int RIGHT = 14;

// int BUT1 = 0;
// int BUT2 = 0;

// int GREEN = 5;
// int RED = 6;
// int BLUE = 7;
int BUZZER = 8;

// const int melody[] PROGMEM = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5 };
// const int noteDurations[] PROGMEM = { 8, 8, 8, 4, 8, 2 };

// int notes;

// const int erase[] PROGMEM = { 2093 };
// const int erase_duration[] PROGMEM = { 100 };
// const int click[] PROGMEM = { 1047 };
// const int click_duration[] PROGMEM = { 100 };

bool b1, b2, b3;
extern bool isGameOver;
extern bool isPaused;

void setup() {
  Serial.begin(9600);
  controlsSetup();
  // pinMode(GREEN, OUTPUT);
  // pinMode(RED, OUTPUT);
  // pinMode(BLUE, OUTPUT);

  SPI.begin();  // Init SPI bus
  setupOLED();
  setupRFID();
  cardScanning();
  drawFrame();
  initialiseGame();
}

void loop() {
  readControls();
  mapControls();
 
  checkGameOver();
  if(isGameOver) return;

  PauseButton();
  if(isPaused) return;

  hardDrop();
  updatePieceGravity(); 

  deadzone();  
 
  PieceRotation();

  delay(100);
  // Serial.println("----------------------------------------------------------");
}
