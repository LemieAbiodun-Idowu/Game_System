/*  Rui Santos & Sara Santos - Random Nerd Tutorials
    THIS EXAMPLE WAS TESTED WITH THE FOLLOWING HARDWARE:
    1) ESP32-2432S028R 2.8 inch 240×320 also known as the Cheap Yellow Display (CYD): https://makeradvisor.com/tools/cyd-cheap-yellow-display-esp32-2432s028r/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/cyd/
    2) REGULAR ESP32 Dev Board + 2.8 inch 240x320 TFT Display: https://makeradvisor.com/tools/2-8-inch-ili9341-tft-240x320/ and https://makeradvisor.com/tools/esp32-dev-board-wi-fi-bluetooth/
      SET UP INSTRUCTIONS: https://RandomNerdTutorials.com/esp32-tft/
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <SPI.h>

/*  Install the "TFT_eSPI" library by Bodmer to interface with the TFT Display - https://github.com/Bodmer/TFT_eSPI
    *** IMPORTANT: User_Setup.h available on the internet will probably NOT work with the examples available at Random Nerd Tutorials ***
    *** YOU MUST USE THE User_Setup.h FILE PROVIDED IN THE LINK BELOW IN ORDER TO USE THE EXAMPLES FROM RANDOM NERD TUTORIALS ***
    FULL INSTRUCTIONS AVAILABLE ON HOW CONFIGURE THE LIBRARY: https://RandomNerdTutorials.com/cyd/ or https://RandomNerdTutorials.com/esp32-tft/   */
#include <TFT_eSPI.h>

// Install the "XPT2046_Touchscreen" library by Paul Stoffregen to use the Touchscreen - https://github.com/PaulStoffregen/XPT2046_Touchscreen
// Note: this library doesn't require further configuration
#include <XPT2046_Touchscreen.h>
#include "LittleFS.h"     //Necessary for File storing in Flash for Arduino 2
#include <FS.h>           //Necessary for File storing in Flash for Arduino 2
using fs::File;           

TFT_eSPI tft = TFT_eSPI();

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

// #define RED_LED 4    //LED TESTs
// #define GREEN_LED 16
// #define BLUE_LED 17

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define FONT_SIZE 2

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

// Size for the largest sprites (Largest Size is actually 32x64 but made slightly bigger just in case)
#define SPRITE_BUF_MAX_W 64
#define SPRITE_BUF_MAX_H 64
static uint16_t spriteBuffer[SPRITE_BUF_MAX_W * SPRITE_BUF_MAX_H];  //Preload Buffer to avoid using dynamic memory allocation to avoid heap fragmentation


// // Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
// void printTouchToSerial(int touchX, int touchY, int touchZ) {
//   Serial.print("X = ");
//   Serial.print(touchX);
//   Serial.print(" | Y = ");
//   Serial.print(touchY);
//   Serial.print(" | Pressure = ");
//   Serial.print(touchZ);
//   Serial.println();
// }

// // Print Touchscreen info about X, Y and Pressure (Z) on the TFT Display
// void printTouchToDisplay(int touchX, int touchY, int touchZ) {
//   // Clear TFT screen
//   tft.fillScreen(TFT_WHITE);
//   tft.setTextColor(TFT_BLACK, TFT_WHITE);


//   int centerX = SCREEN_HEIGHT / 2;
//   int textY = 80;

//   String tempText = "X = " + String(touchX);
//   tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

//   textY += 20;
//   tempText = "Y = " + String(touchY);
//   tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);

//   textY += 20;
//   tempText = "Pressure = " + String(touchZ);
//   tft.drawCentreString(tempText, centerX, textY, FONT_SIZE);
// }

