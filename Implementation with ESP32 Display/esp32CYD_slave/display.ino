#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

TFT_eSPI tft = TFT_eSPI();

int centerX = SCREEN_HEIGHT / 2;
int centerY =  SCREEN_WIDTH / 2;

#define XPT2046_IRQ 36   
#define XPT2046_MOSI 32  
#define XPT2046_MISO 39  
#define XPT2046_CLK 25   
#define XPT2046_CS 33    

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

bool backgroundBlack = true;

#define DEFAULT_MSG "PLEASE SCAN A VALID CARD"
#define DEFAULT_FONT 2
#define NEG_CARD_SCAN_MSG "ACCESS DENIED LMAO"
#define POS_CARD_SCAN_MSG "ACCESS GRANTED TO CARD "

extern HardwareSerial mySerial;

int currentTheme = 0; 

// --- GAME OVER & PAUSE TRACKERS ---
bool isGameOverState = false;
bool isPausedState = false; // NEW SHIELD TRACKER
String currentScoreStr = "0";
int currentLevelDisp = 1;

void touchscreensetup() {
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(2);
}

void displaysetup() {
  tft.init();
  tft.setRotation(2);
  tft.setSwapBytes(true);
}

void displayDefMsg() {
  if (!backgroundBlack) {
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  tft.drawCentreString(DEFAULT_MSG, centerX, centerY, DEFAULT_FONT);
}

void cardMsgDisplay(bool signal, char* cardNumber = NULL) {      
  tft.fillRect(0, centerY - 20, SCREEN_WIDTH, 40, TFT_BLACK);

  if (!signal) {
    tft.drawCentreString(NEG_CARD_SCAN_MSG, centerX, centerY, DEFAULT_FONT);
    delay(2000);
    while (!mySerial.available()) { delay(50); }
    displayDefMsg();
  } else {
    char full_pos_msg[64];
    snprintf(full_pos_msg, sizeof(full_pos_msg), "%s%s", POS_CARD_SCAN_MSG, cardNumber);
    tft.drawCentreString(full_pos_msg, centerX, centerY, DEFAULT_FONT);
    delay(3000);
    tft.fillScreen(TFT_BLACK);
    drawGameLayout();
  }
}

const int GRID_X = 10;
const int GRID_Y = 10;
const int BLOCK_WIDTH = 12; 
const int BLOCK_HEIGHT = 12;

const int SCORE_X = 150;
const int SCORE_Y = 10;
const int USER_X  = 150;
const int USER_Y  = 104;
const int NEXT_X  = 150;
const int NEXT_Y  = 171;
const int HOLD_X  = 150;
const int HOLD_Y  = 250;

char prevGrid[250] = {0};

const char esp_pieces_S_l[2][2][4] = { { {0,0,1,1}, {0,1,1,2} }, { {0,1,1,2}, {1,1,0,0} } };
const char esp_pieces_S_r[2][2][4] = { { {1,1,0,0}, {0,1,1,2} }, { {0,1,1,2}, {0,0,1,1} } };
const char esp_pieces_L_l[4][2][4] = { { {0,0,0,1}, {0,1,2,2} }, { {0,1,2,2}, {1,1,1,0} }, { {0,1,1,1}, {0,0,1,2} }, { {0,0,1,2}, {1,0,0,0} } };
const char esp_pieces_Sq[1][2][4]  = { { {0,1,0,1}, {0,0,1,1} } };
const char esp_pieces_T[4][2][4]   = { { {0,0,1,0}, {0,1,1,2} }, { {0,1,1,2}, {1,0,1,1} }, { {1,0,1,1}, {0,1,1,2} }, { {0,1,1,2}, {0,0,1,0} } };
const char esp_pieces_l[2][2][4]   = { { {0,1,2,3}, {0,0,0,0} }, { {0,0,0,0}, {0,1,2,3} } };

void copyEspPiece(int8_t p[2][4], int type, int rotation) {
  const char (*src)[2][4];
  switch (type) {
    case 0: src = esp_pieces_L_l; break;
    case 1: src = esp_pieces_S_l; break;
    case 2: src = esp_pieces_S_r; break;
    case 3: src = esp_pieces_Sq;  break;
    case 4: src = esp_pieces_T;   break;
    case 5: src = esp_pieces_l;   break;
    default: return;
  }
  for (int i = 0; i < 4; i++) {
    p[0][i] = src[rotation][0][i];
    p[1][i] = src[rotation][1][i];
  }
}

void drawGameLayout() {
  tft.fillScreen(TFT_BLACK);

  uint16_t borderColor = TFT_WHITE;
  if (currentTheme == 1) borderColor = TFT_MAGENTA; 
  if (currentTheme == 2) borderColor = TFT_GREEN;   

  tft.drawRect(GRID_X - 2, GRID_Y - 2, (10 * BLOCK_WIDTH) + 4, (25 * BLOCK_HEIGHT) + 4, borderColor);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.drawRect(SCORE_X, SCORE_Y, 80, 75, borderColor);
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, borderColor);
  tft.drawRect(SCORE_X, SCORE_Y, 58, 38, borderColor);
  tft.drawString("SCORE", SCORE_X + 5, SCORE_Y + 5, 1);
  tft.drawString("LV", SCORE_X + 64, SCORE_Y + 5, 1);
  tft.drawString("POWER UP", SCORE_X + 5, SCORE_Y + 45, 1);

  tft.drawRect(USER_X, USER_Y, 50, 50, borderColor);
  tft.drawString("USER", USER_X + 5, USER_Y - 10, 1);

  tft.drawRect(NEXT_X, NEXT_Y, 80, 60, borderColor);
  tft.drawString("NEXT", NEXT_X + 5, NEXT_Y - 10, 1);

  tft.drawRect(HOLD_X, HOLD_Y, 80, 60, borderColor);
  tft.drawString("HOLD", HOLD_X + 5, HOLD_Y - 10, 1);
}

