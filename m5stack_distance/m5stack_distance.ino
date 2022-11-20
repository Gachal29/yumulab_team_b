#include <M5Stack.h>
#include <ArduinoOSCWiFi.h>


// センサとM5Stackの接続に使うピン
const int TRIG = 22;
const int ECHO = 35;

double duration = 0.0; // センサデータの値
double distance = 0.0; // 距離
void setup() {
  Serial.begin(9600);
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setBrightness(255);

  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  // WiFi接続
  const char* SSID = "ssid";
  const char* PASSWD = "password";
  WiFi.begin(SSID, PASSWD);

  // OSC通信を行う設定
  const char* PUBLISH_IP = "192.168.xxx.xxx"; //受信する端末のIP
  const int PUBLISH_PORT = 10000;
  const char* ADDRESS = "/distance/value";

  OscWiFi.publish(PUBLISH_IP, PUBLISH_PORT, ADDRESS, distance);
}

int menu_id = 0; // 現在のメニュー
void loop() {
  M5.update();
  OscWiFi.update();
  
  digitalWrite(TRIG ,LOW); 
  delayMicroseconds(2); 
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // センサデータの取得
  duration = pulseIn(ECHO, HIGH);

  M5.Lcd.fillScreen(BLACK);

  pressed_btn();
  menu_view();
  device_info_view();
  get_distance();

  delay(200);
}

// メニューの表示
void menu_view() {
  M5.Lcd.setTextSize(3);
  const int BTN_Y = 210;

  // 左のボタン
  if (menu_id == 0) {
    M5.Lcd.setTextColor(YELLOW);
  }else {
    M5.Lcd.setTextColor(WHITE);
  }
  M5.Lcd.setCursor(30, BTN_Y);
  M5.Lcd.print("INFO");

  // 中央のボタン
  if (menu_id == 1) {
    M5.Lcd.setTextColor(YELLOW);
  }else {
    M5.Lcd.setTextColor(WHITE);
  }
  M5.Lcd.setCursor(120, BTN_Y);
  M5.Lcd.print("MENU2");

  // 右のボタン
  if (menu_id == 2) {
    M5.Lcd.setTextColor(YELLOW);
  }else {
    M5.Lcd.setTextColor(WHITE);
  }
  M5.Lcd.setCursor(220, BTN_Y);
  M5.Lcd.print("MENU3");
}

// ボタンが押された時の処理
void pressed_btn() {
  if (M5.BtnA.wasPressed()) {
    menu_id = 0;
  }

  if (M5.BtnB.wasPressed()) {
    menu_id = 1;
  }

  if (M5.BtnC.wasPressed()) {
    menu_id = 2;
  }
}

// デバイス情報の表示
void device_info_view() {
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.setTextSize(2);

  if (WiFi.status()) {
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.print("IP: ");
    M5.Lcd.print(WiFi.localIP());
  }else {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("Not connected.");
  }
}

// 距離の表示
void get_distance() {
  const double SPEED_OF_SOUND = 331.5 + 0.6 * 25; //25℃における音速

  const double MIN_DURATION = 0.0;
  const double MAX_DURATION = 10000.0;

  M5.Lcd.setCursor(10, 40);
  if (duration > MIN_DURATION && duration < MAX_DURATION) {
    duration = duration / 2;
    distance = duration * SPEED_OF_SOUND * 100 / 1000000;

    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("distance: ");
    M5.Lcd.setTextSize(4);
    M5.Lcd.print(distance);
  }else {
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("Please move closer to the sensor");
  }
}
