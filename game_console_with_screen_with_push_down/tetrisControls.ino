const int JOY_X = A0;
const int JOY_Y = A1;
int x_tilt, x_point, y_tilt, y_point; //Mapping joystick positions to the display
int x_last = 64;
int y_last = 32;
bool isPaused = false;
bool pauseButtonReady = true;
int deadZone = 10;

void controlsSetup(){
  pinMode(JOY_B, INPUT_PULLUP);
  pinMode(B_1, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
}

void readControls(){
  
  x_tilt = analogRead(JOY_X);  //Reads a value from 0 to 1023
  y_tilt = analogRead(JOY_Y);  //Reads a value from 0 to 1023

}

void mapControls(){
  x_point = map(x_tilt, 0, 1023, 0, 64);
  y_point = map(y_tilt, 1023, 0, 0, 128);
}

void PauseButton(){
  // --- Pause Button ---
  if (!digitalRead(B_2)) {
    if (pauseButtonReady) {
      isPaused = !isPaused;  // toggle pause
      pauseButtonReady = false;
      delay(200);  // simple debounce
    }
  } else {
    pauseButtonReady = true;
  }
  if (isPaused) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 42);
    display.print("PAUSED");
    display.display();
    return;  // stop everything else
  }
}