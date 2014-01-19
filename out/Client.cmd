REM start python ../tools/can/CanBusServer.py --server 8000
REM start python ../tools/can/uds.py --port 8999 0x731 0x732


REM GTK platform
start GtkUds.exe 8999
start GtkCan.exe 8000 8999

