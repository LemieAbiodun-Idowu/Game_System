uint8_t grid[10][25];
bool isPaused = false;
//bool pauseButtonReady = true;
int interval = 500;
unsigned long score = 0;
bool isGameOver = false;
long timer, delayer;

// word currentType, nextType, rotation; Why were these words???
uint8_t currentType, nextType, rotation;
#define PIECE_TYPES 6
short pieceX, pieceY;
short piece[2][4];
unsigned long lastSoftDropTime = 0;
//#define softDropSpeed 400     // ms between moves
int softDropSpeed;
short holdType = -1;          // minus means hold slot is empty
bool canHold = true;          // Prevents holding more than once per drop
bool holdButtonReady = true;  // track if the button can trigger a hold
bool rotateButtonReady = true;
unsigned long lastMoveTime = 0;
#define moveDelay 300  // ms between moves
unsigned long holdStartTime = 0;
bool isHoldingButton = false;
int level = 1;
int comboCount = -1;  // -1 means no active combo

// 3 types of powerups - doublePoints, clearLine, slowGravity
// doublePoints doubles points gathered from breakLine for 60s - card 2
// clear line clears bottom three lines - card 3
// slowGravity slows the gravity for 30s - card 4
bool doublePointsActive = false;
unsigned long doublePointsStart = 0;
bool slowGravityActive = false;
unsigned long slowGravityStart = 0;

int totalLinesCleared = 0;

unsigned long lockDelayStart = 0;
bool isLocking = false;
#define LOCK_DELAY 200

//tetris skins
int currentTheme = 0;  // NEW: 0 = Default, 1 = Neon, 2 = Gameboy

extern const char pieces_S_l[2][2][4];
;
extern const char pieces_S_r[2][2][4];
extern const char pieces_L_l[4][2][4];
extern const char pieces_Sq[1][2][4];
extern const char pieces_T[4][2][4];
extern const char pieces_l[2][2][4];

uint8_t gameState = 0;  //0 for unintialised, 1 for Play, 2 for Pause
bool scoreUpdated = false;
bool nextTypeUpdated = false;
bool holdTypeUpdated = false;
bool levelUpdated = false;
bool currentThemeUpdated = false;
bool gameInitialised = false;
bool endTetris = false;

void playTetris() {
  endTetris = false;
  if (gameInitialised)
    resetGame();
  while (!endTetris) {
    setVolume();
    if (!gameInitialised) {
      initialiseGame();
    }
    // else resetGame();
    if (gameOver()) continue;  // GAMEOVER WILL HAVE THE FUNC THAT COULD CHANGE ENDTETRIS VAL
    //if(isGameOver) return;

    if (pauseButton()) continue;  //AND SO WILL THIS JUST USE A QUIT SELECTOR
    //if(isPaused) return;

    //powerups
    bool cardEffect = checkCardEffect();
    if (cardEffect) {
      checkDoublePoints();
      // checkPowerUpCard(); //Why check for effect after double points?
      checkSlowGravity();
    }
    //send tetris piece information
    // sendInformation();
    //inputs
    handleLeftRight();
    softDrop();
    hardDrop();
    holdBlocks();
    PieceRotation();
    //game logic
    updatePieceGravity();
    updateMood();
  }
}
bool seedRandomised = false;
void initialiseGame() {
  if (!seedRandomised) {
    randomSeed(analogRead(A5) + analogRead(A4));
    seedRandomised = true;
  }
  isPaused = false;
  isLocking = false;
  comboCount = -1;
  doublePointsActive = false;
  slowGravityActive = false;


  lastMoodUpdate = millis();
  lastMoodScore = 0;
  lastMoodScoreTime = millis();
  currentMood = 50;
  delayer = millis();
  gameState = 1;
  updateState(gameState);
  nextType = random(PIECE_TYPES);
  nextTypeUpdated = true;
  generate();
  timer = millis();
  refreshGrid();
  nextTypeUpdated = false;
  gameInitialised = true;
}


