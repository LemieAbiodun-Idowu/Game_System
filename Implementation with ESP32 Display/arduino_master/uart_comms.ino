// void UARTsetup(){
//   Serial.begin(115200); //we are using the default tx and rx pins for arduino UART
// }

// void failedCardMsg(){
//   Serial.println(F("INVALID_CRD"));
// }

// void passedCardMsg(int CardNumber){
//   char msg[10];
//   snprintf(msg, sizeof(msg), "CRD:%d", CardNumber);
//   Serial.println(msg);
// }

extern byte grid[10][18];
extern short pieceX, pieceY;
extern short piece[2][4];
extern int score;
extern word nextType;
extern short holdType;

void UARTsetup() {
  Serial.begin(115200); 
}

void failedCardMsg() {
  Serial.println(F("INVALID_CRD"));
}

void passedCardMsg(int CardNumber) {
  char msg[10];
  snprintf(msg, sizeof(msg), "CRD:%d", CardNumber);
  Serial.println(msg);
}

// --- NEW GAME COMMUNICATION ---

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

  // 3. Send Grid
  Serial.print("G:");
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 10; x++) {
      Serial.print(tempBoard[x][y]);
    }
  }
  Serial.println();
  
  // 4. Send HUD data
  Serial.print("S:"); Serial.println(score);
  Serial.print("N:"); Serial.println(nextType);
  Serial.print("H:"); Serial.println(holdType);
}