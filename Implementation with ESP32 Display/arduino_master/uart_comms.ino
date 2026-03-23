extern byte grid[10][18];
extern short pieceX, pieceY;
extern short piece[2][4];
extern int score;
extern word nextType;
extern short holdType;

void UARTsetup() {
  // Starts hardware serial on Pins 0 (RX) and 1 (TX)
  Serial.begin(115200);     
  
  delay(100);
  Serial.println("INIT"); // Wakes up the ESP32 screen!
}

void failedCardMsg() {
  Serial.println(F("INVALID_CRD"));
}

void passedCardMsg(int CardNumber) {
  char msg[10];
  snprintf(msg, sizeof(msg), "CRD:%d", CardNumber);
  Serial.println(msg);
}

void refreshGrid() {
  byte tempBoard[10][18];
  
  // 1. Copy static board
  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 18; y++) {
      tempBoard[x][y] = grid[x][y];
    }
  }

  // 2. Add active falling piece
  for (int i = 0; i < 4; i++) {
    int px = pieceX + piece[i]; 
    int py = pieceY + piece[1][i];
    if (px >= 0 && px < 10 && py >= 0 && py < 18) {
      tempBoard[px][py] = 1;
    }
  }

  // 3. Send Grid to ESP32
  Serial.print("G:");
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 10; x++) {
      Serial.print(tempBoard[x][y]);
    }
  }
  Serial.println();
  
  // 4. Send HUD data to ESP32
  Serial.print("S:"); Serial.println(score);
  Serial.print("N:"); Serial.println(nextType);
  Serial.print("H:"); Serial.println(holdType);
}