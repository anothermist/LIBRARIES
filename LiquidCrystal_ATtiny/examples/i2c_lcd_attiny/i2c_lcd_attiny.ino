/*
 * Interactive nametag sketch for ATtiny85. Based on the Digispark
 * ATtiny LCD example and the Adafruit "Measuring Sound Levels" example.
 * Every 12000 milliseconds will change the display from my name to my website.
 * On the second line, will measure and display sound levels.
 * For more info check: https://platis.solutions/blog/2015/03/22/diy-interactive-name-tag/
 * ATtiny85 I2C pins:
 * ATtiny Pin 5 = SDA on DS1621  & GPIO
 * ATtiny Pin 7 = SCK on DS1621  & GPIO
 */

#include <TinyWireM.h>                  // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_attiny.h>          // for LCD w/ GPIO MODIFIED for the ATtiny85

#define GPIO_ADDR     0x27
LiquidCrystal_I2C lcd(GPIO_ADDR, 16, 2); // set address & 16 chars / 2 lines

const short micPin = A3;
const short sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
const unsigned long nameInterval = 12000;
unsigned long prevNameChange = 0;
short state = 1;
unsigned int sample;

void setup() {
  lcd.init();                           // initialize the lcd
  lcd.backlight();                      // Print a message to the LCD.
  lcd.print("Name: Dimitris");
}

void loop() {
  printName();
  printTemp();
}

void printName() {
  if (millis() - prevNameChange > nameInterval) {
    lcd.setCursor(0, 0);
    if (state) {
      lcd.print("platis.solutions");
    } else {
      lcd.print("Name: Dimitris  ");
    }
    prevNameChange = millis();
    state = !state;
  }
}

void printTemp()
{
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(micPin);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  printBars(peakToPeak);
}

void printBars(int bars) {
  if (bars > 14) bars = 14;
  for (int i = 0; i < 15; i++) {
    lcd.setCursor(i, 1);
    if (i <= bars) {
      lcd.print("*");
    } else {
      lcd.print(" ");
    }
  }
}
