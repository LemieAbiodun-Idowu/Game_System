#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setupRFID() {
  SPI.begin();         // <-- CRITICAL FIX: Start the SPI bus!
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);
  mfrc522.PCD_Init();  // Init MFRC522 card
  delay(50);           // give reader time to boot
}

void cardScanning() {
  while (true) {
    if (!mfrc522.PICC_IsNewCardPresent()) continue;
    if (!mfrc522.PICC_ReadCardSerial()) continue;

    bool matchFound = false;
    int matchedIndex = -1;

    // Loop through our new PROGMEM array
    for (uint8_t j = 0; j < NUM_CARDS; j++) {
      bool cardMatch = true;
      
      for (uint8_t i = 0; i < mfrc522.uid.size; i++) {
        // We MUST use pgm_read_byte to get the data out of the Flash "Closet"
        if (mfrc522.uid.uidByte[i] != pgm_read_byte(&(UID_Cards[j][i]))) {
          cardMatch = false;
          break;
        }
      }
      
      if (cardMatch) {
        matchFound = true;
        matchedIndex = j;
        break;
      }
    }

    if (!matchFound) {
      failedCardMsg();
      
      // Stop reading the same invalid card repeatedly to avoid spamming the ESP32
      mfrc522.PICC_HaltA(); 
      delay(2000); // Give the user time to see the "ACCESS DENIED" screen on the ESP32
      
    } else {
      passedCardMsg(matchedIndex + 1);
      
      // Halt the card reader before jumping into the Tetris game
      mfrc522.PICC_HaltA(); 
      return; // Exit loop and start game
    }
  }
}

void checkPowerUpCard() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  int matchedIndex = -1;
  for (uint8_t j = 0; j < NUM_CARDS; j++) {
    bool cardMatch = true;
    for (uint8_t i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] != pgm_read_byte(&(UID_Cards[j][i]))) {
        cardMatch = false;
        break;
      }
    }
    if (cardMatch) {
      matchedIndex = j + 1;
      break;
    }
  }

  if (matchedIndex == 2) { //card 2 is double points
    activateDoublePoints();
  }

  if (matchedIndex == 3) { //card 3 is clear line
    clearBottomThreeLines();
    Serial.println("POWERUP:CLEAR");
  }

  if (matchedIndex == 4) { // card 4 is slow gravity
    activateSlowGravity();
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}