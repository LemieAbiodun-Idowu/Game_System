#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);  //MFRC522 instance

extern int currentTheme;  // Reference the theme variable from TetrisLogic
extern bool currentThemeUpdated;
uint8_t profileCard;
uint8_t matchingCard;
bool invalidCardSerial;
bool noCardPresent;
bool NOVALID_CARD;


void setupRFID() {
  SPI.begin();
  // pinMode(SS_PIN, OUTPUT);
  // digitalWrite(SS_PIN, HIGH);
  mfrc522.PCD_Init();  // Init MFRC522 PCD
  delay(50);           // give reader time to boot
}

void cardScanning() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    noCardPresent = true;
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    invalidCardSerial = true;
    return;
  }
  noCardPresent = false;
  invalidCardSerial = false;

  NOVALID_CARD = true;
  matchingCard = 0;  //To allow access outside of loop

  for (matchingCard = 0; matchingCard < NUM_CARDS; matchingCard++) {  //Look through every stored card
    bool CardMatch = true;

    for (byte i = 0; i < mfrc522.uid.size; i++) {  //Check each byte of the UID

      if (mfrc522.uid.uidByte[i] != pgm_read_byte(&(UID_Cards[matchingCard][i]))) {  //if not equal move onto next card
        CardMatch = false;
        break;
      }
    }

    if (CardMatch) {
      NOVALID_CARD = false;
      matchingCard++;
      break;
    }
  }
}

void bootupCardScan() {
  while (true) {

    cardScanning();
    if (invalidCardSerial || noCardPresent) continue;

    if (NOVALID_CARD) {
      failedCardMsg(0);
      mfrc522.PICC_HaltA();
    } else {
      profileCard = matchingCard;
      passedCardMsg(profileCard);
      mfrc522.PICC_HaltA();
      return;
    }
  }
}



bool checkCardEffect() {
  cardScanning();
  if (invalidCardSerial || noCardPresent) return false;
  mfrc522.PICC_HaltA();
  switch (matchingCard) {
    case 1:  //monochrome
      break;
    case 2:
      activateDoublePoints();
      cardEffectMsg(matchingCard);
      break;

    case 3:
      clearBottomThreeLines();
      cardEffectMsg(matchingCard);
      //Serial.println("POWERUP:CLEAR");
      break;

    case 4:
      activateSlowGravity();
      cardEffectMsg(matchingCard);
      break;

    case 5:  // Neon Theme
      if (currentTheme == 1) {
        currentTheme = 0;
      } else currentTheme = 1;
      currentThemeUpdated = true;
      cardEffectMsg(matchingCard);
      refreshGrid();
      currentThemeUpdated = false;
      break;

    case 6:  // Gameboy Theme
      if (currentTheme == 2) {
        currentTheme = 0;
      } else currentTheme = 2;
      currentThemeUpdated = true;
      cardEffectMsg(matchingCard);
      refreshGrid();
      currentThemeUpdated = false;
      break;

    // case 7: { // Card 7 resets to Default Theme      //No, scanning the same card should reset to default theme
    //   currentTheme = 0;
    //   refreshGrid();
    // }
    case 8:  //Rotate
      break;
    default:
      failedCardMsg(1);
      return false;
  }
  return true;
}