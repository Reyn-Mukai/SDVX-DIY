/*
This code is written for the SDVX DIY tutorial at https://sdvx-diy.github.io/

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "./src/Encoder/Encoder.h"
#include <Keyboard.h>
#include <Mouse.h>

/* Button Input Pins */
#define BTA_PIN A2
#define BTB_PIN A3
#define BTC_PIN A4
#define BTD_PIN A5
#define FxL_PIN A0
#define FxR_PIN A1
#define START_PIN 7

/* LED Output Pins */
#define BTA_LED 9
#define BTB_LED 10
#define BTC_LED 11
#define BTD_LED 12
#define FxL_LED 5
#define FxR_LED 6
#define START_LED 13

/* Software Debounce Interval */
#define DEBOUNCE 10

/* Encoder */
#define ENC_1_PIN_A 0
#define ENC_1_PIN_B 1
#define ENC_2_PIN_A 2
#define ENC_2_PIN_B 3

/* Encoder */
Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);

/* Button */
unsigned int buttonPin[7] = {BTA_PIN, BTB_PIN, BTC_PIN, BTD_PIN, FxL_PIN, FxR_PIN, START_PIN};
unsigned long keyTimer[7] = {0, 0, 0, 0, 0, 0, 0};
bool buttonState[7];
bool switchType[7] = {true, true, true, true, true, true, true};
char asciiKey[7] = {0x64, 0x66, 0x6A, 0x6B, 0x6D, 0x63, 0x31};

/* Lighting */
unsigned int ledPin[7] = {BTA_LED, BTB_LED, BTC_LED, BTD_LED, FxL_LED, FxR_LED, START_LED};

/* Startup Loop */
void setup() {
  Keyboard.begin();
  Mouse.begin();
  pinMode(BTA_PIN, INPUT_PULLUP);
  pinMode(BTB_PIN, INPUT_PULLUP);
  pinMode(BTC_PIN, INPUT_PULLUP);
  pinMode(BTD_PIN, INPUT_PULLUP);
  pinMode(FxL_PIN, INPUT_PULLUP);
  pinMode(FxR_PIN, INPUT_PULLUP);
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(FxL_LED, OUTPUT);
  pinMode(FxR_LED, OUTPUT);
  pinMode(BTA_LED, OUTPUT);
  pinMode(BTB_LED, OUTPUT);
  pinMode(BTC_LED, OUTPUT);
  pinMode(BTD_LED, OUTPUT);
  pinMode(START_LED, OUTPUT);
}

/* Main Loop */
void loop() {
  checkAllKeyEvents();
  encFuncLeft();
  encFuncRight();
}

void checkAllKeyEvents(){
  for(int i = 0; i < sizeof(buttonPin) / 2; i++){
    if(switchType[i] == true){
      if(digitalRead(buttonPin[i]) == LOW && buttonState[i] == false){
        Keyboard.press(asciiKey[i]);
        digitalWrite(ledPin[i], HIGH);
        buttonState[i] = true;
        keyTimer[i] = millis();
      }
      else if(digitalRead(buttonPin[i]) == HIGH && buttonState[i] == true && millis() - keyTimer[i] > DEBOUNCE){
        Keyboard.release(asciiKey[i]);
        digitalWrite(ledPin[i], LOW);
        buttonState[i] = false;
      }
    }
    else{
      if(digitalRead(buttonPin[i]) == HIGH && buttonState[i] == false){
        Keyboard.press(asciiKey[i]);
        digitalWrite(ledPin[i], HIGH);
        buttonState[i] = true;
        keyTimer[i] = millis();
      }
      else if(digitalRead(buttonPin[i]) == LOW && buttonState[i] == true && millis() - keyTimer[i] > DEBOUNCE){
        Keyboard.release(asciiKey[i]);
        digitalWrite(ledPin[i], LOW);
        buttonState[i] = false;
      }
    }
  }
}

void encFuncLeft(){
  updateMousePositionLeft();
  encLeft.write(0);
}

void encFuncRight(){
  updateMousePositionRight();
  encRight.write(0);
}

void updateMousePositionLeft(){
  Mouse.move(encLeft.read(), 0, 0);
}

void updateMousePositionRight(){
  Mouse.move(0, encRight.read(), 0);
}

