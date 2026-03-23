#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <TFT_eSPI.h>
  // #include <Adafruit_GFX.h>
  // #include <Adafruit_SSD1306.h>
  // #include "pitches.h"

int JOY_B = 2; 
int B_1 = 7;      //rotate
int B_2 = 8;      //speed
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

bool b1, b2, b3;    //Find out what these do and give them better names
extern bool isGameOver;
extern bool isPaused;

void setup() {
  UARTsetup();
  controlsSetup();
  setupRFID();
  cardScanning();
  // drawFrame();
  initialiseGame();
}

void loop() {
  readControls();
  mapControls();
  // display.drawPixel(x_point, y_point, SSD1306_WHITE);
  // display.drawLine(x_last, y_last, x_point, y_point, SSD1306_WHITE);
  // x_last = x_point;
  // y_last = y_point;
  // // Serial.print("x value: ");
  // // Serial.println(x_point);

  // // Serial.print("y value: ");
  // // Serial.println(y_point);

  // display.display();
  // if(!digitalRead(JOY_B)){ // BUTTON RETURNS TO OFF STATE AFTER RELEASE
  //   // Serial.println("JOYSTICK BUTTON ON");
  //   digitalWrite(YELLOW, HIGH);
  //   // display.clearDisplay();
  //   // display.setTextSize(1);                  // Text size (1 = small)
  //   // display.setTextColor(SSD1306_WHITE);     // Turn pixels ON
  //   // display.setCursor(64, 32);                // Top-left corner (x, y)
  //   // display.print("YELLOW");
  //   // display.display();

  //   notes = sizeof(melody) / sizeof(melody[0]);

  //   for (int thisNote = 0; thisNote < notes; thisNote++) {
  //     int noteDuration = 1000 / noteDurations[thisNote];
  //     tone(BUZZER, melody[thisNote], noteDuration);
  //     delay(noteDuration * 1.25);
  //     noTone(BUZZER);
  //   }
  // }
  // else{
  //   // Serial.println("JOYSTICK BUTTON OFF");
  //   digitalWrite(YELLOW, LOW);
  // }

  // //MAKE OTHER BUTTONS STAY ON/OFF WHEN PRESSED
  // if(!digitalRead(B_1)){
  //   BUT1 = BUT1 ^ 1;
  // }
  // if(BUT1){
  //   // Serial.println("BUTTON 1 ON");
  //   digitalWrite(GREEN, HIGH);

  //   // display.clearDisplay();
  //   display.setTextSize(1);                  // Text size (1 = small)
  //   display.setTextColor(SSD1306_WHITE);     // Turn pixels ON
  //   display.setCursor(64, 32);                // Top-left corner (x, y)
  //   display.print("GREEN");
  //   display.display();
  // }
  // else{
  //   // Serial.println("BUTTON 1 OFF");
  //   digitalWrite(GREEN, LOW);
  //   // display.clearDisplay();
  // }


  // if(!digitalRead(B_2)){
  //   BUT2 = BUT2 ^ 1;
  // }
  // if(BUT2){
  //   // Serial.println("BUTTON 2 ON");
  //   digitalWrite(RED, HIGH);
  //   // display.clearDisplay();
  //   display.setTextSize(1);                  // Text size (1 = small)
  //   display.setTextColor(SSD1306_WHITE);     // Turn pixels ON
  //   display.setCursor(64, 32);                // Top-left corner (x, y)
  //   display.print("RED");
  //   display.display();
  // }
  // else{
  //   // Serial.println("BUTTON 2 OFF");
  //   digitalWrite(RED, LOW);
  //   // display.clearDisplay();
  // }
  checkGameOver();
  if (isGameOver) return;

  PauseButton();
  if (isPaused) return;

  hardDrop();
  updatePieceGravity();

  deadzone();
  // if (x_point >= 0 && x_point < 32) {
  //   // tone(BUZZER, click[0], 1000 / click_duration[0]);
  //   // delay(100);
  //   // noTone(BUZZER);
  //   if (b2) {
  //     if (!nextHorizontalCollision(piece, 1)) {
  //       pieceX++;
  //       refresh();
  //     }
  //     b2 = false;
  //   }
  // } else {
  //   b2 = true;
  // }
  // if (!digitalRead(speed)) {  //assign to speed
  //   interval = 20;
  // } else {
  //   interval = 400;
  // }
  PieceRotation();

  delay(100);
  // Serial.println("----------------------------------------------------------");
}
