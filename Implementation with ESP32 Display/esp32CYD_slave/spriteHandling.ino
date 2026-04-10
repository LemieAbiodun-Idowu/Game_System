#define NUM_PARTNERS 8

uint16_t spriteBuffer[SPRITE_MAX_W * SPRITE_MAX_H];  //Preload Buffer to avoid using dynamic memory allocation to avoid heap fragmentation
int sprWidth, sprHeight;                             //must be read in as ints
int cachedsprWidth = 0;
int cachedsprHeight = 0;
extern uint8_t menuSpriteIndex;
#define PORTRAIT_BUF_MAX_W 40
#define PORTRAIT_BUF_MAX_H 40
#define PARTNER_NAME 50
#define SPRITE_ANIMATION_TYPE_MAX_LENGTH 10

#define DIM_BUFFER_MAX_LENGTH 8

uint8_t spriteNums;
const char DEF_DIRECTORY[] = "/Pokemon_data_for_CSD";
char uppercasedPartner[PARTNER_NAME];
char capitalisedPartner[PARTNER_NAME];
char spriteAnim[SPRITE_ANIMATION_TYPE_MAX_LENGTH];
char cachedspriteAnim[SPRITE_ANIMATION_TYPE_MAX_LENGTH] = "";
char filename[FILENAME_MAX_LENGTH];

// char myPartner[PARTNER_NAME];
// extern int cardNum;
extern int partnerInd;

// bool noSpriteChosen = true;
uint8_t spriteAnimType;


const char* Partners[NUM_PARTNERS] = {
  "vaporeon",
  "jolteon",
  "flareon",
  "espeon",
  "umbreon",
  "leafeon",
  "glaceon",
  "sylveon"
};


void startupPartnerPopup() {
  // sscanf(cardNum, "%d", &partnerInd);
  // myPartner = partners[partnerInd];
  displayPartnerMsg(Partners[partnerInd]);
  // Serial.println("Displayed partners msg");
  spriteDetails();
  // Serial.println("Found sprite details");
  displayStartupPartnerSprite(Partners[partnerInd], spriteAnim, partnerInd);
}

void menuPartnerPopup() {
  updateMenuPartnerSprite(Partners[partnerInd], spriteAnim, partnerInd);
}

void spriteDetails() {
  if (currentScreen == BOOTUP) {
    chooseSpriteAnim();
  } else if (currentScreen == MAIN_MENU) {
    nextSpriteAnim();
  }
  //findSpriteNum();
  // Serial.println(spriteAnimType);
  // Serial.println(spriteAnim);
  findSpriteSize();

  // Serial.println("Found Sprite Size");


  preloadFrame1();
}

void chooseSpriteAnim() {
  spriteAnimType = random(0, 3);
  // spriteAnimType = 0;
  switch (spriteAnimType) {
    case 0:
      if (strcmp(Partners[partnerInd], "umbreon") != 0) strcpy(spriteAnim, "idle");  //i forgot to rename umbreon's idle sprite and theres like 100+ .bin's for those i aint doing allat
      else strcpy(spriteAnim, "nod");
      break;
    case 1:
      strcpy(spriteAnim, "walk");
      break;
    case 2:
      strcpy(spriteAnim, "sleep");
      break;
  }
}

void nextSpriteAnim() {
  spriteAnimType = (spriteAnimType + 1) % 3;
  switch (spriteAnimType) {
    case 0:
      if (strcmp(Partners[partnerInd], "umbreon") != 0) strcpy(spriteAnim, "idle");  //i forgot to rename umbreon's idle sprite and theres like 100+ .bin's for those i aint doing allat
      else strcpy(spriteAnim, "nod");
      break;
    case 1:
      strcpy(spriteAnim, "walk");
      break;
    case 2:
      strcpy(spriteAnim, "sleep");
      break;
  }
}

