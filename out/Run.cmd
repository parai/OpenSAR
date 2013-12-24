@echo off
REM start python ../tools/can/CanBusServer.py --server 8000	
start python ../tools/can/com.py
REM start python ../tools/can/uds.py --port 8998 0x731 0x732
REM start python ../tools/can/uds.py --port 8999 0x741 0x742
cd ../tools/simulator
python main.py
REM start OpenSAR.exe
pause


