#include <Arduino.h>

#define JOYSTICK_X_PIN A0 // 조이스틱 X축 핀
#define JOYSTICK_Y_PIN A1 // 조이스틱 Y축 핀
#define JOYSTICK_BTN_PIN 8 // 조이스틱 푸쉬 버튼 핀

bool isPaused = false;  // 조이스틱 버튼으로 control 누를 때마다 T, F 값 변환
float speedMultiplier = 1.0;
float waiting_time;

int controlPins[6] = {2, 3, 4, 5, 6, 7};
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
String text;
char letter;
int index_alphabet_to_Braiile;
int index_text = 0;

int braille[26][6] = {
  {1,0,0,0,0,0},  // a
  {1,1,0,0,0,0},  // b
  {1,0,0,1,0,0},  // c
  {1,0,0,1,1,0},  // d
  {1,0,0,0,1,0},  // e
  {1,1,0,1,0,0},  // f
  {1,1,0,1,1,0},  // g
  {1,1,0,0,1,0},  // h
  {0,1,0,1,0,0},  // i
  {0,1,0,1,1,0},  // j
  {1,0,1,0,0,0},  // k
  {1,1,1,0,0,0},  // l
  {1,0,1,1,0,0},  // m
  {1,0,1,1,1,0},  // n
  {1,0,1,0,1,0},  // o
  {1,1,1,1,0,0},  // p
  {1,1,1,1,1,0},  // q
  {1,1,1,0,1,0},  // r
  {0,1,1,1,0,0},  // s
  {0,1,1,1,1,0},  // t
  {1,0,1,0,0,1},  // u
  {1,1,1,0,0,1},  // v
  {0,1,0,1,1,1},  // w
  {1,0,1,1,0,1},  // x
  {1,0,1,1,1,1},  // y
  {1,0,1,0,1,1},  // z
};

void setup() {
  Serial.begin(9600);

  // 점자 셀 설정
  for (int i = 0; i <= 5; i++) {
    pinMode(controlPins[i], OUTPUT);
  }

  // 조이스틱 설정
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);

  // 초기 점자 대기 시간 계산
  waiting_time = 300 / speedMultiplier;
}

char joystick(int xValue, int yValue, int buttonState); // 조이스틱 값을 컨트롤해서 loop 문 안에서 활용

void loop() {
  int xValue = analogRead(JOYSTICK_X_PIN);
  int yValue = analogRead(JOYSTICK_Y_PIN);
  int buttonState = digitalRead(JOYSTICK_BTN_PIN);

  char joystickDirection = joystick(xValue, yValue, buttonState);

  if (joystickDirection == 'L') {
    speedMultiplier *= (2.0 / 3.0);   // L 조작을 하면 읽는 속도가 1.5배 느려짐 (X 2/3)
    speedMultiplier = max(speedMultiplier, 0.25); // 최소 배속(x0.25) 제한
    waiting_time = 300 / speedMultiplier; // 대기 시간 재계산
    delay(200); // 방향 변경 확인을 위한 지연 시간
  } else if (joystickDirection == 'R') {
    speedMultiplier *= 1.5;   // R 조작을 하면 읽는 속도가 1.5배 빨라짐
    speedMultiplier = min(speedMultiplier, 2.0); // 최대 배속(x2.0) 제한
    waiting_time = 300 / speedMultiplier; // 대기 시간 재계산
    delay(200);
  } else if (joystickDirection == 'P') {
    isPaused = !isPaused;   // 조이스틱 버튼을 누르면 일시정지 혹은 재개
    delay(200);
  }

  // 시리얼 입력을 기다림
  if (Serial.available() > 0) {
    text = Serial.readString(); // 컴퓨터로부터 문자열 입력받기
    Serial.print("Received text: ");
    Serial.println(text);
  }

  if (isPaused) {
    for (; index_alphabet_to_Braiile < text.length(); index_alphabet_to_Braiile++) {
      letter = text[index_text];

      // 공백이나 알페벳이 아닌 문자는 무시
      if (isAlpha(letter)) {
        letter = toLowerCase(letter); // 소문자로 변환

        // 입력된 문자에 해당하는 알파벳 배열 인덱스 찾기
        for (int i = 0; i < 26; i++) {
          if (letter == alphabet[i]) {
            index_alphabet_to_Braiile = i;
            break;
          }
        }

        // 점자 패턴 출력
        for (int j = 0; j <= 5; j++) {
          digitalWrite(controlPins[j], braille[index_alphabet_to_Braiile][j]);
        }

        delay(500); // 문자 표시 시간

        // 점자 패턴을 끄고 대기
        for (int k = 0; k <= 5; k++) {
          digitalWrite(controlPins[k], LOW);
        }

        delay(waiting_time);
      }
    }

    if (index_text == text.length()) {
      index_text = 0;
    }
  }
}

char joystick(int xValue, int yValue, int buttonState) {
  // X, Y 값의 범위를 통해 방향 결정, xValue와 yValue가 동시에 변하는 상황 Control (450~550 범위)
  if (xValue < 300 && 450 < yValue && yValue < 550) {
    return 'L';
  } else if (xValue > 700 && 450 < yValue && yValue < 550) {
    return 'R';
  } else if (yValue < 300 && 450 < xValue && xValue < 550) {
    return 'U';
  } else if (yValue > 700 && 450 < xValue && xValue < 550) {
    return 'D';
  }

  // 조이스틱 버튼이 눌렸는지 확인
  if (buttonState == LOW) {
    return 'P';
  }

  return '0'; // 아무 방향도 감지되지 않음
}
