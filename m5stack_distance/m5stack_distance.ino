#include <M5Stack.h>
#include <ArduinoOSCWiFi.h>


// センサとM5Stackの接続に使うピン
const int TRIG = 22;
const int ECHO = 35;

double duration = 0.0; // センサデータの値
double distance = 0.0; // 距離

// OSC通信設定
const char* PUBLISH_IP = "xxx.xxx.xxx.xxx"; //受信する端末のIP
const int PUBLISH_PORT = 10000;
const char* ADDRESS = "/distance/value";

void setup() {
  Serial.begin(9600);
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setBrightness(255);

  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);

  // WiFi接続
  const char* SSID = "ssid";
  const char* PASSWD = "ssid";
  WiFi.begin(SSID, PASSWD);

  // distanceを毎秒30回distanceの値を送信する
  OscWiFi.publish(PUBLISH_IP, PUBLISH_PORT, ADDRESS, distance);
}

int menu_id = 0; // 現在のメニュー
const int MENU_INFO = 0;
const int MENU_2 = 1;
const int MENU_3 = 2;
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
  if (menu_id == MENU_INFO) {
    device_info_view();
    get_distance();
    osc_info_view();
  }

  delay(200);
}

// メニューの表示
void menu_view() {
  M5.Lcd.setTextSize(3);
  const int BTN_Y = 210;

  // 左のボタン
  if (menu_id == MENU_INFO) {
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

  if (WiFi.status() == WL_CONNECTED) {
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.print("Host IP: ");
    M5.Lcd.print(WiFi.localIP());
  }else {
    M5.Lcd.setTextColor(RED);
    M5.Lcd.print("Not connected.");
  }
}

// 距離の表示
void get_distance() {
  const int NOW_TEMP = 20; // 現在の気温
  const double SPEED_OF_SOUND = 331.5 + 0.6 * NOW_TEMP; // 現在温度に対する音速

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
    M5.Lcd.print("Please move closer to the sensor.");
  }
}

// OSC通信送信先の情報
void osc_info_view() {
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  M5.Lcd.setCursor(10, 100);
  M5.Lcd.println("OSC Destination Info");
  
  M5.Lcd.setCursor(10, 125);
  M5.Lcd.print("DST IP: ");
  M5.Lcd.println(PUBLISH_IP);

  M5.Lcd.setCursor(10, 145);  
  M5.Lcd.print("Port: ");
  M5.Lcd.println(PUBLISH_PORT);

  M5.Lcd.setCursor(10, 165);
  M5.Lcd.print("Address: ");
  M5.Lcd.print(ADDRESS);
}
