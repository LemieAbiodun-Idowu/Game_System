#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// #include "pitches.h"

// static const unsigned char PROGMEM mantex_logo [] = {
//}

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI 11  //D1
#define OLED_CLK 13   //D0
#define OLED_DC 19
#define OLED_CS 16
#define OLED_RESET 18
#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

//Define all valid uid
#define UID_CardA \
  { 0x53, 0x2A, 0xE0, 0x2E, 0x02, 0x00, 0x01 }
#define UID_CardB \
  { 0x53, 0xA7, 0x82, 0x2F, 0x02, 0x00, 0x01 }
#define UID_CardC \
  { 0x53, 0xF2, 0x92, 0x2E, 0x02, 0x00, 0x01 }
#define UID_CardD \
  { 0x53, 0x90, 0x69, 0x2E, 0x02, 0x00, 0x01 }
#define UID_CardE \
  { 0x53, 0x3C, 0xA5, 0x2E, 0x02, 0x00, 0x01 }
#define UID_CardF \
  { 0x53, 0xAE, 0xFD, 0x2E, 0x02, 0x00, 0x01 }
#define UID_CardG \
  { 0x53, 0x53, 0x3C, 0x2C, 0x02, 0x00, 0x01 }
#define UID_CardH \
  { 0x53, 0xCE, 0xE4, 0x2D, 0x02, 0x00, 0x01 }
//Invalid Cards
// #define UID_CardI \
//   { 0x53, 0x70, 0x47, 0x2E, 0x02, 0x00, 0x01 }
// #define UID_CardJ \
  { 0x53, 0x2C, 0x72, 0x2E, 0x02, 0x00, 0x01 }

const byte UID_Cards[][7] PROGMEM = {
  UID_CardA,
  UID_CardB,
  UID_CardC,
  UID_CardD,
  UID_CardE,
  UID_CardF,
  UID_CardG,
  UID_CardH
  // UID_CardI,
  // UID_CardJ
};

bool NOVALID_CARD = true;
int NUM_CARDS = sizeof(UID_Cards) / sizeof(UID_Cards[0]);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RESET, OLED_CS);


const int JOY_X = A0;
const int JOY_Y = A1;
int JOY_B = 2;

int B_1 = 3; //rotate
int B_2 = 4; //speed
int BUT1 =0;
int BUT2 =0;


int GREEN = 5;
int RED = 6;
int BLUE = 7;

int BUZZER = 8; 

// const int melody[] PROGMEM = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, NOTE_G4, NOTE_C5 };
// const int noteDurations[] PROGMEM = { 8, 8, 8, 4, 8, 2 };

// int notes;

int x_last = 64;
int y_last = 32;

const short MARGIN_TOP = 19;
const short MARGIN_LEFT = 3;
const short SIZE = 5;
const short TYPES = 6;
const int erase[] PROGMEM = { 2093 };
const int erase_duration[] PROGMEM = { 100 };
const int click[] PROGMEM = { 1047 };
const int click_duration[] PROGMEM = { 100 };
byte grid[10][18]; 
bool b1, b2, b3;
int deadZone = 10;
bool isGameOver = false;

int interval = 500; 
int score = 0;
long timer, delayer;

word currentType, nextType, rotation;
short pieceX, pieceY;
short piece[2][4];

bool isPaused = false;
bool pauseButtonReady = true;

const char pieces_S_l[2][2][4] PROGMEM = { { { 0, 0, 1, 1 }, { 0, 1, 1, 2 } }, { { 0, 1, 1, 2 }, { 1, 1, 0, 0 } } };
const char pieces_S_r[2][2][4] PROGMEM = { { { 1, 1, 0, 0 }, { 0, 1, 1, 2 } },
                                           { { 0, 1, 1, 2 }, { 0, 0, 1, 1 } } };
