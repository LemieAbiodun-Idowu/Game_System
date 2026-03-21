#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include "pitches.h"


int B_1 = 3;  //rotation
int B_2 = 4;  //press to 'hold' block, hold down to pause

int UP = 2;
int DOWN = 15;
int LEFT = 17;
int RIGHT = 14;

// int BUZZER = 8;

bool b1, b2, b3;
extern bool isGameOver;
extern bool isPaused;

void setup() {
  Serial.begin(9600);
  controlsSetup();

  SPI.begin();  // Init SPI bus
  setupOLED();
  setupRFID();
  cardScanning();
  drawFrame();
  initialiseGame();
}

void loop() {

  checkGameOver();
  if(isGameOver) return;

  PauseButton();
  if(isPaused) return;

  //inputs
  handleLeftRight();
  softDrop();
  hardDrop();
  holdBlocks();
  PieceRotation();

  //game logic
  updatePieceGravity(); 
}
