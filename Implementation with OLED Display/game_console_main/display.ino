#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI 11  //D1
#define OLED_CLK 13   //D0
#define OLED_DC 19
#define OLED_CS 16
#define OLED_RESET 18
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RESET, OLED_CS);

const short MARGIN_TOP = 19;
const short MARGIN_LEFT = 3;
const short SIZE = 5;



void setupOLED() {
  pinMode(OLED_CS, OUTPUT);
  digitalWrite(OLED_CS, HIGH);
  pinMode(OLED_RESET, OUTPUT);
  digitalWrite(OLED_RESET, HIGH);
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  delay(50);
  display.setRotation(1);
  display.clearDisplay();
  drawText(F("PLEASE    SCAN A    VALID CARD"), 0, 42);
}

void drawFrame() {

  display.clearDisplay();
  drawLayout();
  display.display();

}

void drawGrid() {
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 18; y++)
      if (grid[x][y])
        display.fillRect(MARGIN_LEFT + (SIZE + 1) * x, MARGIN_TOP + (SIZE + 1) * y, SIZE, SIZE, WHITE);
}

void drawPiece(short type, short x, short y) {
  for (short i = 0; i < 4; i++)
    display.fillRect(MARGIN_LEFT + (SIZE + 1) * (x + piece[0][i]), MARGIN_TOP + (SIZE + 1) * (y + piece[1][i]), SIZE, SIZE, WHITE);
}
void drawNextPiece() {
  short nPiece[2][4];
  copyPiece(nPiece, nextType, 0);
  for (short i = 0; i < 4; i++)
    display.fillRect(50 + 3 * nPiece[0][i], 4 + 3 * nPiece[1][i], 2, 2, WHITE);
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

void displayGameOver() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 25);
  display.print("GAME OVER");
  display.setTextSize(1);
  display.setCursor(0, 95);
  display.print("CLICK TO");
  display.setCursor(0, 105);
  display.print("RESTART");
  display.display();
  isGameOver = true;
  delay(50);
}

void refreshGrid() {
  display.clearDisplay();
  drawLayout();
  drawGrid();
  drawPiece(currentType, pieceX, pieceY);
  display.display();
}
