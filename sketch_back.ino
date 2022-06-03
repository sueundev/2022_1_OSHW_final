// 초음파 센서 init
int trigPin = 11;
int echoPin = 10;

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

// 초음파 센서 함수
float ultra() {
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  // HIGH일 때 초음파가 보냈다가 돌아오는 시간을 가지고 거리를 계산
  // 340은 초당 소리의 속도이고 10000은 밀리세컨드를 세컨드로, 왕복거리이므로 2로 나눔
  duration = pulseIn(echoPin, HIGH);
  distance = ((float)(340 * duration) / 10000) / 2;
  delay(100);

  return distance;
}

// 자동 시작 함수
void autoStart() {
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print(t+1);
  lcd.print(" sec");
  
  if(ultra() < 20) {
    stopMotor();
    delay(1000);
    backward();
    delay(2000);    
  }
  else {
     forward();
  }
 
  if (irrecv.decode(&results)) {
    if (BTN_5 == results.value) {
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
    // 자동_시작
    case BTN_EQ :
      if (t <= 0) {
        lcd.setCursor(0, 1);
        lcd.print("please set time");
        for (int i = 0; i < 3; i++) {
          TimerFreeTone(12, 500, 300);
          delay(150);
        }
        delay(2000);
        lcd.clear();
        return;
      }

      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("Start!");
      for (int i = 0; i < 7; i++)
        TimerFreeTone(12, auto_start[i], auto_start_t[i]);
      delay(1000);
      lcd.clear();
      Serial.write('1');
      
      // 타이머
      while (t >= 0) {
        delay(1000);
        t--;
       
        autoStart();
        
        // 만약 타이머가 0이 되면 삐*3 후 멈춤
        if (t == 0) {
          Serial.write('0');
          stopMotor();
          lcd.setCursor(6, 0);
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
      if (t > 150) {
        t = 150;
      }
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("TIME : ");
      lcd.print(t);

      break;
    // 타이머-
    case BTN_M :
      t -= 15;
      if (t < 0) {
        t = 0;
      }
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("TIME : ");
      lcd.print(t);

      break;
    case BTN_5 :
      Serial.write('5');
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

  // 모터 핀 설정
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // 리모컨 동작 시작
  irrecv.enableIRIn();

  // 이동 바퀴 모터 설정
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);

  //LCD 설정
  lcd.init();
  lcd.backlight();

  // 초음파 센서 핀 설정
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // 리모컨 입력
  if (irrecv.decode(&results)) {
    rmt();
    irrecv.resume();
  }
  delay(100);
}