void updateTetrisGrid(String newGrid) {
  if (isGameOverState || isPausedState) return; // DOUBLE SHIELD!
  if (newGrid.length() != 250) return;

  int charIndex = 0;
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {
      char currentState = newGrid.charAt(charIndex);

      if (currentState != prevGrid[charIndex]) {
        int xPos = GRID_X + (x * BLOCK_WIDTH);
        int yPos = GRID_Y + (y * BLOCK_HEIGHT);

        uint16_t blockColor;
        bool isGhost = false;

        switch (currentState) {
          case '1': blockColor = TFT_ORANGE;  break; 
          case '2': blockColor = TFT_GREEN;   break; 
          case '3': blockColor = TFT_RED;     break; 
          case '4': blockColor = TFT_YELLOW;  break; 
          case '5': blockColor = TFT_MAGENTA; break; 
          case '6': blockColor = TFT_CYAN;    break; 
          case '7': isGhost = true;           break; 
          default:  blockColor = TFT_BLACK;   break; 
        }

        if (currentTheme == 2 && blockColor != TFT_BLACK && !isGhost) {
          blockColor = TFT_GREEN;
        }

        if (isGhost) {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_BLACK);
          tft.drawRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_DARKGREY);
        } else if (blockColor != TFT_BLACK) {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, blockColor);

          if (currentTheme == 1) {
            tft.drawRect(xPos + 2, yPos + 2, BLOCK_WIDTH - 5, BLOCK_HEIGHT - 5, TFT_WHITE);
          } 
          else if (currentTheme == 2) {
            tft.fillRect(xPos + 2, yPos + 2, BLOCK_WIDTH - 5, BLOCK_HEIGHT - 5, TFT_BLACK);
            tft.fillRect(xPos + 4, yPos + 4, BLOCK_WIDTH - 9, BLOCK_HEIGHT - 9, TFT_GREEN);
          }
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
  if (isGameOverState || isPausedState) return; // DOUBLE SHIELD!

  currentScoreStr = scoreTxt; 

  uint16_t borderColor = TFT_WHITE;
  if (currentTheme == 1) borderColor = TFT_MAGENTA;
  if (currentTheme == 2) borderColor = TFT_GREEN;

  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, borderColor);
  tft.drawRect(SCORE_X, SCORE_Y, 58, 38, borderColor);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(scoreTxt, SCORE_X + 5, SCORE_Y + 18, 1);
}

void updateLevel(int lvl) {
  if (isGameOverState || isPausedState) return; // DOUBLE SHIELD!

  currentLevelDisp = lvl; 

  uint16_t borderColor = TFT_WHITE;
  if (currentTheme == 1) borderColor = TFT_MAGENTA;
  if (currentTheme == 2) borderColor = TFT_GREEN;

  tft.fillRect(SCORE_X + 64, SCORE_Y + 15, 50, 15, TFT_BLACK);
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, borderColor);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(String(lvl), SCORE_X + 64, SCORE_Y + 18, 1);
}

