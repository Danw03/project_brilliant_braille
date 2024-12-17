#include <Arduino.h>

#define JOYSTICK_X_PIN A0 // 조이스틱 X축 핀
#define JOYSTICK_Y_PIN A1 // 조이스틱 Y축 핀
#define JOYSTICK_BTN_PIN 2 // 조이스틱 푸쉬 버튼 핀 (인터럽트 핀으로 연결)

// 점자 모듈 핀
int controlPins[6] = {3, 4, 5, 6, 7, 8};
char letter;
bool Capital = false; // 대문자 표시 관련
bool IfNumber = false; // 숫자 표시 관련

// 텍스트 데이터
const char* text[] = {
  "At least 35 people have been killed in a car attack in southern China, believed to be the deadliest known act of public violence in the country in decades.",
  "Police say a man crashed his car into a stadium in Zhuhai on Monday where he ran down groups of people exercising on the sports track.",
  "The \"serious and vicious attack\" also injured 45 people - among them elderly and children, local media report.",
  "Police say the 62-year-old driver, identified as a Mr Fan, appeared to have acted out of unhappiness over a divorce settlement.",
  "The president of COP29’s host country has told the UN climate conference that oil and gas are a \"gift of god\".",
  "Azerbaijan\'s President Ilham Aliyev criticised \"Western fake news\" about the country\'s emissions and said nations \"should not be blamed\" for having fossil fuel reserves.",
  "The country plans to expand gas production by up to a third over the next decade.",
  "Shortly afterwards, UN chief António Guterres told the conference that doubling down on the use of fossil fuels was \"absurd\".",
  "He said the \"clean energy revolution\" had arrived and that no government could stop it.",
  "Separately, UK prime minister Sir Keir Starmer pledged further reductions on emissions, saying the UK will now aim for an 81%% decrease by 2035. The UK called for other countries to match the new target."
};

int currentIndex = 0;
int charIndex = 0;
bool isPaused = false;
float speedMultiplier = 1.0;
int textLength = sizeof(text) / sizeof(text[0]);

unsigned long previousMillis = 0;
unsigned long interval = 300;

// 인터럽트 플래그와 디바운싱 타이머
volatile bool buttonPressed = false;
unsigned long lastInterruptTime = 0;

// 점자 출력 함수
void displayBraille(int symbol[6]) {
  for (int k = 0; k < 6; k++) {
    digitalWrite(controlPins[k], symbol[k]);
  }
  delay(500);
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
    Capital = true;
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
    IfNumber = true;
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
  if(Capital){
    int Capital_symbol[6] = {0,0,0,0,0,1};
    displayBraille(Capital_symbol);
    Capital = false;
  }

  // 숫자라면 숫자 표시를 먼저 출력
  if(IfNumber) {
    int Number_symbol[6] = {0,0,1,1,1,1};
    displayBraille(Number_symbol);
    IfNumber = false;
  }

  displayBraille(symbol);
}


// 조이스틱 방향 계산 함수
char joystick(int xValue, int yValue) {
  if (xValue < 300 && 450 < yValue && yValue < 550) {
    return 'L';
  } else if (xValue > 700 && 450 < yValue && yValue < 550) {
    return 'R';
  } else if (yValue < 300 && 450 < xValue && xValue < 550) {
    return 'U';
  } else if (yValue > 700 && 450 < xValue && xValue < 550) {
    return 'D';
  }
  return '0';
}

// 인터럽트 서비스 루틴 (ISR)
void togglePause() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 200) { // 디바운싱: 200ms 이내 반복 입력 무시
    buttonPressed = true; // 플래그 설정
    lastInterruptTime = interruptTime;
  }
}


void setup() {
  Serial.begin(9600);

  // 점자 모듈 핀 설정
  for (int i = 0; i < 6; i++) {
    pinMode(controlPins[i], OUTPUT);
  }

  // 조이스틱 핀 설정
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);

  // 인터럽트 설정
  attachInterrupt(digitalPinToInterrupt(JOYSTICK_BTN_PIN), togglePause, FALLING);
}

void loop() {
  unsigned long currentMillis = millis();

  // 인터럽트 플래그 확인
  if (buttonPressed) {
    buttonPressed = false; // 플래그 초기화
    isPaused = !isPaused; // 일시정지/재개 토글
    Serial.println(isPaused ? "Paused" : "Resumed");
  }

  // 조이스틱 입력 확인 (주기적으로 처리)
  if (!isPaused) {
    int xValue = analogRead(JOYSTICK_X_PIN);
    int yValue = analogRead(JOYSTICK_Y_PIN);
    char joystickDirection = joystick(xValue, yValue);

    if (joystickDirection == 'L') {
      speedMultiplier *= (2.0 / 3.0);
      speedMultiplier = max(speedMultiplier, 0.25);
      delay(200);
      Serial.println("LEFT");
    } else if (joystickDirection == 'R') {
      speedMultiplier *= 1.5;
      speedMultiplier = min(speedMultiplier, 2.0);
      delay(200);
      Serial.println("RIGHT");
    } else if (joystickDirection == 'U') {
      if (currentIndex > 0) {
        currentIndex--;
        charIndex = 0;
        Serial.println("UP");
      }
    } else if (joystickDirection == 'D') {
      if (currentIndex < textLength - 1) {
        currentIndex++;
        charIndex = 0;
        Serial.println("DOWN");
      }
    }
  }

  // 텍스트 출력
  if (!isPaused && currentMillis - previousMillis >= interval / speedMultiplier) {
    previousMillis = currentMillis;

    char currentChar = text[currentIndex][charIndex];
    if (currentChar != '\0') {
      displayCharacter(currentChar);
      Serial.print(currentChar);
      charIndex++;
    } else {
      charIndex = 0;
      currentIndex++;
      if (currentIndex >= textLength) {
        currentIndex = 0;
      }
      Serial.println();
    }
  }
}

