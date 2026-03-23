#include <TFT_eSPI.h> 
#include <SPI.h>

// Initialize the TFT screen
TFT_eSPI display = TFT_eSPI(); 

// Adjusting sizes for the 320x240 screen
const short SIZE = 12;          // Increased from 5 to 12 so it's not tiny
const short MARGIN_LEFT = 80;   // Centering the grid on the wider screen
const short MARGIN_TOP = 10;
const uint16_t WHITE = TFT_WHITE; // Map old color name to new library
const uint16_t BLACK = TFT_BLACK;

extern short holdType;
extern short getGhostY();
extern short piece;
extern word currentType;
extern short pieceX, pieceY;
extern byte grid;

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
  // Simple wrapper to match your old code's style
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
  
  // Draw the settled blocks in the grid
  for (short i = 0; i < 10; i++) {
    for (short j = 0; j < 18; j++) {
      if (grid[i][j]) {
        display.fillRect(MARGIN_LEFT + (SIZE + 1) * i, MARGIN_TOP + (SIZE + 1) * j, SIZE, SIZE, TFT_WHITE);
      }
    }
  }

  // Draw Ghost Piece
  short ghostY = getGhostY();
  for (short i = 0; i < 4; i++) {
    display.drawRect(MARGIN_LEFT + (SIZE + 1) * (pieceX + piece[i]), 
                     MARGIN_TOP + (SIZE + 1) * (ghostY + piece[i]), 
                     SIZE, SIZE, TFT_WHITE);
  }

  // Draw Active Piece
  for (short i = 0; i < 4; i++) {
    display.fillRect(MARGIN_LEFT + (SIZE + 1) * (pieceX + piece[i]), 
                     MARGIN_TOP + (SIZE + 1) * (pieceY + piece[i]), 
                     SIZE, SIZE, TFT_WHITE);
  }
  
  drawHoldPiece();
  // Note: No display.display() needed for TFT_eSPI!
}

void drawHoldPiece() {
  const int HOLD_X = 10; 
  const int HOLD_Y = 10;
  display.drawRect(HOLD_X, HOLD_Y, 50, 50, TFT_WHITE);
  display.setCursor(HOLD_X + 5, HOLD_Y + 5);
  display.setTextSize(1);
  display.print("HOLD");
}