bool isPaused = false;
bool pauseButtonReady = true;
int deadZone = 10;
// unsigned long pressStartTime = 0;
// bool buttonHeld = false;

void controlsSetup(){
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(B_1, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
}

void PauseButton() {
  if (!digitalRead(B_2)) {
    if (pauseButtonReady) {
      isPaused = !isPaused;
      pauseButtonReady = false;
      delay(200);
      if (isPaused) {  // only draw once on toggle
        display.clearDisplay();
        drawText(F("PAUSED"), 0, 42);
        display.display();
      }
    }
  } else {
    pauseButtonReady = true;
  }
}

// void moveLeft() {
//   if (!nextCollision()) {
//     pieceX--;
//     refreshGrid();
//   }
// }

// void moveRight() {
//   if (!nextCollision()) {
//     pieceX++;
//     refreshGrid();
//   }
// }
