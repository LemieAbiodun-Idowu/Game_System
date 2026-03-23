HardwareSerial mySerial(2);

void UARTsetup() {
  mySerial.begin(115200, SERIAL_8N1, 35, 22);
  
  // Wait on the splash screen until Arduino sends first data
  while (!mySerial.available()) {
    displayDefMsg(); 
  }
}

// Run this constantly in the ESP32 loop()
void listenToArduino() {
  if (mySerial.available()) {
    String data = mySerial.readStringUntil('\n');
    data.trim();

    // Route the data
    if (data.startsWith("G:")) updateTetrisGrid(data.substring(2));
    else if (data.startsWith("S:")) updateScore(data.substring(2));
    else if (data.startsWith("N:")) drawNextPiece(data.substring(2).toInt());
    else if (data.startsWith("H:")) drawHoldPiece(data.substring(2).toInt());
    else if (data == "STATE:OVER") showGameOver();
    else if (data == "STATE:PAUSE") showPause();
    else if (data == "STATE:PLAY") resumeGame();
    else if (data == "INVALID_CRD") cardMsgDisplay(0);
    else if (data.startsWith("CRD:")) cardMsgDisplay(1, (char*)data.substring(4).c_str());
  }
}


// //Software serial is dangerous to use on Esp32
// HardwareSerial mySerial(2);
// #define MAX_MSG_SIZE 100

// void UARTsetup() {
//   mySerial.begin(115200, SERIAL_8N1, 35, 22);
//   // RX = 35, TX = 22
//   while (!mySerial.available()) {
//     //Splash Screen
//     displayDefMsg();
//   }
// }

// //You need a level shift of 3.3V from Rx on the Display to Tx on the Arduino (2kOhm or 6.8kOhm to ground) (1kOhm or 4.7kOhm with Tx on uno)
// //Arduino will send the uid and based on that a different animation and portrait will be played.
// //Optimise loading the bitmaps will be necessary
// //Will need to change the stored cry durations

// void cardScanMsg() {
//   char cardmsg[MAX_MSG_SIZE];
//   int len = mySerial.readBytesUntil('\n', cardmsg, MAX_MSG_SIZE - 1);
//   cardmsg[len] = '\0';
//   if (strcmp(cardmsg, "INVALID_CRD") == 0) {
//     cardMsgDisplay(0);  //negative message
//   } else {
//     char* ptr = strstr(cardmsg, "CRD:");
//     if (ptr != NULL) {
//       ptr += 4;                // skip past "CRD:"
//       cardMsgDisplay(1, ptr);  // ptr now points to just the number
//     }
//   }
// }

