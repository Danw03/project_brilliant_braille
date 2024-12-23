#include <Arduino.h>

#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1
#define JOYSTICK_BTN_PIN 2

// 점자 모듈 핀 설정
int controlPins[6] = {3, 4, 5, 6, 7, 8};

// 상태 변수
char buffer[256];
bool isNewSentence = false;
bool isPaused = false;
bool isCapital = false;
bool isNumber = false;
int charIndex = 0;
float speedMultiplier = 1.0;

// 인터럽트 플래그 및 디바운싱
volatile bool buttonPressed = false;
unsigned long lastInterruptTime = 0;
unsigned long lastBrailleUpdate = 0;
unsigned long lastJoystickUpdate = 0;

void displayBraille(const int symbol[6]) {
  for (int k = 0; k < 6; k++) {
    digitalWrite(controlPins[k], symbol[k]);
  }
  delay(200);
  for (int k = 0; k < 6; k++) {
    digitalWrite(controlPins[k], LOW);
  }
}

void displayCharacter(char letter) {
  int symbol[6] = {0, 0, 0, 0, 0, 0};

  // 소문자 알파벳
  if (letter >= 'a' && letter <= 'z'){
    switch (letter) {
      case 'a': symbol[0] = 1; break;
      case 'b': symbol[0] = 1; symbol[1] = 1; break;
      case 'c': symbol[0] = 1; symbol[3] = 1; break;
      case 'd': symbol[0] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'e': symbol[0] = 1; symbol[4] = 1; break;
      case 'f': symbol[0] = 1; symbol[1] = 1; symbol[3] = 1; break;
      case 'g': symbol[0] = 1; symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'h': symbol[0] = 1; symbol[1] = 1; symbol[4] = 1; break;
      case 'i': symbol[1] = 1; symbol[3] = 1; break;
      case 'j': symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'k': symbol[0] = 1; symbol[2] = 1; break;
      case 'l': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; break;
      case 'm': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; break;
      case 'n': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'o': symbol[0] = 1; symbol[2] = 1; symbol[4] = 1; break;
      case 'p': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; break;
      case 'q': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'r': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[4] = 1; break;
      case 's': symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; break;
      case 't': symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'u': symbol[0] = 1; symbol[2] = 1; symbol[5] = 1; break;
      case 'v': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[5] = 1; break;
      case 'w': symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case 'x': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; symbol[5] = 1; break;
      case 'y': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case 'z': symbol[0] = 1; symbol[2] = 1; symbol[4] = 1; symbol[5] = 1; break;
    }
  }

  // 대문자 알파벳
  else if (letter >= 'A' && letter <= 'Z'){
    isCapital = true;
    switch (letter) {
      case 'A': symbol[0] = 1; break;
      case 'B': symbol[0] = 1; symbol[1] = 1; break;
      case 'C': symbol[0] = 1; symbol[3] = 1; break;
      case 'D': symbol[0] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'E': symbol[0] = 1; symbol[4] = 1; break;
      case 'F': symbol[0] = 1; symbol[1] = 1; symbol[3] = 1; break;
      case 'G': symbol[0] = 1; symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'H': symbol[0] = 1; symbol[1] = 1; symbol[4] = 1; break;
      case 'I': symbol[1] = 1; symbol[3] = 1; break;
      case 'J': symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'K': symbol[0] = 1; symbol[2] = 1; break;
      case 'L': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; break;
      case 'M': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; break;
      case 'N': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'O': symbol[0] = 1; symbol[2] = 1; symbol[4] = 1; break;
      case 'P': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; break;
      case 'Q': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'R': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[4] = 1; break;
      case 'S': symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; break;
      case 'T': symbol[1] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case 'U': symbol[0] = 1; symbol[2] = 1; symbol[5] = 1; break;
      case 'V': symbol[0] = 1; symbol[1] = 1; symbol[2] = 1; symbol[5] = 1; break;
      case 'W': symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case 'X': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; symbol[5] = 1; break;
      case 'Y': symbol[0] = 1; symbol[2] = 1; symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case 'Z': symbol[0] = 1; symbol[2] = 1; symbol[4] = 1; symbol[5] = 1; break;
    }
  }

  // 숫자
  else if (letter >= '1' && letter <= '9'){
    isNumber = true;
    switch (letter) {
      case '1': symbol[0] = 1; break;
      case '2': symbol[0] = 1; symbol[1] = 1; break;
      case '3': symbol[0] = 1; symbol[3] = 1; break;
      case '4': symbol[0] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case '5': symbol[0] = 1; symbol[4] = 1; break;
      case '6': symbol[0] = 1; symbol[1] = 1; symbol[3] = 1; break;
      case '7': symbol[0] = 1; symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; break;
      case '8': symbol[0] = 1; symbol[1] = 1; symbol[4] = 1; break;
      case '9': symbol[1] = 1; symbol[3] = 1; break;
      case '0': symbol[1] = 1; symbol[3] = 1; symbol[4] = 1; break;
    }
  }

  // 특수 기호
  else {
    switch (letter) {
      case ',': symbol[1] = 1; break;
      case ';': symbol[1] = 1; symbol[2] = 1; break;
      case ':': symbol[1] = 1; symbol[4] = 1; break;
      case '.': symbol[1] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '?': symbol[1] = 1; symbol[2] = 1; symbol[5] = 1; break;
      case '!': symbol[1] = 1; symbol[2] = 1; symbol[4] = 1; break;
      case '-': symbol[2] = 1; symbol[5] = 1; break;
      case '(': symbol[4] = 1; symbol[5] = 1; break;
      case ')': symbol[4] = 1; symbol[5] = 1; break;
      case '[': symbol[3] = 1; symbol[5] = 1; break;
      case ']': symbol[3] = 1; symbol[5] = 1; break;
      case '{': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '}': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '/': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '\\': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '&': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '$': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '%': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '@': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '*': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '~': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '_': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '\'': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '\"': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '°': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '+': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '×': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '·': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '÷': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '=': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '<': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      case '>': symbol[3] = 1; symbol[4] = 1; symbol[5] = 1; break;
      default: return; // 유효하지 않은 문자
    }
  }

  // 대문자라면 대문자 표시를 먼저 출력
  if(isCapital){
    int Capital_symbol[6] = {0,0,0,0,0,1};
    displayBraille(Capital_symbol);
    isCapital = false;
  }

  // 숫자라면 숫자 표시를 먼저 출력
  if(isNumber) {
    int Number_symbol[6] = {0,0,1,1,1,1};
    displayBraille(Number_symbol);
    isNumber = false;
  }

  displayBraille(symbol);
}