void drawNextPiece(int nextType) {
  if (isGameOverState || isPausedState) return; // DOUBLE SHIELD!

  tft.fillRect(NEXT_X + 2, NEXT_Y + 15, 76, 43, TFT_BLACK);
  int8_t nPiece[2][4];
  copyEspPiece(nPiece, nextType, 0);

  uint16_t pieceColor = TFT_WHITE;
  if (currentTheme == 2) pieceColor = TFT_GREEN;

  for (int i = 0; i < 4; i++) {
    int px = NEXT_X + 20 + (nPiece[0][i] * 10);
    int py = NEXT_Y + 20 + (nPiece[1][i] * 10);
    tft.fillRect(px, py, 9, 9, pieceColor);

    if (currentTheme == 1) tft.drawRect(px + 2, py + 2, 5, 5, TFT_WHITE);
    else if (currentTheme == 2) {
      tft.fillRect(px + 2, py + 2, 5, 5, TFT_BLACK);
      tft.fillRect(px + 3, py + 3, 3, 3, TFT_GREEN);
    }
  }
}

void drawHoldPiece(int holdType) {
  if (isGameOverState || isPausedState) return; // DOUBLE SHIELD!
  
  tft.fillRect(HOLD_X + 2, HOLD_Y + 15, 76, 43, TFT_BLACK);
  if (holdType == -1) return;

  int8_t hPiece[2][4];
  copyEspPiece(hPiece, holdType, 0);

  uint16_t pieceColor = TFT_WHITE;
  if (currentTheme == 2) pieceColor = TFT_GREEN;

  for (int i = 0; i < 4; i++) {
    int px = HOLD_X + 20 + (hPiece[0][i] * 10);
    int py = HOLD_Y + 20 + (hPiece[1][i] * 10);
    tft.fillRect(px, py, 9, 9, pieceColor);

    if (currentTheme == 1) tft.drawRect(px + 2, py + 2, 5, 5, TFT_WHITE);
    else if (currentTheme == 2) {
      tft.fillRect(px + 2, py + 2, 5, 5, TFT_BLACK);
      tft.fillRect(px + 3, py + 3, 3, 3, TFT_GREEN);
    }
  }
}

void showGameOver() {
  isGameOverState = true; 

  int boxW = 180;
  int boxH = 110;
  int boxX = centerX - (boxW / 2);
  int boxY = centerY - (boxH / 2);

  tft.fillRect(boxX + 6, boxY + 6, boxW, boxH, tft.color565(30, 30, 30));

  tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
  tft.drawRect(boxX, boxY, boxW, boxH, TFT_RED);
  tft.drawRect(boxX + 1, boxY + 1, boxW - 2, boxH - 2, TFT_RED);
  tft.drawRect(boxX + 2, boxY + 2, boxW - 4, boxH - 4, TFT_RED);

  tft.setTextColor(tft.color565(100, 0, 0)); 
  tft.drawCentreString("GAME OVER", centerX + 2, boxY + 17, 4);
  
  tft.setTextColor(TFT_RED); 
  tft.drawCentreString("GAME OVER", centerX, boxY + 15, 4);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("FINAL SCORE", centerX, boxY + 50, 1);
  tft.setTextColor(TFT_YELLOW);
  tft.drawCentreString(currentScoreStr, centerX, boxY + 65, 2);
  
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("LEVEL " + String(currentLevelDisp), centerX, boxY + 85, 1);

  memset(prevGrid, 0, sizeof(prevGrid)); 
}

void showPause() {
  isPausedState = true; // Raise the Pause shield!

  int boxW = 160;
  int boxH = 70;
  int boxX = centerX - (boxW / 2);
  int boxY = centerY - (boxH / 2);

  // 1. Draw a dark shadow
  tft.fillRect(boxX + 6, boxY + 6, boxW, boxH, tft.color565(30, 30, 30));

  // 2. Draw the main box with a thick Yellow Border
  tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
  tft.drawRect(boxX, boxY, boxW, boxH, TFT_YELLOW);
  tft.drawRect(boxX + 1, boxY + 1, boxW - 2, boxH - 2, TFT_YELLOW);
  tft.drawRect(boxX + 2, boxY + 2, boxW - 4, boxH - 4, TFT_YELLOW);

  // 3. Draw "PAUSED" Drop Shadow (Dark Yellow)
  tft.setTextColor(tft.color565(100, 100, 0)); 
  tft.drawCentreString("PAUSED", centerX + 2, boxY + 17, 4);
  
  // 4. Draw "PAUSED" Main Text (Bright Yellow)
  tft.setTextColor(TFT_YELLOW); 
  tft.drawCentreString("PAUSED", centerX, boxY + 15, 4);

  // 5. Instruction text
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("- PRESS TO RESUME -", centerX, boxY + 50, 1);
}

void resumeGame() {
  isGameOverState = false; 
  isPausedState = false; // Lower ALL shields!
  
  drawGameLayout();
  memset(prevGrid, 0, sizeof(prevGrid));
}