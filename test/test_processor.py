import re

class SentenceManager:
    def __init__(self, text):
        self.sentences = re.split(r'(?<=[.!?]) +', text.strip())
        self.current_index = 0

    def get_current_sentence(self):
        """현재 문장을 반환"""
        return self.sentences[self.current_index]

    def move_up(self):
        """이전 문장으로 이동"""
        if self.current_index > 0:
            self.current_index -= 1
        return self.get_current_sentence()

    def move_down(self):
        """다음 문장으로 이동"""
        if self.current_index < len(self.sentences) - 1:
            self.current_index += 1
        return self.get_current_sentence()

    def is_valid_index(self, index):
        """주어진 인덱스가 유효한지 확인"""
        return 0 <= index < len(self.sentences)

    def __len__(self):
        """문장 개수 반환"""
        return len(self.sentences)

    def __repr__(self):
        """전체 문장 리스트와 현재 인덱스 표시"""
        return f"SentenceManager({self.sentences}, Current Index: {self.current_index})"

text = """It was the best of times, it was the worst of times.
We had everything before us, we had nothing before us.
"""
sentence_manager = SentenceManager(text)

print(list(text))