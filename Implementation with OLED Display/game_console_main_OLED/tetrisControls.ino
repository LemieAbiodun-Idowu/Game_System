bool isPaused = false;
bool pauseButtonReady = true;
int deadZone = 10;
unsigned long pressStartTime = 0;
bool buttonHeld = false;

void controlsSetup(){
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(B_1, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
}


void PauseButton() {
  if (!digitalRead(B_2)) {  // button pressed
    if (!buttonHeld) {
      pressStartTime = millis();  // start timing
      buttonHeld = true;
    }

    // check if held for 2 seconds (2000 ms)
    if (buttonHeld && (millis() - pressStartTime >= 2000)) {
      if (pauseButtonReady) {
        isPaused = !isPaused;
        pauseButtonReady = false;

        if (isPaused) {
          display.clearDisplay();
          drawText(F("PAUSED"), 0, 42);
          display.display();
        }
      }
    }

  } else {  // button released
    buttonHeld = false;
    pauseButtonReady = true;
  }
}

