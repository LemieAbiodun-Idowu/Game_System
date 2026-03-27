byte grid[10][25];
int interval = 500;
unsigned long score = 0;
bool isGameOver = false;
long timer, delayer;
const short TYPES = 6;  //type of pieces used
word currentType, nextType, rotation;
short pieceX, pieceY;
short piece[2][4];
// extern Adafruit_SSD1306 display;
//extern int x_point, y_point;
//extern int deadZone;
unsigned long lastSoftDropTime = 0;
const int softDropSpeed = 400; // ms between moves
short holdType = -1; // minus means hold slot is empty
bool canHold = true; // Prevents holding more than once per drop
bool holdButtonReady = true; // track if the button can trigger a hold
unsigned long lastMoveTime = 0;
const int moveDelay = 120; // ms between moves
unsigned long holdStartTime = 0;
bool isHoldingButton = false;
int level = 1;
int comboCount = -1; // -1 means no active combo

// 3 types of powerups - doublePoints, clearLine, slowGravity
// doublePoints doubles points gathered from breakLine for 10s - card 2
// clear line clears bottom three lines - card 3
// slowGravity slows the gravity for 10s - card 4
bool doublePointsActive = false;
unsigned long doublePointsStart = 0;
bool slowGravityActive = false;
unsigned long slowGravityStart = 0;


extern const char pieces_S_l[2][2][4];;
extern const char pieces_S_r[2][2][4];
extern const char pieces_L_l[4][2][4];
extern const char pieces_Sq[1][2][4];
extern const char pieces_T[4][2][4];
extern const char pieces_l[2][2][4];

// --- SERIAL COMMUNICATION TO ESP32 ---

void displayGameOver() {
  Serial.println("STATE:OVER");
  isGameOver = true;
}


void initialiseGame() {
  randomSeed(analogRead(0));  //Need to change this as 1. analogRead(0) can return 0 1/1024 which causes a failure and 2. We will be using pin 0 in future
                              //I might recommend using micros() with some logic i'll add into the display later.
  nextType = random(TYPES);
  generate();
  timer = millis();

  Serial.println("STATE:PLAY");
  refreshGrid();
}

void checkGameOver() {
  if (isGameOver) {
    if (!digitalRead(B_2) || !digitalRead(B_1)) {  
      delay(250);             // simple debounce
      resetGame();
    }
  }
}

void hardDrop() {
  if (!digitalRead(UP)) {  // button pressed (INPUT_PULLUP)
    int cellsDropped = 0;
    // Move piece down until collision
    while (!nextCollision()) {
      pieceY++;
      cellsDropped++;
    }
    score += 2 * cellsDropped; // 2 points per cell

    // Lock piece into grid
    for (short i = 0; i < 4; i++) {
      grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
    }

    generate();

    if (spawnCollision()) {
      displayGameOver();
      return;
    }

    refreshGrid();
    delay(200);  // simple debounce
  }
}

void updatePieceGravity() {
  if(isPaused || isGameOver) return;

  if (slowGravityActive) {
    interval = 1500; // slow
  }else {
    if(interval > 100){
      interval = max(100, 500 - ((level - 1) * 50)); // gets faster each level, minimum 100ms
    }
    else{
      interval = interval = max(50, 100 - ((level - 1) * 2)); //gets faster more slowly after level 10
    }
  }

  if (millis() - timer > interval) {
    checkLines();

    refreshGrid();
    if (nextCollision()) {
      for (short i = 0; i < 4; i++)
        grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
      generate();
      if (spawnCollision()) {
        displayGameOver();
        return;
      }
    } else
      pieceY++;
    timer = millis();
  }
}

void PieceRotation() {
  if (!digitalRead(B_1)) {  //assign to change
    // tone(BUZZER, click[0], 1000 / click_duration[0]);
    // delay(100);
    // noTone(BUZZER);
    if (b3) {
      if (rotation == getMaxRotation(currentType) - 1 && canRotate(0)) {
        rotation = 0;
      } else if (canRotate(rotation + 1)) {
        rotation++;
      }
      copyPiece(piece, currentType, rotation);
      refreshGrid();
      b3 = false;
      delayer = millis();
    }
  } else if (millis() - delayer > 50) {
    b3 = true;
  }
}

