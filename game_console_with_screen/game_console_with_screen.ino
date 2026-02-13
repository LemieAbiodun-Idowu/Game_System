#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   11
#define OLED_CLK   13
#define OLED_DC    9
#define OLED_CS    10
#define OLED_RESET 8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


#define LOGO_HEIGHT   64
#define LOGO_WIDTH    128
const unsigned char SimpleLogo_V1 [] = {};
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

int BUZZER = 12;

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

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  testdrawbitmap();    // Draw a small bitmap image
  delay(5000);

  display.clearDisplay();
}

void loop() {
  //joystick
  int x = analogRead(JOY_X); //Reads a value from 0 to 1023
  int y = analogRead(JOY_Y); //Reads a value from 0 to 1023

  display.clearDisplay();

  int x_point = map(x, 0, 1023, 0, 128);
  int y_point = map(y, 1023, 0, 0, 64);

  display.drawPixel(x_point, y_point, SSD1306_WHITE);
  // display.drawLine(64, 32, x_point, y_point, SSD1306_WHITE);
  // Serial.print("x value: ");
  // Serial.println(x_point);

  // Serial.print("y value: ");
  // Serial.println(y_point);

  display.display();

  if(!digitalRead(JOY_B)){ // BUTTON RETURNS TO OFF STATE AFTER RELEASE
    // Serial.println("JOYSTICK BUTTON ON");
    digitalWrite(YELLOW, HIGH);
    display.clearDisplay();
    // display.setTextSize(1);                  // Text size (1 = small)
    // display.setTextColor(SSD1306_WHITE);     // Turn pixels ON
    // display.setCursor(64, 32);                // Top-left corner (x, y)
    // display.print("YELLOW");
    // display.display();
    
    notes = sizeof(melody) / sizeof(melody[0]);

    for (int thisNote = 0; thisNote < notes; thisNote++) {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER, melody[thisNote], noteDuration);
      delay(noteDuration * 1.25);
      noTone(BUZZER);
    }
  }
  else{
    // Serial.println("JOYSTICK BUTTON OFF");
    digitalWrite(YELLOW, LOW);
  }

  //MAKE OTHER BUTTONS STAY ON/OFF WHEN PRESSED
  if(!digitalRead(B_1)){
    BUT1 = BUT1 ^ 1;
  }
  if(BUT1){
    // Serial.println("BUTTON 1 ON");
    digitalWrite(GREEN, HIGH);  
  	
    display.clearDisplay();
    display.setTextSize(1);                  // Text size (1 = small)
    display.setTextColor(SSD1306_WHITE);     // Turn pixels ON
    display.setCursor(64, 32);                // Top-left corner (x, y)
    display.print("GREEN");
    display.display();
  }
  else{
    // Serial.println("BUTTON 1 OFF"); 
    digitalWrite(GREEN, LOW); 
    display.clearDisplay();   
  }


  if(!digitalRead(B_2)){
    BUT2 = BUT2 ^ 1;
  }
  if(BUT2){
    // Serial.println("BUTTON 2 ON"); 
    digitalWrite(RED, HIGH);
    display.clearDisplay();
    display.setTextSize(1);                  // Text size (1 = small)
    display.setTextColor(SSD1306_WHITE);     // Turn pixels ON
    display.setCursor(64, 32);                // Top-left corner (x, y)
    display.print("RED");
    display.display(); 
  }
  else{
    // Serial.println("BUTTON 2 OFF");
    digitalWrite(RED, LOW);  
    display.clearDisplay();    
  }
  // Serial.println("----------------------------------------------------------"); 
  delay(200);
 
}


void testdrawbitmap(void) {
  display.clearDisplay();
  display.drawBitmap(0, 0, SimpleLogo_V1, LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);
  display.display();
  delay(1000);
}