"""/* Copyright(C) 2013, OpenOSEK by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenOSEK.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Sourrce Open At: https://github.com/parai/OpenOSEK/
 */
"""
__all__ = ['Sleep', 'DeclareTask','ActivateTask','TerminateTask',
           'Can_Init','Can_Write','CanTp_Init','CanTp_Transmit',
           'DeclareEvent','WaitEvent','ClearEvent','SetEvent']

import threading,time
import sys
import socket
import UserString

class ooEvent(threading._Event):
    def __init__(self):
        threading._Event.__init__(self)

def DeclareEvent():
    return ooEvent()

def WaitEvent(tEvent,timeout=None):
    """" tEvent must be declared by """
    assert type(tEvent) is ooEvent
    if( False == tEvent.wait(timeout/1000.0)):
        """No flag set, time out"""
        return False
    else:
        return True

def ClearEvent(tEvent):
    assert type(tEvent) is ooEvent
    tEvent.clear()

def SetEvent(tEvent):
    assert type(tEvent) is ooEvent
    tEvent.set()
    
class ooTask(threading.Thread):
    """ Private Datas """
    __activation = 0
    def __init__(self,pTask):
        threading.Thread.__init__(self)
        self.pTaskEntry = pTask
        self.start()
    def run(self):
        while True:
            if self.__activation > 0:
                self.__activation -= 1
                self.pTaskEntry()
        else:
            Sleep(1)
    def activate(self):
        self.__activation += 1
    def terminate(self):
        pass

def Sleep(timeMs):
    """Sleep """
    time.sleep(timeMs/1000.0)

def DeclareTask(pTask):
    """ pTask should be the type of 'void Function(void)', for example:
        def Task1():
            while True:
                print 'Task1 is running.'
                Sleep(10)   # Sleep 10 ms
    """ 
    """ Return the Task handle, which in the type ooTask"""
    return ooTask(pTask)

def ActivateTask(tTask):
    """ Task should be the type 'ooTask' """
    if(type(tTask) != ooTask):
        print 'ERROR: Parameter <tTask> for ActivateTask Invalid.' 
        sys.exit(-1)
    else:
        tTask.activate()

def TerminateTask():
    if(type(threading.currentThread()) == ooTask):
        threading.currentThread().terminate()
    else:
        print 'ERROR: TerminateTask called in Invalid Context.' 
        sys.exit(-1)
        
