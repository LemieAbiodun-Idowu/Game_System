#include <XPT2046_Touchscreen.h>
// #include "LittleFS.h"  //Necessary for File storing in Flash for Arduino 2
#include <FS.h>
#include <TFT_eSPI.h>
#include "SD.h"  //Using SD cards
#define BOOTUP_CARD_SCAN_MSG_HANDLED 3
#define MENU_ENTERED_ESP 7

using fs::File;
#include "audio.h"


//move to spriteHandling later
#define IDLE_SPRITE_NUM_A 16  //Vap, jolt
#define NOD_SPRITE_NUM 24     //umbreon
#define IDLE_SPRITE_NUM_B 32  //flar, esp, glac, leaf
#define IDLE_SPRITE_NUM_C 64  //sylveon
#define WALK_SPRITE_NUM 32
#define SLEEP_SPRITE_NUM 2
#define PARTNER_POPUP_DONE 5
#define FILENAME_MAX_LENGTH 200
#define IDLE_DELAY 750   //not good for sylv, glac, leaf, umbr, esp, flare, jolt
#define WALK_DELAY 220   //good for all
#define SLEEP_DELAY 650  //good for all
// Size for the largest sprites (Largest Size is actually 32x64 but made slightly bigger just in case)
#define SPRITE_MAX_W 64
#define SPRITE_MAX_H 64

//move to display later
#define centerX SCREEN_HEIGHT / 2
#define centerY SCREEN_WIDTH / 2

#define GRID_X 10
#define GRID_Y 10
#define BLOCK_WIDTH 12
#define BLOCK_HEIGHT 12

#define SCORE_X 150
#define SCORE_Y 10
#define USER_X 150
#define USER_Y 104
#define NEXT_X 150
#define NEXT_Y 171
#define HOLD_X 150
#define HOLD_Y 250



//move to uart comms
#define MENU_MSG_HANDLED 6


//Will be removed once header files are put in
void handleMenuMsg(const char* message);
uint8_t handleMessage();
void UARTsetup();
void startupPartnerPopup();
void displayCurrentScreen();
void sendMsgToArduino(const uint8_t returnMsg);

void bootUp() {

  displayBootUpMsg();  //Should only draw it once
  while (handleMessage() != BOOTUP_CARD_SCAN_MSG_HANDLED)
    ;                        //make sure no other messages cause undefined behaviour
  randomSeed(esp_random());  //Allows for calling random() for min and max freely.
}



void setup() {
  Serial.begin(115200);
  // Serial.println("Im ok here");
  // touchscreenSetup();  //No more Touchscreen as VSPI bus is required by the SD card
  displaySetup();
  // Serial.println("Display done");
  UARTsetup();
  // Serial.println("UART done");
  sdSetup();
  buzzersSetup();
  // Serial.println("sd Setup done");
  //littleFSSetup();

  // currentScreen = BOOTUP;
  bootUp();
  // Serial.println("Booted up");
  //displayDefMsg();
  // cardScanMsg();
  //Go into patrner selection screen
  // prevScreen = currentScreen;
  // currentScreen = BOOTUP;
  startupPartnerPopup();
  // Serial.println("Partner PopUp done");
  // prevScreen = currentScreen;
  // currentScreen = MAIN_MENU;
}

void loop() {
  // listenToArduino();
  // clearPowerUpMsg();
  //Now we loop through menus and games
  //displayMenu();
  displayCurrentScreen();
}