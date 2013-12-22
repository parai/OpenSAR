
# Gen by easyCom

import threading,time
import sys
import socket
import UserString 

def lDebug(stri):
    print stri 
    
cPduTx=0
cPduRx=1
# Id Ref of Pdu
COM_MSG0_RX=0
COM_MSG1_TX=1
# Id Ref of Signal
COM_SID_VehicleSpeed=0
COM_SID_TachoSpeed=1
COM_SID_Led1Sts=2
COM_SID_Led2Sts=3
COM_SID_Led3Sts=4
COM_SID_Year=5
COM_SID_Month=6
COM_SID_Day=7
COM_SID_Hour=8
COM_SID_Minute=9
COM_SID_Second=10
# Pdu Obj = [id,[data],type]
cPduCanId=0
cPduData=1
cPduType=2
PduObjList = [ \
	[0x1ab,[0,0,0,0,0,0,0,0],cPduRx], #MSG0
	[0x1ef,[0,0,0,0,0,0,0,0],cPduTx], #MSG1
]

def __UpdateValue(pduId,SigStart,SigSize,SigValue):
    global PduObjList
    BA = 0
    bitsize = SigSize
    start   = SigStart
    value   = SigValue
    data    = PduObjList[pduId][cPduData]
    pos = start/8
    CrossB = (SigSize+7)/8
    if(SigStart>=(pos*8) and (SigStart+SigSize)<=(pos+CrossB)*8):
        pass
    else:
        CrossB += 1
    for i in range(0,CrossB):
        start   += BA     # bit accessed in this cycle
        bitsize -= BA
        pos = start/8
        offset = start%8 
        if((8-offset) > bitsize):
            BA =  bitsize
        else:
            BA = (8-offset)
        BM = ((1<<BA)-1)<<offset
        data[pos] &=  ~BM
        data[pos] |=  BM&(value<<offset)
        value = value>>(bitsize-BA)
    
def __ReadValue(pduId,SigStart,SigSize):
    global PduObjList
    value   = 0
    data    = PduObjList[pduId][cPduData]
    pos = SigStart/8
    CrossB = (SigSize+7)/8
    if(SigStart>=(pos*8) and (SigStart+SigSize)<=(pos+CrossB)*8):
        pass
    else:
        CrossB += 1
    for i in range(0,CrossB):
        value = value+(data[pos+i]<<(8*i))
    offset = SigStart%8 
    return (value>>offset)&((1<<SigSize)-1)
    

def Com_SendSignal(sigId,value):

    if(sigId == COM_SID_VehicleSpeed):
        lDebug('Send(VehicleSpeed)')
        __UpdateValue(COM_MSG0_RX,0,16,value)
        return 0
            

    if(sigId == COM_SID_TachoSpeed):
        lDebug('Send(TachoSpeed)')
        __UpdateValue(COM_MSG0_RX,16,16,value)
        return 0
            

    if(sigId == COM_SID_Led1Sts):
        lDebug('Send(Led1Sts)')
        __UpdateValue(COM_MSG0_RX,32,2,value)
        return 0
            

    if(sigId == COM_SID_Led2Sts):
        lDebug('Send(Led2Sts)')
        __UpdateValue(COM_MSG0_RX,34,2,value)
        return 0
            

    if(sigId == COM_SID_Led3Sts):
        lDebug('Send(Led3Sts)')
        __UpdateValue(COM_MSG0_RX,36,2,value)
        return 0
            
	print 'Error Signal Id'
	return -1 # error id

