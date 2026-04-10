#include "songs/songs.h"
#include "audio.h"
HardwareSerial ArduinoSerial(2);

char msg[MAX_MSG_SIZE];
int msglen;

// const char ARD_PIECE_XPOS_MSG_PREF[] = "X_pos:";
// const char ARD_CURRENT_PIECE_MSG_PREF[] = "CurType:";
// const char ARD_ROTATION_MSG_PREF[] = "Rot:";
const char ARD_MOOD_MSG_PREF[] = "MOOD:";
const char ARD_NEXT_PIECE_MSG_PREF[] = "NXTTYP:";
const char ARD_HOLD_PIECE_MSG_PREF[] = "HLDTYP:";
const char ARD_SCORE_MSG_PREF[] = "SCORE:";
const char ARD_LVL_MSG_PREF[] = "LVL:";
const char ARD_THEME_MSG_PREF[] = "THEME:";
const char ARD_INVALID_CARD_PREF[] = "INVALID_";
const char ARD_NEG_BOOTUP_CARD_SCAN_MSG[] = "INVALID_PROF_CRD";
const char ARD_NEG_TETRIS_CARD_SCAN_MSG[] = "INVALID_EFF_CRD";
const char ARD_GAMESTATE_PREF[] = "STATE:";
const char ARD_GAMESTATE_PLAY[] = "STATE:PLAY";
const char ARD_GAMESTATE_PAUSE[] = "STATE:PAUSE";
const char ARD_GAMESTATE_GAMEOVER[] = "STATE:OVER";
const char ARD_CARD_EFFECT_PREF[] = "CRDEFF:";
const char ARD_CARD_EFFECT_THEME_NEON[] = "CRDEFF:NEON";
const char ARD_CARD_EFFECT_DOUBLEPOINTS[] = "CRDEFF:DOUBLE";
const char ARD_CARD_EFFECT_LINECLEAR[] = "CRDEFF:CLEAR";
const char ARD_CARD_EFFECT_SLOWGAME[] = "CRDEFF:SLOW";
const char ARD_CARD_EFFECT_THEME_GAMEBOY[] = "CRDEFF:GMEBOY";
const char ARD_CARD_EFFECT_ROTATE[] PROGMEM = "CRDEFF:ROT";
const char ARD_GRID_MSG_PREF[] = "GRID:";
const char ARD_MENU_MSG_PREF[] = "MENU:";
const char ARD_MENU_MSG_ENTER[] = "MENU:ENTR";
const char ARD_MENU_MSG_UP[] = "MENU:UP";
const char ARD_MENU_MSG_DOWN[] = "MENU:DOWN";
const char ARD_MENU_MSG_LEFT[] = "MENU:LEFT";
const char ARD_MENU_MSG_RIGHT[] = "MENU:RIGHT";
const char ARD_MENU_MSG_A[] = "MENU:A";
const char ARD_MENU_MSG_B[] = "MENU:B";
const char ARD_VOLUME_MSG_PREF[] = "VOLUME:";
extern uint16_t volume;
extern bool songPlaying;
extern Buzzer buzzer1, buzzer2, buzzer3;

void UARTsetup() {
  ArduinoSerial.begin(115200, SERIAL_8N1, 35, 22);
}

//You need a level shift of 3.3V from Rx on the Display to Tx on the Arduino (2kOhm or 6.8kOhm to ground) (1kOhm or 4.7kOhm with Tx on uno)
//Arduino will send the uid and based on that a different animation and portrait will be played.
//Optimise loading the bitmaps will be necessary
//Will need to change the stored cry durations
//Message handler should check if a message is a valid message from the arduino, and then handle it accordingly
uint8_t handleMessage() {
  if (ArduinoSerial.available()) {
    msglen = ArduinoSerial.readBytesUntil('\n', msg, MAX_MSG_SIZE - 1);
    msg[msglen] = '\0';
    //For whatever reason arduino sends carraige return so that needs to be trimmed
    if (msglen > 0 && msg[msglen - 1] == '\r') {
      msg[msglen - 1] = '\0';
    }
    // Serial.println(msg);
    return processMsgFromArduino(msg);
  } else {
    // Serial.println("nothing seen");
    return INVALID_MSG;
  }
}

