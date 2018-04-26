#include "./src/NeoPixel/Adafruit_NeoPixel.h"
#include "./src/Encoder/Encoder.h"
#include <Keyboard.h>
#include <Mouse.h>

/* Button Input Pins */
#define BTA_PIN A2
#define BTB_PIN A3
#define BTC_PIN A4
#define BTD_PIN A5
#define FxL_PIN A1
#define FxR_PIN A0
#define START_PIN 7

/* LED Output Pins */
#define BTA_LED 9
#define BTB_LED 10
#define BTC_LED 11
#define BTD_LED 12
#define FxL_LED 6
#define FxR_LED 5
#define START_LED 13

/* Software Debounce Interval */
#define DEBOUNCE 10

/* NeoPixel */
#define SPIN 4
#define RPIN 8
#define NUM_LEDS_STRIP 48
#define NUM_LEDS_RING1 48
#define MAX_STRIP_LEVEL 200
#define MAX_RING_LEVEL 150
#define DEGEN_FADE_INTERVAL 2

/* Encoder */
#define ENC_1_PIN_A 0
#define ENC_1_PIN_B 1
#define ENC_2_PIN_A 2
#define ENC_2_PIN_B 3

/* NeoPixel */
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS_STRIP, SPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_LEDS_RING1, RPIN, NEO_GRB + NEO_KHZ800);

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

  /* Pixel Sectors
   * side lighting 0-11
   * top left 12-14
   * start 15-17
   * top right 18-20
   * right side 21,34-36,47
   * btd 22,23,32,33
   * btc 23,24,31,32
   * btb 24,25,30,31
   * bta 25,26,29,30
   * left side 27,28,41-43
   * fxr 37,38
   * fxl 39,40
   * bottom 44-46
   */

/* Startup Loop */
void setup() {
  Keyboard.begin();
  Mouse.begin();
  strip.begin();
  strip.setBrightness(MAX_STRIP_LEVEL);
  ring.begin();
  ring.setBrightness(MAX_RING_LEVEL);
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
  for(int i = 0; i < NUM_LEDS_STRIP; i++){
    strip.setPixelColor(i, strip.Color(255, 50, 50));
  }
  strip.show();
  for(int i = 0; i < NUM_LEDS_STRIP; i++){
    ring.setPixelColor(i, ring.Color(0, 0, 0));
  }
  ring.show();
}

/* Main Loop */
void loop() {
  //checkAllKeyEvents();
  checkSingleKeyEvent(0);
  checkSingleKeyEvent(1);
  checkSingleKeyEvent(2);
  checkSingleKeyEvent(3);
  checkSingleKeyEvent(4);
  checkSingleKeyEvent(5);
  checkSingleKeyEvent(6);
  encFuncLeft();
  encFuncRight();
  degenFadeLeft();
  degenFadeRight();
}

void checkSingleKeyEvent(int i){
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

void degenFadeLeft(){
  if(millis() - fadeTimerLeft >= DEGEN_FADE_INTERVAL && fadeLevelLeft > 0){
    for(int i = 24; i < 48; i++){
      ring.setPixelColor(i, ring.Color(0, 0, fadeLevelLeft));
    }
    fadeLevelLeft--;
    fadeTimerLeft = millis();
  }
  ring.show();
}
void degenFadeRight(){
  if(millis() - fadeTimerRight >= DEGEN_FADE_INTERVAL && fadeLevelRight > 0){
    for(int i = 0; i < 24; i++){
      ring.setPixelColor(i, ring.Color(fadeLevelRight, 0, 0));
    }
    fadeLevelRight--;
    fadeTimerRight = millis();
  }
  ring.show();
}

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
  if(encLeft.read() != 0 && fadeLevelLeft < MAX_RING_LEVEL) fadeLevelLeft+=2;
}

void updateDegenFadeRight(){
  if(encRight.read() != 0 && fadeLevelRight < MAX_RING_LEVEL) fadeLevelRight+=2;
}

void updateMousePositionLeft(){
  Mouse.move(encLeft.read(), 0, 0);
}

void updateMousePositionRight(){
  Mouse.move(0, encRight.read(), 0);
}