void generate() {
  currentType = nextType;
  nextType = random(PIECE_TYPES);

  pieceX = 3;
  pieceY = 0;
  rotation = 0;
  copyPiece(piece, currentType, rotation);
  canHold = true;  // ADDED: Player can use hold again for this new piece  //bro what in the chatgpt is this statement
  // pieceDataMsg(currentType, rotation, pieceX);
  //   Serial.print("CurType:"); Serial.println(currentType); //only UART comms should have message handling
  //   Serial.print("Rot:"); Serial.println(rotation);
  //   Serial.print("X_pos:"); Serial.println(pieceX);
}

// Puts the correct piece type in based on the rotation value
void copyPiece(short piece[2][4], short type, short rotation) {
  switch (type) {
    case 0:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_L_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_L_l[rotation][1][i]));
      }
      break;
    case 1:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_S_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_S_l[rotation][1][i]));
      }
      break;
    case 2:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_S_r[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_S_r[rotation][1][i]));
      }
      break;
    case 3:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_Sq[0][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_Sq[0][1][i]));
      }
      break;
    case 4:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_T[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_T[rotation][1][i]));
      }
      break;
    case 5:
      for (short i = 0; i < 4; i++) {
        piece[0][i] = pgm_read_byte(&(pieces_l[rotation][0][i]));
        piece[1][i] = pgm_read_byte(&(pieces_l[rotation][1][i]));
      }
      break;
  }
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

bool gameOver() {
  if (isGameOver) {
    gameState = 3;
    updateState(gameState);
    if (!digitalRead(A_btn)) {
      timer = millis();
      while ((millis() - timer) <= 250)
        ;
      resetGame();
    } else if (!digitalRead(B_btn)) {
      timer = millis();
      while ((millis() - timer) <= 250)
        ;
      endTetris = true;
      // gameInitialised = false;  // force reinit next time
      isGameOver = false;
      currentScreen = MAIN_MENU;
    }
    return true;
  } else return false;
}

// Simulates a collision check at a specific simulated Y coordinate
bool checkCollisionAt(short simY) {
  for (short i = 0; i < 4; i++) {
    short y = simY + piece[1][i] + 1;  // Check the next step down
    short x = pieceX + piece[0][i];

    if (y > 24 || grid[x][y]) {
      return true;  // Collision detected
    }
  }
  return false;
}

void refreshGrid() {
  char tempBoard[10][25];

  // Copy grid state
  for (uint8_t x = 0; x < 10; x++) {
    for (uint8_t y = 0; y < 25; y++) {
      tempBoard[x][y] = grid[x][y] + '0';
    }
  }

  // Add Ghost Piece
  short ghostY = getGhostY();
  for (int i = 0; i < 4; i++) {
    uint8_t px = pieceX + piece[0][i];
    uint8_t py = ghostY + piece[1][i];
    if (px >= 0 && px < 10 && py >= 0 && py < 25) {
      if (tempBoard[px][py] == '0') {  // Only draw ghost on empty space
        tempBoard[px][py] = '7';       // '7' tells the ESP32 to draw a ghost block
      }
    }
  }

  // Add active falling piece (OVERWRITES GHOST IF OVERLAPPING)
  for (int i = 0; i < 4; i++) {
    int px = pieceX + piece[0][i];
    int py = pieceY + piece[1][i];
    if (px >= 0 && px < 10 && py >= 0 && py < 25) {
      tempBoard[px][py] = currentType + 1 + '0';  // Uses colors 1-6
    }
  }
  //Should only really send things when theyre updated
  nextTypeUpdated = true;
  gridData(score, nextType, holdType, level, currentTheme, scoreUpdated,
           nextTypeUpdated, holdTypeUpdated, levelUpdated, currentThemeUpdated, tempBoard);
  //Send Grid to ESP32
  // Serial.print("Grid:");
  //int charCount = 0;
  // for (int y = 0; y < 25; y++) {
  //   for (int x = 0; x < 10; x++) {
  //     // Serial.print(tempBoard[x][y]);

  //     //charCount++;
  //     //if (charCount % 25 == 0) delay(1); // Crucial for preventing buffer overflow  //Never use delay just use a millis() timer
  //   }
  // }
}