uint8_t processMsgFromArduino(const char* message) {
  if (message != NULL) {
    // len = ArduinoSerial.readBytesUntil("\n", msg, MAX_MSG_SIZE - 1);
    // msg[len] = '\0';
    //These shouldve been ordered with thought. youre supposed to think what messages are better to group together to ensure faster calls
    if (tetrisDataMsg(message)) {
      handleTetrisDataMsg(message);

      return TETRIS_DATA_MSG_HANDLED;
    } else if (menuMsg(message)) {
      handleMenuMsg(message);
      return MENU_MSG_HANDLED;
    } else if (gameStateMsg(message)) {
      handleGameStateMsg(message);

      return GAMESTATE_MSG_HANDLED;
    } else if (moodMsg(message)) {
      handleMoodMsg(message);
      return MOOD_MSG_HANDLED;
    } else if (bootUpCardScanMsg(message)) {
      // Serial.println("Validated Bootup Msg");
      handleBootUpCardScanMsg(message);
      currentScreen = BOOTUP;
      return BOOTUP_CARD_SCAN_MSG_HANDLED;
    } else if (cardEffectsMsg(message)) {
      handleCardEffectMsg(message);
      return CARD_EFFECT_MSG_HANDLED;
    } else if (volumeMsg(message)) {
      handleVolumeMessage(message);
      return VOLUME_MSG_HANDLED;
    } else return INVALID_MSG;

  } else return INVALID_MSG;
}

bool startsWith(const char* message, const char* prefix) {
  while (*prefix) {
    if (*message == *prefix) {
      message++;
      prefix++;
    } else return false;
  }
  return true;
}


bool bootUpCardScanMsg(const char* message) {
  if (startsWith(message, ARD_INVALID_CARD_PREF)) {
    // Serial.println("message starts with INVALID_");
    // Serial.println(message);
    if (strcmp(message, ARD_NEG_BOOTUP_CARD_SCAN_MSG) == 0) {
      // Serial.println("Returning true on cardscan invalid message");
      return true;
    } else {
      // Serial.println(strcmp(message, ARD_NEG_BOOTUP_CARD_SCAN_MSG));
      // Serial.println(message);
      // Serial.println(ARD_NEG_BOOTUP_CARD_SCAN_MSG);
      return false;
    }
  } else if (startsWith(message, "CRD:")) {  //I dont know how to expressly check if the integer sent after is valid, so this isn't very well done tbh
    return true;
  } else return false;
}

void handleBootUpCardScanMsg(const char* message) {
  if (strcmp(message, ARD_NEG_BOOTUP_CARD_SCAN_MSG) == 0) {
    cardMsgDisplay(0);  //negative message
  } else {
    char* ptr = strstr(message, "CRD:");
    if (ptr != NULL) {
      ptr += 4;                // skip past "CRD:"
      cardMsgDisplay(1, ptr);  // ptr now points to just the number
    }
  }
}

uint8_t V_menuPointer = 0;  //Controls where the menu arrow is looking at. 0 means it starts pointing at nothing

bool H_menuPointer = 0;               //We will have a background changer as well but that will only be at the top so a bool is ok here (i think)
#define NUM_MENU_ITEMS NUM_GAMES + 2  //the games, the song and the sprite




void handleMenuMsg(const char* message) {
  if (strcmp(message, ARD_MENU_MSG_ENTER) == 0) {
    currentScreen = MAIN_MENU;
    prevScreen = BOOTUP;
    gameDisplayUpdate = true;
    songDisplayUpdate = true;
    menubackgroundUpdate = true;
    menuMsgSent = false;
    menuPointerChanged = false;
    V_menuPointer = 0;
    H_menuPointer = 0;
    return;
  } else if (strcmp(message, ARD_MENU_MSG_UP) == 0) {
    if (V_menuPointer <= 1) {
      //will add in thud
      return;
    } else {
      V_menuPointer--;
      menuPointerChanged = true;
    }
  } else if (strcmp(message, ARD_MENU_MSG_DOWN) == 0) {
    if ((V_menuPointer >= NUM_MENU_ITEMS) || H_menuPointer) {
      //thud noise
      return;
    } else {
      V_menuPointer++;
      menuPointerChanged = true;
    }
  } else if (strcmp(message, ARD_MENU_MSG_LEFT) == 0) {
    if (!H_menuPointer) {
      //thud
      return;
    } else {
      H_menuPointer = 0;
      menuPointerChanged = true;
      V_menuPointer = 1;  //reset back to top
    }
  } else if (strcmp(message, ARD_MENU_MSG_RIGHT) == 0) {
    if ((V_menuPointer != 1) || (H_menuPointer)) {
      //thud
      return;
    } else {
      H_menuPointer = 1;
      menuPointerChanged = true;
      V_menuPointer = 0;
    }
  } else if (strcmp(message, ARD_MENU_MSG_A) == 0) {
    switch (V_menuPointer) {
      case 0:
        if (H_menuPointer) {
          backgroundClr = (backgroundClr == TFT_BLACK) ? TFT_WHITE : TFT_BLACK;
          backgroundClrNameUpdate();
          gameDisplayUpdate = true;
          menubackgroundUpdate = true;
          songDisplayUpdate = true;
          old_sprWidth = 0;  // force sprite clear on next frame
          old_sprHeight = 0;
          prevScreen = BOOTUP;  // force the fillScreen at top of displayCurrentScreen
        }
        break;
      case 1:
        playNextSong();
        songDisplayUpdate = true;
        break;
      case 2:
        prevScreen = currentScreen;
        currentScreen = IN_GAME;
        sendMsgToArduino(GAME1_ENTERED);
        break;
      case 3:
        //thud
        break;
      case 4:
        // spriteAnim = (spriteAnim + 1) % 4;
        spriteDetails();
        break;
      default:
        //thud
        break;
    }
    return;
  } else if (strcmp(message, ARD_MENU_MSG_B) == 0) {
    //thud
  }
}

