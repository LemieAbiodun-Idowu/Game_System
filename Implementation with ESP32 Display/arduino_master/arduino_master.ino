#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
// #include <TFT_eSPI.h>

// int JOY_B = 2; //No longer exists
int B_1 = 7;      //rotate
int B_2 = 8;      //speed

// int BUZZER = 8;

bool b1, b2, b3;    //Find out what these do and give them better names
extern bool isGameOver;
extern bool isPaused;

void setup() {
  UARTsetup();
  controlsSetup();
  setupRFID();
  cardScanning();

  initialiseGame();
}

void loop() {
  readControls();
  mapControls();
  checkGameOver();
  if (isGameOver) return;

  PauseButton();
  if (isPaused) return;

  hardDrop();
  updatePieceGravity();

  deadzone();

  PieceRotation();

  delay(100);
  // Serial.println("----------------------------------------------------------");
}
