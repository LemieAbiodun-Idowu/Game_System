#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

bool NOVALID_CARD = true;
void setupRFID() {
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  mfrc522.PCD_Init();  // Init MFRC522 card
  delay(50);           // give reader time to boot
  Serial.println("Card Initialised");
  display.display();
}
void cardScanning() {
  while (true) {
    if (!mfrc522.PICC_IsNewCardPresent()) continue;
    if (!mfrc522.PICC_ReadCardSerial()) continue;
    NOVALID_CARD = true;
    int j = 0;                                                              //To allow access outside of loop
    for (j; j < NUM_CARDS; j++) {                                           //Look through every storedcard
      bool CardMatch = true;                                                //Controls if bytes are matching are matching or not
      for (byte i = 0; i < mfrc522.uid.size; i++) {                         //Check each byte of the UID
        if (mfrc522.uid.uidByte[i] != pgm_read_byte(&(UID_Cards[j][i]))) {  //if not equal move onto next card
                                                                            // Serial.print("This Card doesnt match Card ");
                                                                            // Serial.println(j+1);
          CardMatch = false;
          break;
        }
        //   if (j == (NUM_CARDS - 1))  //if byte of final card then no cards are valid
        //     INVALID_CARD = true;
        // }
      }
      if (CardMatch) {
        NOVALID_CARD = false;
        break;
      }
    }
    if (NOVALID_CARD) {
      display.clearDisplay();
      drawText(F("ACCESS    DENIED              INVALID   CARD LMAO"), 0, 32);
      display.display();
      delay(2000);
    } else {
      display.clearDisplay();
      drawText(F("ACCESS GRANTED TO   CARD"), 0, 42);
      display.display();
      delay(5000);
      // mfrc522.PICC_HaltA();
      // mfrc522.PCD_StopCrypto1();
      digitalWrite(SS_PIN, HIGH);  //Turn rfid off
      break;
    }
  }
}