void handleGameStateMsg(const char* message) {
  if (strcmp(message, ARD_GAMESTATE_PLAY) == 0) {
    songPlaying = true;
    applyRotationIfPending();  
    resumeGame();
    prevScreen = currentScreen;
    // Serial.println("Playing Tetris");
    return;
  } else if (strcmp(message, ARD_GAMESTATE_PAUSE) == 0) {
    showPause();
    stopSong();
    return;
  } else if (strcmp(message, ARD_GAMESTATE_GAMEOVER) == 0) {
    showGameOver();
    return;
  }
}

void handleTetrisDataMsg(const char* message) {
  char* ptr;
  if (startsWith(message, ARD_GRID_MSG_PREF)) {
    ptr = strstr(message, ARD_GRID_MSG_PREF);
    ptr += strlen(ARD_GRID_MSG_PREF);
    updateTetrisGrid(ptr);
  }
  // else if (startsWith(message, ARD_PIECE_XPOS_MSG_PREF)) {
  //   ptr = strstr(message, ARD_PIECE_XPOS_MSG_PREF);
  //   ptr += strlen(ARD_PIECE_XPOS_MSG_PREF);

  // }
  // else if (startsWith(message, ARD_CURRENT_PIECE_MSG_PREF)) {
  //   ptr = strstr(message, ARD_CURRENT_PIECE_MSG_PREF);
  //   ptr += strlen(ARD_CURRENT_PIECE_MSG_PREF);
  // } else if (startsWith(message, ARD_ROTATION_MSG_PREF)) {
  //   ptr = strstr(message, ARD_ROTATION_MSG_PREF);
  //   ptr += strlen(ARD_ROTATION_MSG_PREF);
  // }
  else if (startsWith(message, ARD_NEXT_PIECE_MSG_PREF)) {
    ptr = strstr(message, ARD_NEXT_PIECE_MSG_PREF);
    ptr += strlen(ARD_NEXT_PIECE_MSG_PREF);
    int8_t pos = -1;
    uint8_t tempnxttyp = 0;
    while (ptr[++pos] != '\0') {
      tempnxttyp = tempnxttyp * 10 + (ptr[pos] - '0');
    }
    drawNextPiece(tempnxttyp);
  } else if (startsWith(message, ARD_HOLD_PIECE_MSG_PREF)) {
    ptr = strstr(message, ARD_HOLD_PIECE_MSG_PREF);
    ptr += strlen(ARD_HOLD_PIECE_MSG_PREF);
    if (ptr[0] == '-') {
      drawHoldPiece(-1);  // empty hold slot
    } else {
      int8_t pos = -1;
      uint8_t tempholdtyp = 0;
      while (ptr[++pos] != '\0') {
        tempholdtyp = tempholdtyp * 10 + (ptr[pos] - '0');
      }
      drawHoldPiece(tempholdtyp);
    }
  } else if (startsWith(message, ARD_SCORE_MSG_PREF)) {
    ptr = strstr(message, ARD_SCORE_MSG_PREF);
    ptr += strlen(ARD_SCORE_MSG_PREF);
    updateScore(ptr);
  } else if (startsWith(message, ARD_LVL_MSG_PREF)) {
    ptr = strstr(message, ARD_LVL_MSG_PREF);
    ptr += strlen(ARD_LVL_MSG_PREF);
    // int8_t pos = -1;
    // uint16_t templvl = 0;
    // while (ptr[++pos] != '\0') {
    //   templvl = templvl * 10 + (ptr[pos] - '0');
    // }

    updateLevel(ptr);
  } else if (startsWith(message, ARD_THEME_MSG_PREF)) {
    ptr = strstr(message, ARD_THEME_MSG_PREF);
    ptr += strlen(ARD_THEME_MSG_PREF);
    int8_t pos = -1;
    uint16_t newTheme = 0;
    while (ptr[++pos] != '\0') {
      newTheme = newTheme * 10 + (ptr[pos] - '0');
    }
    if (newTheme != currentTheme) {
      currentTheme = newTheme;
    }
  } else return;
}