void findSpriteSize() {
  if (strcmp(spriteAnim, cachedspriteAnim) == 0) {
    sprWidth = cachedsprWidth;
    sprHeight = cachedsprHeight;
    return;
  }
  snprintf(filename, sizeof(filename), "%s/%s_data/Sprites_CLR/%s_clr_%s/%s_clr_%s_size.txt", DEF_DIRECTORY,
           Partners[partnerInd], Partners[partnerInd], spriteAnim,
           Partners[partnerInd], spriteAnim);
  // Serial.println(filename);
  // Serial.println(sizeof(filename));
  File dimensionsFile = SD.open(filename, FILE_READ);
  if (!dimensionsFile) {
    Serial.println("Error opening File");
    return;
  }

  char dimBuffer[DIM_BUFFER_MAX_LENGTH];
  int dimlen = dimensionsFile.readBytes(dimBuffer, DIM_BUFFER_MAX_LENGTH - 1);
  dimBuffer[dimlen] = '\0';
  sscanf(dimBuffer, "%d,%d", &sprWidth, &sprHeight);
  // Serial.println(sprWidth);
  // Serial.println(sprHeight);
  dimensionsFile.close();
  if (sprWidth > SPRITE_MAX_W || sprHeight > SPRITE_MAX_H) {
    Serial.println("Sprite dimensions too big, sorry lil bro");
    return;
  }
  cachedsprWidth = sprWidth;
  cachedsprHeight = sprHeight;
  strcpy(cachedspriteAnim, spriteAnim);
}

bool spritePreloaded = false;
void preloadFrame1() {
  menuSpriteIndex = 1;
  snprintf(filename, sizeof(filename), "%s/%s_data/Sprites_CLR/%s_clr_%s/%s_clr_%s1_%s.bin",
           DEF_DIRECTORY, Partners[partnerInd], Partners[partnerInd], spriteAnim,
           Partners[partnerInd], spriteAnim, backgroundClrName);
  File sprite = SD.open(filename, FILE_READ);
  if (sprite) {
    sprite.read((uint8_t*)spriteBuffer, sprWidth * sprHeight * 2);
    sprite.close();
  }
  lastSpriteFrameTime = 0;  // force immediate display on next menuPartnerPopup call
  spritePreloaded = true;
}


void drawPortrait(uint8_t mood) {  //mood can be 1-5
  uint8_t portraitInd;
  if (rotated) portraitInd = 6;
  else {
    switch (mood) {
      case 1:  //very angry
        portraitInd = 5;
        break;
      case 2:  //slightly less angry
        portraitInd = 4;
        break;
      case 3:  //sad neutral
        portraitInd = 3;
        break;
      case 4:  //neutral
        portraitInd = 1;
        break;
      case 5:  //happy
        portraitInd = 2;
        break;
      default:
        Serial.println("drawPortrait: invalid mood value");
        return;
    }
  }


  snprintf(filename, sizeof(filename), "%s/%s_data/Sprites_CLR/%s_clr_portraits/%s_clr_portraits%d_%s.bin", DEF_DIRECTORY,
           Partners[partnerInd], Partners[partnerInd], Partners[partnerInd], portraitInd, backgroundClrName);
  //Serial.println(filename2);
  File portrait = SD.open(filename, FILE_READ);
  if (!portrait) {
    Serial.println("Failed to open bin");
    return;
  }
  portrait.read((uint8_t*)spriteBuffer, PORTRAIT_BUF_MAX_W * PORTRAIT_BUF_MAX_H * 2);
  portrait.close();

  // uint16_t x_displ = (SCREEN_HEIGHT - sprWidth) * 0.5;
  // uint16_t y_displ = (SCREEN_WIDTH - sprHeight);
  tft.pushImage(USER_X, USER_Y, PORTRAIT_BUF_MAX_W, PORTRAIT_BUF_MAX_H, spriteBuffer);
  Serial.println(mood);
  Serial.println(portraitInd);
}