void resetGame() {
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 25; y++)
      grid[x][y] = 0;
  
  score = 0;
  interval = 500;
  holdType = -1;    
  canHold = true;
  nextType = random(TYPES);
  level = 1;
  generate();
  isGameOver = false;

  Serial.println("STATE:PLAY");
  refreshGrid();
}

void generate() {
  currentType = nextType;
  nextType = random(TYPES);
  pieceX = 3;
  pieceY = 0;
  rotation = 0;
  copyPiece(piece, currentType, rotation);
  canHold = true; // ADDED: Player can use hold again for this new piece
  Serial.print("CT:"); Serial.println(currentType);
  Serial.print("CR:"); Serial.println(rotation);
  Serial.print("CX:"); Serial.println(pieceX);
}

void checkLines() {
  int linesCleared = 0;

  for (short y = 24; y >= 0; y--) {
    bool full = true;
    for (short x = 0; x < 10; x++) {
      full = full && grid[x][y];
    }
    if (full) {
      breakLine(y);
      y++;
      linesCleared++;
    }
  }

  if (linesCleared > 0) {
    comboCount++;

  	int multiplier = doublePointsActive ? 2 : 1;
    // Base score by lines cleared
    switch (linesCleared) {
      case 1: score += 100 * level * multiplier; break;  // Single
      case 2: score += 300 * level * multiplier; break;  // Double
      case 3: score += 500 * level * multiplier; break;  // Triple
      case 4: score += 800 * level * multiplier; break;  // Tetris
    }

    // Combo bonus
    if (comboCount > 0) {
      score += 50 * comboCount * level * multiplier;
    }

    // Increase level every 10 lines
    level = (score / 1000) + 1;

  } else {
    comboCount = -1; // reset combo if no lines cleared
  }
}
void breakLine(short line) {
  for (short y = line; y > 0; y--) {
    for (short x = 0; x < 10; x++) {
      grid[x][y] = grid[x][y - 1];
    }
  }
  for (short x = 0; x < 10; x++) {
    grid[x][0] = 0;
  }
}

bool nextHorizontalCollision(short piece[2][4], int amount) {
  for (short i = 0; i < 4; i++) {
    short newX = pieceX + piece[0][i] + amount;
    if (newX > 9 || newX < 0 || grid[newX][pieceY + piece[1][i]])
      return true;
  }
  return false;
}
bool nextCollision() {
  for (short i = 0; i < 4; i++) {
    short y = pieceY + piece[1][i] + 1;
    short x = pieceX + piece[0][i];
    if (y > 24 || grid[x][y])
      return true;
  }
  return false;
}

bool spawnCollision() {
  for (short i = 0; i < 4; i++) {
    short x = pieceX + piece[0][i];
    short y = pieceY + piece[1][i];

    if (grid[x][y] || y >= 14) {
      return true;
    }
  }
  return false;
}
void copyPiece(short piece[2][4], short type, short rotation) {
  switch (type) {
    case 0:  //L_l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_L_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_L_l[rotation][1][i]));
      }
      break;
    case 1:  //S_l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_S_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_S_l[rotation][1][i]));
      }
      break;
    case 2:  //S_r
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_S_r[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_S_r[rotation][1][i]));
      }
      break;
    case 3:  //Sq
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_Sq[0][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_Sq[0][1][i]));
      }
      break;
    case 4:  //T
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_T[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_T[rotation][1][i]));
      }
      break;
    case 5:  //l
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_l[rotation][1][i]));
      }
      break;
  }
}

short getMaxRotation(short type) {
  if (type == 1 || type == 2 || type == 5)
    return 2;
  else if (type == 0 || type == 4)
    return 4;
  else if (type == 3)
    return 1;
  else
    return 0;
}

bool canRotate(short rotation) {
  short piece[2][4];
  copyPiece(piece, currentType, rotation);
  return !nextHorizontalCollision(piece, 0);
}

