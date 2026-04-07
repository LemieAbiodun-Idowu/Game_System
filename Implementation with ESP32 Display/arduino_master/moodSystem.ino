#define MOOD_UPDATE_INTERVAL 3000
#define MOOD_HISTORY_WINDOW 10000

unsigned long lastMoodUpdate = 0;
uint8_t currentMood = 50;  //start at neutral

unsigned long lastMoodScore = 0;
unsigned long lastMoodScoreTime = 0;

extern unsigned long score;
extern uint8_t grid[10][25];
extern bool isGameOver, isPaused;
extern int comboCount;

uint8_t calculateMood() {


  //check how quick score increases
  // float scoreRate = (float)scoreDelta / (timeDelta / 1000.0f);
  // float scoreSignal = constrain(scoreRate / 200.0f, 0.0f, 1.0f);

  // stack height signal — empty board = 1.0, full = 0.0
  int highestFilledRow = 24;
  for (int y = 0; y < 25; y++) {
    for (int x = 0; x < 10; x++) {
      if (grid[x][y]) {
        highestFilledRow = y;
        goto foundTop;
      }
    }
  }
foundTop:
  float stackSignal = (float)highestFilledRow / 24.0f;

  // blend — weights sum to 1.0
  float rawMood = (stackSignal);

  // smooth toward target
  uint8_t targetMood = (uint8_t)(rawMood * 100.0f);
  // int8_t delta = (int8_t)targetMood - (int8_t)currentMood;
  currentMood = targetMood;



  return currentMood;
}

void updateMood() {
  if (millis() - lastMoodUpdate < MOOD_UPDATE_INTERVAL) return;
  lastMoodUpdate = millis();

  if (isGameOver || isPaused) return;  // freeze mood on pause/gameover

  currentMood = calculateMood();

  sendMoodMsg(currentMood);
}