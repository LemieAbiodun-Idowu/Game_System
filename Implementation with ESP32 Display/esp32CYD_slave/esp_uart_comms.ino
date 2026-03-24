HardwareSerial mySerial(2);

void UARTsetup() {
  // Set baud to 38400 (stable) and INCREASE buffer to 512 bytes
  mySerial.begin(38400, SERIAL_8N1, 35, 22);
  mySerial.setRxBufferSize(512); 

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