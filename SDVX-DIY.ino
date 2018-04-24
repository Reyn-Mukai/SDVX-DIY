#include <Keyboard.h>
#include <Mouse.h>
#include "./src/NeoPixel/Adafruit_NeoPixel.h"
#include "./src/Encoder/Encoder.h"

/* Buttons */
#define BTA_PIN A2
#define BTB_PIN A3
#define BTC_PIN A4
#define BTD_PIN A5
#define FxL_PIN A0
#define FxR_PIN A1
#define START_PIN SCK
#define BTA_LED 9
#define BTB_LED 10
#define BTC_LED 11
#define BTD_LED 12
#define FxL_LED 5
#define FxR_LED 6
#define START_LED 13
#define DEBOUNCE 10

/* NeoPixel */
#define SPIN 4
#define R1PIN 8
#define NUM_LEDS_STRIP 48
#define NUM_LEDS_RING1 48
#define MAX_LEVEL 150
#define DEGEN_FADE_INTERVAL 2

/* Encoder */
#define ENC_1_PIN_A 0
#define ENC_1_PIN_B 1
#define ENC_2_PIN_A 2
#define ENC_2_PIN_B 3

/* NeoPixel */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS_STRIP, SPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_LEDS_RING1, R1PIN, NEO_GRB + NEO_KHZ800);

/* Encoder */
Encoder encLeft(ENC_1_PIN_A, ENC_1_PIN_B);
Encoder encRight(ENC_2_PIN_A, ENC_2_PIN_B);

/* Button */
unsigned int buttonPin[7] = {BTA_PIN, BTB_PIN, BTC_PIN, BTD_PIN, FxL_PIN, FxR_PIN, START_PIN};
unsigned long keyTimer[7] = {0, 0, 0, 0, 0, 0, 0};
bool state[7] = {false, false, false, false, false, false, false};
bool switchType[7] = {true, true, true, true, false, false, true};
char asciiKey[7] = {0x64, 0x66, 0x6A, 0x6B, 0x6D, 0x63, 0x31};

/* Lighting */
unsigned int fadeLevelLeft = 180;
unsigned int fadeLevelRight = 180;
unsigned long fadeTimerLeft = 0;
unsigned long fadeTimerRight = 0;
unsigned int ledPin[7] = {BTA_LED, BTB_LED, BTC_LED, BTD_LED, FxL_LED, FxR_LED, START_LED};


/* Startup Loop */
void setup() {
  Keyboard.begin();
  Mouse.begin();
  strip.begin();
  strip.setBrightness(MAX_LEVEL);
  ring.begin();
  ring.setBrightness(MAX_LEVEL);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(SCK, INPUT_PULLUP);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  for(int i = 0; i < NUM_LEDS_STRIP; i++){
    strip.setPixelColor(i, strip.Color(255, 50, 50));
  }
  strip.show();
  for(int i = 0; i < NUM_LEDS_STRIP; i++){
    ring.setPixelColor(i, ring.Color(255, 50, 50));
  }
  ring.show();
}

/* Main Loop */
void loop() {
  checkAllKeyEvents();
  //encFuncLeft();
  //encFuncRight();
}

void checkAllKeyEvents(){
  for(int i = 0; i < sizeof(buttonPin); i++){
    if(switchType[i] == true){
      if(digitalRead(buttonPin[i]) == LOW && state[i] == false){
        Keyboard.press(asciiKey[i]);
        digitalWrite(ledPin[i], HIGH);
        state[i] = true;
        keyTimer[i] = millis();
      }
      else if(digitalRead(buttonPin[i]) == HIGH && state[i] == true && millis() - keyTimer[i] > DEBOUNCE){
        Keyboard.release(asciiKey[i]);
        digitalWrite(ledPin[i], LOW);
        state[i] = false;
      }
    }
    else{
      if(digitalRead(buttonPin[i]) == HIGH && state[i] == false){
        Keyboard.press(asciiKey[i]);
        digitalWrite(ledPin[i], HIGH);
        state[i] = true;
        keyTimer[i] = millis();
      }
      else if(digitalRead(buttonPin[i]) == LOW && state[i] == true && millis() - keyTimer[i] > DEBOUNCE){
        Keyboard.release(asciiKey[i]);
        digitalWrite(ledPin[i], LOW);
        state[i] = false;
      }
    }
  }
}
/*
void degenFadeLeft(){
  if(millis() - fadeTimerLeft >= DEGEN_FADE_INTERVAL && fadeLevelLeft > 0){
    for(int i = 0; i < MAX_LEVEL; i++){
      ring1.setPixelColor(i, ring1.Color(0, 0, fadeLevelLeft));
    }
    fadeLevelLeft--;
    fadeTimerLeft = millis();
  }
  ring.show();
}
void degenFadeRight(){
  if(millis() - fadeTimerRight >= DEGEN_FADE_INTERVAL && fadeLevelRight > 0){
    for(int i = 0; i < MAX_LEVEL; i++){
      ring2.setPixelColor(i, ring2.Color(fadeLevelRight, 0, 0));
    }
    fadeLevelRight--;
    fadeTimerRight = millis();
  }
  ring.show();
}
*/
void encFuncLeft(){
  updateDegenFadeLeft();
  updateMousePositionLeft();
  encLeft.write(0);
}

void encFuncRight(){
  updateDegenFadeRight();
  updateMousePositionRight();
  encRight.write(0);
}

void updateDegenFadeLeft(){
  if(encLeft.read() != 0 && fadeLevelLeft < MAX_LEVEL) fadeLevelLeft++;
}

void updateDegenFadeRight(){
  if(encRight.read() != 0 && fadeLevelRight < MAX_LEVEL) fadeLevelRight++;
}

void updateMousePositionLeft(){
  Mouse.move(encLeft.read(), 0, 0);
}

void updateMousePositionRight(){
  Mouse.move(0, encRight.read(), 0);
}

