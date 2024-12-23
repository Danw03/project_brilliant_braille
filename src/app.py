from flask import Flask, jsonify, send_from_directory
import subprocess
import os

app = Flask(__name__, static_folder='../docs')

# 프로젝트 루트 경로 설정
BASE_DIR = os.path.abspath(os.path.dirname(__file__))
PYTHON_SCRIPT_PATH = os.path.join(BASE_DIR, "text_to_uno.py")

# 전역 변수: 실행된 프로세스를 저장
process = None

@app.route('/')
def index():
    return send_from_directory('../', 'index.html')  # 루트 경로의 index.html 반환

@app.route('/run-python', methods=['POST'])
def run_python():
    global process
    try:
        if process is None:  # 프로세스가 실행 중이 아닐 때만 실행
            process = subprocess.Popen(["python", PYTHON_SCRIPT_PATH], shell=True)
            return jsonify({"status": "success", "message": "text_to_uno.py is running!"})
        else:
            return jsonify({"status": "running", "message": "text_to_uno.py is already running."})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)})

@app.route('/stop-python', methods=['POST'])
def stop_python():
    global process
    try:
        if process:
            process.terminate()  # 실행 중인 text_to_uno.py 프로세스 종료
            process = None
            return jsonify({"status": "success", "message": "text_to_uno.py has been stopped!"})
        else:
            return jsonify({"status": "error", "message": "No process is running."})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)})

# PDF 파일 서빙 라우트 추가
@app.route('/docs/<path:filename>')
def serve_pdf(filename):
    return send_from_directory('../docs', filename)  # docs 폴더에서 파일 서빙

if __name__ == "__main__":
    app.run(debug=True)