const char pieces_L_l[4][2][4] PROGMEM = { { { 0, 0, 0, 1 }, { 0, 1, 2, 2 } },
                                           { { 0, 1, 2, 2 }, { 1, 1, 1, 0 } },
                                           { { 0, 1, 1, 1 }, { 0, 0, 1, 2 } },
                                           { { 0, 0, 1, 2 }, { 1, 0, 0, 0 } } };
const char pieces_Sq[1][2][4] PROGMEM = { { { 0, 1, 0, 1 }, { 0, 0, 1, 1 } } };
const char pieces_T[4][2][4] PROGMEM = { { { 0, 0, 1, 0 }, { 0, 1, 1, 2 } },
                                         { { 0, 1, 1, 2 }, { 1, 0, 1, 1 } },
                                         { { 1, 0, 1, 1 }, { 0, 1, 1, 2 } },
                                         { { 0, 1, 1, 2 }, { 0, 0, 1, 0 } } };
const char pieces_l[2][2][4] PROGMEM = { { { 0, 1, 2, 3 }, { 0, 0, 0, 0 } },
                                         { { 0, 0, 0, 0 }, { 0, 1, 2, 3 } } };

void checkLines(){
    bool full;
    for(short y = 17; y >= 0; y--){
      full = true;
      for(short x = 0; x < 10; x++){
        full = full && grid[x][y];
      }
      if(full){
        breakLine(y);
        y++;
      }
    }
  }
void breakLine(short line){
    // tone(BUZZER, erase[0], 1000 / erase_duration[0]); 
    // delay(100);
    // noTone(BUZZER);
    for(short y = line; y > 0; y--){
  for(short x = 0; x < 10; x++){
    grid[x][y] = grid[x][y-1];
  }
}

  for(short x = 0; x < 10; x++){
    grid[x][0] = 0;
  }
  display.invertDisplay(true);
  delay(50);
  display.invertDisplay(false);
  score += 10;
}
void refresh(){
    display.clearDisplay();
    drawLayout();
    drawGrid();
    drawPiece(currentType, 0, pieceX, pieceY);
    display.display();
}
void drawGrid(){
  for(short x = 0; x < 10; x++)
    for(short y = 0; y < 18; y++)
      if(grid[x][y])
        display.fillRect(MARGIN_LEFT + (SIZE + 1)*x, MARGIN_TOP + (SIZE + 1)*y, SIZE, SIZE, WHITE);
}
bool nextHorizontalCollision(short piece[2][4], int amount){
  for(short i = 0; i < 4; i++){
    short newX = pieceX + piece[0][i] + amount;
    if(newX > 9 || newX < 0 || grid[newX][pieceY + piece[1][i]])
      return true;
  }
  return false;
}
bool nextCollision(){
  for(short i = 0; i < 4; i++){
    short y = pieceY + piece[1][i] + 1;
    short x = pieceX + piece[0][i];
    if(y > 17 || grid[x][y])
      return true;
  }
  return false;
}

void generate(){
    currentType = nextType;
    nextType = random(TYPES);
    pieceX = 3;
    pieceY = 0;
    rotation = 0;
    copyPiece(piece, currentType, rotation);
  }
void drawPiece(short type, short rotation, short x, short y){
  for(short i = 0; i < 4; i++)
    display.fillRect(MARGIN_LEFT + (SIZE + 1)*(x + piece[0][i]), MARGIN_TOP + (SIZE + 1)*(y + piece[1][i]), SIZE, SIZE, WHITE);
}
void drawNextPiece(){
  short nPiece[2][4];
  copyPiece(nPiece, nextType, 0);
  for(short i = 0; i < 4; i++)
    display.fillRect(50 + 3*nPiece[0][i], 4 + 3*nPiece[1][i], 2, 2, WHITE);
}
bool spawnCollision() {
  for (short i = 0; i < 4; i++) {
    short x = pieceX + piece[0][i];
    short y = pieceY + piece[1][i];

    if (grid[x][y]|| y >= 14) {
      return true;
    }
  }
  return false;
}
void copyPiece(short piece[2][4], short type, short rotation) {
  switch (type) {
    case 0:  //L_l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_L_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_L_l[rotation][1][i]));
      }
      break;
    case 1:  //S_l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_S_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_S_l[rotation][1][i]));
      }
      break;
    case 2:  //S_r
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_S_r[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_S_r[rotation][1][i]));
      }
      break;
    case 3:  //Sq
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_Sq[0][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_Sq[0][1][i]));
      }
      break;
    case 4:  //T
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_T[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_T[rotation][1][i]));
      }
      break;
    case 5:  //l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_l[rotation][1][i]));
      }
      break;
  }
}

