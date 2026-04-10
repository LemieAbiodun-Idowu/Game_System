#define A_btn 7  //rotate
#define B_btn 8  //pause and hold
#define UP_btn 3
#define DOWN_btn 5
#define LEFT_btn 2
#define RIGHT_btn 4
#define BTN_DEBOUNCE 50
#define VOLUME_CTRL 14
#define PAUSE_btn 6
// bool UPbuttonReady = true;
// bool DOWNbuttonReady = true;
// bool LEFTbuttonReady = true;
// bool RIGHTbuttonReady = true;
// bool AbuttonReady = true;
// bool BbuttonReady = true;
int16_t volume;
int16_t prevVolume = 0;
unsigned long prevAPress = 0;
unsigned long prevBPress = 0;
unsigned long prevUPPress = 0;
unsigned long prevDOWNPress = 0;
unsigned long prevLEFTPress = 0;
unsigned long prevRIGHTPress = 0;
void controlsSetup() {
  pinMode(UP_btn, INPUT_PULLUP);
  pinMode(DOWN_btn, INPUT_PULLUP);
  pinMode(LEFT_btn, INPUT_PULLUP);
  pinMode(RIGHT_btn, INPUT_PULLUP);
  pinMode(A_btn, INPUT_PULLUP);
  pinMode(B_btn, INPUT_PULLUP);
  pinMode(VOLUME_CTRL, INPUT);
  pinMode(PAUSE_btn, INPUT_PULLUP);
}

void setVolume() {
  //volume = analogRead(VOLUME_CTRL);
  volume = map(analogRead(VOLUME_CTRL), 0, 1023, 0, 255);
  if (abs(volume - prevVolume) >= 10) {
    sendVolume(volume);
    // Serial.print("volum is ");
    // Serial.println(volume);
    // Serial.print("prevol is ");
    // Serial.println(prevVolume);
    // Serial.print("vol dif is ");
    // Serial.println(abs(volume - prevVolume));
    prevVolume = volume;
  }
}


void enterMainMenu() {  //alternate name suggestions?
  while (digitalRead(A_btn) && digitalRead(B_btn)) { setVolume(); }
  enterMenuMsg();
  currentScreen = MAIN_MENU;
  while (handleMessage() != MENU_ENTERED_ESP) { setVolume(); }
  while (!digitalRead(A_btn) || !digitalRead(B_btn)) {
    // Serial.println("Stuck here");
    setVolume();
  }
}
bool prevMENUAState = HIGH;
bool prevMENUBState = HIGH;
bool prevMENUUPState = HIGH;
bool prevMENUDOWNState = HIGH;
bool prevMENULEFTState = HIGH;
bool prevMENURIGHTState = HIGH;

void controlMainMenu() {
  bool currA = digitalRead(A_btn);
  bool currB = digitalRead(B_btn);
  bool currUP = digitalRead(UP_btn);
  bool currDOWN = digitalRead(DOWN_btn);
  bool currLEFT = digitalRead(LEFT_btn);
  bool currRIGHT = digitalRead(RIGHT_btn);

  // only fire on the transition from HIGH to LOW (moment of press)
  if (currA == LOW && prevMENUAState == HIGH) {
    // Serial.print("A FIRED, prevA was: ");
    // Serial.println(prevAState);
    if (millis() - prevAPress >= BTN_DEBOUNCE) {
      A_MenuPress();
      prevAPress = millis();
    }
  }
  if (currB == LOW && prevMENUBState == HIGH) {
    if (millis() - prevBPress >= BTN_DEBOUNCE) {
      B_MenuPress();
      prevBPress = millis();
    }
  }
  if (currUP == LOW && prevMENUUPState == HIGH) {
    if (millis() - prevUPPress >= BTN_DEBOUNCE) {
      UP_MenuPress();
      prevUPPress = millis();
    }
  }
  if (currDOWN == LOW && prevMENUDOWNState == HIGH) {
    if (millis() - prevDOWNPress >= BTN_DEBOUNCE) {
      DOWN_MenuPress();
      prevDOWNPress = millis();
    }
  }
  if (currLEFT == LOW && prevMENULEFTState == HIGH) {
    if (millis() - prevLEFTPress >= BTN_DEBOUNCE) {
      LEFT_MenuPress();
      prevLEFTPress = millis();
    }
  }
  if (currRIGHT == LOW && prevMENURIGHTState == HIGH) {
    if (millis() - prevRIGHTPress >= BTN_DEBOUNCE) {
      RIGHT_MenuPress();
      prevRIGHTPress = millis();
    }
  }

  prevMENUAState = currA;
  prevMENUBState = currB;
  prevMENUUPState = currUP;
  prevMENUDOWNState = currDOWN;
  prevMENULEFTState = currLEFT;
  prevMENURIGHTState = currRIGHT;
}