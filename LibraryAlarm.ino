#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <avr/wdt.h>  // Include the watchdog timer library

SoftwareSerial mySoftwareSerial(10, 11);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the I2C address, and 16x2 display

int num_Measure = 128;  // Set the number of measurements
int pinSignal = A3;     // pin connected to the sound sensor
long Sound_signal;      // Store the value read from the sound sensor
long sum = 0;           // Store the total value of n measurements
long level = 0;         // Store the average value
int soundlow = 100;
int soundmedium = 400;
bool soundPlaying = false;

void setup() {
  pinMode(pinSignal, INPUT);  // Set the signal pin as input
  Serial.begin(9600);
  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the backlight

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Unable to begin DFPlayer. Check your connections and SD card."));
    lcd.setCursor(0, 0);
    lcd.print("cannot read sd card");
    while (true)
      ;
  }
  lcd.print("Sound Monitoring System");
  delay(5000);

  lcd.clear();
  Serial.println(F("DFPlayer Mini online."));
  lcd.setCursor(0, 0);
  lcd.print("Wait....");
  myDFPlayer.volume(30);  // Set volume value (0-30).
  delay(5000);
}

void loop() {
  if (!soundPlaying) {
    sum = 0;
    for (int i = 0; i < num_Measure; i++) {
      Sound_signal = analogRead(pinSignal);
      sum += Sound_signal;
    }
    level = sum / num_Measure;

    lcd.setCursor(0, 1);
    lcd.print("Sound Level: ");
    lcd.println(level);
    Serial.print("Sound Level: ");
    Serial.println(level);

    lcd.setCursor(0, 0);

    if (level < soundlow) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Intensity= Low");
      lcd.setCursor(0, 1);
      lcd.print("Sound Level: ");
      lcd.print(level);
    } else if (level > soundlow && level < soundmedium) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Intensity= Medium");
      lcd.setCursor(0, 1);
      lcd.print("Sound Level: ");
      lcd.print(level);
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Intensity= High");
      lcd.setCursor(0, 1);
      lcd.print("Sound Level: ");
      lcd.print(level);
      // Rest of your code


      myDFPlayer.play(1);  // Replace "1" with the index of the sound file you want to play
      soundPlaying = true;
    }
  } else {
    // Check if sound is still playing
    if (soundPlaying = true) {
      delay(5000);
      wdt_enable(WDTO_15MS);  // Enable watchdog timer with a 15ms timeout (shortest available)
      while (1)
        ;  // Wait for the watchdog timer to reset the Arduino
    }
  }
  delay(100);
}
