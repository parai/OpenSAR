@echo off
start python ../tools/can/CanBusServer.py --server 8000
start python ../tools/can/uds.py --port 8998 0x731 0x732
start python ../tools/can/uds.py --port 8999 0x741 0x742
REM start OpenSAR.exe


