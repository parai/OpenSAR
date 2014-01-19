@echo off
REM start python ../tools/can/CanBusServer.py --server 8000	
REM start python ../tools/can/com.py
REM start python ../tools/can/uds.py --port 8998 0x731 0x732
REM REM start python ../tools/can/uds.py --port 8999 0x741 0x742
REM cd ../tools/simulator
REM python main.py
REM start OpenSAR.exe

REM GTK platform
start OpenSAR.exe
start GtkUds.exe 8999
start GtkCan.exe 8000 8999

