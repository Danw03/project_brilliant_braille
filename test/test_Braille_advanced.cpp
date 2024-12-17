#include <Arduino.h>

/*

The objective of this program is to take a string and represent it
using the braille display one letter at a time.

The braille alphabet uses characters with six dots
in three rows and two columns. the dots are labeled 1-6
as follows:

  1  4
  2  5
  3  6

*/

// store the code for each letter a-z in a 2D array
int braille[68][2][6] = {
  //알파벳 26개
  {{1,0,0,0,0,0}, {0,0,0,0,0,0}},  // a (first bump is raised, others are lowered)
  {{1,1,0,0,0,0}, {0,0,0,0,0,0}}, // b (first and second bumps are raised)
  {{1,0,0,1,0,0}, {0,0,0,0,0,0}}, // c (and so on...)
  {{1,0,0,1,1,0}, {0,0,0,0,0,0}}, // d
  {{1,0,0,0,1,0}, {0,0,0,0,0,0}}, // e
  {{1,1,0,1,0,0}, {0,0,0,0,0,0}}, // f
  {{1,1,0,1,1,0}, {0,0,0,0,0,0}}, // g
  {{1,1,0,0,1,0}, {0,0,0,0,0,0}}, // h
  {{0,1,0,1,0,0}, {0,0,0,0,0,0}}, // i
  {{0,1,0,1,1,0}, {0,0,0,0,0,0}}, // j
  {{1,0,1,0,0,0}, {0,0,0,0,0,0}}, // k
  {{1,1,1,0,0,0}, {0,0,0,0,0,0}}, // l
  {{1,0,1,1,0,0}, {0,0,0,0,0,0}}, // m
  {{1,0,1,1,1,0}, {0,0,0,0,0,0}}, // n
  {{1,0,1,0,1,0}, {0,0,0,0,0,0}}, // o
  {{1,1,1,1,0,0}, {0,0,0,0,0,0}}, // p
  {{1,1,1,1,1,0}, {0,0,0,0,0,0}}, // q
  {{1,1,1,0,1,0}, {0,0,0,0,0,0}}, // r
  {{0,1,1,1,0,0}, {0,0,0,0,0,0}}, // s
  {{0,1,1,1,1,0}, {0,0,0,0,0,0}}, // t
  {{1,0,1,0,0,1}, {0,0,0,0,0,0}}, // u
  {{1,1,1,0,0,1}, {0,0,0,0,0,0}}, // v
  {{0,1,0,1,1,1}, {0,0,0,0,0,0}}, // w
  {{1,0,1,1,0,1}, {0,0,0,0,0,0}}, // x
  {{1,0,1,1,1,1}, {0,0,0,0,0,0}}, // y
  {{1,0,1,0,1,1}, {0,0,0,0,0,0}}, // z

  //기호 일단 26개(+2 작은따옴표 큰따옴표 닫는것 까지 있으면)
  {{0,1,0,0,0,0}, {0,0,0,0,0,0}}, // ,
  {{0,1,1,0,0,0}, {0,0,0,0,0,0}}, // ;
  {{0,1,0,0,1,0}, {0,0,0,0,0,0}}, // :
  {{0,1,0,0,1,1}, {0,0,0,0,0,0}}, // .
  {{0,1,1,0,0,1}, {0,0,0,0,0,0}}, // ?
  {{0,1,1,0,1,0}, {0,0,0,0,0,0}}, // !
  {{0,0,1,0,0,1}, {0,0,0,0,0,0}}, // -
  {{0,0,0,0,1,0}, {1,1,0,0,0,1}}, // (
  {{0,0,0,0,1,0}, {0,0,1,1,1,0}}, // )
  {{0,0,0,1,0,1}, {1,1,0,0,0,1}}, // [
  {{0,0,0,1,0,1}, {0,0,1,1,1,0}}, // ]
  {{0,0,0,1,1,1}, {1,1,0,0,0,1}}, // {
  {{0,0,0,1,1,1}, {0,0,1,1,1,0}}, // }
  {{0,0,0,1,1,1}, {0,0,1,1,0,0}}, // /
  {{0,0,0,1,1,1}, {1,0,0,0,0,1}}, // '\'(백슬래시)
  {{0,0,0,1,0,0}, {1,1,1,1,0,1}}, // &
  {{0,0,0,1,0,0}, {0,1,1,1,0,0}}, // $
  {{0,0,0,1,0,1}, {0,0,1,0,1,1}}, // %
  // {{0,0,1,0,0,0}, {0,0,0,0,0,0}}, // '(아포스트로피) ***작은 따옴표랑 어떻게 구별할 건지 해야 됨.
  {{0,0,0,1,0,0}, {1,0,0,0,0,0}}, // @
  {{0,0,0,0,1,0}, {0,0,1,0,1,0}}, // *
  {{0,0,0,1,0,0}, {0,0,1,0,0,1}}, // ~
  {{0,0,0,1,0,1}, {0,0,1,0,0,1}}, // _
  {{0,0,1,0,0,0}, {0,1,1,0,0,1}}, // '  // 내 컴퓨터로는 여는 따옴표 닫는 따옴표가 구별되지 않아 그냥 하나로 통일했음 - 다른거에서 넣었을때 여는거 닫는거 구별이 되는지 테스트해보기(꼭 테스트해봐야됨!!)
  {{0,1,1,0,0,1}, {0,0,0,0,0,0}}, // " 
  {{0,0,0,1,1,0}, {0,1,0,1,1,0}}, // °

  //연산부호 11개(생각보다 문자 부분에서 겹칠수 있기때문에 따로하는게 나을 수도 있을 것 같음)
  {{0,0,0,0,1,0}, {0,1,1,0,1,0}}, // +
  // {{0,0,0,0,1,0}, {0,0,1,0,0,1}}, // - ***하이픈이랑 어떻게 구별한건지 고민
  {{0,0,0,0,1,0}, {0,1,1,0,0,1}}, // × 3개 다 곱하기 기호
  // {{0,0,0,0,1,0}, {0,1,1,0,0,1}}, // *
  {{0,0,0,0,1,0}, {0,1,1,0,0,1}}, // · 
  {{0,0,0,0,1,0}, {0,0,1,1,0,0}}, // ÷ 2개 다 곱하기 기호
  // {{0,0,0,0,1,0}, {0,0,1,1,0,0}}, // / ***슬래시랑 어떻게 구별할건지 고민
  {{0,0,0,0,1,0}, {0,1,1,0,1,1}}, // =
  {{0,0,0,1,0,0}, {0,0,1,1,0,1}}, // <
  {{0,0,0,1,0,0}, {0,0,1,0,1,1}}, // >
  // {{0,1,0,0,1,1}, {0,0,0,0,0,0}}, // .(소수점) ***마침표랑 어떻게 구별할 건지 고민

  /*
  2번 점자가 필요한 기호들은 어떻게할지 고민해보기
  */

  // 숫자 10개
  {{1,0,0,0,0,0}, {0,0,0,0,0,0}}, // 1
  {{1,1,0,0,0,0}, {0,0,0,0,0,0}}, // 2
  {{1,0,0,1,0,0}, {0,0,0,0,0,0}}, // 3
  {{1,0,0,1,1,0}, {0,0,0,0,0,0}}, // 4
  {{1,0,0,0,1,0}, {0,0,0,0,0,0}}, // 5
  {{1,1,0,1,0,0}, {0,0,0,0,0,0}}, // 6
  {{1,1,0,1,1,0}, {0,0,0,0,0,0}}, // 7
  {{1,1,0,0,1,0}, {0,0,0,0,0,0}}, // 8
  {{0,1,0,1,0,0}, {0,0,0,0,0,0}}, // 9
  {{0,1,0,1,1,0}, {0,0,0,0,0,0}} // 0

};

