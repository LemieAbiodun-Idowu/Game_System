

byte grid[10][18];
int interval = 500;
int score = 0;
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
const int softDropSpeed = 100; // ms between moves
short holdType = -1; // minus means hold slot is empty
bool canHold = true; // Prevents holding more than once per drop
bool holdButtonReady = true; // track if the button can trigger a hold
unsigned long lastMoveTime = 0;
const int moveDelay = 120; // ms between moves
unsigned long holdStartTime = 0;
bool isHoldingButton = false;

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

    // Move piece down until collision
    while (!nextCollision()) {
      pieceY++;
    }

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

// void deadzone() { // Collision Blocks Identifier
//   if(isPaused || isGameOver) return;
//   if (x_point < 32 - deadZone) {
//     if (!nextHorizontalCollision(piece, -1)) {
//       pieceX--;
//       refreshGrid();
//     }
//   } else if (x_point > 32 + deadZone) {
//     if (!nextHorizontalCollision(piece, 1)) {
//       pieceX++;
//       refreshGrid();
//     }
//   }
// }

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
    for (short y = 0; y < 18; y++)
      grid[x][y] = 0;
  
  score = 0;
  interval = 500;
  holdType = -1;    
  canHold = true;
  nextType = random(TYPES);
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
}

void checkLines() {
  bool full;
  for (short y = 17; y >= 0; y--) {
    full = true;
    for (short x = 0; x < 10; x++) {
      full = full && grid[x][y];
    }
    if (full) {
      breakLine(y);
      y++;
    }
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
  
  score += 10;
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
    if (y > 17 || grid[x][y])
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
    if (isHoldingButton && (millis() - holdStartTime >= 2000)) {

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
    
    if (y > 17 || grid[x][y]) {
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

