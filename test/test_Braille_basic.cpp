#include <Arduino.h>

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

int controlPins[6] = {2, 3, 4, 5, 6, 7};
char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
String text;
char letter;
int index;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i <= 5; i++) {
    pinMode(controlPins[i], OUTPUT);
  }
}

void loop() {
  // 시리얼 입력을 기다림
  if (Serial.available() > 0) {
    text = Serial.readString(); // 컴퓨터로부터 문자열 입력받기
    Serial.print("Received text: ");
    Serial.println(text);

    for (int i = 0; i < text.length(); i++) {
      letter = text[i];
      
      // 공백이나 알파벳이 아닌 문자는 무시
      if (isAlpha(letter)) {
        letter = toLowerCase(letter); // 소문자로 변환

        // 입력된 문자에 해당하는 알파벳 배열 인덱스 찾기
        for (int j = 0; j < 26; j++) {
          if (letter == alphabet[j]) {
            index = j;
            break;
          }
        }

        // 점자 패턴 출력
        for (int k = 0; k <= 5; k++) {
          digitalWrite(controlPins[k], braille[index][k]);
        }
        
        delay(500);  // 문자 표시 시간

        // 점자 패턴을 끄고 0.5초 대기
        for (int k = 0; k <= 5; k++) {
          digitalWrite(controlPins[k], LOW);
        }
        delay(500);  // 패턴을 끈 상태로 대기
      }
    }
  }
}