void handleCardEffectMsg(const char* message) {
  if (strcmp(message, ARD_CARD_EFFECT_DOUBLEPOINTS) == 0) showDoublePointsMsg();
  else if (strcmp(message, ARD_CARD_EFFECT_LINECLEAR) == 0) showClearLineMsg();
  else if (strcmp(message, ARD_CARD_EFFECT_SLOWGAME) == 0) showSlowGravityMsg();
  else if (strcmp(message, ARD_CARD_EFFECT_THEME_NEON) == 0 || strcmp(message, ARD_CARD_EFFECT_THEME_GAMEBOY) == 0) {

    // drawGameLayout();                       // Redraws borders in the new color
    memset(prevGrid, 0, sizeof(prevGrid));  // Forces blocks to redraw with new skin
  } else if (strcmp(message, ARD_CARD_EFFECT_ROTATE) == 0) {
    triggerRotationEffect();
  }
}

extern uint8_t mood;
void handleMoodMsg(const char* message) {
  if (startsWith(message, ARD_MOOD_MSG_PREF)) {
    char* ptr;
    ptr = strstr(message, ARD_MOOD_MSG_PREF);
    ptr += strlen(ARD_MOOD_MSG_PREF);
    int8_t pos = -1;
    uint8_t newMood = 0;
    while (ptr[++pos] != '\0') {
      newMood = newMood * 10 + (ptr[pos] - '0');
    }
    uint8_t newMoodBand;
    if (newMood >= 80) newMoodBand = 5;       // happy
    else if (newMood >= 60) newMoodBand = 4;  // neutral
    else if (newMood >= 40) newMoodBand = 3;  // sad neutral
    else if (newMood >= 20) newMoodBand = 2;  // slightly angry
    else newMoodBand = 1;                     // very angry

    if (newMoodBand != mood) {  // only redraw on band change, not every UART tick
      mood = newMoodBand;
      if (!isGameOverState && !isPausedState) drawPortrait(mood);
    }
  }
}

void handleVolumeMessage(const char* message) {
  char* ptr;
  ptr = strstr(message, ARD_VOLUME_MSG_PREF);
  ptr += strlen(ARD_VOLUME_MSG_PREF);
  int8_t pos = -1;
  uint16_t newVolume = 0;
  while (ptr[++pos] != '\0') {
    newVolume = newVolume * 10 + (ptr[pos] - '0');
    // Serial.println(ptr[pos]);
  }
  volume = newVolume;
}
// HardwareSerial mySerial(2);
// int nextEspType = 0;
// int activeEspType = 0;
// int activeEspRotation = 0;
// int activeEspX = 0;

// void UARTsetup() {
//   // mySerial.setRxBufferSize(512);   //What is the point in this?
//   mySerial.begin(115200, SERIAL_8N1, 35, 22);
//   displayDefMsg();
// }

// Run this constantly in the ESP32 loop()       -> ????Dont really get the point in this, using readstringuntil is not memory efficient
//Strings use dynamic memory allocation and that can cause crashes if not handled well, so its better to use c style strings. It is unpredictable and causes memory fragmentation
//also this implementation is slower
//The skeleton is good but i combined it with the cardscan logic to make it actually efficient
// void listenToArduino() {
// if (ArduinoSerial.available()) {
//   String data = mySerial.readStringUntil('\n');
//   data.trim(); // Clean up hidden newlines

