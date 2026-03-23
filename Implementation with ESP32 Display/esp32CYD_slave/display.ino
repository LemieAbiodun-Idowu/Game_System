#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

TFT_eSPI tft = TFT_eSPI();

int centerX = SCREEN_HEIGHT / 2;
int centerY = SCREEN_WIDTH / 2;

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

bool bckground = 1;  //0 for white, 1 for black
#define DEFAULT_MSG "PLEASE SCAN A VALID CARD"
#define DEFAULT_FONT 2
#define DEFAULT_MSG_WIDTH 30   //untested
#define DEFAULT_MSG_HEIGHT 30  //untested

#define NEG_CARD_SCAN_MSG "ACCESS DENIED LMAO"
#define POS_CARD_SCAN_MSG "ACCESS GRANTED TO CARD "
extern HardwareSerial mySerial;



void touchscreensetup() {
  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(2);
}

void displaysetup() {
  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(2);
  tft.setSwapBytes(true);  //Mistakenly assumed in the RGB565 converter that display wanted in little endian, so need to make this swap, this can be optimised in later versions
  // // LittleFS.begin();                       //This is for flash

  // SPIClass spi = SPIClass(VSPI);             //This is for SD handling, unsure if we'll need it

  // if (!SD.begin(SS, spi, 80000000)) {
  //   Serial.println("Card Mount Failed");
  //   return;
  // }

  // Clear the screen before writing to it
}

// void displayDefMsg() {
//   if (!bckground) {
//     tft.fillScreen(TFT_WHITE);
//     tft.setTextColor(TFT_BLACK, TFT_WHITE);
//   } else {
//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   }

//   tft.drawCentreString(DEFAULT_MSG, centerX, centerY, DEFAULT_FONT);
// }

// void cardMsgDisplay(bool signal, char* cardNumber = NULL) {      //Rename from signal, idk what fits better
//   tft.fillRect(centerX, centerY, DEFAULT_MSG_WIDTH, DEFAULT_MSG_HEIGHT, TFT_BLACK);
//   if (!signal) {
//     tft.drawCentreString(NEG_CARD_SCAN_MSG, centerX, centerY, DEFAULT_FONT);
//     delay(2000);        //display neg message for 2s
//     while(!mySerial.available()){
//       displayDefMsg();  //then go back to default
//     }
//     cardScanMsg();
//   } else {
//     char full_pos_msg [strlen(POS_CARD_SCAN_MSG) + 2];    //For NULL AND THE NUMBER SENT
//     snprintf(full_pos_msg, sizeof(full_pos_msg), "%s%s", POS_CARD_SCAN_MSG, cardNumber);
//     tft.drawCentreString(full_pos_msg, centerX, centerY, DEFAULT_FONT);
//   }
// }


// --- ADD THIS TO THE BOTTOM OF display.ino ---

const int GRID_X = 10;
const int GRID_Y = 10;
const int BLOCK_WIDTH = 12; 
const int BLOCK_HEIGHT = 12;

const int SCORE_X = 150;
const int SCORE_Y = 10;
const int NEXT_X = 150;
const int NEXT_Y = 60;
const int HOLD_X = 150;
const int HOLD_Y = 130;

char prevGrid = {0}; // Buffer prevents screen flicker

void drawGameLayout() {
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(GRID_X - 2, GRID_Y - 2, (10 * BLOCK_WIDTH) + 4, (18 * BLOCK_HEIGHT) + 4, TFT_WHITE);

  tft.drawRect(SCORE_X, SCORE_Y, 80, 40, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("SCORE", SCORE_X + 5, SCORE_Y + 5, 1);

  tft.drawRect(NEXT_X, NEXT_Y, 80, 60, TFT_WHITE);
  tft.drawString("NEXT", NEXT_X + 5, NEXT_Y + 5, 1);

  tft.drawRect(HOLD_X, HOLD_Y, 80, 60, TFT_WHITE);
  tft.drawString("HOLD", HOLD_X + 5, HOLD_Y + 5, 1);
}

void updateTetrisGrid(String newGrid) {
  int charIndex = 0;
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 10; x++) {
      char currentState = newGrid.charAt(charIndex);
      
      if (currentState != prevGrid[charIndex]) {
        int xPos = GRID_X + (x * BLOCK_WIDTH);
        int yPos = GRID_Y + (y * BLOCK_HEIGHT);
        
        if (currentState == '1') {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_CYAN); 
        } else {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_BLACK); 
        }
        prevGrid[charIndex] = currentState; 
      }
      charIndex++;
    }
  }
}

void updateScore(String scoreTxt) {
  tft.fillRect(SCORE_X + 5, SCORE_Y + 20, 70, 15, TFT_BLACK); 
  tft.drawString(scoreTxt, SCORE_X + 5, SCORE_Y + 20, 2);
}

// NOTE: Ensure your copyPiece function and tetris arrays are also inside the ESP32 files!
void drawNextPiece(int nextType) {
  tft.fillRect(NEXT_X + 2, NEXT_Y + 15, 76, 43, TFT_BLACK); 
  int8_t nPiece[2][4];
  copyPiece(nPiece, nextType, 0); 
  for (int i = 0; i < 4; i++) {
    tft.fillRect(NEXT_X + 20 + (nPiece[i] * 10), NEXT_Y + 25 + (nPiece[1][i] * 10), 9, 9, TFT_WHITE);
  }
}

void drawHoldPiece(int holdType) {
  tft.fillRect(HOLD_X + 2, HOLD_Y + 15, 76, 43, TFT_BLACK); 
  if (holdType == -1) return; 
  int8_t hPiece[2][4];
  copyPiece(hPiece, holdType, 0); 
  for (int i = 0; i < 4; i++) {
    tft.fillRect(HOLD_X + 20 + (hPiece[i] * 10), HOLD_Y + 25 + (hPiece[1][i] * 10), 9, 9, TFT_WHITE);
  }
}

void showGameOver() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.drawCentreString("GAME OVER", centerX, centerY, 4);
  memset(prevGrid, 0, sizeof(prevGrid));
}

void showPause() {
  tft.setTextColor(TFT_YELLOW);
  tft.drawCentreString("PAUSED", centerX, centerY, 4);
}

void resumeGame() {
  drawGameLayout();
  memset(prevGrid, 0, sizeof(prevGrid));
}