short getMaxRotation(short type){
  if(type == 1 || type == 2 || type == 5)
    return 2;
  else if(type == 0 || type == 4)
    return 4;
  else if(type == 3)
    return 1;
  else
    return 0;
}

/////////////////////////START HERE
bool canRotate(short rotation){
  short piece[2][4];
  copyPiece(piece, currentType, rotation);
  return !nextHorizontalCollision(piece, 0);
}

void drawLayout() {
  // display.drawLine(0, 64, SCREEN_WIDTH, 32, WHITE);
  display.drawRect(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, WHITE);
  drawNextPiece();
  char text[6];
  itoa(score, text, 10);
  drawTextLayout(text, 7, 4);
  display.drawLine(0, 14, SCREEN_WIDTH, 14, WHITE);
}
// short getNumberLength(int n){
//   short counter = 1;
//   while(n >= 10){
//     n /= 10;
//     counter++;
//   }
//   return counter;
// }

void drawTextLayout(const char* text, int x, int y) {

  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(x, y);              // Start at top-left corner
  display.cp437(true);                  // Use full 256 char 'Code Page 437' font
  display.print(text);
}

void drawText(const __FlashStringHelper* text, int x, int y) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x, y);
  display.cp437(true);
  display.print(text);  // print() handles F() strings natively
}

void gameOver() {


  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15, 25);
  display.print("GAME OVER");
  display.display();

  isGameOver = true;

}

void resetGame() {

  // Clear grid
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 18; y++)
      grid[x][y] = 0;

  score = 0;
  interval = 500;

  display.clearDisplay();
  display.display();

  nextType = random(TYPES);
  generate();

  isGameOver = false;
}

void setup() {
  Serial.begin(9600);
  pinMode(JOY_B, INPUT_PULLUP);
  pinMode(B_1, INPUT_PULLUP);
  pinMode(B_2, INPUT_PULLUP);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);

  SPI.begin();  // Init SPI bus
  pinMode(OLED_CS, OUTPUT);
  digitalWrite(OLED_CS, HIGH);
  digitalWrite(SS_PIN, LOW);
  pinMode(OLED_RESET, OUTPUT);
  digitalWrite(OLED_RESET, HIGH);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }

  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  mfrc522.PCD_Init();  // Init MFRC522 card
  delay(50);           // give reader time to boot
  Serial.println("Card Initialised");

  display.setRotation(1);
  display.clearDisplay();
    drawText(F("PLEASE    SCAN A    VALID CARD"), 0, 42);  
  
  display.display();
  
while (true) {
    if (!mfrc522.PICC_IsNewCardPresent()) continue;
    if (!mfrc522.PICC_ReadCardSerial()) continue;
    NOVALID_CARD = true;
    int j = 0;                                                              //To allow access outside of loop
    for (j; j < NUM_CARDS; j++) {                                           //Look through every storedcard
      bool CardMatch = true;                                                //Controls if bytes are matching are matching or not
      for (byte i = 0; i < mfrc522.uid.size; i++) {                         //Check each byte of the UID
        if (mfrc522.uid.uidByte[i] != pgm_read_byte(&(UID_Cards[j][i]))) {  //if not equal move onto next card
                                                                            // Serial.print("This Card doesnt match Card ");
                                                                            // Serial.println(j+1);
          CardMatch = false;
          break;
        }
        //   if (j == (NUM_CARDS - 1))  //if byte of final card then no cards are valid
        //     INVALID_CARD = true;
        // }
      }
      if (CardMatch) {
        NOVALID_CARD = false;
        break;
      }
    }
    if (NOVALID_CARD) {
      display.clearDisplay();
      drawText(F("ACCESS    DENIED              INVALID   CARD LMAO"), 0, 32);
      display.display();
      delay(2000);
    } else {
      display.clearDisplay();
      drawText(F("ACCESS GRANTED TO   CARD"), 0, 42);
      display.display();
      delay(5000);
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
      digitalWrite(SS_PIN, HIGH);  //Turn rfid off
      break;
    }
  }

  display.clearDisplay();
  drawLayout();  
  display.display();
  randomSeed(analogRead(0));
  nextType = random(TYPES);
  generate();
  timer = millis();
}

