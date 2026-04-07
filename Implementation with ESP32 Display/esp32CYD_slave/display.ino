#include "songs/songs.h"
#include "audio.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

TFT_eSPI tft = TFT_eSPI();

//const int highY = SCREEN_WIDTH*0.25;

// #define XPT2046_IRQ 36
// #define XPT2046_MOSI 32
// #define XPT2046_MISO 39
// #define XPT2046_CLK 25
// #define XPT2046_CS 33

// SPIClass touchscreenSPI = SPIClass(VSPI);
// XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

uint16_t backgroundClr = TFT_BLACK;
char backgroundClrName[] = "black";
//uint16_t textClr;

#define DEFAULT_FONT 2  //Only 1 and 2 work in this case
#define DEFAULT_TEXT_SIZE 1
#define LARGE_FONT 1  //YES I KNOW ITS SMALLEr, but just make sure that large large and default default is used
#define LARGE_TEXT_SIZE 2
#define BOOTUP_MSG_WIDTH 30   //untested
#define BOOTUP_MSG_HEIGHT 30  //untested
#define SONG_BORDER_LINE 46
#define Y_GAME1 50
#define GAME_CLEARANCE 10
#define PARTNER_MSG_CLEARANCE 10
const char BOOTUP_MSG[] PROGMEM = "PLEASE SCAN A VALID CARD";
const char NEG_BOOTUP_CARD_SCAN_MSG[] PROGMEM = "ACCESS DENIED LMAO";
const char POS_BOOTUP_CARD_SCAN_MSG[] PROGMEM = "ACCESS GRANTED TO CARD ";
const char TETRIS_CARD_SCAN_MSG[] PROGMEM = "SCAN A CARD FOR AN EFFECT";
const char NEG_TETRIS_CARD_SCAN_MSG[] PROGMEM = "LMAO NO EFFECT FOR YOU LOSER";
const char POS_TETRIS_CARD_SCAN_MSG[] PROGMEM = "EFFECT TRIGGERED!";
const char PARTNER_MSG[] PROGMEM = "YOU HAVE CHOSEN";
#define Y_PARTNER_MSG SCREEN_WIDTH * 0.15
extern HardwareSerial ArduinoSerial;
//extern char myPartner;
extern char uppercasedPartner[];
extern char capitalisedPartner[];
extern char filename[FILENAME_MAX_LENGTH];
extern const char DEF_DIRECTORY[];
extern uint16_t spriteBuffer[];
extern int sprWidth, sprHeight;
extern uint8_t spriteNums;
extern uint8_t V_menuPointer;
extern bool H_menuPointer;
extern bool spritePreloaded;
int partnerInd = 0;
int displtimer;
uint8_t currentTheme;
bool isGameOverState = false;
bool isPausedState = false;  // NEW SHIELD TRACKER
String currentScoreStr = "0";
int currentLevelDisp = 1;
unsigned long timer;
const char SD_MOUNT_FAIL[] = "CARD MOUNT FAILED";
const char NO_SD_ATTCHD[] = "NO SD CARD ATTACHED";
const char RESTART_PROMPT[] = "PLEASE RESET DEVICE";
const char ADVANCE_TO_MENU[] = "PRESS TO ADVANCE";


// void touchscreenSetup() {
//   touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
//   touchscreen.begin(touchscreenSPI);
//   touchscreen.setRotation(2);
// }

void displaySetup() {
  tft.init();
  tft.setRotation(2);
  tft.setSwapBytes(true);  //Mistakenly assumed in the RGB565 converter that display wanted in little endian, so need to make this swap.
}

// void displayFailedSDMsg() {
//   tft.fillScreen(backgroundClr);
//   tft.setTextColor(updateTextClr(), backgroundClr);
//   tft.drawCentreString(SD_MOUNT_FAIL, centerX, centerY, LARGE_FONT);
//   timer = millis();
//   while (millis() - timer <= 3000)
//     ;
//   int end_FailSD_MSG_Y = tft.fontHeight() + centerY;  //might have to add clearance
//   tft.drawCentreString(RESTART_PROMPT, centerX, end_FailSD_MSG_Y, LARGE_FONT);
// }

