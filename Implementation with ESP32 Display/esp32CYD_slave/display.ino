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

void displayDefMsg() {
  if (!bckground) {
    tft.fillScreen(TFT_WHITE);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
  } else {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
  }

  tft.drawCentreString(DEFAULT_MSG, centerX, centerY, DEFAULT_FONT);
}

void cardMsgDisplay(bool signal, char* cardNumber = NULL) {      //Rename from signal, idk what fits better
  tft.fillRect(centerX, centerY, DEFAULT_MSG_WIDTH, DEFAULT_MSG_HEIGHT, TFT_BLACK);
  if (!signal) {
    tft.drawCentreString(NEG_CARD_SCAN_MSG, centerX, centerY, DEFAULT_FONT);
    delay(2000);        //display neg message for 2s
    while(!mySerial.available()){
      displayDefMsg();  //then go back to default
    }
    cardScanMsg();
  } else {
    char full_pos_msg [strlen(POS_CARD_SCAN_MSG) + 2];    //For NULL AND THE NUMBER SENT
    snprintf(full_pos_msg, sizeof(full_pos_msg), "%s%s", POS_CARD_SCAN_MSG, cardNumber);
    tft.drawCentreString(full_pos_msg, centerX, centerY, DEFAULT_FONT);
  }
}



// const short MARGIN_TOP = 19;
// const short MARGIN_LEFT = 3;
// const short SIZE = 5;


// void drawFrame() {

//   display.clearDisplay();
//   drawLayout();
//   display.display();
// }

// void drawGrid() {
//   for (short x = 0; x < 10; x++)
//     for (short y = 0; y < 18; y++)
//       if (grid[x][y])
//         display.fillRect(MARGIN_LEFT + (SIZE + 1) * x, MARGIN_TOP + (SIZE + 1) * y, SIZE, SIZE, WHITE);
// }

// void drawPiece(short type, short x, short y) {
//   for (short i = 0; i < 4; i++)
//     display.fillRect(MARGIN_LEFT + (SIZE + 1) * (x + piece[0][i]), MARGIN_TOP + (SIZE + 1) * (y + piece[1][i]), SIZE, SIZE, WHITE);
// }
// void drawNextPiece() {
//   short nPiece[2][4];
//   copyPiece(nPiece, nextType, 0);
//   for (short i = 0; i < 4; i++)
//     display.fillRect(50 + 3 * nPiece[0][i], 4 + 3 * nPiece[1][i], 2, 2, WHITE);
// }

// void drawLayout() {
//   // display.drawLine(0, 64, SCREEN_WIDTH, 32, WHITE);
//   display.drawRect(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH, WHITE);
//   drawNextPiece();
//   char text[6];
//   itoa(score, text, 10);
//   drawTextLayout(text, 7, 4);
//   display.drawLine(0, 14, SCREEN_WIDTH, 14, WHITE);
// }
// // short getNumberLength(int n){
// //   short counter = 1;
// //   while(n >= 10){
// //     n /= 10;
// //     counter++;
// //   }
// //   return counter;
// // }

// void drawTextLayout(const char* text, int x, int y) {

//   display.setTextSize(1);               // Normal 1:1 pixel scale
//   display.setTextColor(SSD1306_WHITE);  // Draw white text
//   display.setCursor(x, y);              // Start at top-left corner
//   display.cp437(true);                  // Use full 256 char 'Code Page 437' font
//   display.print(text);
// }

// void drawText(const __FlashStringHelper* text, int x, int y) {
//   display.setTextSize(1);
//   display.setTextColor(SSD1306_WHITE);
//   display.setCursor(x, y);
//   display.cp437(true);
//   display.print(text);  // print() handles F() strings natively
// }

// void displayGameOver() {

//   display.clearDisplay();
//   display.setTextSize(2);
//   display.setTextColor(WHITE);
//   display.setCursor(10, 25);
//   display.print("GAME OVER");
//   display.setTextSize(1);
//   display.setCursor(0, 95);
//   display.print("CLICK TO");
//   display.setCursor(0, 105);
//   display.print("RESTART");
//   display.display();
//   isGameOver = true;
//   delay(50);
// }

// void refreshGrid() {
//   display.clearDisplay();
//   drawLayout();
//   drawGrid();
//   drawPiece(currentType, pieceX, pieceY);
//   display.display();
// }
