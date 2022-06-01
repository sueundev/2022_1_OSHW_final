// 부저 include
#include <TimerFreeTone.h>

// 리모컨 include & init
#include <IRremote.h>
int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

// LCD include & init
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 상태 변수
int onof_state = 0;   // 전원 : 1은 ON, 0은 OFF
int t = 0;     // 타이머 기본 설정
float duration; // 초음파 센서 시간
float distance; // 초음파 센서 거리

// 부저 음악 배열
int auto_start[] = {523, 523, 784, 784, 880, 880, 784};
int auto_start_t[] = {400, 400, 400, 400, 400, 400, 600};

int onof[] = {587, 659, 784};
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

// 자동 시작 함수
void autoStart() {
  lcd.setCursor(5, 1);
  lcd.print(t);
  lcd.print(" sec");
  lcd.clear();

  forward();

  if (irrecv.decode(&results)) {
    if(BTN_5 == results.value) {
      stopMotor();
      t = 0;
      
      return;
    }
    irrecv.resume();
  }
}

// 리모컨 조작 함수
int rmt() {
  switch (results.value) {
    //전원 ON/OFF
    case BTN_EQ :
      if(t <= 0) {
        lcd.setCursor(0, 0);
        lcd.print("please set time");
        for (int i = 0; i<3; i++) {
          TimerFreeTone(12, 500, 300);
          delay(150);
        }
        delay(3000);
        lcd.clear();
        return;
      }

      lcd.setCursor(5, 0);
      lcd.print("Start!");
      for (int i = 0; i < 7; i++)
        TimerFreeTone(12, auto_start[i], auto_start_t[i]);
      delay(1000);
      lcd.clear();
      Serial.println("auto_start");

      // 타이머
      while (t >= 1) {
        delay(1000);
        t--;

        autoStart();
        
        // 만약 타이머가 0이 되면 삐*3 후 멈춤
        if (t == 0) {
          stopMotor();
          lcd.setCursor(0, 0);
          lcd.print("END");
          
          for (int i = 0; i < 3; i++)
            TimerFreeTone(12, 784, 500);
          delay(100);

          lcd.clear();
          return;
        }
      }
      break;

    // 수동_앞으로
    case BTN_2 : forward(); delay(200); TimerFreeTone(12, 659, 500); stopMotor(); break;
    // 수동_뒤로
    case BTN_8 : backward(); delay(200); TimerFreeTone(12, 659, 500); stopMotor(); break;
    // 수동_왼쪽
    case BTN_4 : turnLeft(); delay(200); TimerFreeTone(12, 659, 500); stopMotor(); break;
    // 수동_오른쪽
    case BTN_6 : turnRight(); delay(200); TimerFreeTone(12, 659, 500); stopMotor(); break;
    // 타이머+
    case BTN_P :
      t += 15;
      Serial.println(t);
      lcd.setCursor(0, 0);
      lcd.print("TIME : ");
      lcd.print(t);
      lcd.clear();
      break;
    // 타이머-
    case BTN_M :
      t -= 15;
      if (t < 0) {
        t = 0;
      }
      Serial.println(t);
      lcd.setCursor(0, 0);
      lcd.print("TIME : ");
      lcd.print(t);
      lcd.clear();
      break;
    // 자동_시작
    case BTN_PP :
      break;
  }
}

// 모터 PIN 설정
#define ENB 9
#define IN4 8
#define IN3 7
#define IN2 6
#define IN1 4
#define ENA 3

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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// 뒤로
void backward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// 왼쪽
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// 오른쪽
void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
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
    irrecv.resume();
  }

  delay(100);

}
