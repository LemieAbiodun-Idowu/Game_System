#include <XPT2046_Touchscreen.h>
#include "LittleFS.h"  //Necessary for File storing in Flash for Arduino 2
#include <FS.h>        //Necessary for File storing in Flash for Arduino 2
#include <TFT_eSPI.h>
#include "SD.h"  //Using SD cards

void setup() {
  Serial.begin(115200);
  touchscreensetup();
  displaysetup();
  UARTsetup();
  //cardScanMsg();
  drawGameLayout();
}

void loop(){
  ListenToArduino();
}