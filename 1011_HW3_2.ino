// --- HW3-2 第5題 ---
// LED 切換閃爍模式

const int buttonPin = 2;   // 按鈕腳位
const int ledPin = 3;      // LED腳位

int buttonState = 0;       // 當前按鈕狀態
int lastButtonState = 0;   // 前一次按鈕狀態
int mode = 0;              // LED模式 0-恆亮,1-慢閃,2-中速,3-快閃

unsigned long previousMillis = 0;   // 記錄上次閃爍時間
unsigned long interval = 1000;      // 閃爍間隔（預設慢閃）

void setup() {
  pinMode(buttonPin, INPUT);   // 或 INPUT_PULLUP
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // 初始恆亮
}

void loop() {
  buttonState = digitalRead(buttonPin);

  // 偵測按鈕由 LOW->HIGH（單次按下）
  if (buttonState == HIGH && lastButtonState == LOW) {
    mode++;                    // 切換模式
    if (mode > 3) mode = 0;    // 超過3回到0
    // 設定不同模式的閃爍間隔
    switch(mode) {
      case 0: digitalWrite(ledPin, HIGH); break; // 恆亮
      case 1: interval = 1000; break;            // 慢閃 (1秒)
      case 2: interval = 500; break;             // 中速閃 (0.5秒)
      case 3: interval = 200; break;             // 快閃 (0.2秒)
    }
    delay(50); // 按鈕消抖
  }
  lastButtonState = buttonState;

  // 控制閃爍 (非阻塞)
  if (mode != 0) { // 閃爍模式
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      int ledState = digitalRead(ledPin);
      digitalWrite(ledPin, !ledState); // 反轉LED
    }
  }
}