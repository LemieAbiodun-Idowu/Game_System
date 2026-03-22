//Software serial is dangerous to use on Esp32
HardwareSerial mySerial(2);
#define MAX_MSG_SIZE 100

void UARTsetup() {
  mySerial.begin(115200, SERIAL_8N1, 35);
  // RX = 35, TX = 22
  while (!mySerial.available()) {
    //Splash Screen
    displayDefMsg();
  }
}

//You need a level shift of 3.3V from Rx on the Display to Tx on the Arduino (2kOhm or 6.8kOhm to ground) (1kOhm or 4.7kOhm with Tx on uno)

//Arduino will send the uid and based on that a different animation and portrait will be played.
//Optimise loading the bitmaps will be necessary
//Will need to change the stored cry durations

void cardScanMsg() {
  char cardmsg[MAX_MSG_SIZE];
  int len = mySerial.readBytesUntil('\n', cardmsg, MAX_MSG_SIZE - 1);
  cardmsg[len] = '\0';
  if (strcmp(cardmsg, "INVALID_CRD") == 0) {
    cardMsgDisplay(0);  //negative message
  } else {
    char* ptr = strstr(cardmsg, "CRD:");
    if (ptr != NULL) {
      ptr += 4;                // skip past "CRD:"
      cardMsgDisplay(1, ptr);  // ptr now points to just the number
    }
  }
}