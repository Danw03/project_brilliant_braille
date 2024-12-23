import serial
import time
import threading

# 문장 관리 클래스
class SentenceManager:
    def __init__(self, sentences):
        self.sentences = sentences
        self.current_index = 0

    def get_current_sentence(self):
        return self.sentences[self.current_index]

    def move_up(self):
        if self.current_index > 0:
            self.current_index -= 1
        return self.get_current_sentence()

    def move_down(self):
        if self.current_index < len(self.sentences) - 1:
            self.current_index += 1
        else:
            self.current_index = 0  # 첫 문장으로 돌아가기
        return self.get_current_sentence()
# 시리얼 포트 설정
try:
    arduino = serial.Serial(port='COM6', baudrate=9600, timeout=1)
    time.sleep(2)  # 아두이노 초기화 시간
    print("Arduino 연결 성공!")
except serial.SerialException as e:
    print(f"시리얼 포트 에러: {e}")
    exit(1)

# 문장 리스트 설정
sentences = [
    "All happy families are alike; each unhappy family is unhappy in its own way.",
    "Tomorrow is another day.",
    "Truth is, once you tell someone the truth, it becomes their problem.",
    "God is dead. God remains dead. And we have killed him.",
    "It is a truth universally acknowledged, that a single man in possession of a good fortune, must be in want of a wife.",
    "Not all those who wander are lost.",
    "There is no greater agony than bearing an untold story inside you.",
    "So we beat on, boats against the current, borne back ceaselessly into the past.",
    "It was the best of times, it was the worst of times.",
    "Stay gold, Ponyboy, stay gold.",
    "The only way out of the labyrinth of suffering is to forgive.",
    "You never really understand a person until you consider things from his point of view."
]

sentence_manager = SentenceManager(sentences)

# 문장을 아두이노로 전송
def send_sentence(sentence):
    try:
        arduino.write(f"{sentence}\n".encode())
        print(f"Sent to Arduino: {sentence}")
    except Exception as e:
        print(f"문장 전송 중 에러 발생: {e}")

# 아두이노 데이터 수신 및 처리
def read_from_arduino():
    while True:
        if arduino.in_waiting > 0:  # 아두이노에서 데이터 수신
            message = arduino.readline().decode().strip()
            print(f"Received: {message}")

            if message == "Up":
                current_sentence = sentence_manager.move_up()
                send_sentence(current_sentence)

            elif message == "Down":
                current_sentence = sentence_manager.move_down()
                send_sentence(current_sentence)

            elif message == "Speed Down" or message == "Speed Up":
                print(message)  # 속도 변경 메시지 출력 (추가 기능 가능)

            elif message == "Done":
                print("Arduino completed reading.")
                current_sentence = sentence_manager.move_down()
                send_sentence(current_sentence)

# 쓰레드로 데이터 수신 실행
arduino_thread = threading.Thread(target=read_from_arduino, daemon=True)
arduino_thread.start()

# 메인 실행 코드
if __name__ == "__main__":
    print("Starting to send sentences to Arduino...")
    try:
        # 초기 문장 전송
        send_sentence(sentence_manager.get_current_sentence())

        while True:  # 메인 루프
            time.sleep(0.1)  # CPU 점유율 낮춤
    except KeyboardInterrupt:
        print("프로그램 종료.")
    finally:
        arduino.close()
        print("Arduino 연결 종료.")
