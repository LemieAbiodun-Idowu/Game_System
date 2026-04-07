#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define PARTNER_POPUP_DONE 5
#define MENU_ENTERED_ESP 7
#define GAME1_ENTERED 8


//only print on change in state, fix hold
enum Menu {
  MAIN_MENU,
  IN_GAME,
};

Menu currentScreen;
Menu prevScreen;




void waitForEspBoot() {
  while (handleMessage() != PARTNER_POPUP_DONE) {
    // Serial.println("esp booted");
  }
  setVolume();
}


void setup() {
  UARTsetup();
  controlsSetup();
  setupRFID();
  bootupCardScan();
  waitForEspBoot();
  enterMainMenu();
}

void loop() {
  if (currentScreen == MAIN_MENU) {
    // while (handleMessage() != GAME1_ENTERED) {
    // unsigned long t = millis();
    setVolume();
    controlMainMenu();
    // Serial.print("after controlMainMenu: "); Serial.println(millis() - t);
    // unsigned long t = millis();
    uint8_t msg = handleMessage();
    if (msg == GAME1_ENTERED) {
      currentScreen = IN_GAME;
    }

    // Serial.print("after handleMessage: ");
    // Serial.println(millis() - t);
  }

  else if (currentScreen == IN_GAME) {
    playTetris();

    if (currentScreen == MAIN_MENU) {
      enterMenuMsg();
      while (handleMessage() != MENU_ENTERED_ESP) {}
      setVolume();
    }
  }
}