void loop() {
  //joystick
  int x = analogRead(JOY_X); //Reads a value from 0 to 1023
  int y = analogRead(JOY_Y); //Reads a value from 0 to 1023

  // display.clearDisplay();

  int x_point = map(x, 0, 1023, 0, 64);
  int y_point = map(y, 1023, 0, 0, 128);

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
  if (isGameOver) {

    if (!digitalRead(B_2)) {   // press joystick button to restart
    delay(200);                // simple debounce
    resetGame();
  }
  return;
  }
  // --- Pause Button ---
  if (!digitalRead(B_2)) {
    if (pauseButtonReady) {
      isPaused = !isPaused;   // toggle pause
      pauseButtonReady = false;
      delay(200);             // simple debounce
    }
  } else {
    pauseButtonReady = true;
  }
  if (isPaused) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 25);
  display.print("PAUSED");
  display.display();
  return;   // stop everything else
}

// --- HARD DROP (Joystick Button) ---
if (!digitalRead(JOY_B)) {   // button pressed (INPUT_PULLUP)
  
  // Move piece down until collision
  while (!nextCollision()) {
    pieceY++;
  }

  // Lock piece into grid
  for (short i = 0; i < 4; i++) {
    grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
  }

  generate();

  if (spawnCollision()) {
    gameOver();
  }

  refresh();
  delay(200);   // simple debounce
}

  if(millis() - timer > interval){
    checkLines();
    refresh();
    if(nextCollision()){
      for(short i = 0; i < 4; i++)
        grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
      generate();
      if (spawnCollision()) {
      gameOver();
      return;
  }
    }else
      pieceY++;
    timer = millis();
  }

  if (x_point < 32 - deadZone) {
    if (!nextHorizontalCollision(piece, -1)) {
      pieceX--;
      refresh();
    }
  }
  else if (x_point > 32 + deadZone) {
    if (!nextHorizontalCollision(piece, 1)) {
      pieceX++;
      refresh();
    }
  }
  // if(x_point >=0 && x_point < 32){
  //     // tone(BUZZER, click[0], 1000 / click_duration[0]);
  //     // delay(100);
  //     // noTone(BUZZER);
  //     if(b2){
  //       if(!nextHorizontalCollision(piece, 1)){
  //         pieceX++;
  //         refresh();
  //       }
  //       b2 = false;
  //     }
  //   }
  // else{
  //     b2 = true;
  //     }
  // if(!digitalRead(speed)){ //assign to speed
  //     interval = 20;
  //   } else{
  //     interval = 400;
  //     }
 
  if(!digitalRead(B_1)){ //assign to change
      // tone(BUZZER, click[0], 1000 / click_duration[0]);
      // delay(100);
      // noTone(BUZZER);
      if(b3){
        if(rotation == getMaxRotation(currentType) - 1 && canRotate(0)){
          rotation = 0;
        }else if(canRotate(rotation + 1)){
          rotation++;
        }  
        copyPiece(piece, currentType, rotation);
        refresh();
        b3 = false;
        delayer = millis();
      }
    }else if(millis() - delayer > 50){
      b3 = true;
    }

      delay(100);
    // Serial.println("----------------------------------------------------------"); 
}