// void displayNoSDMsg() {
//   tft.fillScreen(backgroundClr);
//   tft.setTextColor(updateTextClr(), backgroundClr);
//   tft.drawCentreString(NO_SD_ATTCHD, centerX, centerY, LARGE_FONT);
//   timer = millis();
//   while (millis() - timer <= 3000)
//     ;
//   int end_NoSD_MSG_Y = tft.fontHeight() + centerY;
//   tft.drawCentreString(RESTART_PROMPT, centerX, end_NoSD_MSG_Y, LARGE_FONT);
// }
SPIClass sdSPI = SPIClass(VSPI);
void sdSetup() {

  if (!SD.begin(SS, sdSPI, 80000000)) {
    // displayFailedSDMsg();
    // while(true);
    Serial.println("Failed SD Setup");
    while (true)
      ;
  }

  if (SD.cardType() == CARD_NONE) {
    // displayNoSDMsg();
    Serial.println("No SD Attached");
    while (true)
      ;
  }
  // File root = SD.open("/");
  // if (!root) {
  //   Serial.println("Failed to open root");
  // } else {
  //   Serial.println("Root opened OK");
  //   File entry = root.openNextFile();
  //   while (entry) {
  //     Serial.println(entry.name());
  //     entry = root.openNextFile();
  //   }
}


//   // Clear the screen before writing to it
// }

// void littleFSSetup(){
//  // LittleFS.begin();                       //This is for flash

// }


// void displayBootUpMsg() {
//   if (backgroundClr == TFT_WHITE) {
//     tft.fillScreen(TFT_WHITE);
//     tft.setTextColor(TFT_BLACK, TFT_WHITE);
//   }
//   else if(backgroundClr == TFT_BLACK) {
//     tft.fillScreen(TFT_BLACK);
//     tft.setTextColor(TFT_WHITE, TFT_BLACK);
//   }
//     tft.drawCentreString(BOOTUP_MSG, centerX, centerY, DEFAULT_FONT);
// }
uint16_t updateTextClr() {
  return (backgroundClr == TFT_BLACK) ? TFT_WHITE : TFT_BLACK;
}

void backgroundClrNameUpdate() {
  if (backgroundClr == TFT_BLACK) {
    strcpy(backgroundClrName, "black");
    return;
  } else if (backgroundClr == TFT_WHITE) {
    strcpy(backgroundClrName, "white");
    return;
  }
}

void displayBootUpMsg() {
  tft.fillScreen(backgroundClr);
  tft.setTextColor(updateTextClr(), backgroundClr);
  tft.drawCentreString(BOOTUP_MSG, centerX, centerY, DEFAULT_FONT);
}

void cardMsgDisplay(bool successfulScan, char* cardNumber = NULL) {
  tft.fillRect(centerX, centerY, BOOTUP_MSG_WIDTH, BOOTUP_MSG_HEIGHT, backgroundClr);
  if (!successfulScan) {
    tft.fillScreen(backgroundClr);
    tft.drawCentreString(NEG_BOOTUP_CARD_SCAN_MSG, centerX, centerY, DEFAULT_FONT);
    timer = millis();
    bool redrawnBootup = false;
    //Keep nesting handling messages while a 2s timer checks to re-display boot message
    //This ensures only a positive card scan ends the loop
    while (handleMessage() != BOOTUP_CARD_SCAN_MSG_HANDLED) {
      if ((millis() - timer) >= 2000 && !redrawnBootup) {
        // Serial.println(millis() - timer);
        displayBootUpMsg();
        redrawnBootup = true;
      }
    }
  } else {
    char full_pos_msg[strlen(POS_BOOTUP_CARD_SCAN_MSG) + 2];  //For NULL AND THE NUMBER SENT
    snprintf(full_pos_msg, sizeof(full_pos_msg), "%s%s", POS_BOOTUP_CARD_SCAN_MSG, cardNumber);
    tft.drawCentreString(full_pos_msg, centerX, centerY, DEFAULT_FONT);
    bool partnerIndFound = false;
    timer = millis();
    while ((millis() - timer) <= 2000) {
      if (!partnerIndFound) {
        int pos = -1;
        while (cardNumber[++pos] != '\0') {
          partnerInd = partnerInd * 10 + (cardNumber[pos] - '0');
        }
        partnerInd--;
        partnerIndFound = true;
      }
    }
  }
}

// void cardMsgDisplay(bool signal, char* cardNumber = NULL) {
//   tft.fillRect(0, centerY - 20, SCREEN_WIDTH, 40, TFT_BLACK);

//   if (!signal) {
//     tft.drawCentreString(NEG_CARD_SCAN_MSG, centerX, centerY, DEFAULT_FONT);
//     delay(2000);  //display neg message for 2s                 //delays are dumb, dont use those
//     while (!ArduinoSerial.available()) {
//       displayDefMsg();  //then go back to default
//     }
//     cardScanMsg();
//     displayBootUpMsg();
//   } else {
//     char full_pos_msg[64];             //hence why using a macro is useful so we can already know the size.
//     snprintf(full_pos_msg, sizeof(full_pos_msg), "%s%s", POS_CARD_SCAN_MSG, cardNumber);
//     tft.drawCentreString(full_pos_msg, centerX, centerY, DEFAULT_FONT);
//     delay(3000);
//     tft.fillScreen(TFT_BLACK);
//     drawGameLayout();
//   }
// }




