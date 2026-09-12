# Step by Step Process

## What it does
Monitors temperature and humidity using a sensor and displays the readings on an OLED screen. A buzzer alerts when temperature crosses a set threshold.

## Components used
- Arduino Uno
- DHT11 Temperature & Humidity Sensor
- OLED display
- Breadboard and jumper wires

## How to build
1. Connect the sensor to the Arduino (VCC with 5V, GND with GND, data pin with Digital pin 2)
2. Connect the OLED display (GND with GND, VCC with 5V, SCL with A5, SDA with A4)
3. Download DHT sensor Library by Adafruit, Adafruit SSD1306, Adafruit GFX Library in your Arduino IDE software from Library manager
4. Upload the code from `temp._humi.ino` this folder or copy paste it
5. Power the Arduino and check the OLED for live readings 
## It Should Look Like this
![Circuit Diagram](Screenshot (490).png)


## Code
See `temp._humi.ino` in this folder.

## Simulation
Try it live on Wokwi: [Temperature and Humidity Monitor Simulation](https://wokwi.com/projects/474955029538443265)