//이게 두개 점자 읽는 코드, Capital and Number indicators
int Capital_symbol[6] = {0,0,0,0,0,1};
int Number_symbol[6] = {0,0,1,1,1,1};

// 변수 정의
int controlPins[6] = {2,3,4,5,6,7}; //array of the Arduino pins used to control the solenoids
int buttonPin = 8; // pin for the button

String text = "Hello"; // **string to convert to braille

char letter; // variable for getting the individual letter of the text[] array
int index; // variable for finding the index of letter in alphabet
int length = text.length(); // length of the text[] array (I this adds one because of the weird zero padding thing?)
bool Capital = false; //대문자표 관련
bool IfNumber = false; //수표 관련
char previousLetter = '\0'; //중복 문자 관련



void setup(){ // setup code that only runs once
  // initialize serial communication (allows printing to serial monitor)
  Serial.begin(9600);
  // set control pins as outputs
  for(int i=0; i<6; i++){
    pinMode(controlPins[i],OUTPUT);
  }
  // set button pin as input
  pinMode(buttonPin,INPUT);
}

void displayBraille(int symbol[2][6]){
  for (int part = 0; part < 2; part++){ // 두 칸 점자를 차례로 출력
    if (symbol[part][0] == 0 && symbol[part][1] == 0 && symbol[part][2] == 0 && symbol[part][3] == 0 && symbol[part][4] == 0 && symbol[part][5] == 0){
      break; // 두 번째 칸이 비어 있으면 중단
    }
    for (int k = 0; k < 6; k++){
      digitalWrite(controlPins[k], symbol[part][k]);
      Serial.println(symbol[part][k]);
    }
    delay(500); // 각 칸사이에 잠깐의 딜레이를 추가
  }
}