void capitaliseName(const char* nameToCapitalise, char* destination) {
  uint8_t i = 0;
  while (nameToCapitalise[i] != '\0') {
    if (i == 0) {
      destination[i] = toupper((unsigned char)nameToCapitalise[i]);
    } else destination[i] = tolower((unsigned char)nameToCapitalise[i]);
    i++;
  }
  destination[i] = '\0';
}

void uppercaseName(const char* nameToUppercase, char* destination) {
  uint8_t i = 0;
  while (nameToUppercase[i] != '\0') {
    destination[i] = toupper((unsigned char)nameToUppercase[i]);
    i++;
  }
  destination[i] = '\0';
}

void displayPartnerMsg(const char* partnerName) {
  tft.fillScreen(backgroundClr);
  tft.setTextSize(LARGE_TEXT_SIZE);
  tft.drawCentreString(PARTNER_MSG, centerX, Y_PARTNER_MSG, LARGE_FONT);
  // Serial.println("showing partneer msg");
  int textWidth = tft.textWidth(PARTNER_MSG, LARGE_FONT);
  // tft.setTextSize(DEFAULT_TEXT_SIZE);
  int end_partnerMsg_X = centerX + (textWidth / 2);
  bool nameCapitalised = false;  //need this for the displayed name so might as well do it now
  bool nameUppercased = false;   //in the same vain
  for (uint8_t i = 0; i < 3; i++) {
    timer = millis();
    while (((millis() - timer) <= 1000) || !nameCapitalised || !nameUppercased) {
      if (!nameCapitalised) {
        capitaliseName(partnerName, capitalisedPartner);
        nameCapitalised = true;
        // Serial.println("NAME CAPITALISED");
        // Serial.println(partnerName);
        // Serial.println(capitalisedPartner);
      }
      if (!nameUppercased) {
        uppercaseName(partnerName, uppercasedPartner);
        nameUppercased = true;
        // Serial.println("NAME UPPERCASED");
        // Serial.println(partnerName);
        // Serial.println(uppercasedPartner);
      }
    }
    // Serial.println(millis() - timer);
    tft.drawString(".", end_partnerMsg_X, Y_PARTNER_MSG, LARGE_FONT);
    end_partnerMsg_X = end_partnerMsg_X + 6;  //Spacing between dots
  }

  int end_partnerMsg_Y = tft.fontHeight() + Y_PARTNER_MSG;
  tft.drawCentreString(uppercasedPartner, centerX, end_partnerMsg_Y + PARTNER_MSG_CLEARANCE, LARGE_FONT);
  tft.setTextSize(DEFAULT_TEXT_SIZE);
}

