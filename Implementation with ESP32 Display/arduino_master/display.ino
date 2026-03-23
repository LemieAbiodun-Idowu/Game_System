#include <TFT_eSPI.h> 
#include <SPI.h>

// Initialize the TFT screen
TFT_eSPI display = TFT_eSPI(); 

// Display Settings
const short SIZE = 12;          
const short MARGIN_LEFT = 80;   
const short MARGIN_TOP = 10;
const uint16_t WHITE = TFT_WHITE; 
const uint16_t BLACK = TFT_BLACK;

// --- FIXED EXTERN SECTION ---
// These MUST match the types and sizes in TetrisLogic.ino exactly
extern int8_t pieceX, pieceY;
extern int8_t piece;    // Fixed: Added dimensions
extern uint8_t currentType;
extern int8_t holdType;
extern byte grid;     // Fixed: Added dimensions

// Function from TetrisLogic
extern short getGhostY();

void setupOLED() {
  display.init();
  display.setRotation(1); // Landscape
  display.fillScreen(TFT_BLACK);
  
  // Initial message
  display.setTextColor(TFT_WHITE);
  display.setTextSize(2);
  display.setCursor(20, 100);
  display.print("PLEASE SCAN A VALID CARD");
}

void drawText(String text, int x, int y) {
  display.setCursor(x, y);
  display.print(text);
}

void drawFrame() {
  display.fillScreen(TFT_BLACK);
  // Draw the boundary of the well
  display.drawRect(MARGIN_LEFT - 2, MARGIN_TOP - 2, (SIZE + 1) * 10 + 4, (SIZE + 1) * 18 + 4, TFT_WHITE);
}

void refreshGrid() {
  drawFrame();
  
  // 1. Draw the settled blocks in the grid
  // Using int8_t for loop counters saves a tiny bit more RAM
  for (int8_t i = 0; i < 10; i++) {
    for (int8_t j = 0; j < 18; j++) {
      if (grid[i][j]) {
        display.fillRect(MARGIN_LEFT + (SIZE + 1) * i, MARGIN_TOP + (SIZE + 1) * j, SIZE, SIZE, TFT_WHITE);
      }
    }
  }

  // 2. Draw Ghost Piece (Fixed for 2D array)
  short ghostY = getGhostY();
  for (int8_t i = 0; i < 4; i++) {
    display.drawRect(MARGIN_LEFT + (SIZE + 1) * (pieceX + piece[i]), 
                     MARGIN_TOP + (SIZE + 1) * (ghostY + piece[i]), 
                     SIZE, SIZE, TFT_WHITE);
  }

  // 3. Draw Active Piece (Fixed for 2D array)
  for (int8_t i = 0; i < 4; i++) {
    display.fillRect(MARGIN_LEFT + (SIZE + 1) * (pieceX + piece[i]), 
                     MARGIN_TOP + (SIZE + 1) * (pieceY + piece[i]), 
                     SIZE, SIZE, TFT_WHITE);
  }
}

void displayGameOver() {
  display.fillScreen(TFT_BLACK);
  display.setTextColor(TFT_RED);
  display.setTextSize(3);
  display.setCursor(50, 100);
  display.print("GAME OVER");
}