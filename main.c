#include <Servo.h>//引入舵机库文件
const int TrigPin = 8;//设定超声波SR04连接的Arduino引脚
const int EchoPin = 9;
const int L298nIn1 = 2;//L298N模块信号输入引脚
const int L298nIn2 = 3;
const int L298nIn3 = 4;
const int L298nIn4 = 5;
const int KeepAway = 25;//离障碍最近距离
const int LookDelay = 300;//转动舵机的停顿时间
const int TurnDelay = 500;//转向停顿时间，通过停顿时间控制转弯角度大小
const int BackDelay = 600;//倒退时间
const int ReboundDelay = 150;//反弹时间
Servo myservo;//定义舵机变量名
void setup()
{
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(L298nIn1, OUTPUT);
  pinMode(L298nIn2, OUTPUT);
  pinMode(L298nIn3, OUTPUT);
  pinMode(L298nIn4, OUTPUT);
  myservo.attach(10);//定义舵机接口（引脚9、10 都可以，且只能为这两个）
  Serial.begin(9600);
}
void goForward() {//小车前进
  digitalWrite(L298nIn1, HIGH);
  digitalWrite(L298nIn2, LOW);
  digitalWrite(L298nIn3, LOW);
  digitalWrite(L298nIn4, HIGH);
}
void goBack() {//小车后退
  digitalWrite(L298nIn1, LOW);
  digitalWrite(L298nIn2, HIGH);
  digitalWrite(L298nIn3, HIGH);
  digitalWrite(L298nIn4, LOW);
}
void goStop() {//小车停止
  digitalWrite(L298nIn1, LOW);
  digitalWrite(L298nIn2, LOW);
  digitalWrite(L298nIn3, LOW);
  digitalWrite(L298nIn4, LOW);
}
void turnRight() {//小车右转
  digitalWrite(L298nIn1, LOW);
  digitalWrite(L298nIn2, HIGH);
  digitalWrite(L298nIn3, LOW);
  digitalWrite(L298nIn4, HIGH);

}
void turnLeft() {//小车左转
  digitalWrite(L298nIn1, HIGH);
  digitalWrite(L298nIn2, LOW);
  digitalWrite(L298nIn3, HIGH);
  digitalWrite(L298nIn4, LOW);
}
void goRebound() {//小车遇到障碍反弹
  digitalWrite(L298nIn1, LOW);
  digitalWrite(L298nIn2, HIGH);
  digitalWrite(L298nIn3, HIGH);
  digitalWrite(L298nIn4, LOW);
  delay(ReboundDelay);
  goStop();
}
void lookRight1() {//向右看90度
  myservo.write(5);
  delay(LookDelay);
}
void lookRight2() {//向右看45度
  myservo.write(45);
  delay(LookDelay);
}
void lookForward() {//向右看前
  myservo.write(90);
  delay(LookDelay);
}
void lookLeft4() {//向左看45度
  myservo.write(135);
  delay(LookDelay);
}
void lookLeft5() {//向左看90度
  myservo.write(175);
  delay(LookDelay);
}
float checkDistance(int TrigPin, int EchoPin) {//超声波探测障碍物距离函数
  float distance;
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  // 检测脉冲宽度，并计算出距离
  distance = pulseIn(EchoPin, HIGH) / 58.00;
  return distance;
}
int lookAround() {//观察周围情况，并返回方向代码
  lookRight1();
  int D1 = checkDistance(TrigPin, EchoPin);
  lookRight2();
  int D2 = checkDistance(TrigPin, EchoPin);
  lookForward();
  int D3 = checkDistance(TrigPin, EchoPin);
  lookLeft4();
  int D4 = checkDistance(TrigPin, EchoPin);
  lookLeft5();
  int D5 = checkDistance(TrigPin, EchoPin);
  if (D1 > D2 && D1 > D3 && D1 > D4 && D1 > D5 && D1 > KeepAway)return 1;
  if (D2 > D1 && D2 > D3 && D2 > D4 && D2 > D5 && D2 > KeepAway)return 2;
  if (D3 > D2 && D3 > D1 && D3 > D4 && D3 > D5 && D3 > KeepAway)return 3;
  if (D4 > D2 && D4 > D3 && D4 > D1 && D4 > D5 && D4 > KeepAway)return 4;
  if (D5 > D2 && D5 > D3 && D5 > D4 && D5 > D1 && D5 > KeepAway)return 5;
  return 0;
}
void loop()//主循环函数
{
  lookForward();//调整超声波探测模块向前看
  int distance = checkDistance(TrigPin, EchoPin);//查看距离
  if (distance > KeepAway) {//如果距离大于最小距离
    goForward();//一直向前
  } else {
    for (int i = 0; i < 100; i++)
    {
      goStop();
      goRebound();
      switch (lookAround()) {//判断观察周围情况结果，并执行相应动作
        case 1:
          turnRight();
          delay(TurnDelay);
          goStop();
          return;
        case 2:
          turnRight();
          delay(TurnDelay / 2);
          goStop();
          return;
        case 3:
          goForward();
          return;
        case 4:
          turnLeft();
          delay(TurnDelay / 2);
          goStop();
          return;
        case 5:
          turnLeft();
          delay(TurnDelay);
          goStop();
          return;
        default:
          break;
      }
      goBack();
      delay(BackDelay);
      goStop();
    }
  }
}
