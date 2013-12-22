from ComApi import *
from Osek import *
import re

def tInt(strnum):
    if(strnum.find('0x')!=-1 or strnum.find('0X')!=-1):
        return int(strnum,16)
    else:
        return int(strnum,10)
    
def Task100ms():
    VehicleSpeed = 0
    while True:
        VehicleSpeed += 16
        Com_SendSignal(COM_SID_VehicleSpeed,VehicleSpeed)
        Sleep(100)

def Task1000ms():
    VehicleSpeed = 0
    while True:
        VehicleSpeed += 16
        Com_SendSignal(COM_SID_VehicleSpeed,VehicleSpeed)
        Sleep(1000)
        
TASK1 = DeclareTask(Task100ms) 
TASK2 = DeclareTask(Task1000ms)
             
if __name__ == '__main__':   
#     ActivateTask(TASK1)
#     ActivateTask(TASK2)
    reCmdS = re.compile(r'(send)\s+(\d+)\s+(\d+)')
    reCmdR = re.compile(r'(read)\s+(\d+)')
    while True:
        try:
            input = raw_input("(read/send sigId [value]:")
            if(reCmdS.search(input)):
                reG = reCmdS.search(input).groups()
                Com_SendSignal(tInt(reG[1]),tInt(reG[2]))
            elif(reCmdR.search(input)):
                reG = reCmdR.search(input).groups()
                print Com_ReadSignal(tInt(reG[1]))
            else:
                print 'Error Command!'
        except:
            print 'Error Command!'
        
