#include <Arduino.h>

const int RledPin = 9;
const int GledPin = 10;
const int BledPin = 11;
const int buttonPin = 2;

int mood = 0;
const int neutralMood = 10;

bool ButtonPressed = false;
bool rainbowMode = false;
bool buttonHandled = false;

unsigned long touchedTimer = 0;
unsigned long reducedTimer = 0;
unsigned long rainbowTimer = 0;

const long unTouchInterval = 5000;   // 超过5秒未触摸开始变伤心
const long reducedInterval = 1000;   // 每1秒心情下降1
const long rainbowDuration = 4000;   // 彩虹模式持续4秒

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  mood = neutralMood;
}

void loop() {
  unsigned long currentTimer = millis();
  int buttonState = digitalRead(buttonPin);

  // ---------------- 按钮按下检测 ----------------
  if (buttonState == HIGH && !ButtonPressed) {
    ButtonPressed = true;
    buttonHandled = false; // 新按下按钮，准备处理

    if (!buttonHandled) {
      if (!rainbowMode) {
        if (mood >= 15) {
          rainbowMode = true;
          rainbowTimer = currentTimer;
        } else {
          mood++;
          if (mood > 20) mood = 20;
          touchedTimer = currentTimer;
        }
      }
      buttonHandled = true; // 本次按下处理完成
    }
  }

  // 按钮松开
  if (buttonState == LOW && ButtonPressed) {
    ButtonPressed = false;
  }

  // ---------------- 彩虹模式计时 ----------------
  if (rainbowMode && currentTimer - rainbowTimer >= rainbowDuration) {
    rainbowMode = false;
    mood = 15; // 回到红色
  }

  // ---------------- 自动心情下降 ----------------
  if (!rainbowMode && (currentTimer - touchedTimer > unTouchInterval)) {
    if (currentTimer - reducedTimer > reducedInterval) {
      mood--;
      if (mood < 0) mood = 0;
      reducedTimer = currentTimer;
    }
  }

  // ---------------- 显示 LED ----------------
  if (rainbowMode) {
    showRainbow(currentTimer);
  } else {
    showLEDState(mood);
  }
}

// ==================== 心情颜色 ====================
void showLEDState(int state) {
  int r, g, b;

  if (state == 0) {
    // 紫色慢闪
    int blink = (millis() / 500) % 2;
    r = blink ? 128 : 0;
    g = 0;
    b = blink ? 128 : 0;
  } 
  else if (state <= 5) {
    // 蓝 → 绿
    r = 0;
    g = map(state, 0, 5, 0, 255);
    b = map(state, 0, 5, 255, 0);
  } 
  else if (state <= 10) {
    // 绿 → 黄
    r = map(state, 5, 10, 0, 255);
    g = 255;
    b = 0;
  } 
  else if (state <= 15) {
    // 黄 → 红
    r = 255;
    g = map(state, 10, 15, 255, 0);
    b = 0;
  } 
  else {
    // 红（最开心）
    r = 255;
    g = 0;
    b = 0;
  }

  // 共阳极反转输出
  analogWrite(RledPin, 255 - r);
  analogWrite(GledPin, 255 - g);
  analogWrite(BledPin, 255 - b);
}

// ==================== 彩虹模式（兴奋闪烁） ====================
void showRainbow(unsigned long t) {
  float phase = (t % 1000) / 1000.0;  // 每1秒循环一次颜色
  float h = phase * 360.0;
  float s = 1.0;

  // 快速闪烁亮度
  float brightness = 0.5 + 0.5 * sin(2 * PI * t / 200); // 约5Hz明显闪烁
  float v = brightness;

  float C = v * s;
  float X = C * (1 - abs(fmod(h / 60.0, 2) - 1));
  float m = v - C;
  float r1, g1, b1;

  if (h < 60) { r1 = C; g1 = X; b1 = 0; }
  else if (h < 120) { r1 = X; g1 = C; b1 = 0; }
  else if (h < 180) { r1 = 0; g1 = C; b1 = X; }
  else if (h < 240) { r1 = 0; g1 = X; b1 = C; }
  else if (h < 300) { r1 = X; g1 = 0; b1 = C; }
  else { r1 = C; g1 = 0; b1 = X; }

  int r = (r1 + m) * 255;
  int g = (g1 + m) * 255;
  int b = (b1 + m) * 255;

  analogWrite(RledPin, 255 - r);
  analogWrite(GledPin, 255 - g);
  analogWrite(BledPin, 255 - b);
}
