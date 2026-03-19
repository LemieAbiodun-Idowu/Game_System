#include "pitches.h"

const int JOY_X = A0;
const int JOY_Y = A1;
int JOY_B = 2;

int B_1 = 3;
int B_2 = 4;
int BUT1 =0;
int BUT2 =0;

int GREEN = 5;
int RED = 6;
int YELLOW = 7;

int BUZZER = 13;

int melody[] = {
  NOTE_C4, NOTE_E4, NOTE_G4,
  NOTE_C5, NOTE_G4,
  NOTE_C5
};

int noteDurations[] = {
  8, 8, 8,
  4, 8,
  2
};

int notes;

void setup() {
  Serial.begin(9600);
  pinMode(JOY_B, INPUT_PULLUP);
  pinMode(B_1, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
}

void loop() {
  //joystick
  int x = analogRead(JOY_X); //Reads a value from 0 to 1023
  int y = analogRead(JOY_Y); //Reads a value from 0 to 1023

  Serial.print("x value: ");
  Serial.println(x);

  Serial.print("y value: ");
  Serial.println(y);

  if(!digitalRead(JOY_B)){ // BUTTON RETURNS TO OFF STATE AFTER RELEASE
    Serial.println("JOYSTICK BUTTON ON");
    digitalWrite(YELLOW, HIGH);
    
    notes = sizeof(melody) / sizeof(melody[0]);

    for (int thisNote = 0; thisNote < notes; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER, melody[thisNote], noteDuration);
      delay(noteDuration * 1.25);
      noTone(BUZZER);
    }
  }
  else{
    Serial.println("JOYSTICK BUTTON OFF");
    digitalWrite(YELLOW, LOW);
  }

  //MAKE OTHER BUTTONS STAY ON/OFF WHEN PRESSED
  if(!digitalRead(B_1)){
    BUT1 = BUT1 ^ 1;
  }
  if(BUT1){
    Serial.println("BUTTON 1 ON");
    digitalWrite(GREEN, HIGH);  
  }
  else{
    Serial.println("BUTTON 1 OFF"); 
    digitalWrite(GREEN, LOW);     
  }


  if(!digitalRead(B_2)){
    BUT2 = BUT2 ^ 1;
  }
  if(BUT2){
    Serial.println("BUTTON 2 ON"); 
    digitalWrite(RED, HIGH); 
  }
  else{
    Serial.println("BUTTON 2 OFF");
    digitalWrite(RED, LOW);      
  }
  Serial.println("----------------------------------------------------------"); 
  delay(100);
 
}
