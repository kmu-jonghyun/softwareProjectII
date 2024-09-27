#define PIN_LED 9        // LED 연결 핀
#define PIN_TRIG 12      // 초음파 센서 TRIG 핀
#define PIN_ECHO 13      // 초음파 센서 ECHO 핀

#define SND_VEL 346.0    // 음속 (m/sec)
#define INTERVAL 25      // 측정 주기 (msec)
#define PULSE_DURATION 10 // 초음파 송출 시간 (usec)
#define _DIST_MIN 100     // 최소 거리 (mm)
#define _DIST_MAX 300    // 최대 거리 (mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // 초음파 펄스 echo 대기 최대 시간
#define SCALE (0.001 * 0.5 * SND_VEL)     // 시간 -> 거리 환산 계수

void setup() {
  pinMode(PIN_LED, OUTPUT);     // LED 핀을 출력으로 설정
  pinMode(PIN_TRIG, OUTPUT);    // 초음파 센서의 TRIG 핀 설정
  pinMode(PIN_ECHO, INPUT);     // 초음파 센서의 ECHO 핀 설정
  digitalWrite(PIN_TRIG, LOW);  // TRIG 핀을 0으로 초기화

  Serial.begin(57600);          // 시리얼 통신 속도 설정 (57,600bps)
}

void loop() {
  // 거리 측정
  float distance = USS_measure(PIN_TRIG, PIN_ECHO);

  // 거리 범위에 따라 LED 켜기/끄기
  if ((distance == 0.0) || (distance > _DIST_MAX)) {
    distance = _DIST_MAX + 10.0;
    analogWrite(PIN_LED, 255);  // LED 꺼짐 (가장 어두움)
  } else if (distance < _DIST_MIN) {
    distance = _DIST_MIN - 10.0;
    analogWrite(PIN_LED, 255);  // LED 꺼짐 (가장 어두움)
  } else {
    // 거리 범위에 따른 LED 밝기 조절 (0이 가장 밝고 255가 가장 어두움)
    
    int bright = 0;
    if (distance >= 100.0 && distance <= 200.0) {
      bright = 255 - ((distance - 100.0) / 100.0 * 255.0);
    } else if (distance > 200.0 && distance <= 300.0) {
      bright = ((distance - 200.0) / 100.0 * 255.0); 
    }
    analogWrite(PIN_LED, bright);
  }

  // 측정 결과 시리얼 출력 (원하는 경우)
  Serial.print("Min: ");        Serial.print(_DIST_MIN);
  Serial.print(", distance: ");  Serial.print(distance);
  Serial.print(", Max: ");       Serial.print(_DIST_MAX);
  Serial.println("");

  // 다음 측정까지 대기
  delay(INTERVAL);
}

float USS_measure(int TRIG, int ECHO) {
  // 초음파 발신
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  // Echo 핀에서 응답 시간 측정 후 거리 계산
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}
