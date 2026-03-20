byte grid[10][18];
int interval = 500;
int score = 0;
bool isGameOver = false;
long timer, delayer;
const short TYPES = 6;  //type of pieces used
word currentType, nextType, rotation;
short pieceX, pieceY;
short piece[2][4];
extern Adafruit_SSD1306 display;
extern int x_point, y_point;
extern int deadZone;

extern const char pieces_S_l[2][2][4];;
extern const char pieces_S_r[2][2][4];
extern const char pieces_L_l[4][2][4];
extern const char pieces_Sq[1][2][4];
extern const char pieces_T[4][2][4];
extern const char pieces_l[2][2][4];



void initialiseGame() {
  randomSeed(analogRead(0));  //Need to change this as 1. analogRead(0) can return 0 1/1024 which causes a failure and 2. We will be using pin 0 in future
                              //I might recommend using micros() with some logic i'll add into the display later.
  nextType = random(TYPES);
  generate();
  timer = millis();
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
  if (!digitalRead(JOY_B)) {  // button pressed (INPUT_PULLUP)

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

void deadzone() { // Collision Blocks Identifier
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
  // Clear grid
  for (short x = 0; x < 10; x++)
    for (short y = 0; y < 18; y++)
      grid[x][y] = 0;

  score = 0;
  interval = 500;

  display.clearDisplay();
  display.display();

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
  // tone(BUZZER, erase[0], 1000 / erase_duration[0]);
  // delay(100);
  // noTone(BUZZER);
  for (short y = line; y > 0; y--) {
    for (short x = 0; x < 10; x++) {
      grid[x][y] = grid[x][y - 1];
    }
  }

  for (short x = 0; x < 10; x++) {
    grid[x][0] = 0;
  }
  display.invertDisplay(true);
  delay(50);
  display.invertDisplay(false);
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

/////////////////////////START HERE
bool canRotate(short rotation) {
  short piece[2][4];
  copyPiece(piece, currentType, rotation);
  return !nextHorizontalCollision(piece, 0);
}
