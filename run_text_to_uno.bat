@echo off
REM 아나콘다 가상환경 활성화
call "C:\Users\danw0\anaconda3\Scripts\activate.bat" project_bb

REM text_to_uno.py 실행
python "C:\Users\danw0\Project\The Brilliant Braille\src\text_to_uno.py"

REM 실행 결과 확인
pause
