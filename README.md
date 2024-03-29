# 2022년 1학기 오픈소스HW 기말 프로젝트
> 2022-1 OPEN SOURCE HW FINAL PROJECT

<br>

## 주제 : 아두이노를 이용한 물청소 로봇
### 1. 프로젝트 개요

<div align="center">
  <img src="https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/ff8ab3d7-1783-4f33-be4f-c84dfa0d41ba" width="90%"></img>
</div>

<br>

__수동 및 자동 모드로 사용할 수 있는 물청소 로봇__

워터 펌프로 로봇이 가는 길목에 물이 뿌려지고, 천이 붙어 있는 앞부분의 바퀴가 돌아가며 바닥을 청소한다. LCD에는 각종 정보가 출력되고, 특별한 상황(물 부족, 배터리 부족 등)이 발생하면 부저가 울린다. 초음파 센서로 주변 사물을 인식하여 장애물을 비껴간다. 전체적인 조작은 리모컨으로 이루어진다.

---

### 2. 초기 시스템 구성 및 기능

- 수동과 자동, 2가지 모드
- 물탱크와 연결된 워터 펌프가 로봇이 가는 길목에 물을 공급함
- 이동을 도와주는 뒷바퀴
- 천이 붙은 앞바퀴가 돌아가면서 바닥을 청소함
- LCD가 on/off, 물탱크의 수위, 배터리 잔량, 타이머 시간 등을 출력함
- 수동 부저가 on/off, 물 부족, 배터리 부족, 타이머 종료, 리모컨 조작 시 소리가 울림
- 리모컨이 전체적인 조작을 담당함
- 수위 센서가 물탱크의 수위 정보를 수집함
- 초음파 센서가 주변 사물을 인식하여 장애물이 있으면 정지하고 다른 방향으로 회전함

| 항목             | 기능                                                         |
| ---------------- | ------------------------------------------------------------ |
| 워터 펌프 + 호스 | 물 공급용 펌프와 연결 호스                                   |
| DC 모터 + 바퀴   | 1. 로봇이 이동할 수 있는 뒷바퀴<br>2. 청소할 때 회전하는 천이 붙은 앞바퀴 |
| 모터 드라이버    | 모터의 속도와 방향을 제어함                                  |
| LCD              | 각종 정보 출력                                               |
| 수동 부저        | 리모컨 조작, 특이사항 발생 시 울림                           |
| 리모컨           | 전체적인 조작                                                |
| 수위 센서        | 물탱크의 수위 정보 수집함                                    |
| 초음파 센서      | 주변의 사물을 인식함                                         |

---

### 3. 계획 대비 변경 내용

| 이전 계획                                             | 변경 내용                            |
| ----------------------------------------------------- | ------------------------------------ |
| LCD로 물탱크의 수위, 배터리 잔량, on/off 출력         | 출력 없음                            |
| 수동 부저로 on/off, 물 부족, 배터리 부족 시 소리 울림 | 소리 출력 없음                       |
| 워터 펌프 사용 시 키트 내 드라이버 사용               | 워터 펌프 전용 드라이버 추가         |
| 초음파 센서를 Slave 아두이노에 연결하여 사용          | 초음파 센서를 Master 아두이노로 이동 |

---

### 4. 최종 시스템 구성 및 기능

