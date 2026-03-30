HardwareSerial mySerial(2);
int nextEspType = 0; 
int activeEspType = 0;
int activeEspRotation = 0;
int activeEspX = 0;

  // Set baud to 38400 (stable) and INCREASE buffer to 512 bytes
void UARTsetup() {
  mySerial.setRxBufferSize(512);
  mySerial.begin(38400, SERIAL_8N1, 35, 22);
  displayDefMsg();
}

// Run this constantly in the ESP32 loop()
void listenToArduino() {
  if (mySerial.available()) {
    String data = mySerial.readStringUntil('\n');
    data.trim(); // Clean up hidden newlines

    // Route the data - If it's random noise, it safely ignores it!
    if (data.startsWith("G:")) {
      updateTetrisGrid(data.substring(2));
    } 
    else if (data.startsWith("S:")) {
      updateScore(data.substring(2));
    } 
    else if (data.startsWith("N:")) {
      nextEspType = data.substring(2).toInt();
      drawNextPiece(nextEspType);
    }
    else if (data.startsWith("H:")) {
      drawHoldPiece(data.substring(2).toInt());
    } 
    else if (data == "STATE:OVER") {
      showGameOver();
    } 
    else if (data == "STATE:PAUSE") {
      showPause();
    } 
    else if (data == "STATE:PLAY") {
      resumeGame();
    } 
    else if (data == "INVALID_CRD") {
      cardMsgDisplay(0);
    } 
    else if (data.startsWith("CRD:")) {
      String cardNum = data.substring(4);
      cardMsgDisplay(1, (char*)cardNum.c_str());
    }
    // else if (data.startsWith("GY:")) {
    //   int ghostY = data.substring(3).toInt();
    //   int8_t gPiece[2][4];
    //   copyEspPiece(gPiece, activeEspType, activeEspRotation);
    //   updateGhostPiece(ghostY, activeEspX, gPiece);
    // }
    else if (data.startsWith("CT:")) {
      activeEspType = data.substring(3).toInt();
    }
    else if (data.startsWith("CR:")) {
      activeEspRotation = data.substring(3).toInt();
    }
    else if (data.startsWith("CX:")) {
      activeEspX = data.substring(3).toInt();
    }
    else if (data.startsWith("LV:")) {
      updateLevel(data.substring(3).toInt());
    }
    else if (data == "POWERUP:CLEAR") {
      showClearLineMsg();
    }
    else if (data == "POWERUP:DOUBLE") {
      showDoublePointsMsg();
    }
    else if (data == "POWERUP:SLOW") {
      showSlowGravityMsg();
    }
  }
}