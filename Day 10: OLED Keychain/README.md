# OLED Robo Keychain

A small ESP32 project that turns a 128x64 I2C OLED into an animated robot-eye keychain companion. It idles with lifelike eye movement, shows the time and date on a quick tap, and reacts to being petted on a long touch.

## What it does

- Displays animated robot eyes (roaming gaze, blinking, random mood changes) when idle
- Quick tap on the touch sensor shows the current time and date for a few seconds, styled with a border, day name, and a sun/moon icon based on the hour
- Long touch (petting) locks the eyes forward, sets a happy mood, and loops a laugh animation for as long as it's held
- Releasing after a long touch plays a short angry, then upset, reaction before returning to normal idle behavior

## Components used

- ESP32-C3 Super Mini
- 0.96" OLED display, SSD1306 driver, 128x64, I2C
- TTP223 capacitive touch sensor module

## Wiring

| Pin | ESP32-C3 Super Mini |
|---|---|
| OLED VCC | 3.3V |
| OLED GND | G |
| OLED SDA | GPIO8 |
| OLED SCL | GPIO9 |
| Touch VCC | 3.3V |
| Touch GND | G |
| Touch OUT | GPIO0 |

## Libraries required

Install via Arduino IDE Library Manager:
- **Adafruit SSD1306**
- **Adafruit GFX Library** (installs automatically as a dependency)
- **FluxGarage RoboEyes**

Board package: `esp32 by Espressif Systems` (via Boards Manager). Requires **Tools > USB CDC On Boot > Enabled** for Serial Monitor to work on the C3's native USB port.

## Circuit

![Breadboard circuit](keychain.png)

## How to use

1. Upload `desk_buddy.ino` to the ESP32-C3.
2. Set the current date and time in `setManualTime()` before uploading. There's no RTC yet, so this resets on every reupload or power loss.
3. Power on, the eyes start idling immediately.
4. Quick tap the touch sensor to check the time.
5. Hold the touch sensor down to pet it.

## Known limitations

- Time is hardcoded per upload with no RTC module yet, so it resets to whatever was typed in the code on every reflash or power cycle.
- Still on a breadboard, not yet soldered into the keychain body.
- Only one touch zone, so tap and hold are the only two gestures available.

## Possible future improvements

- Add a DS3231 RTC module so time keeps running independently of code uploads or power loss.
- Add a LiPo battery and charging circuit to make it fully portable.
- Move the whole build onto a soldered perfboard, housed as a small keychain.
- Replace the "pet to laugh" long-touch action with something more useful day to day, current plan is a live prayer-time status readout.
