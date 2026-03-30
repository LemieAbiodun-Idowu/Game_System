#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include <TFT_eSPI.h>

// int JOY_B = 2; //No longer exists
int B_1 = 7;      //rotate
int B_2 = 8;      //pause and hold


int UP = 5; //change pins
int DOWN = 3;
int LEFT = 4;
int RIGHT = 2;
// int BUZZER = 8;

bool b1, b2, b3;    //Find out what these do and give them better names
extern bool isGameOver;
extern bool isPaused;

void setup() {
  UARTsetup();
  controlsSetup();
  setupRFID();
  cardScanning();
  //send message to draw frame
  initialiseGame();
}

void loop() {

  checkGameOver();
  if(isGameOver) return;

  PauseButton();
  if(isPaused) return;

  //powerups
  checkDoublePoints();
  checkPowerUpCard();
  checkSlowGravity();

  //send tetris piece information
  // sendInformation();
  //inputs
  handleLeftRight();
  softDrop();
  hardDrop();
  holdBlocks();
  PieceRotation();

  //game logic
  updatePieceGravity(); 


  
  // readControls();
  // mapControls();
  // checkGameOver();
  // if (isGameOver) return;

  // PauseButton();
  // if (isPaused) return;

  // hardDrop();
  // updatePieceGravity();

  // deadzone();

  // PieceRotation();

  // delay(100);
  // // Serial.println("----------------------------------------------------------");
}