def Com_ReadSignal(sigId):

    if(sigId == COM_SID_VehicleSpeed):
        lDebug('Read(VehicleSpeed)')
        return __ReadValue(COM_MSG0_RX,0,16)
            

    if(sigId == COM_SID_TachoSpeed):
        lDebug('Read(TachoSpeed)')
        return __ReadValue(COM_MSG0_RX,16,16)
            

    if(sigId == COM_SID_Led1Sts):
        lDebug('Read(Led1Sts)')
        return __ReadValue(COM_MSG0_RX,32,2)
            

    if(sigId == COM_SID_Led2Sts):
        lDebug('Read(Led2Sts)')
        return __ReadValue(COM_MSG0_RX,34,2)
            

    if(sigId == COM_SID_Led3Sts):
        lDebug('Read(Led3Sts)')
        return __ReadValue(COM_MSG0_RX,36,2)
            

    if(sigId == COM_SID_Year):
        lDebug('Read(Year)')
        return __ReadValue(COM_MSG1_TX,0,16)
            

    if(sigId == COM_SID_Month):
        lDebug('Read(Month)')
        return __ReadValue(COM_MSG1_TX,16,8)
            

    if(sigId == COM_SID_Day):
        lDebug('Read(Day)')
        return __ReadValue(COM_MSG1_TX,24,8)
            

    if(sigId == COM_SID_Hour):
        lDebug('Read(Hour)')
        return __ReadValue(COM_MSG1_TX,32,8)
            

    if(sigId == COM_SID_Minute):
        lDebug('Read(Minute)')
        return __ReadValue(COM_MSG1_TX,40,8)
            

    if(sigId == COM_SID_Second):
        lDebug('Read(Second)')
        return __ReadValue(COM_MSG1_TX,48,8)
            
	print 'Error Signal Id'
	return -1 # error id


def ShowSignalList():
    print """SignalIdList:
VehicleSpeed    =0    ;  TachoSpeed      =1    ;  Led1Sts         =2    ;  
Led2Sts         =3    ;  Led3Sts         =4    ;  Year            =5    ;  
Month           =6    ;  Day             =7    ;  Hour            =8    ;  
Minute          =9    ;  Second          =10   ;  \n"""
    


class ComServerTx(threading.Thread): 
    __txPort = 8000
    __rxPort = 60001
    def __init__(self,txPort=8000,rxPort = 60001):
        self.__txPort = txPort
        self.__rxPort = rxPort
        threading.Thread.__init__(self)
        self.start()
    def run(self):
        global PduObjList
        while(True):
            for pdu in PduObjList:
                if(pdu[cPduType] == cPduRx):
                    self.transmit(pdu[cPduCanId],pdu[cPduData])
            time.sleep(0.100)  # 100ms
            
    def transmit(self,canId,data,length=None):
        msg = UserString.MutableString("c" * 17)
        msg[0] = '%c'%((canId>>24)&0xFF)
        msg[1] = '%c'%((canId>>16)&0xFF)
        msg[2] = '%c'%((canId>>8)&0xFF)
        msg[3] = '%c'%(canId&0xFF)
        if(None == length):
            length = len(data)
        assert length <= 8
        msg[4] = '%c'%(length) #DLC
        for i in range(0,length):
            msg[5+i] = '%c'%((data[i])&0xFF)
        for i in range(length,8):
            msg[5+i] = '%c'%(0x55)
        msg[13] = '%c'%((self.__rxPort>>24)&0xFF)
        msg[14] = '%c'%((self.__rxPort>>16)&0xFF)
        msg[15] = '%c'%((self.__rxPort>>8)&0xFF)
        msg[16] = '%c'%((self.__rxPort)&0xFF)
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect(('127.0.0.1', self.__txPort ))  
            sock.send(msg.data)
            sock.close()
        except:
            print 'ERROR: CanBusServer isn\'t started.'     
class ComServerRx(threading.Thread): 
    __rxPort = 60001
    def __init__(self,rxPort=60001):
        self.__rxPort = rxPort
        threading.Thread.__init__(self)
        self.start()
    def receive(self,msg):
        global PduObjList
        canId = (ord(msg[0])<<24)+(ord(msg[1])<<16)+(ord(msg[2])<<8)+(ord(msg[3]))
        for pdu in PduObjList:
            if(pdu[cPduCanId] == canId and pdu[cPduType] == cPduTx):
                data = pdu[cPduData]
                for i in range(0,8):
                    data[i] = ord(msg[5+i])
                break
                print pdu
    def run(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        sock.bind(('127.0.0.1', self.__rxPort))  
        sock.listen(32) 
        while True:  
            try:
                connection,address = sock.accept() 
                connection.settimeout(1)
                msg = connection.recv(17)  
                connection.close()
                if(len(msg) == 17):
                    self.receive(msg)
            except socket.timeout:  
                continue  
            connection.close() 

ComServerTx(8000,60001)
ComServerRx(60001)        
    