void setup() {
  Serial.begin(115200);


  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
  touchscreen.setRotation(2);
  // pinMode(RED_LED, OUTPUT);
  // pinMode(GREEN_LED, OUTPUT);
  // pinMode(BLUE_LED, OUTPUT);
  // digitalWrite(RED_LED, HIGH);
  // digitalWrite(GREEN_LED, HIGH);
  // digitalWrite(BLUE_LED, HIGH);
  // Start the tft display
  tft.init();
  // Set the TFT display rotation in landscape mode
  tft.setRotation(2);
  tft.setSwapBytes(true); //Mistakenly assumed in the RGB565 converter that display wanted in little endian, so need to make this swap, this can be optimised in later versions
  LittleFS.begin(); 
  // Clear the screen before writing to it
  tft.fillScreen(TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  File dim = LittleFS.open("/laughing_cat.txt", "r"); //I store a .txt file that reads the size of the image in the format "w,h"
  if (!dim) {
    Serial.println("Failed to open dimension file");
    return;
  }
  String buf1 = dim.readString(); //This section can definitely be cleaner but idk
  int w, h;
  sscanf(buf1.c_str(), "%d,%d", &w, &h);
  //test with fscanf - file reading func
  dim.close();
  uint16_t* buffer = (uint16_t*)malloc(w * h * 2);  //In actual implementation dont dynamically allocate memory
  File img = LittleFS.open("/laughing_cat.bin", "r");
  if (!img) {
    Serial.println("Failed to open binary image");
    return;
  }
  img.read((uint8_t*)buffer, w * h * 2);    
  img.close();
  // Set X and Y coordinates for center of display
  int centerX = (SCREEN_HEIGHT - w) / 2;
  int centerY = (SCREEN_WIDTH - h) / 2;

  tft.pushImage(centerX, centerY, w, h, buffer);
  free(buffer);
  // tft.drawCentreString("Hello, world!", centerX, 30, FONT_SIZE);
  // tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);
  delay(5000);
}

void loop() {
  tft.fillScreen(TFT_WHITE);
  for (int i = 0; i < 16; i++) {
    char filename1[100];
    snprintf(filename1, sizeof(filename1), "/vaporeon_clr_idle%d_white.txt", i + 1);
    Serial.println(filename1);
    File dim = LittleFS.open(filename1, "r");
    if (!dim) {
      Serial.println("Failed to open dimension file");
      return;
    }
    String buf1 = dim.readString();
    int w, h;
    sscanf(buf1.c_str(), "%d,%d", &w, &h);
    dim.close();
    if(w > SPRITE_BUF_MAX_W || h > SPRITE_BUF_MAX_H){
      Serial.println("Sprite dimensions too big, sorry lil bro");
      return;
    }
    //uint16_t* buffer;
    char filename2[100];
    sprintf(filename2, "/vaporeon_clr_idle%d_white.bin", i + 1);
    Serial.println(filename2);
    File img = LittleFS.open(filename2, "r");
    if (!img) {
      Serial.println("Failed to open bin img");
      return;
    }
    img.read((uint8_t*)spriteBuffer, w * h * 2);
    img.close();
    // Set X and Y coordinates for center of display
    int centerX = (SCREEN_HEIGHT - w) / 2;
    int centerY = (SCREEN_WIDTH - h) / 2;

    tft.pushImage(centerX, centerY, w, h, spriteBuffer);
    //free(spriteBuffer);
    delay(300);
  }


  delay(2000);
}




// void setup() {
//   Serial.begin(115200);

//   // Start the SPI for the touchscreen and init the touchscreen
//   touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
//   touchscreen.begin(touchscreenSPI);
//   // Set the Touchscreen rotation in landscape mode
//   // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 3: touchscreen.setRotation(3);
//   touchscreen.setRotation(1);

//   // Start the tft display
//   tft.init();
//   // Set the TFT display rotation in landscape mode
//   tft.setRotation(1);

//   // Clear the screen before writing to it
//   tft.fillScreen(TFT_WHITE);
//   tft.setTextColor(TFT_BLACK, TFT_WHITE);
  
//   // Set X and Y coordinates for center of display
//   int centerX = SCREEN_WIDTH / 2;
//   int centerY = SCREEN_HEIGHT / 2;

//   tft.drawCentreString("Hello, world!", centerX, 30, FONT_SIZE);
//   tft.drawCentreString("Touch screen to test", centerX, centerY, FONT_SIZE);
// }

// void loop() {
//   // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z) info on the TFT display and Serial Monitor
//   if (touchscreen.tirqTouched() && touchscreen.touched()) {
//     // Get Touchscreen points
//     TS_Point p = touchscreen.getPoint();
//     // Calibrate Touchscreen points with map function to the correct width and height
//     x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
//     y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
//     z = p.z;

//     printTouchToSerial(x, y, z);
//     printTouchToDisplay(x, y, z);

//     delay(100);
//   }
// }