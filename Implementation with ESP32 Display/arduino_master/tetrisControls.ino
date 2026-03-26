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
      pauseButtonReady = false;
      unsigned long pressStart = millis();

      // Wait to see how long the button is held
      while (!digitalRead(B_2)) {
        if (millis() - pressStart > 1000) return; // held too long, ignore
      }

      // Only toggle if released before 2 seconds
      isPaused = !isPaused;
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