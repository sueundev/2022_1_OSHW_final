// 워터 펌프 드라이버 init
int AA = 12;
int AB = 13;

// 수위 센서 함수
int water_level() {
  int level = analogRead(A0);
  Serial.println(level);

  if (level < 555) {
    return 1;
  }
  return 0;
}

// 워터 펌프 출력 함수
void water_pump() {
  digitalWrite(AA, HIGH);  // 모터 회전
  digitalWrite(AB, LOW);
  delay(500);
  digitalWrite(AA, LOW);  // 모터 정지
  digitalWrite(AB, LOW);
  delay(3000);
}

// 워터 펌프 stop 함수
void water_pump_stop() {
  digitalWrite(AA, LOW);  // 모터 정지
  digitalWrite(AB, LOW);
}

// 모터 PIN 설정
#define ENB 6
#define IN4 7
#define IN3 8
#define IN2 9
#define IN1 10
#define ENA 11

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

// 회전
void rotation() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
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

  // 이동 바퀴 모터 핀 설정
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // 이동 바퀴 모터 설정
  setMotorSpeed(ALL_CH, 120);

  // 워터 펌프 핀 설정
  pinMode(AA, OUTPUT);
  pinMode(AB, OUTPUT);
}

void loop() {
  char r;
  if (Serial.available()) {
    r = Serial.read();
  }

  if (r == '1') {
    rotation();
    water_pump();
  }
  else {
    stopMotor();
    water_pump_stop();
  }

  if (water_level() == 1) {
    water_pump_stop();
  }
}
