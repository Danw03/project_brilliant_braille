#include <Arduino.h>

#define JOYSTICK_X_PIN A0 // 조이스틱 X축 핀
#define JOYSTICK_Y_PIN A1 // 조이스틱 Y축 핀
#define JOYSTICK_BTN_PIN 8 // 조이스틱 푸쉬 버튼 핀

void setup() {
  Serial.begin(9600);
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
}

char joystick(int xValue, int yValue, int buttonState); // 조이스틱 값을 컨트롤해서 loop 문 안에서 활용

void loop() {
  int xValue = analogRead(JOYSTICK_X_PIN);
  int yValue = analogRead(JOYSTICK_Y_PIN);
  int buttonState = digitalRead(JOYSTICK_BTN_PIN);

  char joystickDirection = joystick(xValue, yValue, buttonState);

  Serial.println(joystickDirection);

  delay(200);


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