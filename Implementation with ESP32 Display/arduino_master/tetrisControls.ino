bool isPaused = false;
bool pauseButtonReady = true;
int deadZone = 10;

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
      
      if (isPaused) {
        Serial.println("STATE:PAUSE");
      } else {
        Serial.println("STATE:PLAY");
        refreshGrid();
      }
    }
  } else {
    pauseButtonReady = true;
  }
}