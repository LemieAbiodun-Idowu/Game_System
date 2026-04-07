#define FPSTR(pstr_pointer) (reinterpret_cast<const __FlashStringHelper *>(pstr_pointer))
#define BOOTUP_CARD_SCAN_MSG_HANDLED 3

#define INVALID_MSG 0
extern byte grid[10][25];
extern short pieceX, pieceY;
extern short piece[2][4];
extern unsigned long score;
extern uint8_t nextType;
extern short holdType;
char msg[350];
const char MOOD_MSG[] PROGMEM = "MOOD:";
const char NEG_BOOTUP_CARD_SCAN_MSG[] PROGMEM = "INVALID_PROF_CRD";
const char NEG_TETRIS_CARD_SCAN_MSG[] PROGMEM = "INVALID_EFF_CRD";
const char GAMESTATE_PLAY[] PROGMEM = "STATE:PLAY";
const char GAMESTATE_PAUSE[] PROGMEM = "STATE:PAUSE";
const char GAMESTATE_GAMEOVER[] PROGMEM = "STATE:OVER";
const char CARD_EFFECT_THEME_NEON[] PROGMEM = "CRDEFF:NEON";
const char CARD_EFFECT_DOUBLEPOINTS[] PROGMEM = "CRDEFF:DOUBLE";
const char CARD_EFFECT_LINECLEAR[] PROGMEM = "CRDEFF:CLEAR";
const char CARD_EFFECT_SLOWGAME[] PROGMEM = "CRDEFF:SLOW";
const char CARD_EFFECT_THEME_GAMEBOY[] PROGMEM = "CRDEFF:GMEBOY";
const char MENU_ENTERED[] PROGMEM = "MENU:ENTR";
const char MENU_UP[] PROGMEM = "MENU:UP";
const char MENU_DOWN[] PROGMEM = "MENU:DOWN";
const char MENU_LEFT[] PROGMEM = "MENU:LEFT";
const char MENU_RIGHT[] PROGMEM = "MENU:RIGHT";
const char MENU_A[] PROGMEM = "MENU:A";
const char MENU_B[] PROGMEM = "MENU:B";
uint8_t cydMsg;

void UARTsetup() {
  Serial.begin(115200);
  //Why make this intentionally lower, theres no reason why both devices cant send info quick enough????
  //Serial.begin(38400);
}

void failedCardMsg(int i) {
  switch (i) {
    case 0:
      Serial.println(FPSTR(NEG_BOOTUP_CARD_SCAN_MSG));
      break;
    case 1:
      Serial.println(FPSTR(NEG_TETRIS_CARD_SCAN_MSG));
      break;
  }
}

void passedCardMsg(int CardNumber) {
  snprintf(msg, sizeof(msg), "CRD:%d", CardNumber);  //It's always more flexible to use a single complete message.
  Serial.println(msg);
}

void enterMenuMsg() {
  Serial.println(FPSTR(MENU_ENTERED));
}

void A_MenuPress() {
  Serial.println(FPSTR(MENU_A));
}

void B_MenuPress() {
  Serial.println(FPSTR(MENU_B));
}

void UP_MenuPress() {
  Serial.println(FPSTR(MENU_UP));
}

void DOWN_MenuPress() {
  Serial.println(FPSTR(MENU_DOWN));
}

void LEFT_MenuPress() {
  Serial.println(FPSTR(MENU_LEFT));
}

void RIGHT_MenuPress() {
  Serial.println(FPSTR(MENU_RIGHT));
}




// void pieceDataMsg(int pieceType, int rotation, int pieceX_pos) {
//   snprintf(msg, sizeof(msg), "CurType:%d", pieceType);
//   Serial.println(msg);
//   snprintf(msg, sizeof(msg), "Rot:%d", rotation);
//   Serial.println(msg);
//   snprintf(msg, sizeof(msg), "X_pos:%d", pieceX_pos);
//   Serial.println(msg);
// }
uint8_t prevState = 0;
void updateState(uint8_t State) {
  if (State != prevState) {
    switch (State) {
      case 1:
        Serial.println(FPSTR(GAMESTATE_PLAY));
        break;
      case 2:
        Serial.println(FPSTR(GAMESTATE_PAUSE));
        break;
      case 3:
        Serial.println(FPSTR(GAMESTATE_GAMEOVER));
    }
  }
  prevState = State;
}


