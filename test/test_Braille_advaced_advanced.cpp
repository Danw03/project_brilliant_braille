#include <Arduino.h>
#define JOYSTICK_X_PIN A0 // 조이스틱 X축 핀
#define JOYSTICK_Y_PIN A1 // 조이스틱 Y축 핀
#define JOYSTICK_BTN_PIN 8 // 조이스틱 푸쉬 버튼 핀
//점자관련 변수 및 함수
int controlPins[6] = {2, 3, 4, 5, 6, 7};
char letter;
bool Capital = false; // 대문자 표시 관련
bool IfNumber = false; // 숫자 표시 관련

void setup() {
  //점자 관련
  Serial.begin(9600);
  for (int i = 0; i <= 5; i++) {
    pinMode(controlPins[i], OUTPUT);
  }
  // 조이스틱 관련
  Serial.begin(9600);
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(JOYSTICK_BTN_PIN, INPUT_PULLUP);
}

void displayBraille(int symbol[6]){
  for (int k = 0; k < 6; k++){
    digitalWrite(controlPins[k], symbol[k]);
    Serial.println(symbol[k]);
  }
  delay(500); // 각 칸 사이에 잠깐의 딜레이를 추가

  for (int k = 0; k <= 5; k++) {
    digitalWrite(controlPins[k], LOW);
  }
  delay(500);
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

// 조이스틱 관련 변수 및 함수


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

int currentIndex = 0;   // 한 text 안에서 문장 자체에 접근하는 index (조이스틱 U, D로 control)
int charIndex = 0;    // 한 문장 안에서 각각의 character에 접근하는 index (조이스틱 L, R로 control)
bool isPaused = false;  // 조이스틱 버튼으로 control 누를 때 마다 T, F 값 변환
float speedMultiplier = 1.0;
int textLength = sizeof(text) / sizeof(text[0]); // 배열의 크기를 구함



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

void loop() {
  int xValue = analogRead(JOYSTICK_X_PIN);
  int yValue = analogRead(JOYSTICK_Y_PIN);
  int buttonState = digitalRead(JOYSTICK_BTN_PIN);

  char joystickDirection = joystick(xValue, yValue, buttonState);

  if (joystickDirection == 'L') {
    speedMultiplier *= (2.0 / 3.0);   // L 조작을 하면 읽는 속도가 1.5배 느려짐 (X 2/3)
    delay(200); // 방향 변경 확인을 위한 지연 시간
  } else if (joystickDirection == 'R') {
    speedMultiplier *= 1.5;   // R 조작을 하면 읽는 속도가 1.5배 빨라짐
    delay(200);
  } else if (joystickDirection == 'U') {
    if (currentIndex > 0) {
      currentIndex -= 1;   // U 조작을 하고, currentIndex가 유효한 범위에 있는 경우
      charIndex = 0;
      delay(200);
    }
  } else if (joystickDirection == 'D') {
    if (currentIndex < textLength - 1) {
      currentIndex += 1;   // D 조작을 하고, currentIndex가 유효한 범위에 있는 경우
      charIndex = 0;
      delay(200);
    }
  } else if (joystickDirection == 'P') {
    isPaused = !isPaused;   // 조이스틱 버튼을 누르면 일시정지 혹은 재개
    delay(200);
  }

  if (!isPaused) {
    char currentChar = text[currentIndex][charIndex];
    if (currentChar != '\0') { // Null character check
      displayCharacter(currentChar); 
      Serial.print(currentChar);
      charIndex++;
    }

    if (text[currentIndex][charIndex] == '\0') {   // charIndex가 문장 끝에 도달했으면
      charIndex = 0;
      currentIndex++;
      if (currentIndex >= textLength) { // 배열 크기에 맞게 수정
        currentIndex = 0; // 텍스트가 끝나면 처음으로 돌아감
      }
      Serial.println();
    }
    delay(300 / speedMultiplier); // 읽는 속도 조절
  }
}