void resetGame() {
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 25; y++)
      grid[x][y] = 0;

  isPaused = false;
  isLocking = false;
  comboCount = -1;
  doublePointsActive = false;
  slowGravityActive = false;


  score = 0;
  scoreUpdated = true;
  interval = 750;
  holdType = -1;
  holdTypeUpdated = true;
  canHold = true;
  nextType = random(PIECE_TYPES);
  nextTypeUpdated = true;
  level = 1;
  levelUpdated = true;
  generate();
  isGameOver = false;
  gameState = 1;
  updateState(gameState);
  refreshGrid();
  scoreUpdated = false;
  holdTypeUpdated = false;
  nextTypeUpdated = false;
  levelUpdated = false;
  totalLinesCleared = 0;
}

bool pauseButton() {
  if (!digitalRead(B_btn)) {
    //if (pauseButtonReady) {
    //pauseButtonReady = false;
    unsigned long pressStart = millis();

    // Wait to see how long the button is held
    while (!digitalRead(B_btn)) {
      if (millis() - pressStart > 750) return false;  // held too long, ignore
    }

    // Only toggle if released before 1.5 seconds
    isPaused = !isPaused;
    if (isPaused) {
      //Serial.println("STATE:PAUSE");
      gameState = 2;
      updateState(gameState);
    } else {
      //Serial.println("STATE:PLAY");
      gameState = 1;
      updateState(gameState);
      scoreUpdated = true;
      nextTypeUpdated = true;
      holdTypeUpdated = true;
      levelUpdated = true;
      refreshGrid();
      scoreUpdated = false;
      nextTypeUpdated = false;
      holdTypeUpdated = false;
      levelUpdated = false;
    }
    return true;
    //}
  } else return false;
  // else {
  //   pauseButtonReady = true;
  // }
}

unsigned long lastHarddrop = 0;
void hardDrop() {
  if (!digitalRead(UP_btn)) {
    if (millis() - lastHarddrop >= 250) {
      int cellsDropped = 0;
      while (!nextCollision()) {
        pieceY++;
        cellsDropped++;
      }
      score += 2 * cellsDropped;
      scoreUpdated = true;
      lockPiece();
      refreshGrid();
      scoreUpdated = false;
      lastHarddrop = millis();
    }
  }
}

void updatePieceGravity() {
  if (isPaused || isGameOver) return;

  if (slowGravityActive) {
      interval = 1500;
  } else {
      if (level < 16) {
          interval = 500 - (level - 1) * 25;
      } else {
          interval = max(80, 125 - ((level - 16) * 2));
      }
  }

  if (isLocking) {
    if (nextCollision()) {
      // If 500ms has passed while touching the bottom, lock it!
      if (millis() - lockDelayStart >= LOCK_DELAY) {
        lockPiece();
        refreshGrid();
        timer = millis();  // Reset gravity timer
        return;
      }
    } else {
      // The player slid the piece off a ledge! Cancel the lock timer.
      isLocking = false;
    }
  }

  // --- STANDARD GRAVITY ---
  if (millis() - timer > interval) {
    bool gridChanged = false;
    if (!nextCollision()) {
      pieceY++;
      gridChanged = true;
    } else if (!isLocking) {
      // We just hit the bottom! Start the lock delay timer.
      isLocking = true;
      lockDelayStart = millis();
    }
    if (gridChanged) refreshGrid();
    timer = millis();
  }
}

