// #include <TFT_eSPI.h>

// 1. Memory Optimization: The Grid
byte grid[10][18];

// 2. Memory Optimization: Small types for game state
uint16_t interval = 500;
uint16_t score = 0;
bool isGameOver = false;
bool isPaused = false; 

long timer, delayer;
const int8_t TYPES = 6;  // Matches the 6 pieces in your switch statement

// 3. Memory Optimization: 1-byte variables for pieces
uint8_t currentType, nextType, rotation;
int8_t pieceX, pieceY;
int8_t piece[2][4];
int8_t holdType = -1;
bool b3 = true; 

// // Point to the screen object created in display.ino
// extern TFT_eSPI display;

// References to controls and pieces defined in other files
extern int x_point, y_point;
extern int deadZone;
extern int B_1, B_2; // Ensure these match your actual pin variable types

extern const char pieces_S_l;
extern const char pieces_S_r;
extern const char pieces_L_l;
extern const char pieces_Sq;
extern const char pieces_T;
extern const char pieces_l;

// --- External Functions ---
extern void refreshGrid();
extern void displayGameOver();

void initialiseGame() {
  randomSeed(analogRead(0) + analogRead(A0)); //Why would analog 0 break it?
  nextType = random(TYPES);
  generate();
  timer = millis();
}

void checkGameOver() {
  if (isGameOver) {
    if (!digitalRead(B_2) || !digitalRead(B_1)) {  
      delay(250); 
      resetGame();
    }
  }
}

void hardDrop() {
  if (!digitalRead() { 
    while (!nextCollision()) {
      pieceY++;
    }
    for (int8_t i = 0; i < 4; i++) {
      // FIXED: Used[i] and[i] instead of
      grid[pieceX + piece[i]][pieceY + piece[i]] = 1;
    }
    generate();
    if (spawnCollision()) {
      displayGameOver();
      return;
    }
    refreshGrid();
    delay(200); 
  }
}

void updatePieceGravity() {
  if(isPaused || isGameOver) return;
  if (millis() - timer > interval) {
    checkLines();
    refreshGrid();
    if (nextCollision()) {
      for (int8_t i = 0; i < 4; i++)
        grid[pieceX + piece[i]][pieceY + piece[i]] = 1;
      generate();
      if (spawnCollision()) {
        displayGameOver();
        return;
      }
    } else {
      pieceY++;
    }
    timer = millis();
  }
}

void deadzone() { 
  if(isPaused || isGameOver) return;
  if (x_point < 32 - deadZone) {
    if (!nextHorizontalCollision(piece, -1)) {
      pieceX--;
      refreshGrid();
    }
  } else if (x_point > 32 + deadZone) {
    if (!nextHorizontalCollision(piece, 1)) {
      pieceX++;
      refreshGrid();
    }
  }
}

short getMaxRotation(int8_t type) {
  if (type == 1 || type == 2 || type == 5) return 2;
  else if (type == 0 || type == 4) return 4;
  else if (type == 3) return 1;
  return 0;
}

bool canRotate(uint8_t rot) {
  int8_t tempPiece;
  copyPiece(tempPiece, currentType, rot);
  return !nextHorizontalCollision(tempPiece, 0);
}

void PieceRotation() {
  if (!digitalRead(B_1)) { 
    if (b3) {
      uint8_t nextRot = rotation;
      if (rotation == getMaxRotation(currentType) - 1) {
        nextRot = 0;
      } else {
        nextRot++;
      }

      if (canRotate(nextRot)) {
        rotation = nextRot;
        copyPiece(piece, currentType, rotation);
        refreshGrid();
      }
      b3 = false;
      delayer = millis();
    }
  } else if (millis() - delayer > 50) {
    b3 = true;
  }
}

void resetGame() {
  for (int8_t x = 0; x < 10; x++)
    for (int8_t y = 0; y < 18; y++)
      grid[x][y] = 0;

  score = 0;
  interval = 500;

  // display.fillScreen(TFT_BLACK); //This should send reset game or equivalent to esp 



  nextType = random(TYPES);
  generate();
  isGameOver = false;
}

void generate() {
  currentType = nextType;
  nextType = random(TYPES);
  pieceX = 3;
  pieceY = 0;
  rotation = 0;
  copyPiece(piece, currentType, rotation);
}

void checkLines() {
  for (int8_t y = 17; y >= 0; y--) {
    bool full = true;
    for (int8_t x = 0; x < 10; x++) {
      full = full && grid[x][y];
    }
    if (full) {
      breakLine(y);
      y++; 
    }
  }
}

void breakLine(int8_t line) {
  for (int8_t y = line; y > 0; y--) {
    for (int8_t x = 0; x < 10; x++) {
      grid[x][y] = grid[x][y - 1];
    }
  }
  for (int8_t x = 0; x < 10; x++) {
    grid[x] = 0;
  }
  score += 10;
}

bool nextHorizontalCollision(int8_t p, int8_t amount) {
  for (int8_t i = 0; i < 4; i++) {
    int8_t newX = pieceX + p[i] + amount;
    int8_t newY = pieceY + p[i];
    if (newX > 9 || newX < 0 || grid[newX][newY]) return true;
  }
  return false;
}

bool nextCollision() {
  for (int8_t i = 0; i < 4; i++) {
    int8_t y = pieceY + piece[i] + 1;
    int8_t x = pieceX + piece[i];
    if (y > 17 || grid[x][y]) return true;
  }
  return false;
}

bool spawnCollision() {
  for (int8_t i = 0; i < 4; i++) {
    int8_t x = pieceX + piece[i];
    int8_t y = pieceY + piece[i];
    // FIXED: Corrected y boundary check
    if (grid[x][y] || y >= 18) return true;
  }
  return false;
}

void copyPiece(int8_t p, uint8_t type, uint8_t rot) {
  switch (type) {
    case 0: // L_l
      for (int8_t i = 0; i < 4; i++) {
        p[i] = pgm_read_byte(&(pieces_L_l[rot][i]));
        p[i] = pgm_read_byte(&(pieces_L_l[rot][i]));
      }
      break;
    case 1: // S_l
      for (int8_t i = 0; i < 4; i++) {
        p[i] = pgm_read_byte(&(pieces_S_l[rot][i]));
        p[i] = pgm_read_byte(&(pieces_S_l[rot][i]));
      }
      break;
    case 2: // S_r
      for (int8_t i = 0; i < 4; i++) {
        p[i] = pgm_read_byte(&(pieces_S_r[rot][i]));
        p[i] = pgm_read_byte(&(pieces_S_r[rot][i]));
      }
      break;
    case 3: // Sq
      for (int8_t i = 0; i < 4; i++) {
        p[i] = pgm_read_byte(&(pieces_Sq[i]));
        p[i] = pgm_read_byte(&(pieces_Sq[i]));
      }
      break;
    case 4: // T
      for (int8_t i = 0; i < 4; i++) {
        p[i] = pgm_read_byte(&(pieces_T[rot][i]));
        p[i] = pgm_read_byte(&(pieces_T[rot][i]));
      }
      break;
    case 5: // I (Line)
      for (int8_t i = 0; i < 4; i++) {
        p[i] = pgm_read_byte(&(pieces_l[rot][i]));
        p[i] = pgm_read_byte(&(pieces_l[rot][i]));
      }
      break;
  }
}