void displayStartupPartnerSprite(const char* partner, const char* Animation, const uint8_t partnerIndex) {
  //First assign the num of sprites being used
  if (strcmp(Animation, "idle") == 0) {
    switch (partnerIndex) {
      case 0:  //Vaporeon
        spriteNums = IDLE_SPRITE_NUM_A;
        break;
      case 1:  //Jolteon
        spriteNums = IDLE_SPRITE_NUM_A;
        break;
      case 2:  //Flareon
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 3:  //Espeon, Umbreon not possible
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 5:  //Leafeon
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 6:  //gGlaceon
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 7:  //Sylveon
        spriteNums = IDLE_SPRITE_NUM_C;
        break;
    }
  } else if (strcmp(Animation, "nod") == 0 && partnerIndex == 4) {
    spriteNums = NOD_SPRITE_NUM;
  } else if (strcmp(Animation, "walk") == 0) {
    spriteNums = WALK_SPRITE_NUM;
  } else if (strcmp(Animation, "sleep") == 0) {
    spriteNums = SLEEP_SPRITE_NUM;
  } else Serial.println("INVALID SPRITE");

  //Now find the sprite you want    //Try and implement a response to A press
  //need to improve on this implementation
  uint8_t loopcount = 0;
  uint8_t i = 1;
  sendMsgToArduino(PARTNER_POPUP_DONE);
  while (handleMessage() != MENU_MSG_HANDLED) {
    // for (uint8_t i = 1; i <= spriteNums; i++) {
    snprintf(filename, sizeof(filename), "%s/%s_data/Sprites_CLR/%s_clr_%s/%s_clr_%s%d_%s.bin", DEF_DIRECTORY,
             partner, partner, Animation, partner, Animation, i++, backgroundClrName);
    // Serial.println(filename);
    File sprite = SD.open(filename, FILE_READ);
    if (!sprite) {
      Serial.println("Failed to open bin");
      return;
    }
    sprite.read((uint8_t*)spriteBuffer, sprWidth * sprHeight * 2);
    sprite.close();
    // Set X and Y coordinates for center of display
    uint16_t x_displ = (SCREEN_HEIGHT - sprWidth) * 0.5;
    uint16_t y_displ = (SCREEN_WIDTH - sprHeight) * 0.75;
    tft.pushImage(x_displ, y_displ, sprWidth, sprHeight, spriteBuffer);
    timer = millis();
    if (strcmp(Animation, "idle") == 0 || strcmp(Animation, "nod") == 0) {
      while (millis() - timer <= IDLE_DELAY)
        ;
    } else if (strcmp(Animation, "sleep") == 0) {
      while (millis() - timer <= SLEEP_DELAY)
        ;
    } else if (strcmp(Animation, "walk") == 0) {
      while (millis() - timer <= WALK_DELAY)
        ;
    }
    // }
    if (loopcount == 2) {
      uint16_t y_advance_to_menu = SCREEN_WIDTH - (2 * tft.fontHeight());
      tft.drawCentreString(ADVANCE_TO_MENU, centerX, y_advance_to_menu, DEFAULT_FONT);
    } else if (loopcount == 255) {  //preventing it from blowing up as im using uint8
      loopcount = 0;
    }
    if (i > spriteNums) {
      i = 1;
      loopcount++;
    }
  }




  // while (handleMessage() != MENU_MSG_HANDLED) {
  //   for (uint8_t i = 1; i <= spriteNums; i++) {
  //     snprintf(filename, sizeof(filename), "%s/%s_data/Sprites_CLR/%s_clr_%s/%s_clr_%s%d_%s.bin", DEF_DIRECTORY,
  //              partner, partner, Animation, partner, Animation, i, backgroundClrName);
  //     //Serial.println(filename2);
  //     File sprite = SD.open(filename, FILE_READ);
  //     if (!sprite) {
  //       Serial.println("Failed to open bin");
  //       return;
  //     }
  //     sprite.read((uint8_t*)spriteBuffer, sprWidth * sprHeight * 2);
  //     sprite.close();
  //     // Set X and Y coordinates for center of display
  //     int y_displ = (SCREEN_WIDTH - sprHeight) * 0.75;

  //     tft.pushImage(centerX, y_displ, sprWidth, sprHeight, spriteBuffer);
  //     timer = millis();
  //     if (strcmp(Animation, "idle") == 0 || strcmp(Animation, "nod") == 0) {
  //       while (millis() - timer <= IDLE_DELAY)
  //         ;
  //     } else if (strcmp(Animation, "sleep") == 0) {
  //       while (millis() - timer <= SLEEP_DELAY)
  //         ;
  //     } else if (strcmp(Animation, "walk") == 0) {
  //       while (millis() - timer <= WALK_DELAY)
  //         ;
  //     }
  //   }
  // }
  startSong(random(0, sizeof(songList) / sizeof(songList[0])));
}




void displaySong() {
  //displayCurrentlyPlayingSong
  tft.fillRect(0, 0, SCREEN_HEIGHT, SONG_BORDER_LINE, backgroundClr);
  tft.drawFastHLine(0, SONG_BORDER_LINE, SCREEN_HEIGHT, updateTextClr());
  tft.setTextColor(updateTextClr(), backgroundClr);
  char currentSong[501];
  strncpy(currentSong, getCurrentSongName(), 500);
  currentSong[500] = '\0';
  tft.drawString(currentSong, 11, (10 + Y_GAME1 - 10) / 2, LARGE_FONT);
  if (V_menuPointer == 1) {
    tft.drawTriangle(2, 10,
                     2, Y_GAME1 - 10,
                     10, (10 + Y_GAME1 - 10) / 2,
                     TFT_RED);
  }
}

void displayBackgroundName() {
  char capitalisedBackgroundName[6];
  capitaliseName(backgroundClrName, capitalisedBackgroundName);
  uint8_t textWidth = tft.textWidth(capitalisedBackgroundName, DEFAULT_FONT);
  if (!H_menuPointer) {
    tft.setTextColor(updateTextClr(), backgroundClr);
    tft.drawString(capitalisedBackgroundName, SCREEN_HEIGHT - textWidth, 0, DEFAULT_FONT);
  } else {
    tft.fillRect((SCREEN_HEIGHT - textWidth), 0, textWidth, 2 * tft.fontHeight(), updateTextClr());  //not centered dipshit
    tft.setTextColor(backgroundClr, updateTextClr());
    tft.drawString(capitalisedBackgroundName, SCREEN_HEIGHT - textWidth, 0, DEFAULT_FONT);
    tft.setTextColor(updateTextClr(), backgroundClr);
  }
}

void displayBackground() {
}