int getBrailleIndex(char letter) {
  //소문자 알파벳
  if (letter >= 'a' && letter <= 'z'){
    return (int)(letter - 'a'); // a부터 시작하는 인덱스 (0-25 중 인덱스 반환)
  }

  //대문자 알파벳
  else if (letter >= 'A' && letter <= 'Z'){
    Capital = true;
    return (int)(letter - 'A'); // 대문자와 소문자 배열을 구분하지 않고 (0-25 인덱스 반환)
  }

    // 숫자
  else if (letter >= '1' && letter <= '9'){
    IfNumber = true;
    return (int)(letter - '1') + 58;
  }
  else if (letter == '0'){
    IfNumber = true;
    return 67; // 0은 마지막 숫자로 위치
  }

  //특수 기호
  switch (letter) {
    case ',': return 26;
    case ';': return 27;
    case ':': return 28;
    case '.': return 29;
    case '?': return 30;
    case '!': return 31;
    case '-': return 32;
    case '(': return 33;
    case ')': return 34;
    case '[': return 35;
    case ']': return 36;
    case '{': return 37;
    case '}': return 38;
    case '/': return 39;
    case '\\': return 40; //이렇게 하는거 맞는지 확인 - 백슬래시 하나만 하면 인식 안됨
    case '&': return 41;
    case '$': return 42;
    case '%': return 43;
    // case '\'': return 44; 작은따옴표랑 같은 걸로 취급
    case '@': return 44;
    case '*': return 45;
    case '~': return 46;
    case '_': return 47;
    case '\'': return 48; //마찬가지로 어퍼스트로피랑 비교
    case '\"': return 59;
    case '°': return 50;

    case '+': return 51;
    // case '-': return 53; 하이픈이랑 뺄샘을 같은걸로 알게 해야함 - 처음 사용설명서때 "하이픈, 뺄셈"이라고 이야기 하기
    case '×': return 52;
    //case '*': return 55; 그냥 아스테리크로 읽어서 알아서 곱셈
    case '·': return 53;
    case '÷': return 54;
    // case '/': return 58; 그냥 /로 읽어서 알아서 나누기
    case '=': return 55;
    case '<': return 56;
    case '>': return 57;
    // case '.': return 62; // 마찬가지로 그냥 마침표로 알아두기
  }

  return -1; //추가로 -1을 받았을 때 문자를 배열로 넣어서 마지막에 "오류난 문자는 이거 입니다"이런식으로 음성으로 나오게 한다든가 체크해두는 식으로 하기
}

void loop(){  // code that loops forever

  // 여기를 나중에 알파벳이냐 기호냐 숫자냐 이런거를 변수를 만들어서 확인하고 기어가 움직이도록 만들면 될것 같음

  
  // loop through each letter of text input
  


  for(int i=0; i<length; i++){
    letter = text[i]; // get i'th letter of text
    if (letter == previousLetter) {
      for (int k = 0; k < 6; k++) digitalWrite(controlPins[k], LOW);
      delay(500); //Avoid repeat singal overlap
      continue;
    }

    index = getBrailleIndex(letter); // find index of this letter in the alphabet
    Serial.println(index);
    
    if (index == -1) {
      Serial.println("Invalid character");
      delay(500);
      continue;
    }

    // Capital 알파벳이라면 대문자표를 먼저 출력
    if(Capital){
      for(int k = 0; k<=5; k++) digitalWrite(controlPins[k], Capital_symbol[k]);
      delay(500);
      Capital = false;
    }
    
    // Number이라면 수표를 먼저 출력
    if(IfNumber) {
      for(int k = 0; k<=5; k++) digitalWrite(controlPins[k], Number_symbol[k]);
      delay(500);
      IfNumber = false;
    }

    displayBraille(braille[index]);
    
    // print the letter and its index to make sure this is correct
    Serial.print(letter);
    Serial.print(" ");
    Serial.println(index);
    previousLetter = letter;

    // Button press for next letter
    while (digitalRead(buttonPin) == LOW) {
    //delay(50);
    };
    delay(1000); // Slight delay after button press to prepare for the next character

  }
}  
    
  
    /*
    ***중복되는 문자 구분할 수 있게하도록 만들어야 함 예)Hello에서 l연속으로 두번
    digital write하기 전에 즉 솔레노이드가 출력되기 전에서 조건문을 만들어서 
    원래 controlPins의 상태와 새롭게 두번째 인수에 넣는 6요소배열 요소랑 비교해서
    같으면 {0,0,0,0,0,0}을 출력하고 바로 다음 문자가 나오는 방식으로 만들기
    위에 있는 3개 조건문 보다 더 상위계층의 if문을 만들어 중복되는 문자인지 파악하기
    */

    // wait for button press before showing the next letter
    //계속 돌아가도록 제어문을 만든것