void softDrop() {
  if (!digitalRead(DOWN)) { 

    if (millis() - lastSoftDropTime >= softDropSpeed) {
      lastSoftDropTime = millis();

      if (!nextCollision()) {
        pieceY++;
        score += 1; // 1 point per cell soft drop
      } else {
        // lock piece if it hits something
        for (short i = 0; i < 4; i++) {
          grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
        }

        generate();

        if (spawnCollision()) {
          displayGameOver();
          return;
        }

        refreshGrid();
      }
    }
  }
}

void holdBlocks() {
  if (!digitalRead(B_2)) {  // Button pressed (LOW)
    
    if (!isHoldingButton) {
      // First moment button is pressed
      holdStartTime = millis();
      isHoldingButton = true;
    }

    // Check if held for 2 seconds
    if (isHoldingButton && (millis() - holdStartTime >= 1000)) {

      if (holdButtonReady) {
        // Prevent invalid states
        if (!canHold || isGameOver) return;

        if (holdType == -1) {
          // Store current piece
          holdType = currentType;
          generate();
        } else {
          // Swap pieces
          short temp = currentType;
          currentType = holdType;
          holdType = temp;

          pieceX = 3;
          pieceY = 0;
          rotation = 0;
          copyPiece(piece, currentType, rotation);
        }

        canHold = false;
        refreshGrid();

        holdButtonReady = false;  // prevent repeat trigger
      }
    }

  } else {
    // Button released → reset state
    isHoldingButton = false;
    holdButtonReady = true;
  }
}

// Simulates a collision check at a specific simulated Y coordinate
bool checkCollisionAt(short simY) {
  for (short i = 0; i < 4; i++) {
    short y = simY + piece[1][i] + 1; // Check the next step down
    short x = pieceX + piece[0][i];
    
    if (y > 24 || grid[x][y]) {
      return true; // Collision detected
    }
  }
  return false;
}

// Finds the lowest valid Y coordinate for the current piece
short getGhostY() {
  short ghostY = pieceY;
  // Keep pushing the ghost piece down until it hits something
  while (!checkCollisionAt(ghostY)) {
    ghostY++;
  }
  return ghostY;
}

void handleLeftRight() {
  //if (isPaused || isGameOver) return;  irrelevant

  // Only allow movement every "moveDelay" milliseconds
  if (millis() - lastMoveTime < moveDelay) return;

  // LEFT button (INPUT_PULLUP → LOW when pressed)
  if (!digitalRead(LEFT)) {
    if (!nextHorizontalCollision(piece, -1)) {
      pieceX--;
      refreshGrid();
      lastMoveTime = millis();
    }
  }
  // RIGHT button
  if (!digitalRead(RIGHT)) {
    if (!nextHorizontalCollision(piece, 1)) {
      pieceX++;
      refreshGrid();
      lastMoveTime = millis();
    }
  }
}
//========================================
// POWERUPS
//=========================================

void clearBottomThreeLines() {
  for (short y = 22; y <= 24; y++) {
    for (short x = 0; x < 10; x++) {
      grid[x][y] = 0;
    }
  }

  // Shift all rows above y=22 down by 3
  for (short y = 21; y >= 0; y--) {
    for (short x = 0; x < 10; x++) {
      grid[x][y + 3] = grid[x][y];
      grid[x][y] = 0;
    }
  }

  refreshGrid();
}

void activateDoublePoints() {
  doublePointsActive = true;
  doublePointsStart = millis();
  Serial.println("POWERUP:DOUBLE");
}

void checkDoublePoints() {
  if (doublePointsActive && millis() - doublePointsStart >= 10000) {
    doublePointsActive = false;
  }
}

void activateSlowGravity() {
  slowGravityActive = true;
  slowGravityStart = millis();
  Serial.println("POWERUP:SLOW");
}

void checkSlowGravity() {
  if (slowGravityActive && millis() - slowGravityStart >= 10000) {
    slowGravityActive = false;
  }
}