char getJoystickDirection(int xValue, int yValue) {
  if (xValue < 300 && yValue > 450 && yValue < 550) {
    return 'U';
  } else if (xValue > 700 && yValue > 450 && yValue < 550) {
    return 'D';
  } else if (yValue < 300 && xValue > 450 && xValue < 550) {
    return 'R';
  } else if (yValue > 700 && xValue > 450 && xValue < 550) {
    return 'L';
  }
  return '0';
}

void handlePauseToggle() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 800) {
    buttonPressed = true;
    lastInterruptTime = interruptTime;
  }
}

void updateReadingSpeed(char direction) {
  if (direction == 'L') {
    speedMultiplier = max(0.25, speedMultiplier * 0.67);
  } else if (direction == 'R') {
    speedMultiplier = min(2.0, speedMultiplier * 1.5);
  }
}

void receiveSentence() {
  if (Serial.available() > 0) {
    int length = Serial.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
    buffer[length] = '\0';
    isNewSentence = true;
    charIndex = 0;
  }
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 6; i++) {
    pinMode(controlPins[i], OUTPUT);
  }

  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(JOYSTICK_BTN_PIN), handlePauseToggle, FALLING);
}

void loop() {
  unsigned long currentMillis = millis();

  // 일시정지 토글
  if (buttonPressed) {
    buttonPressed = false;
    isPaused = !isPaused;
    Serial.println(isPaused ? "Paused" : "Resumed");
  }

  // 조이스틱 입력 처리
  if (!isPaused && (currentMillis - lastJoystickUpdate >= 200)) {
    lastJoystickUpdate = currentMillis;
    int xValue = analogRead(JOYSTICK_X_PIN);
    int yValue = analogRead(JOYSTICK_Y_PIN);
    char direction = getJoystickDirection(xValue, yValue);

    if (direction == 'U') Serial.println("Up");
    if (direction == 'D') Serial.println("Down");
    if (direction == 'L' || direction == 'R') {
      updateReadingSpeed(direction);
      Serial.println(direction == 'L' ? "Speed Down" : "Speed Up");
    }
  }

  // 새 문장 읽기
  if (!isPaused && isNewSentence && (currentMillis - lastBrailleUpdate >= 800 / speedMultiplier)) {
    lastBrailleUpdate = currentMillis;
    char currentChar = buffer[charIndex];
    if (currentChar != '\0') {
      displayCharacter(currentChar);
      charIndex++;
    } else {
      isNewSentence = false;
      Serial.println("Done");
    }
  }

  // 문장 수신
  receiveSentence();
}