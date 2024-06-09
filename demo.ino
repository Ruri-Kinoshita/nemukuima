#include <VarSpeedServo.h>  //ライブラリのインポート

VarSpeedServo robot;  //サーボオブジェクトの作成

#define PR 6        // アナログ入力ピン6番にフォトリフレクタを割り当てる
#define motorPin 3  //デジタルピン3番にサーボモータを割り当てる
#define LED_PIN1 5  // デジタルピン5番に赤LEDを割り当てる
#define LED_PIN2 4  // デジタルピン4番に白LEDを割り当てる

int val;
int th = 1015;        // しきい値：各自の環境で調整してください
int currentPos = 0;   // サーボの現在の位置
bool moving = false;  // サーボが動作中かどうかのフラグ
bool bamen1 = true;
bool bamen2 = true;
bool bamen3 = true;
int after = 0;

void move1() {  　　　　　　　　　　// mode1:眠気を吸い取る時（センサー反応あり）
  digitalWrite(LED_PIN1, LOW);   // 赤LEDを消灯
  digitalWrite(LED_PIN2, HIGH);  // 白LEDを点灯
  //Serial.println("sensor_on"); 
}

void move2() {                   // mode2:眠気を吐き出す時（センサー反応あり）
  Serial.println("sensor_on");   // Pythonと通信
  digitalWrite(LED_PIN2, LOW);   // 白LEDを消灯
  digitalWrite(LED_PIN1, HIGH);  // 赤LEDを点灯
}

void setup() {
  Serial.begin(9600);         // シリアル通信の開始
  robot.attach(motorPin);     // サーボ接続ピンをmotorPin(3番ピン)に
  pinMode(LED_PIN1, OUTPUT);  // 赤LEDピン(5番ピン)を出力に設定
  pinMode(LED_PIN2, OUTPUT);  // 白LEDピン(4番ピン)を出力に設定
}

void loop() {
  val = analogRead(PR);
  //Serial.println(val);  // センサーの値を出力して確認

  if (val < th) {
    if (bamen1 == true) {
      if (moving) {
        robot.detach();  // サーボの動きを停止
        moving = false;
        after = 1;
      }
      move1();
    }

    if (bamen2 == true && bamen1 == false ) {
      if (moving) {
        robot.detach();  // サーボの動きを停止
        moving = false;
        after = 3;
      }
      move1();
    }
    
    if (bamen2 == false && bamen3 == true ) {
      Serial.println("sensor_on");   // Pythonと通信
      if (moving) {
        robot.detach();  // サーボの動きを停止
        moving = false;
        after = 5;
      }
      move2();
    }

  } else {
    digitalWrite(LED_PIN2, LOW);  // 白LEDを消灯
    digitalWrite(LED_PIN1, LOW);  // 赤LEDを消灯
    
    if (after == 1 && bamen1 == true) {
      bamen1 = false;
      after = 2;
    }
    if (after == 3 && bamen2 == true) {
      bamen2 = false;
      after = 4;
    }
    if (after == 5 && bamen3 == true) {
      Serial.println("sensor_on");   // Pythonと通信
      bamen3 = false;
      after = 6;
    }
    if (!moving) {
      robot.attach(motorPin);  // サーボを再接続
      currentPos = 0;
      moving = true;
    }
    if (currentPos == 0) {
      robot.write(90, 20, true);  // 90度まで動かす
      currentPos = 35;
    } else {
      robot.write(0, 20, true);  // 0度まで戻す
      currentPos = 0;
    }
    delay(200);  // 200ms待つ
    
    if (bamen3 == false && after == 6) {
      bamen1 = true;
      bamen2 = true;
      bamen3 = true;
      after = 0;
    }
  }
}
