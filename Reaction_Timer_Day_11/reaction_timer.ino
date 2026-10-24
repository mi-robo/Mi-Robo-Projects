// Reaction Timer: ESP32 + SSD1306 OLED + LED + button + TTP223 touch
// Libraries: Adafruit SSD1306, Adafruit GFX

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LED_PIN    27   // LED + 220 ohm to GND
#define BTN_PIN    32   // button to GND (INPUT_PULLUP)
#define TOUCH_PIN  33   // TTP223 OUT (active HIGH)
#define SDA_PIN    26
#define SCL_PIN    25

Adafruit_SSD1306 display(128, 64, &Wire, -1);

float bestMs = 0;  // 0 = no best yet

bool pressed() {
  return digitalRead(BTN_PIN) == LOW || digitalRead(TOUCH_PIN) == HIGH;
}

void waitRelease() {
  while (pressed()) delay(10);
  delay(80);
}

void screen(const char* top, const char* mid, uint8_t midSize) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(top);
  display.setTextSize(midSize);
  display.setCursor(0, 22);
  display.print(mid);
  display.setTextSize(1);
  display.setCursor(0, 56);
  if (bestMs > 0) {
    display.print("Best: ");
    display.print(bestMs, 1);
    display.print(" ms");
  }
  display.display();
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(TOUCH_PIN, INPUT_PULLDOWN);
  Wire.begin(SDA_PIN, SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void loop() {
  // IDLE
  digitalWrite(LED_PIN, LOW);
  screen("REACTION TIMER", "Press to\nstart", 2);
  while (!pressed()) delay(5);
  waitRelease();

  // WAIT for random delay (2 to 5 s)
  screen("Get ready...", "Wait...", 2);
  uint32_t waitMs = 2000 + (esp_random() % 3000);
  uint32_t t0 = millis();
  bool early = false;
  while (millis() - t0 < waitMs) {
    if (pressed()) { early = true; break; }
    delay(2);
  }

  if (early) {
    screen("FOUL!", "Too early", 2);
    waitRelease();
    delay(1500);
    return;
  }

  // GO
  digitalWrite(LED_PIN, HIGH);
  uint32_t start = micros();
  while (!pressed()) { /* tight loop for accuracy */ }
  uint32_t elapsed = micros() - start;
  digitalWrite(LED_PIN, LOW);

  float ms = elapsed / 1000.0;
  bool record = (bestMs == 0 || ms < bestMs);
  if (record) bestMs = ms;

  char buf[16];
  snprintf(buf, sizeof(buf), "%.1f ms", ms);
  screen(record ? "NEW BEST!" : "Your time:", buf, 2);
  waitRelease();
  delay(2500);
}