class ooCanController(threading.Thread): 
    """ See CanBusServer.py, just one Controller was supported."""
    __rxIndication = None
    __rxPort = 8999
    __txPort = 8000
    def __init__(self,rxIndication = None,rxPort=8999,txPort=8000):
        """rxIndication must be the type: void Function(uint32 canId,uint8* data,uint8 length)"""
        if rxIndication is None:
            self.__rxIndication = Can_RxIndication
        else:
            # assert type(rxIndication) is function
            self.__rxIndication = rxIndication
        self.__rxPort = rxPort
        self.__txPort = txPort
        threading.Thread.__init__(self)
        self.start()
    def write(self,canId,data,length=None):
        """  """
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
    def trace(self,msg):
        if(len(msg) != 17):
            print 'Error: length of msg is invalid!'
            return
        #get CanID
        canid =  (ord(msg[0])<<24)+(ord(msg[1])<<16)+(ord(msg[2])<<8)+(ord(msg[3]))
        # get Port
        port = (ord(msg[13])<<24)+(ord(msg[14])<<16)+(ord(msg[15])<<8)+(ord(msg[16]))
        dlc = ord(msg[4])
        cstr = 'ID=%s, DLC=%s: ['%(hex(canid),dlc)
        for i in range(0,8):
            cstr += '0x%-2x, '%(ord(msg[5+i]))
        cstr += ']'
        cstr += '<->['
        for i in range(0,8):
            if(i<dlc and msg[5+i] != '\n' and msg[5+i] != '\t'):
                cstr += '%s'%(msg[5+i])
            else:
                cstr += '.'
        cstr += '] From %s'%(port)
        print cstr
    def run(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        sock.bind(('127.0.0.1', self.__rxPort))  
        sock.listen(32) 
        while True:  
            try:
                connection,address = sock.accept() 
                connection.settimeout(1)
                msg = connection.recv(1024)  
                connection.close()
                if(len(msg) == 17):
                    #self.trace(msg)
                    data = []
                    canId = (ord(msg[0])<<24)+(ord(msg[1])<<16)+(ord(msg[2])<<8)+(ord(msg[3]))
                    length = ord(msg[4])
                    if(length > 8):
                        length = 8
                    for i in range(0,length):
                        data.append(ord(msg[5+i]))
                    self.__rxIndication(canId,data,length)
            except socket.timeout:  
                continue  
            connection.close()
            
__CanController = None

def Can_Init(indication=None,rxPort=8999,txPort=8000):
    global __CanController
    if indication is None:
        indication = Can_RxIndication
    __CanController = ooCanController(indication,rxPort,txPort)  

def Can_Write(canId,data,length=None):
    global __CanController
    assert type(__CanController) == ooCanController
    __CanController.write(canId,data,length)

CanTp_stIdle = 0
CanTp_stReceiving = 1
CanTp_stBusy = 2
CanTp_stStartToSend = 3
CanTp_stSending = 4
CanTp_stSendFinished = 5
CanTp_stWaitFC = 6
CanTp_stSendCF = 7
class ooCanTpController(threading.Thread):
    __LocalData = []
    __state = CanTp_stIdle
    __counter = 0
    __timer = 0
    __rxIndication = None
    __rxCanId = 0
    __txCanId = 0
    __BS = 0
    __STMin = 10
    __SN = 0
    def __init__(self,rxIndication,rxCanId = 0x732,txCanId = 0x731):
        """ must be of the type void Function(uint8* data,uint8 length) """
        self.__rxIndication = rxIndication
        self.__rxCanId = rxCanId
        self.__txCanId = txCanId
        self.__Event = DeclareEvent()
        threading.Thread.__init__(self)
        self.start()
    def getRxCanId(self):
        return self.__rxCanId
    def copyToLocal(self,data,length):
        if length is None:
            length = len(data)
        self.__LocalData = []
        for i in range(0,length):
            self.__LocalData.append(data[i])

    def receiveSF(self,data,length):
        datar = []
        lenSF = data[0]&0x07
        for i in range(0,lenSF):
            datar.append(data[1+i])
        self.__rxIndication(datar)

    def receiveFC(self,data,length):
        if(data[0]&0x0F == 0x00): #CTS
            if(data[1] != 0):
                self.__BS = data[1] + 1
            else:
                self.__BS = 0 # receive all without FC
            self.__STMin = data[2]
            SetEvent(self.__Event)
        elif(data[0]&0x0F == 0x02):
            print 'Server Buffer size overflow.'

    def sendFC(self):
        cfgSTmin = 10
        cfgBS = 8
        data = [0x30,cfgBS,cfgSTmin]
        Can_Write(self.__txCanId,data)
        if cfgBS is 0:
            self.__BS = 0
        else:
            self.__BS = cfgBS + 1
    
    def receiveFF(self,data,length):
        self.__LocalData = []
        self.__counter = ((data[0]&0x0F)<<8) + data[1];
        for i in range(0,6):
            self.__LocalData.append(data[2+i])
        self.sendFC()
        self.__state = CanTp_stReceiving;
        self.__SN = 0;

    def receiveCF(self,data,length):
        size = self.__counter - len(self.__LocalData)
        if(size > 7):
            size = 7  
        self.__SN += 1
        if self.__SN > 15:
            self.__SN = 0
        if (data[0]&0x0F) == self.__SN:
            for i in range(0,size):
                self.__LocalData.append(data[1+i])
            if(len(self.__LocalData) >= self.__counter):
                #print 'Goto Idle, Receiving Done!'
                self.__state = CanTp_stIdle;
                self.__rxIndication(self.__LocalData)
            if(self.__BS > 1):
                self.__BS -= 1
                if self.__BS == 1:
                    self.sendFC()             
        else:
            self.__state = CanTp_stIdle;
            print 'Error of the Sequence Number for Receiving.','RSN = 0x%x'%(data[0]),'LSN = 0x%x'%(self.__SN)
        SetEvent(self.__Event)

    def indication(self,data,length):
        if(data[0]&0xF0 == 0x00): #SF
            self.receiveSF(data,length)
        elif(data[0]&0xF0 == 0x10): #FF
            self.receiveFF(data,length)
        elif(data[0]&0xF0 == 0x20): #CF
            self.receiveCF(data,length)
        elif(data[0]&0xF0 == 0x30): # FC
            self.receiveFC(data,length)

    def transmit(self,data,length=None):
        if(CanTp_stIdle == self.__state):
            self.copyToLocal(data,length)
            self.__state = CanTp_stStartToSend
        else:
            print 'CanTp is busy now.'
    def sendSF(self):
        data = []
        data.append(len(self.__LocalData) & 0x07)  # SF
        for i in range(0,len(self.__LocalData)):
            data.append(self.__LocalData[i])
        Can_Write(self.__txCanId,data)
        self.__state = CanTp_stIdle
    def sendFF(self):
        data = []
        length = len(self.__LocalData)
        assert length < 4096
        data.append( 0x10 | ((length>>8)&0x0F) )  # FF
        data.append(length&0xFF)
        for i in range(0,6):
            data.append(self.__LocalData[i])
        self.__counter = 6
        Can_Write(self.__txCanId,data)
        self.__state = CanTp_stWaitFC   
        self.__SN = 0       
    def sendCF(self):
        data = [] 
        self.__SN += 1
        if(self.__SN > 15):
            self.__SN = 0
        data.append(0x20|self.__SN) # CF
        if(len(self.__LocalData) >= self.__counter + 7):
            size = 7
        else:
            size = len(self.__LocalData) - self.__counter
        for i in range(0,size):
            data.append(self.__LocalData[self.__counter + i])
        self.__counter += size
        Can_Write(self.__txCanId,data)
        if(self.__counter >= len(self.__LocalData)):
            """ Finshed """
            self.__state = CanTp_stIdle
            #print "    Segmented Message Transmission Done!"
        else:
            if(self.__BS > 1):
                self.__BS -= 1
                if(self.__BS == 1):
                    self.__state = CanTp_stWaitFC
                else:
                    self.__state = CanTp_stSendCF
            else:
                self.__state = CanTp_stSendCF
    def run(self):
        while True:
            if(CanTp_stStartToSend == self.__state):
                if(len(self.__LocalData) <= 7):
                    self.sendSF()
                else:
                    self.sendFF()
            elif(CanTp_stWaitFC == self.__state):
                if(True == WaitEvent(self.__Event,1000)):
                    ClearEvent(self.__Event)
                    self.__state = CanTp_stSendCF
                    continue
                else: # Time-out abort
                    self.__state = CanTp_stIdle  
                    print '    Timeout for  Waiting the Flow Control.'
            elif(CanTp_stReceiving == self.__state):
                if(True == WaitEvent(self.__Event,1000)):
                    ClearEvent(self.__Event)
                    continue
                else: # Time-out abort
                    self.__state = CanTp_stIdle  
                    print '    Timeout When receiving.'        
            elif(CanTp_stSendCF == self.__state):
                self.sendCF()
                Sleep(self.__STMin)
                continue
            Sleep(10)

__CanTpController = None

def Can_RxIndication(canid,data,length):
    global __CanTpController
    if(__CanTpController.getRxCanId() == canid):
        CanTp_RxIndication(data,length)

def CanTp_RxIndication(data,length=None):
    global __CanTpController
    __CanTpController.indication(data,length)

def CanTp_Init(rxIndication,rxCanId = 0x732,txCanId = 0x731):    
    global __CanTpController
    __CanTpController = ooCanTpController(rxIndication,rxCanId,txCanId)  

def CanTp_Transmit(data,length=None):
    global __CanTpController
    __CanTpController.transmit(data,length)
    