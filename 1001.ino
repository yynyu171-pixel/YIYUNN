const int buttonPin = 2;
const int RledPin = 3;                         
const int GledPin = 4;
const int BledPin = 5;

int buttonState = 0;
int ledcolor = 0;
bool ButtonPressed = false;
String currentcolor="led";

void setup() {
  // put your setup code here, to run once:
pinMode(RledPin,OUTPUT);
pinMode(GledPin,OUTPUT);
pinMode(BledPin,OUTPUT);
pinMode(buttonPin,INPUT);
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(LED_BUILTIN,HIGH);
delay(1000);
digitalWrite(LED_BUILTIN,LOW);
delay(1000);
buttonState = digitalRead(buttonPin);
Serial.print("Current Color");
Serial.println(currentcolor);
if(buttonState == HIGH && !ButtonPressed){
    ledcolor = ledcolor + 1;
    ButtonPressed = true;
  }
  if(buttonState == LOW && ButtonPressed){
    ButtonPressed = false;
}
if(ledcolor == 0){
  currentcolor="LED off";
  digitalWrite(RledPin,HIGH);
  digitalWrite(GledPin,HIGH);
  digitalWrite(BledPin,HIGH);
}
else if(ledcolor == 1){
  currentcolor="Red";
  digitalWrite(RledPin,LOW);
  digitalWrite(GledPin,HIGH);
  digitalWrite(BledPin,HIGH);
}
else if(ledcolor == 2){
  currentcolor="Green";
  digitalWrite(RledPin,HIGH);
  digitalWrite(GledPin,LOW);
  digitalWrite(BledPin,HIGH);
}
else if(ledcolor == 3){
  currentcolor="Blue";
  digitalWrite(RledPin,HIGH);
  digitalWrite(GledPin,HIGH);
  digitalWrite(BledPin,LOW);
}
else if(ledcolor == 4){
  currentcolor="Yellow";
  digitalWrite(RledPin,LOW);
  digitalWrite(GledPin,LOW);
  digitalWrite(BledPin,HIGH);
}
else if(ledcolor == 5){
  currentcolor="Purple";
  digitalWrite(RledPin,LOW);
  digitalWrite(GledPin,HIGH);
  digitalWrite(BledPin,LOW);
}
else if(ledcolor == 6){
  currentcolor="Cyan";
  digitalWrite(RledPin,HIGH);
  digitalWrite(GledPin,LOW);
  digitalWrite(BledPin,LOW);
}
else if(ledcolor == 7){
  currentcolor="White";
  digitalWrite(RledPin,LOW);
  digitalWrite(GledPin,LOW);
  digitalWrite(BledPin,LOW);
}
else if(ledcolor == 8){
  ledcolor = 0;
}
}
