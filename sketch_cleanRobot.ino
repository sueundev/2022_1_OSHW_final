// 부저 include
#include <TimerFreeTone.h>

// 리모컨 include & init
#include <IRremote.h>
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

//LCD include & init
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 상태 변수
int onof_state = 0;   // 전원 : 1은 ON, 0은 OFF
int t = 30;     // 타이머 기본 설정

// 부저 음악 배열
int auto_start[] = {523, 523, 784, 784, 880, 880, 784};
int auto_start_t[] = {400, 400, 400, 400, 400, 400, 600};

int onof[] = {784, 659, 587};
int onof_t[] = {400, 400, 400};

// 리모컨 값 설정
#define BTN_CH_M       0xFFA25D  // CH- Button
#define BTN_CH         0xFF629D  // CH Button
#define BTN_CH_P       0xFFE21D  // CH+ Button

#define BTN_PREV       0xFF22DD  // PREV Button
#define BTN_NEXT       0xFF02FD  // NEXT Button
#define BTN_PP         0xFFC23D  // PLAY-PAUSE Button

#define BTN_M          0xFFE01F  // -(VOL-) Button
#define BTN_P          0xFFA857  // +(VOL+) Button
#define BTN_EQ         0xFF906F  // EQ Button

#define BTN_0          0xFF6897  // 배터리 잔량 확인 버튼
#define BTN_100P       0xFF9867  // 100+ Button
#define BTN_200P       0xFFB04F  // 100- Button

#define BTN_1          0xFF30CF
#define BTN_2          0xFF18E7
#define BTN_3          0xFF7A85

#define BTN_4          0xFF10EF
#define BTN_5          0xFF38C7
#define BTN_6          0xFF5AA5

#define BTN_7          0xFF42BD
#define BTN_8          0xFF4AB5
#define BTN_9          0xFF52AD

// 리모컨 조작 함수
int rmt() {
  switch (results.value) {
    //전원 ON/OFF
    case BTN_EQ :
      if (onof_state == 0) {
        for (int i = 0; i < 3; i++)
          TimerFreeTone(10, on[i], on_t[i]);
        Serial.println("ON");
        onof_state = 1;
        lcd.setCursor(7, 0);
        lcd.print("^^");
        lcd.setCursor(5, 1);
        lcd.print("Hello!");
        for (int k = 0; k < 3; k++) {
          lcd.noDisplay();
          delay(1000);
          lcd.display();
          delay(1000);
        }
        lcd.clear();
      }
      else {
        Serial.println("OFF");
        for (int i = 3; i > 0; i--)
          TimerFreeTone(10, onof[i], onof_t[i]);
        onof_state = 0;
        lcd.setCursor(7, 0);
        lcd.print("Zzz");
        lcd.setCursor(6, 1);
        lcd.print("Bye..");
        for (int k = 0; k < 3; k++) {
          lcd.noDisplay();
          delay(1000);
          lcd.display();
          delay(1000);
        }
        lcd.clear();
      }
      break;

    // 수동_앞으로
    case BTN_2 : forward(); delay(200); TimerFreeTone(10, 659, 500); stopMotor(); break;
    // 수동_뒤로
    case BTN_8 : backward(); delay(200); TimerFreeTone(10, 659, 500); stopMotor(); break;
    // 수동_왼쪽
    case BTN_4 : turnLeft(); delay(200); TimerFreeTone(10, 659, 500); stopMotor(); break;
    // 수동_오른쪽
    case BTN_6 : turnRight(); delay(200); TimerFreeTone(10, 659, 500); stopMotor(); break;
    // 수동_정지
    case BTN_5 :
      stopMotor();
      TimerFreeTone(10, 659, 500);
      lcd.setCursor(6, 0);
      lcd.print("Stop");
      delay(3000);
      lcd.clear();
      break;
    // 자동_시작
    case BTN_PP :
      for (int i = 0; i < 7; i++)
        TimerFreeTone(10, auto_start[i], auto_start_t[i]);
      lcd.setCursor(5, 0);
      lcd.print("Start!");
      delay(3000);
      lcd.clear();
      Serial.println("auto_start");
      /*
      while (1) {
        lcd.setCursor(0, 0);
        lcd.print("TIME : ");
        lcd.print(t);
        irrecv.resume();
        if (irrecv.decode(&results)) {
          Serial.println(results.value, HEX);
          
          int temp=rmt();
          Serial.println(temp);
          if(temp==9)
            break;
          Serial.println("second button");
          irrecv.resume();
        }
        
      }
      for (; t >= 0; t--) {
        forward();
        Serial.print(t, "초 진행중");
      }
      */
      forward();
      break;

  }
}

// 모터 PIN 설정
#define ENB 3
#define IN4 4
#define IN3 5
#define IN2 6
#define IN1 7
#define ENA 9

// 모터 번호
#define CH1 0
#define CH2 1
#define ALL_CH 2

// 속도 설정
// mode : 각 모터를 선택적으로 변경 가능 (CH1/CH2/ALL_CH)
// speed : 0 ~ 255 까지 가능 (60 이하로는 모터가 거의 동작안함)
void setMotorSpeed(unsigned char mode, unsigned char speed) {
  if (mode == CH1) {
    analogWrite(ENA, speed);
  }
  else if (mode == CH2) {
    analogWrite(ENB, speed);
  }
  else {
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
  }
}

// 앞으로
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// 뒤로
void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// 왼쪽
void turnLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

// 오른쪽
void turnRight() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// 스톱
void stopMotor() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}



void setup() {
  Serial.begin(9600);

  // 이동 바퀴 모터 핀 설정
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // 리모컨 동작 시작
  Serial.println("Enabling IRin");
  irrecv.enableIRIn();
  Serial.println("Enabled IRin");

  // 이동 바퀴 모터 설정
  setMotorSpeed(ALL_CH, 255);

  //LCD 설정
  lcd.init();
  lcd.backlight();
}

void loop() {
  // 리모컨 입력
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    rmt();
    Serial.println("hello");
    irrecv.resume();
  }

  delay(100);
}
