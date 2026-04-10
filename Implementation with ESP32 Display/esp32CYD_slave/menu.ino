#include "songs/songs.h"
#include "audio.h"
enum Menu {
  BOOTUP,
  MAIN_MENU,
  IN_GAME,
};

Menu currentScreen;
Menu prevScreen;
bool gameDisplayUpdate = true;
bool songDisplayUpdate = true;
bool menubackgroundUpdate = true;
bool menuMsgSent = false;
bool menuPointerChanged = false;



void displayCurrentScreen() {
  updateSong();
  clearPowerUpMsg();  // must always run, move to top
  checkRotationExpiry();
  if (prevScreen != currentScreen) {
    tft.fillScreen(backgroundClr);
    // Serial.println("Filling Screen");
    // Serial.print("current screen is");
    // Serial.println(currentScreen);
    // Serial.print("prevscreen is ");
    // Serial.println(prevScreen);
    prevScreen = currentScreen;
  }
  if (currentScreen == MAIN_MENU) {

    //let song and games only be drawn if they havent been drawn before, ill put in a song updater bool later
    prevScreen = currentScreen;
    if (songDisplayUpdate || menuPointerChanged) {
      displaySong();
      // Serial.println(volume);
      songDisplayUpdate = true;
      // Serial.println("updated songs");
    }
    if (gameDisplayUpdate || menuPointerChanged) {  //think i can remove gamedisplayupdate later
      displayGames();
      gameDisplayUpdate = false;
      // Serial.println("updated games displayed");
    }
    if (menubackgroundUpdate || songDisplayUpdate) {
      displayBackgroundName();
      displayBackground();
      menubackgroundUpdate = false;
      songDisplayUpdate = false;
      // Serial.println("updated menu pointer");
    }
    menuPointerChanged = false;
    menuPartnerPopup();
    if (!menuMsgSent) {
      sendMsgToArduino(MENU_ENTERED_ESP);
      menuMsgSent = true;
    }

  }
  //remove check here for advancements
  else {
    // handle card effects being shown during pause
    if (isPausedState && !pauseCardScanHandled) {
      uint8_t cardmsg = handleMessage();
      if (cardmsg == CARD_EFFECT_MSG_HANDLED) {
        pauseCardScanHandled = true;
        // show confirmation
        tft.fillRect(centerX - 80, centerY + 20, 160, 20, TFT_BLACK);
        tft.setTextColor(TFT_GREEN);
        tft.drawCentreString("EFFECT APPLIED!", centerX, centerY + 22, 1);
      }
      return;  // don't process other screen logic while paused
    }
  }
  handleMessage();
}