void gridData(unsigned long score, uint8_t nextPiece, int holdPiece, int level, int theme, bool scoreUpdated,
              bool nextPieceUpdated, bool holdPieceUpdated, bool levelUpdated, bool themeUpdated, char newGrid[10][25]) {
  char tempMsg[251];
  int idx = 0;
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {
      tempMsg[idx++] = newGrid[x][y];
    }
  }
  tempMsg[idx] = '\0';
  snprintf(msg, sizeof(msg), "GRID:%s", tempMsg);
  Serial.println(msg);

  //Only print on updates
  if (scoreUpdated) {
    //Serial.print("SCORE:"); Serial.println(score);
    snprintf(msg, sizeof(msg), "SCORE:%lu", score);
    Serial.println(msg);
  }
  if (nextPieceUpdated) {
    //Serial.print("NXTTYP:"); Serial.println(nextType);
    snprintf(msg, sizeof(msg), "NXTTYP:%d", nextPiece);
    Serial.println(msg);
  }
  if (holdPieceUpdated) {
    snprintf(msg, sizeof(msg), "HLDTYP:%d", holdPiece);
    Serial.println(msg);
  }
  if (levelUpdated) {
    //Serial.print("LVL:"); Serial.println(level);
    snprintf(msg, sizeof(msg), "LVL:%d", level);
    Serial.println(msg);
  }
  if (themeUpdated) {
    //Serial.print("THEME:"); Serial.println(currentTheme);
    snprintf(msg, sizeof(msg), "THEME:%d", theme);
    Serial.println(msg);
  }
}

void cardEffectMsg(int cardNum) {
  switch (cardNum) {
    case 1:
      break;
    case 2:
      Serial.println(FPSTR(CARD_EFFECT_DOUBLEPOINTS));
      break;
    case 3:
      Serial.println(FPSTR(CARD_EFFECT_LINECLEAR));
      break;
    case 4:
      Serial.println(FPSTR(CARD_EFFECT_SLOWGAME));
      break;
    case 5:
      Serial.println(FPSTR(CARD_EFFECT_THEME_NEON));
      break;
    case 6:
      Serial.println(FPSTR(CARD_EFFECT_THEME_GAMEBOY));
      break;
  }
}

uint8_t handleMessage() {

  if (Serial.available()) {
    char buf[8];
    int len = Serial.readBytesUntil('\n', buf, sizeof(buf) - 1);
    buf[len] = '\0';
    // strip \r if present
    if (len > 0 && buf[len - 1] == '\r') buf[len - 1] = '\0';
    uint8_t val = (uint8_t)atoi(buf);
    if (val != 0) return val;
  }
  return INVALID_MSG;
}

uint8_t prevMoodVal = 0;
void sendMoodMsg(uint8_t moodVal) {
  if (moodVal != prevMoodVal) {
    snprintf(msg, sizeof(msg), "MOOD:%d", moodVal);
    Serial.println(msg);
  }
  prevMoodVal = moodVal;
}

void sendVolume(uint16_t volume){
  snprintf(msg, sizeof(msg), "VOLUME:%d", volume);
  Serial.println(msg);
}

// uint8_t processMsgFromCYD(uint8_t message){


// }
// void sendInformation(){
//   Serial.print("CURTYP:"); Serial.println(currentType); //Do not send in segements
//   Serial.print("CR:"); Serial.println(rotation);
//   Serial.print("CX:"); Serial.println(pieceX);
//   Serial.print("GY:"); Serial.println(getGhostY());
// }