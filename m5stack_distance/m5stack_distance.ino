#include<M5Stack.h>

// HC-SR04の値の読み出しと距離への変換は
// "超音波センサHC-SR04とArduinoで距離を測定する - Robo Station"
//  https://rb-station.com/blogs/article/hc-sr04-arduino
// のサンプルコードを参考にしました

int TRIG = 22;
int ECHO = 35;
double duration = 0;
double distance = 0;
double speed_of_sound = 331.5 + 0.6 * 25; //25℃における音速

void setup() {
  Serial.begin(9600);
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setBrightness(255);

  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
}

void loop() {
  M5.update(); 
  
  digitalWrite(TRIG,LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10); 
  digitalWrite(TRIG,LOW);
  duration = pulseIn(ECHO,HIGH);

  if (duration > 0) {
    duration = duration / 2;
    distance = duration * speed_of_sound * 100 / 1000000;

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(6);
    M5.Lcd.print(distance);
  }

  delay(200);
}