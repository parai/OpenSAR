@echo off
start python ../tools/can/CanBusServer.py --server 8000
start python ../tools/can/uds.py --port 8999
start OpenSAR.exe


