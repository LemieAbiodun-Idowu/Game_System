#include "audio.h"
unsigned long clearLineMsgStart = 0;
unsigned long clearLineFlashTimer = 0;
bool showingClearLineMsg = false;
bool clearLineMsgVisible = false;


unsigned long doublePointsMsgStart = 0;
unsigned long doublePointsFlashTimer = 0;
bool showingDoublePointsMsg = false;
bool doublePointsMsgVisible = false;

unsigned long slowGravityMsgStart = 0;
unsigned long slowGravityFlashTimer = 0;
bool showingSlowGravityMsg = false;
bool slowGravityMsgVisible = false;

void showDoublePointsMsg() {
  doublePointsMsgStart = millis();
  doublePointsFlashTimer = millis();
  showingDoublePointsMsg = true;
  doublePointsMsgVisible = false;
}

void showClearLineMsg() {
  clearLineMsgStart = millis();
  clearLineFlashTimer = millis();
  showingClearLineMsg = true;
  clearLineMsgVisible = false;
}

void clearDoublePointsMsg() {
  if (!showingDoublePointsMsg) return;

  if (millis() - doublePointsMsgStart >= 10000) {
    tft.setTextColor(TFT_BLACK, TFT_BLACK);
    tft.drawCentreString("2X POINTS!", SCORE_X + 41, SCORE_Y + 57, 2);
    showingDoublePointsMsg = false;
    return;
  }

  if (millis() - doublePointsFlashTimer >= 150) {
    doublePointsFlashTimer = millis();
    if (doublePointsMsgVisible) {
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawCentreString("2X POINTS!", SCORE_X + 41, SCORE_Y + 57, 2);
    } else {
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.drawCentreString("2X POINTS!", SCORE_X + 41, SCORE_Y + 57, 2);
    }
    doublePointsMsgVisible = !doublePointsMsgVisible;
  }
}

void clearClearLineMsg() {
  if (!showingClearLineMsg) return;

  // Stop after 1 second total
  if (millis() - clearLineMsgStart >= 1000) {
    tft.setTextColor(TFT_BLACK, TFT_BLACK);
    tft.drawCentreString("LINE CLEAR!", SCORE_X + 41, SCORE_Y + 57, 2);
    showingClearLineMsg = false;
    return;
  }

  // Flash every 150ms
  if (millis() - clearLineFlashTimer >= 150) {
    clearLineFlashTimer = millis();
    if (clearLineMsgVisible) {
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawCentreString("LINE CLEAR!", SCORE_X + 41, SCORE_Y + 57, 2);
    } else {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.drawCentreString("LINE CLEAR!", SCORE_X + 41, SCORE_Y + 57, 2);
    }
    clearLineMsgVisible = !clearLineMsgVisible;
  }
}

void showSlowGravityMsg() {
  slowGravityMsgStart = millis();
  slowGravityFlashTimer = millis();
  showingSlowGravityMsg = true;
  slowGravityMsgVisible = false;
}

void clearSlowGravityMsg() {
  if (!showingSlowGravityMsg) return;

  if (millis() - slowGravityMsgStart >= 10000) {
    tft.setTextColor(TFT_BLACK, TFT_BLACK);
    tft.drawCentreString("SLOW MODE!", SCORE_X + 41, SCORE_Y + 57, 2);
    showingSlowGravityMsg = false;
    return;
  }

  if (millis() - slowGravityFlashTimer >= 150) {
    slowGravityFlashTimer = millis();
    if (slowGravityMsgVisible) {
      tft.setTextColor(TFT_BLACK, TFT_BLACK);
      tft.drawCentreString("SLOW MODE!", SCORE_X + 41, SCORE_Y + 57, 2);
    } else {
      tft.setTextColor(TFT_CYAN, TFT_BLACK);
      tft.drawCentreString("SLOW MODE!", SCORE_X + 41, SCORE_Y + 57, 2);
    }
    slowGravityMsgVisible = !slowGravityMsgVisible;
  }
}

void clearPowerUpMsg() {
  clearDoublePointsMsg();
  clearClearLineMsg();
  clearSlowGravityMsg();
}
bool rotated = false;
bool rotationPending = false;
unsigned long rotationStart = 0;

void triggerRotationEffect() {
  rotationPending = true;  // don't rotate yet, wait for unpause
}

void applyRotationIfPending() {
  if (!rotationPending) return;
  rotationPending = false;
  rotated = !rotated;
  tft.setRotation(rotated ? 0 : 2);
  rotationStart = millis();
  drawGameLayout();
  memset(prevGrid, 0, sizeof(prevGrid));
}
extern uint8_t mood;
void checkRotationExpiry() {
  if (!rotated) return;
  if (millis() - rotationStart >= ROTATION_DURATION) {
    rotated = false;
    tft.setRotation(2);
    drawGameLayout();
    drawPortrait(mood);
    memset(prevGrid, 0, sizeof(prevGrid));
  }
}