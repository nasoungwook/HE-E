#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);
int bufferPosition;
int echoPin = 6; 
int trigPin = 7;
int relay = 13;
// 초음파 거리 센서는 6, 7번핀, 릴레이는 13번 핀에 연결 했습니다.
float senVal = 0;
float senDur = 0;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  bufferPosition = 0;
  pinMode(relay, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
void loop() {
  if (BTSerial.available()) {
    byte data = BTSerial.read(); // 블루투스로 안드로이드에서 보내준 데이터를 받는다.
    if (data == '2') { // 안드로이드에서 on을 입력 했을 경우
      digitalWrite(relay, HIGH);
    } else if (data == '1') { // 안드로이드에서 off를 입력 했을 경우
      digitalWrite(relay, LOW);
    } else if (data == '3') { // 안드로이드에서 auto를 입력 했을 경우
      while (1) {
        float duration, distance;
        digitalWrite(trigPin, HIGH);
        delay(10);
        digitalWrite(trigPin, LOW);
        senDur = pulseIn(echoPin, HIGH);
        senVal = ((float)(340 * senDur) / 10000) / 2;
        // 초음파 거리 센서에서 보낸 데이터에서 거리를 측정 한다.
        // 초음파 거리 센서 원리는 튜토리얼에 있습니다.
        Serial.println(senVal);
        if (senVal < 40) // 40cm내에 사람이 있을 경우
        {
          digitalWrite(relay, LOW); // 릴레이를 작동
        } else {
          digitalWrite(relay, HIGH); // 없을 경우 릴레이를 끔
        }
        delay(200); // 0.2초 대기
        if (BTSerial.read() == '4') { // 안드로이드에서 stop을 입력 했을 경우
          break; // 초음파 거리 센서로 스탠드를 조정 하는 것을 멈춘다.
        }
      }
    }
  }
} 
