import serial
import time
import threading

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
        return self.get_current_sentence()

# 아두이노 설정
arduino = serial.Serial(port='COM3', baudrate=9600, timeout=1)
time.sleep(2)

# 문장 리스트 설정
sentences = [
    "asfjkasdhfuhas", "safjkah", "asfkjlsdal", "fsdaff" "55555.",
    "66666.", "77777.", "88888.", "99999."
]
sentence_manager = SentenceManager(sentences)

# 아두이노로 문장 전송

def send_sentence(sentence):
    arduino.write(f"{sentence}\n".encode())
    print(f"Sent: {sentence}")

# 아두이노로부터 데이터 읽기

def read_from_arduino():
    while True:
        if arduino.in_waiting > 0:
            message = arduino.readline().decode().strip()
            print(f"Received: {message}")

            if message == "Up":
                current_sentence = sentence_manager.move_up()
                send_sentence(current_sentence)

            elif message == "Down":
                current_sentence = sentence_manager.move_down()
                send_sentence(current_sentence)

            elif message == "Speed Down" or message == "Speed Up":
                print(message)

            elif message == "Done":
                print("Arduino completed reading.")
                current_sentence = sentence_manager.move_down()
                send_sentence(current_sentence)

# 쓰레드 생성 및 실행
arduino_thread = threading.Thread(target=read_from_arduino, daemon=True)
arduino_thread.start()

# 초기 문장 전송
send_sentence(sentence_manager.get_current_sentence())

# 메인 루프
try:
    while arduino_thread.is_alive():
        time.sleep(0.1)
except KeyboardInterrupt:
    send_sentence("Done")
    print("Program terminated by user.")