#define NUM_GAMES 2
const char* GAMES[] = { "TETRIS", " " };


void displayGames() {
  tft.setTextSize(LARGE_TEXT_SIZE);
  for (uint8_t gameInd = 0; gameInd < NUM_GAMES; gameInd++) {
    if (V_menuPointer - 2 == gameInd) {
      if (backgroundClr == TFT_BLACK) {
        tft.fillRect(0, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), SCREEN_HEIGHT,
                     2 * tft.fontHeight() + GAME_CLEARANCE, TFT_NAVY);
        tft.setTextColor(TFT_RED, TFT_NAVY);
        tft.drawCentreString(GAMES[gameInd], centerX, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), DEFAULT_FONT);
        tft.setTextColor(updateTextClr(), backgroundClr);
      } else {
        tft.fillRect(0, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), SCREEN_HEIGHT,
                     2 * tft.fontHeight() + GAME_CLEARANCE, TFT_LIGHTGREY);
        tft.setTextColor(TFT_VIOLET, TFT_LIGHTGREY);
        tft.drawCentreString(GAMES[gameInd], centerX, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), DEFAULT_FONT);
        tft.setTextColor(updateTextClr(), backgroundClr);
      }
      tft.drawTriangle(2, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE) + 10,
                       2, Y_GAME1 + 2 * tft.fontHeight() + gameInd * (tft.fontHeight() + GAME_CLEARANCE) - 10,
                       10, (Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE) + 10 + Y_GAME1 + 2 * tft.fontHeight() + gameInd * (tft.fontHeight() + GAME_CLEARANCE) - 10) / 2,
                       TFT_RED);
      // backgroundClr = TFT_RED;
      // tft.drawCentreString(GAMES[gameInd], centerX, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), DEFAULT_FONT);
      // backgroundClr = TFT_RED;
    } else {
      tft.fillRect(0, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), SCREEN_HEIGHT,
                   2 * tft.fontHeight() + GAME_CLEARANCE, backgroundClr);
      // Serial.println(backgroundClr);
      tft.setTextColor(updateTextClr(), backgroundClr);
      tft.drawCentreString(GAMES[gameInd], centerX, Y_GAME1 + gameInd * (tft.fontHeight() + GAME_CLEARANCE), DEFAULT_FONT);
    }
  }
  tft.setTextSize(DEFAULT_TEXT_SIZE);
}


uint8_t menuSpriteIndex = 0;
uint16_t old_x_displ;
uint16_t old_y_displ;
int old_sprWidth, old_sprHeight;
unsigned long lastSpriteFrameTime = 0;
void updateMenuPartnerSprite(const char* partner, const char* Animation, const uint8_t partnerIndex) {
  uint16_t frameDelay;
  if (V_menuPointer == (NUM_GAMES + 2)) {
    tft.drawTriangle(2, SCREEN_WIDTH - 10,
                     2, SCREEN_WIDTH - SPRITE_MAX_H,
                     10, (SCREEN_WIDTH - 10 + SCREEN_WIDTH - SPRITE_MAX_H) / 2,
                     TFT_RED);
  } else {
    tft.drawTriangle(2, SCREEN_WIDTH - 10,
                     2, SCREEN_WIDTH - SPRITE_MAX_H,
                     10, (SCREEN_WIDTH - 10 + SCREEN_WIDTH - SPRITE_MAX_H) / 2,
                     backgroundClr);
  }

  if (strcmp(Animation, "idle") == 0) {
    frameDelay = IDLE_DELAY;
    switch (partnerIndex) {
      case 0:  //Vaporeon
        spriteNums = IDLE_SPRITE_NUM_A;
        break;
      case 1:  //Jolteon
        spriteNums = IDLE_SPRITE_NUM_A;
        break;
      case 2:  //Flareon
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 3:  //Espeon, Umbreon not possible
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 5:  //Leafeon
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 6:  //gGlaceon
        spriteNums = IDLE_SPRITE_NUM_B;
        break;
      case 7:  //Sylveon
        spriteNums = IDLE_SPRITE_NUM_C;
        break;
    }
  } else if (strcmp(Animation, "nod") == 0 && partnerIndex == 4) {
    frameDelay = IDLE_DELAY;
    spriteNums = NOD_SPRITE_NUM;
  } else if (strcmp(Animation, "walk") == 0) {
    frameDelay = WALK_DELAY;
    spriteNums = WALK_SPRITE_NUM;
  } else if (strcmp(Animation, "sleep") == 0) {
    frameDelay = SLEEP_DELAY;
    spriteNums = SLEEP_SPRITE_NUM;
  } else Serial.println("INVALID SPRITE");
  if (millis() - lastSpriteFrameTime < frameDelay) return;  // not time yet, skip
  if (menuSpriteIndex + 1 > spriteNums) menuSpriteIndex = 0;
  if (spritePreloaded) {
    spritePreloaded = false;
  } else {
    snprintf(filename, sizeof(filename), "%s/%s_data/Sprites_CLR/%s_clr_%s/%s_clr_%s%d_%s.bin", DEF_DIRECTORY,
             partner, partner, Animation, partner, Animation, ++menuSpriteIndex, backgroundClrName);
    //Serial.println(filename2);
    File sprite = SD.open(filename, FILE_READ);
    if (!sprite) {
      Serial.println("Failed to open bin");
      return;
    }
    sprite.read((uint8_t*)spriteBuffer, sprWidth * sprHeight * 2);
    sprite.close();
  }
  uint16_t x_displ = (SCREEN_HEIGHT - sprWidth) * 0.5;
  uint16_t y_displ = (SCREEN_WIDTH - sprHeight);
  // Serial.print("PARTNER IND =");
  // Serial.println(partnerInd);
  // Serial.print("OLD X = ");
  // Serial.println(old_x_displ);
  // Serial.print("OLD Y = ");
  // Serial.println(old_y_displ);
  // Serial.print("OLD SPRWIDTH = ");
  // Serial.println(old_sprWidth);
  // Serial.print("OLD SPRHEIGHT = ");
  // Serial.println(old_sprHeight);
  // if (partnerInd == 7) {  //sylveon is the only one with tthis issue because its sprite is bigger theya ll have it dipshit
  if (sprWidth != old_sprWidth || sprHeight != old_sprHeight) {
    tft.fillRect(old_x_displ, old_y_displ, old_sprWidth, old_sprHeight, backgroundClr);
  }
  // }
  tft.pushImage(x_displ, y_displ, sprWidth, sprHeight, spriteBuffer);
  lastSpriteFrameTime = millis();
  // if (partnerInd == 7) {
  old_x_displ = x_displ;
  old_y_displ = y_displ;
  old_sprHeight = sprHeight;
  old_sprWidth = sprWidth;
  // }
}




