//   // Route the data - If it's random noise, it safely ignores it!
//   if (data.startsWith("G:")) {
//     updateTetrisGrid(data.substring(2));
//   }
//   else if (data.startsWith("S:")) {
//     updateScore(data.substring(2));
//   }
//   else if (data.startsWith("N:")) {
//     nextEspType = data.substring(2).toInt();
//     drawNextPiece(nextEspType);
//   }
//   else if (data.startsWith("H:")) {
//     drawHoldPiece(data.substring(2).toInt());
//   }
//   else if (data == "STATE:OVER") {
//     showGameOver();
//   }
//   else if (data == "STATE:PAUSE") {
//     showPause();
//   }
//   else if (data == "STATE:PLAY") {
//     resumeGame();
//   }
//   else if (data == "INVALID_CRD") {
//     cardMsgDisplay(0);
//   }
//   else if (data.startsWith("CRD:")) {
//     String cardNum = data.substring(4);
//     cardMsgDisplay(1, (char*)cardNum.c_str());
//   }
//   else if (data.startsWith("CT:")) {
//     activeEspType = data.substring(3).toInt();
//   }
//   else if (data.startsWith("CR:")) {
//     activeEspRotation = data.substring(3).toInt();
//   }
//   else if (data.startsWith("CX:")) {
//     activeEspX = data.substring(3).toInt();
//   }
//   else if (data.startsWith("LV:")) {
//     updateLevel(data.substring(3).toInt());
//   }
//   else if (data.startsWith("TH:")) {
//     int newTheme = data.substring(3).toInt();
//     if (newTheme != currentTheme) {
//       currentTheme = newTheme;
//       drawGameLayout(); // Redraws borders in the new color
//       memset(prevGrid, 0, sizeof(prevGrid)); // Forces blocks to redraw with new skin
//     }
//   }
//   else if (data == "POWERUP:CLEAR") {
//     showClearLineMsg();
//   }
//   else if (data == "POWERUP:DOUBLE") {
//     showDoublePointsMsg();
//   }
//     else if (data == "POWERUP:SLOW") {
//       showSlowGravityMsg();
//     }
//   }
// }


//Because of this you also need to have error checking

bool tetrisDataMsg(const char* message) {
  //Same issues as cardscan, i dont check for valid numbers/values sent so that could be a potential bug
  if (startsWith(message, ARD_GRID_MSG_PREF)) {
    return true;
  }
  // else if (startsWith(message, ARD_PIECE_XPOS_MSG_PREF)) {
  //   return true;
  // } else if (startsWith(message, ARD_CURRENT_PIECE_MSG_PREF)) {
  //   return true;
  // } else if (startsWith(message, ARD_ROTATION_MSG_PREF)) {
  //   return true;
  // }

  else if (startsWith(message, ARD_NEXT_PIECE_MSG_PREF)) {
    return true;
  } else if (startsWith(message, ARD_HOLD_PIECE_MSG_PREF)) {
    return true;
  } else if (startsWith(message, ARD_SCORE_MSG_PREF)) {
    return true;
  } else if (startsWith(message, ARD_LVL_MSG_PREF)) {
    return true;
  } else if (startsWith(message, ARD_THEME_MSG_PREF)) {
    return true;
  } else return false;
}

bool gameStateMsg(const char* message) {
  if (startsWith(message, ARD_GAMESTATE_PREF)) {
    if (strcmp(message, ARD_GAMESTATE_PLAY) == 0) return true;
    else if (strcmp(message, ARD_GAMESTATE_PAUSE) == 0) return true;
    else if (strcmp(message, ARD_GAMESTATE_GAMEOVER) == 0) return true;
    else return false;
  } else return false;
}

bool cardEffectsMsg(const char* message) {
  if (startsWith(message, ARD_CARD_EFFECT_PREF))
    return true;
  else return false;
}

bool enterMsg(const char* message) {
  if (startsWith(message, ARD_MENU_MSG_PREF) == 0) return true;
  else return false;
}


bool menuMsg(const char* message) {
  if (startsWith(message, ARD_MENU_MSG_PREF)) return true;
  else return false;
}

bool moodMsg(const char* message) {
  if (startsWith(message, ARD_MOOD_MSG_PREF)) return true;
  else return false;
}

bool volumeMsg(const char* message) {
  if (startsWith(message, ARD_VOLUME_MSG_PREF)) return true;
  else return false;
}

void sendMsgToArduino(const uint8_t returnMsg) {
  ArduinoSerial.println(returnMsg);
}
