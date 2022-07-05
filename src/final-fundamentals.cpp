/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/maxwell/CTD-IoT/d5/final-fundamentals/src/final-fundamentals.ino"
/*
 * Project final-fundamentals
 * Description:
 * Author:
 * Date:
 */
//#define BLYNK
void normalState();
void mainRun(int reading);
void setup();
void loop();
#line 8 "/home/maxwell/CTD-IoT/d5/final-fundamentals/src/final-fundamentals.ino"
#ifndef BLYNK
SYSTEM_MODE(MANUAL);
#endif
#include "Wire.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include "oled-wing-adafruit.h"
#include "blynk.h"

#define GREEN D6
#define BLUE D7
#define RED D8
#define POT A4
#define BUTTON A5
#define TMP36 A0

VCNL4040 proximitySensor;
OledWingAdafruit display;

int currentState = 0;
int minValue = 0;
int maxValue = 0;

BLYNK_WRITE (V0)
{
  currentState = 3;
}

void normalState() {
  double reading = analogRead(TMP36);
  double millivolts = (reading / 4095) * 3300; // I looked up the formula before you shared it with the class lol
  double temp = (millivolts - 500) / 10;
  double fahrenheit = (temp * 1.8) + 32;
  display.print("temp: ");
  display.println(fahrenheit);
  //unsigned int proxValue = proximitySensor.getProximity();
  unsigned int lightValue = proximitySensor.getAmbient();
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  if (lightValue < minValue) {
    digitalWrite(BLUE, HIGH);
  } else if (lightValue < maxValue) {
    digitalWrite(GREEN, HIGH);
  } else {
    digitalWrite(RED, HIGH);
  }

}

void mainRun(int reading) {
  switch (currentState) {
    case 0:
      display.println("select min brightness");
      display.print("current value: ");
      display.println(reading);
      display.print("current brightness: ");
      display.println(proximitySensor.getAmbient());
      if (digitalRead(BUTTON)) {
        currentState++;
        minValue = reading;
        delay(500ms);
      }
      break;
    case 1:
      display.println("select max brightness");
      display.print("current value: ");
      display.println(reading);
      display.print("current brightness: ");
      display.println(proximitySensor.getAmbient());
      if (digitalRead(BUTTON)) {
        currentState++;
        maxValue = reading;
        delay(500ms);
      }
      break;
    case 2:
      normalState();
      break;
    case 3:
      display.setTextSize(5);
      display.println("Hell");
  }
}

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Wire.begin();
  Wire.setClock(400000);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(POT, INPUT);
  pinMode(BUTTON, INPUT_PULLDOWN);
  pinMode(TMP36, INPUT);
  #ifdef BLYNK
  Blynk.begin("QnAwxbXEtUAXJ_pHkObTkMagiLke5zMZ", IPAddress(167, 172, 234, 162), 8080);
  #endif
  display.setup();
  proximitySensor.begin();
  proximitySensor.powerOnAmbient();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  //
  #ifdef BLYNK
  Blynk.run();
  #endif
  display.loop();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(WHITE);
  int mappedRead = map(analogRead(POT), 0, 4095, 0, 65535);
  mainRun(mappedRead);
  display.display();
}