char prevGrid[250] = { 0 };

const char esp_pieces_S_l[2][2][4] = { { { 0, 0, 1, 1 }, { 0, 1, 1, 2 } }, { { 0, 1, 1, 2 }, { 1, 1, 0, 0 } } };
const char esp_pieces_S_r[2][2][4] = { { { 1, 1, 0, 0 }, { 0, 1, 1, 2 } }, { { 0, 1, 1, 2 }, { 0, 0, 1, 1 } } };
const char esp_pieces_L_l[4][2][4] = { { { 0, 0, 0, 1 }, { 0, 1, 2, 2 } }, { { 0, 1, 2, 2 }, { 1, 1, 1, 0 } }, { { 0, 1, 1, 1 }, { 0, 0, 1, 2 } }, { { 0, 0, 1, 2 }, { 1, 0, 0, 0 } } };
const char esp_pieces_Sq[1][2][4] = { { { 0, 1, 0, 1 }, { 0, 0, 1, 1 } } };
const char esp_pieces_T[4][2][4] = { { { 0, 0, 1, 0 }, { 0, 1, 1, 2 } }, { { 0, 1, 1, 2 }, { 1, 0, 1, 1 } }, { { 1, 0, 1, 1 }, { 0, 1, 1, 2 } }, { { 0, 1, 1, 2 }, { 0, 0, 1, 0 } } };
const char esp_pieces_l[2][2][4] = { { { 0, 1, 2, 3 }, { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 }, { 0, 1, 2, 3 } } };

void copyEspPiece(int8_t p[2][4], int type, int rotation) {
  const char(*src)[2][4];
  switch (type) {
    case 0: src = esp_pieces_L_l; break;
    case 1: src = esp_pieces_S_l; break;
    case 2: src = esp_pieces_S_r; break;
    case 3: src = esp_pieces_Sq; break;
    case 4: src = esp_pieces_T; break;
    case 5: src = esp_pieces_l; break;
    default: return;
  }
  for (int i = 0; i < 4; i++) {
    p[0][i] = src[rotation][0][i];
    p[1][i] = src[rotation][1][i];
  }
}

uint8_t mood = 4;  //def value

void resumeGame() {
  isGameOverState = false;
  isPausedState = false;  // Lower ALL shields!
  drawGameLayout();

  // Serial.println("Drawed Layout");
  memset(prevGrid, 0, sizeof(prevGrid));  //clear the previous grid
  drawPortrait(mood);
}

