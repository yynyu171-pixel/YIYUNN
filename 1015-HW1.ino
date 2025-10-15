// 引脚定义
const int buttonPin = 2;
const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

// 模式与状态
int mode = 0; // 0: 固定顏色, 1: 呼吸, 2: 彩虹
bool isOff = false;

// 按钮处理变量
bool lastButtonState = HIGH;
unsigned long buttonPressTime = 0;
bool longPressDetected = false;
const unsigned long longPressDuration = 1500; // 1.5 秒

// 呼吸模式变量
int brightness = 0;
int fadeAmount = 5;
unsigned long lastFadeTime = 0;

// 彩虹变量
int rainbowIndex = 1;
unsigned long lastRainbowTime = 0;

// 顏色設定
int currentColor = 1; // 1~7
int colorPresets[8][3] = {
  {0, 0, 0}, // 0: 關
  {1, 0, 0}, // 1: 紅
  {0, 1, 0}, // 2: 綠
  {0, 0, 1}, // 3: 藍
  {1, 1, 0}, // 4: 黃
  {1, 0, 1}, // 5: 品紅
  {0, 1, 1}, // 6: 青
  {1, 1, 1}  // 7: 白
};

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  handleButton();

  if (isOff) {
    setColor(0, 0, 0);
    return;
  }

  if (mode == 0) {
    displayColor(currentColor, 255);
  } else if (mode == 1) {
    handleBreathing();
  } else if (mode == 2) {
    handleRainbow();
  }
}

// 短按切模式，長按關閉
void handleButton() {
  bool currentState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && currentState == LOW) {
    // 按下
    buttonPressTime = millis();
    longPressDetected = false;
  }

  if (currentState == LOW && !longPressDetected &&
      millis() - buttonPressTime > longPressDuration) {
    // 長按：關閉/開啟 LED
    isOff = !isOff;
    Serial.println(isOff ? "LED Off" : "LED On");
    longPressDetected = true;
  }

  if (lastButtonState == LOW && currentState == HIGH) {
    // 放開按鈕
    if (!longPressDetected) {
      // 短按：切換模式
      mode = (mode + 1) % 3;
      Serial.print("Mode: ");
      Serial.println(mode);
      if (mode == 0) {
        currentColor = (currentColor % 7) + 1;
      }
    }
  }

  lastButtonState = currentState;
}

// 設定顏色
void setColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

void displayColor(int index, int brightness) {
  int r = colorPresets[index][0] * brightness;
  int g = colorPresets[index][1] * brightness;
  int b = colorPresets[index][2] * brightness;
  setColor(r, g, b);
}

// 呼吸燈
void handleBreathing() {
  if (millis() - lastFadeTime >= 30) {
    brightness += fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    displayColor(currentColor, brightness);
    lastFadeTime = millis();
  }
}

// 彩虹燈
void handleRainbow() {
  if (millis() - lastRainbowTime >= 400) {
    rainbowIndex = (rainbowIndex % 7) + 1;
    displayColor(rainbowIndex, 255);
    lastRainbowTime = millis();
  }
}
