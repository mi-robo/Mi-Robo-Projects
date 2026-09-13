#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

struct LyricLine {
  unsigned long time;
  const char* text;
  int size;
};

LyricLine lyrics[] = {
  {0,     "",                     3},
  {500,   "Ami Vabte Parini",     2},
  {2600,  "Tumiii",               3},
  {3300,  "Buker  Vetor",         3},
  {4500,  "FATCHOO",              3},
  {5233,  "Amaaar",               3},
  {5633,  "Shorir Juure",         3},
  {7067,  "Tomar  Premer",        3},
  {8300,  "Beeeej",               3},
  {10033, "",                     3},
  {10400, "Ami Thamte Parini",    2},
  {12467, "Tomar  Galee",         3},
  {13733, "Norom  Dukkho",        3},
  {15133, "Amaaay",               3},
  {15600, "Duhat  diyee",         3},
  {16700, "Muchte dioo",          3},
  {18167, "Plea~se",              3}
};

int totalLines = 17;
int currentLine = 0;

unsigned long outroStartTime = 20000;
unsigned long songStartTime;
unsigned long blinkTimer = 0;
bool subscribeVisible = true;

void setup() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
  oled.setTextColor(SSD1306_WHITE);

  showReady();
  delay(2000);

  runCountdown();
  songStartTime = millis();
}

void loop() {
  unsigned long elapsed = millis() - songStartTime;

  if (currentLine < totalLines && elapsed >= lyrics[currentLine].time) {
    oled.clearDisplay();
    oled.setTextSize(lyrics[currentLine].size);

    if (lyrics[currentLine].size == 2) {
      printCentered(lyrics[currentLine].text, 2, 25); // vertically placed mid-screen too
    } else {
      oled.setCursor(0, 0);
      oled.print(lyrics[currentLine].text);
    }

    oled.display();
    currentLine++;
  }

  if (currentLine >= totalLines && elapsed >= outroStartTime) {
    animateOutro();
  }
}

// Calculates horizontal center based on character count, then prints there
void printCentered(const char* text, int size, int y) {
  int charWidth = 6 * size; // each character is roughly 6px wide per size multiplier
  int textWidth = strlen(text) * charWidth;
  int x = (SCREEN_WIDTH - textWidth) / 2;
  if (x < 0) x = 0; // safety clamp if text is too wide to fully center

  oled.setCursor(x, y);
  oled.print(text);
}

void animateOutro() {
  if (millis() - blinkTimer >= 500) {
    blinkTimer = millis();
    subscribeVisible = !subscribeVisible;

    oled.clearDisplay();
    oled.setTextSize(2);
    oled.setCursor(15, 5);
    oled.print("See you");
    oled.setCursor(10, 25);
    oled.print("in Day 7");

    if (subscribeVisible) {
      oled.setCursor(5, 48);
      oled.print("SUBSCRIBE");
    }

    oled.display();
  }
}

void showReady() {
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setCursor(0, 0);
  oled.print("Ready");
  oled.display();
}

void runCountdown() {
  int nums[] = {3, 2, 1};
  for (int i = 0; i < 3; i++) {
    oled.clearDisplay();
    oled.setTextSize(4);
    oled.setCursor(56, 24);
    oled.print(nums[i]);
    oled.display();
    delay(1000);
  }
  oled.clearDisplay();
  oled.display();
}