void drawGameLayout() {
  tft.fillScreen(backgroundClr);

  uint16_t borderColor = updateTextClr();
  if (currentTheme == 1) borderColor = TFT_MAGENTA;
  if (currentTheme == 2) borderColor = TFT_GREEN;

  tft.drawRect(GRID_X - 2, GRID_Y - 2, (10 * BLOCK_WIDTH) + 4, (25 * BLOCK_HEIGHT) + 4, borderColor);

  tft.setTextColor(updateTextClr(), backgroundClr);

  tft.drawRect(SCORE_X, SCORE_Y, 80, 75, borderColor);
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, borderColor);
  tft.drawRect(SCORE_X, SCORE_Y, 58, 38, borderColor);
  tft.drawString("SCORE", SCORE_X + 5, SCORE_Y + 5, 1);
  tft.drawString("LV", SCORE_X + 64, SCORE_Y + 5, 1);
  tft.drawString("CRD EFFECT", SCORE_X + 5, SCORE_Y + 45, 1);

  tft.drawRect(USER_X, USER_Y, 40, 40, borderColor);
  //tft.drawString("USER", USER_X + 5, USER_Y - 10, 1);

  tft.drawRect(NEXT_X, NEXT_Y, 80, 60, borderColor);
  tft.drawString("NEXT", NEXT_X + 5, NEXT_Y - 10, 1);

  tft.drawRect(HOLD_X, HOLD_Y, 80, 60, borderColor);
  tft.drawString("HOLD", HOLD_X + 5, HOLD_Y - 10, 1);
}

void showGameOver() {
  isGameOverState = true;

  int boxW = 180;
  int boxH = 110;
  int boxX = centerX - (boxW / 2);
  int boxY = centerY - (boxH / 2);

  tft.fillRect(boxX + 6, boxY + 6, boxW, boxH, tft.color565(30, 30, 30));

  tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
  tft.drawRect(boxX, boxY, boxW, boxH, TFT_RED);
  tft.drawRect(boxX + 1, boxY + 1, boxW - 2, boxH - 2, TFT_RED);
  tft.drawRect(boxX + 2, boxY + 2, boxW - 4, boxH - 4, TFT_RED);

  tft.setTextColor(tft.color565(100, 0, 0));
  tft.drawCentreString("GAME OVER", centerX + 2, boxY + 17, 4);

  tft.setTextColor(TFT_RED);
  tft.drawCentreString("GAME OVER", centerX, boxY + 15, 4);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("FINAL SCORE", centerX, boxY + 50, 1);
  tft.setTextColor(TFT_YELLOW);
  tft.drawCentreString(currentScoreStr, centerX, boxY + 65, 2);

  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("LEVEL " + String(currentLevelDisp), centerX, boxY + 85, 1);
  memset(prevGrid, 0, sizeof(prevGrid));
}

void showPause() {
  isPausedState = true;  // Raise the Pause shield!

  int boxW = 160;
  int boxH = 70;
  int boxX = centerX - (boxW / 2);
  int boxY = centerY - (boxH / 2);

  // 1. Draw a dark shadow
  tft.fillRect(boxX + 6, boxY + 6, boxW, boxH, tft.color565(30, 30, 30));

  // 2. Draw the main box with a thick Yellow Border
  tft.fillRect(boxX, boxY, boxW, boxH, TFT_BLACK);
  tft.drawRect(boxX, boxY, boxW, boxH, TFT_YELLOW);
  tft.drawRect(boxX + 1, boxY + 1, boxW - 2, boxH - 2, TFT_YELLOW);
  tft.drawRect(boxX + 2, boxY + 2, boxW - 4, boxH - 4, TFT_YELLOW);

  // 3. Draw "PAUSED" Drop Shadow (Dark Yellow)
  tft.setTextColor(tft.color565(100, 100, 0));
  tft.drawCentreString("PAUSED", centerX + 2, boxY + 17, 4);

  // 4. Draw "PAUSED" Main Text (Bright Yellow)
  tft.setTextColor(TFT_YELLOW);
  tft.drawCentreString("PAUSED", centerX, boxY + 15, 4);

  // 5. Instruction text
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("- PRESS TO RESUME -", centerX, boxY + 50, 1);
}


