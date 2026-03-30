extern byte grid[10][25];
extern short pieceX, pieceY;
extern short piece[2][4];
extern unsigned long score;
extern word nextType;
extern short holdType;


void UARTsetup() {
  // Starts hardware serial on Pins 0 (RX) and 1 (TX)
  Serial.begin(38400);     
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
  byte tempBoard[10][25];
  
  // 1. Copy static board
  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 25; y++) {
      tempBoard[x][y] = grid[x][y];
    }
  }

  // 2. Add the Ghost Piece (Using '7')
  short ghostY = getGhostY();
  for (int i = 0; i < 4; i++) {
    int px = pieceX + piece[0][i];
    int py = ghostY + piece[1][i];
    if (px >= 0 && px < 10 && py >= 0 && py < 25) {
      if (tempBoard[px][py] == 0) { // Only draw ghost on empty space
        tempBoard[px][py] = 7;      // '7' tells the ESP32 to draw a ghost block
      }
    }
  }

  // 3. Add active falling piece (OVERWRITES GHOST IF OVERLAPPING)
  for (int i = 0; i < 4; i++) {
    int px = pieceX + piece[0][i];
    int py = pieceY + piece[1][i];
    if (px >= 0 && px < 10 && py >= 0 && py < 25) {
      tempBoard[px][py] = currentType + 1; // Uses colors 1-6
    }
  }

  // 4. Send Grid to ESP32 with PACING
  Serial.print("G:");
  int charCount = 0;
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {
      Serial.print(tempBoard[x][y]);
      charCount++;
      if (charCount % 25 == 0) delay(1); // Crucial for preventing buffer overflow
    }
  }
  Serial.println();
  
  // 5. Send HUD data
  Serial.print("S:"); Serial.println(score);
  Serial.print("N:"); Serial.println(nextType);
  Serial.print("H:"); Serial.println(holdType);
  Serial.print("LV:"); Serial.println(level);
  Serial.print("TH:"); Serial.println(currentTheme);
}

void sendInformation(){
  Serial.print("CT:"); Serial.println(currentType); //sends information every loop
  Serial.print("CR:"); Serial.println(rotation);
  Serial.print("CX:"); Serial.println(pieceX);
  Serial.print("GY:"); Serial.println(getGhostY());
}