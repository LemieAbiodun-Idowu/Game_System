HardwareSerial mySerial(2);

void UARTsetup() {
  // Listen to pins 35 (RX) and 22 (TX)
  mySerial.begin(115200, SERIAL_8N1, 35, 22);

  // Draw the splash screen message ONCE and move on
  displayDefMsg();
  
  // REMOVED: The while(!mySerial.available()) trap loop is gone!
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
      drawNextPiece(data.substring(2).toInt());
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
      cardMsgDisplay(1, (char*)data.substring(4).c_str());
    }
  }
}