void updateTetrisGrid(char* newGrid) {
  if (isGameOverState || isPausedState) return;  // DOUBLE SHIELD!
  if (strlen(newGrid) != 250) return;

  int charIndex = 0;
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {
      char currentState = newGrid[charIndex];

      if (currentState != prevGrid[charIndex]) {
        int xPos = GRID_X + (x * BLOCK_WIDTH);
        int yPos = GRID_Y + (y * BLOCK_HEIGHT);

        uint16_t blockColor;
        bool isGhost = false;

        switch (currentState) {
          case '1': blockColor = TFT_ORANGE; break;
          case '2': blockColor = TFT_GREEN; break;
          case '3': blockColor = TFT_RED; break;
          case '4': blockColor = TFT_YELLOW; break;
          case '5': blockColor = TFT_MAGENTA; break;
          case '6': blockColor = TFT_CYAN; break;
          case '7': isGhost = true; break;
          default: blockColor = TFT_BLACK; break;
        }

        if (currentTheme == 2 && blockColor != TFT_BLACK && !isGhost) {
          blockColor = TFT_GREEN;
        }

        if (isGhost) {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_BLACK);
          tft.drawRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_DARKGREY);
        } else if (blockColor != TFT_BLACK) {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, blockColor);

          if (currentTheme == 1) {
            tft.drawRect(xPos + 2, yPos + 2, BLOCK_WIDTH - 5, BLOCK_HEIGHT - 5, TFT_WHITE);
          } else if (currentTheme == 2) {
            tft.fillRect(xPos + 2, yPos + 2, BLOCK_WIDTH - 5, BLOCK_HEIGHT - 5, TFT_BLACK);
            tft.fillRect(xPos + 4, yPos + 4, BLOCK_WIDTH - 9, BLOCK_HEIGHT - 9, TFT_GREEN);
          }
        } else {
          tft.fillRect(xPos, yPos, BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, TFT_BLACK);
        }

        prevGrid[charIndex] = currentState;
      }
      charIndex++;
    }
  }
}

void updateScore(char* scoreTxt) {
  if (isGameOverState || isPausedState) return;  // DOUBLE SHIELD!

  currentScoreStr = scoreTxt;

  uint16_t borderColor = TFT_WHITE;
  if (currentTheme == 1) borderColor = TFT_MAGENTA;
  if (currentTheme == 2) borderColor = TFT_GREEN;

  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, borderColor);
  tft.drawRect(SCORE_X, SCORE_Y, 58, 38, borderColor);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(scoreTxt, SCORE_X + 5, SCORE_Y + 18, 1);
}

void updateLevel(int lvl) {
  if (isGameOverState || isPausedState) return;  // DOUBLE SHIELD!

  currentLevelDisp = lvl;

  uint16_t borderColor = TFT_WHITE;
  if (currentTheme == 1) borderColor = TFT_MAGENTA;
  if (currentTheme == 2) borderColor = TFT_GREEN;

  tft.fillRect(SCORE_X + 64, SCORE_Y + 15, 50, 15, TFT_BLACK);
  tft.drawRect(SCORE_X, SCORE_Y, 80, 38, borderColor);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(String(lvl), SCORE_X + 64, SCORE_Y + 18, 1);
}

void drawNextPiece(int nextType) {
  if (isGameOverState || isPausedState) return;  // DOUBLE SHIELD!

  tft.fillRect(NEXT_X + 2, NEXT_Y + 15, 76, 43, TFT_BLACK);
  int8_t nPiece[2][4];
  copyEspPiece(nPiece, nextType, 0);

  uint16_t pieceColor = TFT_WHITE;
  if (currentTheme == 2) pieceColor = TFT_GREEN;

  for (int i = 0; i < 4; i++) {
    int px = NEXT_X + 20 + (nPiece[0][i] * 10);
    int py = NEXT_Y + 20 + (nPiece[1][i] * 10);
    tft.fillRect(px, py, 9, 9, pieceColor);

    if (currentTheme == 1) tft.drawRect(px + 2, py + 2, 5, 5, TFT_WHITE);
    else if (currentTheme == 2) {
      tft.fillRect(px + 2, py + 2, 5, 5, TFT_BLACK);
      tft.fillRect(px + 3, py + 3, 3, 3, TFT_GREEN);
    }
  }
}

void drawHoldPiece(int holdType) {
  if (isGameOverState || isPausedState) return;  // DOUBLE SHIELD!
  tft.fillRect(HOLD_X + 2, HOLD_Y + 15, 76, 43, TFT_BLACK);
  if (holdType == -1) return;

  int8_t hPiece[2][4];
  copyEspPiece(hPiece, holdType, 0);

  uint16_t pieceColor = TFT_WHITE;
  if (currentTheme == 2) pieceColor = TFT_GREEN;

  for (int i = 0; i < 4; i++) {
    int px = HOLD_X + 20 + (hPiece[0][i] * 10);
    int py = HOLD_Y + 20 + (hPiece[1][i] * 10);
    tft.fillRect(px, py, 9, 9, pieceColor);

    if (currentTheme == 1) tft.drawRect(px + 2, py + 2, 5, 5, TFT_WHITE);
    else if (currentTheme == 2) {
      tft.fillRect(px + 2, py + 2, 5, 5, TFT_BLACK);
      tft.fillRect(px + 3, py + 3, 3, 3, TFT_GREEN);
    }
  }
}
