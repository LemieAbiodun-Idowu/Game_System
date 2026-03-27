#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

TFT_eSPI tft = TFT_eSPI();

int centerX = SCREEN_HEIGHT / 2;
int centerY =  SCREEN_WIDTH/ 2;

// Touchscreen pins
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

// ==========================================
// SETUP
// ==========================================

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

// ==========================================
// DISPLAY MESSAGES
// ==========================================

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

    while (!mySerial.available()) {
      delay(50); // prevent freeze
    }

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

// ==========================================
// TETRIS DISPLAY
// ==========================================

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

// --- PIECES ---
const char esp_pieces_S_l[2][2][4] = {
  { {0,0,1,1}, {0,1,1,2} },
  { {0,1,1,2}, {1,1,0,0} }
};

const char esp_pieces_S_r[2][2][4] = {
  { {1,1,0,0}, {0,1,1,2} },
  { {0,1,1,2}, {0,0,1,1} }
};

const char esp_pieces_L_l[4][2][4] = {
  { {0,0,0,1}, {0,1,2,2} },
  { {0,1,2,2}, {1,1,1,0} },
  { {0,1,1,1}, {0,0,1,2} },
  { {0,0,1,2}, {1,0,0,0} }
};

const char esp_pieces_Sq[1][2][4] = {
  { {0,1,0,1}, {0,0,1,1} }
};

const char esp_pieces_T[4][2][4] = {
  { {0,0,1,0}, {0,1,1,2} },
  { {0,1,1,2}, {1,0,1,1} },
  { {1,0,1,1}, {0,1,1,2} },
  { {0,1,1,2}, {0,0,1,0} }
};

const char esp_pieces_l[2][2][4] = {
  { {0,1,2,3}, {0,0,0,0} },
  { {0,0,0,0}, {0,1,2,3} }
};

// ==========================================
// FIXED PIECE COPY
// ==========================================

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

// ==========================================


void drawGameLayout() {
  tft.fillScreen(TFT_BLACK);

  // Main tetris grid
  tft.drawRect(GRID_X - 2, GRID_Y - 2, (10 * BLOCK_WIDTH) + 4, (25 * BLOCK_HEIGHT) + 4, TFT_WHITE);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // SCORE box
  tft.drawRect(SCORE_X, SCORE_Y, 80, 75, TFT_WHITE);
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, TFT_WHITE);
  tft.drawRect(SCORE_X, SCORE_Y, 58, 38, TFT_WHITE);

  tft.drawString("SCORE", SCORE_X + 5, SCORE_Y + 5, 1);
  tft.drawString("LV", SCORE_X + 64, SCORE_Y + 5, 1);

  tft.drawString("POWER UP", SCORE_X + 5, SCORE_Y + 45, 1);

  // USER box
  tft.drawRect(USER_X, USER_Y, 50, 50, TFT_WHITE);
  tft.drawString("USER", USER_X + 5, USER_Y - 10, 1);

  // NEXT box
  tft.drawRect(NEXT_X, NEXT_Y, 80, 60, TFT_WHITE);
  tft.drawString("NEXT", NEXT_X + 5, NEXT_Y - 10, 1);

  // HOLD box
  tft.drawRect(HOLD_X, HOLD_Y, 80, 60, TFT_WHITE);
  tft.drawString("HOLD", HOLD_X + 5, HOLD_Y - 10, 1);

}
// ==========================================

void updateTetrisGrid(String newGrid) {
  if (newGrid.length() != 250) return;

  int charIndex = 0;

  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {

      char currentState = newGrid.charAt(charIndex);

      if (currentState != prevGrid[charIndex]) {

        int xPos = GRID_X + (x * BLOCK_WIDTH);
        int yPos = GRID_Y + (y * BLOCK_HEIGHT);

        if (currentState == '1') {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_CYAN);
        } 
        else if (currentState == '2') {
          tft.drawRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_DARKGREY);
        } 
        else {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_BLACK);
        }

        prevGrid[charIndex] = currentState;
      }

      charIndex++;
    }
  }
}

// ==========================================

void updateScore(String scoreTxt) {
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, TFT_WHITE);
  tft.drawRect(SCORE_X, SCORE_Y, 58, 38, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(scoreTxt, SCORE_X + 5, SCORE_Y + 18, 1);
}

void updateLevel(int lvl) {
  tft.fillRect(SCORE_X + 64, SCORE_Y + 15, 50, 15, TFT_BLACK);
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(String(lvl), SCORE_X + 64, SCORE_Y + 18, 1);
}

// ==========================================

void drawNextPiece(int nextType) {
  tft.fillRect(NEXT_X + 2, NEXT_Y + 15, 76, 43, TFT_BLACK);

  int8_t nPiece[2][4];
  copyEspPiece(nPiece, nextType, 0);

  for (int i = 0; i < 4; i++) {
    tft.fillRect(
      NEXT_X + 20 + (nPiece[0][i] * 10),
      NEXT_Y + 20 + (nPiece[1][i] * 10),
      9, 9, TFT_WHITE
    );
  }
}

// ==========================================

void drawHoldPiece(int holdType) {
  tft.fillRect(HOLD_X + 2, HOLD_Y + 15, 76, 43, TFT_BLACK);

  if (holdType == -1) return;

  int8_t hPiece[2][4];
  copyEspPiece(hPiece, holdType, 0);

  for (int i = 0; i < 4; i++) {
    tft.fillRect(
      HOLD_X + 20 + (hPiece[0][i] * 10),
      HOLD_Y + 20 + (hPiece[1][i] * 10),
      9, 9, TFT_WHITE
    );
  }
}

// ==========================================

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

void updateGhostPiece(int ghostY, int ghostX, int8_t gPiece[2][4]) {
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {
      int idx = y * 10 + x;
      if (prevGrid[idx] == '3') {
        tft.fillRect(GRID_X + (x * BLOCK_WIDTH), GRID_Y + (y * BLOCK_HEIGHT),
                     BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_BLACK);
        prevGrid[idx] = '0';
      }
    }
  }
  // Draw new ghost
  for (int i = 0; i < 4; i++) {
    int x = ghostX + gPiece[0][i];
    int y = ghostY + gPiece[1][i];
    if (x >= 0 && x < 10 && y >= 0 && y < 25) {
      int idx = y * 10 + x;
      if (prevGrid[idx] != '1') {  // only draw if no active piece here
        tft.drawRect(GRID_X + (x * BLOCK_WIDTH), GRID_Y + (y * BLOCK_HEIGHT),
                     BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_DARKGREY);
        prevGrid[idx] = '3';
      }
    }
  }
}
