#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>
#include "time.h"
#include <sys/time.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
RoboEyes eyes(display);

#define TOUCH_PIN 0
bool touchActive = false;
unsigned long touchStartTime = 0;
bool showingTime = false;
unsigned long timeDisplayStart = 0;

unsigned long lastLaughTrigger = 0;
const unsigned long LAUGH_RETRIGGER_MS = 400;
const unsigned long QUICK_TAP_LIMIT = 600;
bool isPetting = false;

enum ReleaseState { NONE, ANGRY_PHASE, UPSET_PHASE, DONE };
ReleaseState releaseState = NONE;
unsigned long releasePhaseStart = 0;
const unsigned long ANGRY_DURATION = 700;
const unsigned long UPSET_DURATION = 700;

unsigned long lastMoodChange = 0;
unsigned long nextMoodInterval = 4000;

void setIdleRoaming(bool on) {
  eyes.setIdleMode(on ? ON : OFF, 2, 2);
}

// TEMPORARY: manual time set on every upload (until DS3231 is added)
void setManualTime(int year, int month, int day, int hour, int minute, int second) {
  struct tm t = {0};
  t.tm_year = year - 1900;
  t.tm_mon = month - 1;
  t.tm_mday = day;
  t.tm_hour = hour;
  t.tm_min = minute;
  t.tm_sec = second;
  time_t timeSinceEpoch = mktime(&t);
  struct timeval now = { .tv_sec = timeSinceEpoch };
  settimeofday(&now, NULL);
}

void drawSunIcon(int x, int y) {
  display.fillCircle(x, y, 6, SSD1306_WHITE);
  for (int i = 0; i < 8; i++) {
    float angle = i * 45 * PI / 180;
    int x1 = x + cos(angle) * 9;
    int y1 = y + sin(angle) * 9;
    int x2 = x + cos(angle) * 12;
    int y2 = y + sin(angle) * 12;
    display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
  }
}

void drawMoonIcon(int x, int y) {
  display.fillCircle(x, y, 7, SSD1306_WHITE);
  display.fillCircle(x + 4, y - 2, 6, SSD1306_BLACK);
}

void showTimeDate() {
  time_t now;
  time(&now);
  struct tm* timeinfo = localtime(&now);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.drawRoundRect(0, 0, 128, 64, 6, SSD1306_WHITE);

  char dayStr[10];
  strftime(dayStr, sizeof(dayStr), "%A", timeinfo);
  display.setTextSize(1);
  display.setCursor(8, 6);
  display.println(dayStr);

  bool isDay = (timeinfo->tm_hour >= 6 && timeinfo->tm_hour < 18);
  if (isDay) drawSunIcon(112, 12);
  else drawMoonIcon(112, 12);

  char timeStr[6];
  strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);
  display.setTextSize(3);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 24);
  display.println(timeStr);

  if (timeinfo->tm_sec % 2 == 0) {
    display.fillCircle(122, 30, 2, SSD1306_WHITE);
  }

  display.drawFastHLine(14, 50, 100, SSD1306_WHITE);

  char dateStr[16];
  strftime(dateStr, sizeof(dateStr), "%d %b %Y", timeinfo);
  display.setTextSize(1);
  display.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((128 - w) / 2, 54);
  display.println(dateStr);

  display.display();
}

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);
  Wire.begin(8, 9);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (true);
  }

  eyes.begin(128, 64, 100);
  eyes.setAutoblinker(ON, 3, 2);
  setIdleRoaming(true);
  eyes.setMood(DEFAULT);

  // SET YOUR CURRENT DATE/TIME HERE (24hr format), until DS3231 replaces this:
  setManualTime(2026, 9, 27, 19, 45, 0);

  randomSeed(analogRead(2));
}

void loop() {
  bool touchNow = digitalRead(TOUCH_PIN) == HIGH;
  unsigned long now = millis();

  if (touchNow && !touchActive) {
    touchActive = true;
    touchStartTime = now;
    lastLaughTrigger = 0;
    releaseState = NONE;
  }

  if (touchNow && touchActive) {
    unsigned long heldFor = now - touchStartTime;
    if (heldFor >= QUICK_TAP_LIMIT && !isPetting) {
      isPetting = true;
      setIdleRoaming(false);
    }
    if (isPetting) {
      if (now - lastLaughTrigger >= LAUGH_RETRIGGER_MS) {
        eyes.setMood(HAPPY);
        eyes.anim_laugh();
        lastLaughTrigger = now;
      }
    }
  }

  if (!touchNow && touchActive) {
    unsigned long heldFor = now - touchStartTime;
    touchActive = false;

    if (heldFor < QUICK_TAP_LIMIT) {
      showingTime = true;
      timeDisplayStart = now;
    } else {
      isPetting = false;
      releaseState = ANGRY_PHASE;
      releasePhaseStart = now;
      eyes.setMood(ANGRY);
    }
  }

  if (releaseState == ANGRY_PHASE && now - releasePhaseStart >= ANGRY_DURATION) {
    releaseState = UPSET_PHASE;
    releasePhaseStart = now;
    eyes.setMood(TIRED);
  } else if (releaseState == UPSET_PHASE && now - releasePhaseStart >= UPSET_DURATION) {
    releaseState = DONE;
    eyes.setMood(DEFAULT);
    setIdleRoaming(true);
  }

  bool fullyIdle = (!touchActive && releaseState == NONE || releaseState == DONE);
  if (fullyIdle && !showingTime) {
    if (now - lastMoodChange > nextMoodInterval) {
      lastMoodChange = now;
      nextMoodInterval = random(3000, 7000);
      int r = random(0, 4);
      switch (r) {
        case 0: eyes.setMood(HAPPY); break;
        case 1: eyes.setMood(ANGRY); break;
        case 2: eyes.setMood(TIRED); break;
        case 3: eyes.setMood(DEFAULT); break;
      }
    }
  }

  if (showingTime) {
    showTimeDate();
    if (millis() - timeDisplayStart > 3000) {
      showingTime = false;
    }
  } else {
    eyes.update();
  }
}