void PieceRotation() {
  if (!digitalRead(A_btn)) {
    if (rotateButtonReady) {
      if (rotation == getMaxRotation(currentType) - 1 && canRotate(0)) {
        rotation = 0;
      } else if (canRotate(rotation + 1)) {
        rotation++;
      }
      copyPiece(piece, currentType, rotation);
      refreshGrid();
      rotateButtonReady = false;
      delayer = millis();
    }
  } else if (millis() - delayer > 250) {
    rotateButtonReady = true;
  }
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

      // Track total lines
      totalLinesCleared += linesCleared;

      // Increase level every 10 lines

      level = (totalLinesCleared / 10) + 1;
      scoreUpdated = true;
      levelUpdated = true;
      refreshGrid();
      scoreUpdated = false;
      levelUpdated = false;

    } else {
      comboCount = -1;  // reset combo if no lines cleared
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
    if (!digitalRead(DOWN_btn)) {
    softDropSpeed = interval * 0.8;
      if (millis() - lastSoftDropTime >= softDropSpeed) {
        lastSoftDropTime = millis();

        if (!nextCollision()) {
          pieceY++;
          score += 1;  // 1 point per cell soft drop
          scoreUpdated = true;
          isLocking = false;  // Cancel lock if falling
          refreshGrid();
          scoreUpdated = false;

        } else if (!isLocking) {
          // Hitting the ground with soft drop starts the slide timer
          isLocking = true;
          lockDelayStart = millis();
        }
      }
    }
  }

  void holdBlocks() {
    if (!digitalRead(B_btn)) {  // Button pressed (LOW)

      if (!isHoldingButton) {
        // First moment button is pressed
        holdStartTime = millis();
        isHoldingButton = true;
      }
      if (isHoldingButton && (millis() - holdStartTime >= 750)) {

        if (holdButtonReady) {
          // Prevent invalid states
          if (!canHold || isGameOver) return;

          if (holdType == -1) {
            // Store current piece
            holdType = currentType;
            generate();
          } else {
            // Swap pieces
            uint8_t temp = currentType;
            currentType = holdType;
            holdType = temp;

            pieceX = 3;
            pieceY = 0;
            rotation = 0;
            copyPiece(piece, currentType, rotation);
          }
          holdTypeUpdated = true;
          canHold = false;
          refreshGrid();
          holdTypeUpdated = false;

          holdButtonReady = false;  // prevent repeat trigger
        }
      }

    } else {
      // Button released → reset state
      isHoldingButton = false;
      holdButtonReady = true;
    }
  }





  void handleLeftRight() {
    //if (isPaused || isGameOver) return;  irrelevant

    // Only allow movement every "moveDelay" milliseconds
    if (millis() - lastMoveTime < moveDelay) return;

    if (!digitalRead(LEFT_btn)) {
      if (!nextHorizontalCollision(piece, -1)) {
        pieceX--;
        refreshGrid();
        lastMoveTime = millis();
      }
    }

    if (!digitalRead(RIGHT_btn)) {
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

  void activateDoublePoints() {
    //No timer pause implemented when paused
    doublePointsActive = true;
    doublePointsStart = millis();
    // cardEffectMsg(cardNum);
    //Serial.println("POWERUP:DOUBLE");
  }

  void checkDoublePoints() {
    if (doublePointsActive && (millis() - doublePointsStart) >= 60000) {
      doublePointsActive = false;
    }
  }

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


  void activateSlowGravity() {
    slowGravityActive = true;
    slowGravityStart = millis();
    // Serial.println("POWERUP:SLOW");
  }

  void checkSlowGravity() {
    if (slowGravityActive && millis() - slowGravityStart >= 30000) {
      slowGravityActive = false;
    }
  }

  void lockPiece() {
    // 1. Lock piece into grid with its specific color
    for (short i = 0; i < 4; i++) {
      grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = currentType + 1;
    }

    isLocking = false;  // Reset lock state

    checkLines();  // Check for completed lines immediately
    generate();    // Spawn the next piece

    if (spawnCollision()) {
      isGameOver = true;
    }
  }