| 구성                                                         | 기능                                                         |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| <div align="center">![image02](https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/9d5d9362-0dba-40fa-806d-70bfb65ff5a1)</div> | 1. 전진 : 수동으로 조작할 때 전진하는 버튼<br>2. 후진 : 수동으로 조작할 때 후진하는 버튼<br>3. 우회전 : 수동으로 조작할 때 우회전하는 버튼<br>4. 좌회전 : 수동으로 조작할 때 좌회전하는 버튼<br>5. 타이머+ : 누를 때마다 15초씩 늘어난다. 150초가 되면 더 늘어나지 않는다.<br>6. 타이머- : 누를 때마다 15초씩 줄어든다. 0초가 되면 더 줄어들지 않는다.<br>7. 자동 시작 : 입력된 시간에 맞춰 자동으로 움직인다. 앞쪽 바퀴 모터, 워터 펌프, 초음파 센서, 수위 센서가 활성화된다.<br>8. 정지 : 자동 시작 중인 경우에만 활성화되며, 모든 모터와 초음파 센서, 수위 센서가 정지된다. |
| ![image03](https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/2d5a7e09-9689-462f-bbf9-9ff73592a971) | 자동 시작 버튼이 눌리면 Master 아두이노에서 Slave 아두이노로 ‘1’ 신호가 Write 되며, 앞쪽 바퀴가 돌아가는 rotation() 함수와 워터 펌프가 작동되는 water_pump() 함수가 시작된다.<br>만약 모종의 이유로 ‘1’이 아닌 다른 신호가 Write 되면 앞쪽 바퀴 모터가 멈추는 stopMotor() 함수와 함께 워터 펌프 동작이 멈추는 water_pump_stop() 함수가 실행 된다.<br>수위 센서는 if 문에 따라 계속해서 수위를 체크 하게 되는데 만약 수위가 555 이하가 되면 1을 리턴 하는 water_level() 함수에 의해 워터 펌프의 작동이 정지된다. |
| ![image04](https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/a526db67-1a03-4f7f-be28-2d1f7764d2be) | 뒤쪽 바퀴의 사용은 수동과 자동으로 나뉘게 되는데, rmt() 함수를 통해 리모컨값을 받는 switch()가 실행된다.<br>수동의 경우 2, 4, 6, 8 버튼을 눌렀을 때 바퀴 모터를 조작하는 forward(), backward(), turnLeft(), turnRight()가 호출되며 전후좌우로 움직인다.<br>자동의 경우 타이머가 설정되었을 때만 활성화되며, 타이머 변수인 t가 0이 될 때까지 while 문을 통해 autoStart() 함수가 실행된다. |
| ![image05](https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/270a4c43-7df1-401e-955b-488d6c13acdd) | - 초음파 센서 : 자동 시작을 통해 autoStart() 함수를 호출하게 되면 그 안에서 ultra() 함수가 호출되는데, 이 함수는 사물과의 거릿값을 리턴 한다. 만약 리턴 된 거리가 20보다 짧으면 뒤쪽 바퀴 모터가 1초 동안 멈추고 2초 동안 후진하며 사물을 피하도록 한다.<br><br>- 앞쪽 바퀴 모터 : 자동 시작 버튼이 눌리면 Master 아두이노에서 Slave 아두이노로 ‘1’ 신호가 Write 되며, 앞쪽 바퀴가 돌아가는 rotation() 함수가 시작된다. |
| ![image06](https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/38cd7d16-6abb-43e9-b0bc-018600469075) | EQ 버튼 :<br>타이머가 설정되지 않았을 때 누르면 "please set time"가 출력된다.<br>타이머가 설정되었을 때 누르면 "Start!"가 출력되며, 자동 시작된다.<br><br>타이머+ 버튼 : 설정된 타이머의 +15초가 출력된다.<br>타이머- 버튼 : 설정된 타이머의 –15초가 출력된다.<br><br>자동 시작 시 : 타이머가 1초씩 감소하는 것이 출력된다.<br>타이머 종료/정지 시 : "END"가 출력된다. |
| ![image07](https://github.com/sueundev/2022_1_OSHW_final/assets/106625107/47ee2a7b-a3d6-4de8-98fa-4f30719813eb) | - 수동 부저 : 2, 4, 6, 8 버튼이 눌릴 때마다 부저가 울리며, 자동 시작, 타이머 종료, 타이머 설정 전 상황에서 전용 주파수 배열이 울린다.<br>- 리모컨&센서 : 사용자의 리모컨 조작 신호를 받는다. |

---

### 5. 문제점 및 해결 방법

| **문제점**                                                   | **해결 방법**                                                |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| 소자들을 담는 상자가 너무 크고 무거워서 바퀴가 움직이지 않음 | 처음에는 캔버스를 이용해서 만들었는데 크기도 작고, 옆면을 받쳐주지 못해 위태로운 형태였다. 그래서 바구니로 바꿔 문제를 해결했지만, 무게가 무거워져서 바퀴가 움직이지 않았다. 마지막으로 키트 상자를 사용하여 제작해 봤는데 바퀴도 움직이고, 소자들이 들어가는 자리 또한 충분하여 문제를 해결했다. |
| 수위 센서가 짧아 긴 물통을 사용할 수 없었음 | 원래는 물탱크로 500mL 페트병을 사용하려 했는데 수위 센서의 길이가 짧아 사용할 수 없었다. 물탱크를 종이컵으로 바꾸고 사용되는 물의 양을 조절하기 위해 워터 펌프가 물을 뿜는 시간을 조정했다. |
| 초음파 센서의 값을 Slave아두이노에서 Master 아두이노로 보낼 수 없음 | 초음파 센서에서 읽어 들이는 값을 이용하여 만약 사물과 거리가 가까울 시 이동을 담당하는 뒤쪽 바퀴의 조작이 필요한데 초기에 초음파 센서는 Slave 아두이노에서 담당하고, 뒤쪽 바퀴는 Master 아두이노에서 담당했다. Slave 아두이노에서 Master 아두이노로 신호를 보내기가 매우 까다로워서 초음파 센서의 위치를 Master 아두이노로 이동하였다. |
