from ComApi import *
from Osek import *
import re

def tInt(strnum):
    if(strnum.find('0x')!=-1 or strnum.find('0X')!=-1):
        return int(strnum,16)
    else:
        return int(strnum,10)
    
def Task10ms():
    stage = 0
    time  = 0
    percent = 0  # 1 = 0.1%
    while True:
        if(stage == 0):
            percent += 5
            if(percent > 998):
                percent == 998 
                stage = 1
                time = 0
        elif(stage == 1):
            time += 1
            if(time > 200):
                stage = 2
        elif(stage == 2):
            percent -= 5
            if(percent < 5):
                percent = 5
                stage =3
                time = 0
        elif(stage == 3):
            time += 1
            if(time > 200):
                stage = 0
        VehicleSpeed = 28000*percent/1000
        TachoSpeed  = 10000*percent/1000
        lSetDebug(False)
        Com_SendSignal(COM_SID_VehicleSpeed,VehicleSpeed)
        Com_SendSignal(COM_SID_TachoSpeed,TachoSpeed)
        lSetDebug(True)
        Sleep(10)

def Task1000ms():
    LedSts = 0
    while True:
        LedSts += 1
        lSetDebug(False)
        Com_SendSignal(COM_SID_Led1Sts,LedSts%3)
        Com_SendSignal(COM_SID_Led2Sts,LedSts%3)
        Com_SendSignal(COM_SID_Led3Sts,LedSts%3)
        lSetDebug(True)
        Sleep(1000)
        
TASK1 = DeclareTask(Task10ms) 
TASK2 = DeclareTask(Task1000ms)
             
if __name__ == '__main__':   
    ActivateTask(TASK1)
    ActivateTask(TASK2)
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
                if(input == ''):
                    ShowSignalList()
                else:
                    print 'Error Command!'
        except:
            print 'Error Command!'
        
