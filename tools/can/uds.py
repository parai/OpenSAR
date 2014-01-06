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
from Osek import *
import re
import traceback

# uds_tx_id = 0x731
# uds_rx_id = 0x732
uds_tx_id = 0x741
uds_rx_id = 0x742
UdsAckEvent = DeclareEvent()

uds_ack = []
uds_ack_len = 0

def tInt(strnum):
    if(strnum.find('0x')!=-1 or strnum.find('0X')!=-1):
        return int(strnum,16)
    else:
        return int(strnum,10)

def UdsOnCanUsage():
    print "Usage:"
    print "\t python uds.py --port port txid rxid"
    print "Example: python uds.py --port 8999 0x731 0x732"

def UdsConfig():
    global uds_tx_id, uds_rx_id
    print 'Welcome to OpenOSEK UDS client cnter!'
    value = raw_input('Please Input the UDS client Tx CAN ID(default = %s):'%(hex(uds_tx_id)))
    if('' != value):
        uds_tx_id = int(value,16)
    value = raw_input('Please Input the UDS client Rx CAN ID(default = %s):'%(hex(uds_rx_id)))
    if('' != value):
        uds_rx_id = int(value,16)
    print 'Tx = %s, Rx = %s.'%(hex(uds_tx_id),hex(uds_rx_id))

def Uds_RxIndication(data):
    global uds_ack,uds_ack_len
    uds_ack = []
    uds_ack_len = len(data)
    cstr = '    Response: ['
    for i in range(0,len(data)):
        cstr += '0x%-2x,'%(data[i])
        uds_ack.append(data[i])
    cstr += ']..['
    for i in range(0,len(data)):
        if re.match(r'[^\s]','%c'%(data[i])):
            cstr += '%c'%(data[i])
        else:
            cstr += '.'
    cstr += ']'
    print cstr
    Sleep(10)
    SetEvent(UdsAckEvent)

def UdsOnCanClient(port = 8999):
    global uds_tx_id, uds_rx_id
    UdsConfig()
    Can_Init(None,port,port-port%1000)
    CanTp_Init(Uds_RxIndication,uds_rx_id,uds_tx_id)
    while True:
        data = []
        value = raw_input("UDS Send [ 3E 00 ]:")
        if(value.find('$') != -1): # a commend
            fnc = re.compile(r'\s+').split(value)[0]
            if(fnc[0] != '$'):
                continue
            fnc = fnc[1:]
            if len(re.compile(r'\s+').split(value)) > 1:
                argv = re.compile(r'\s+').split(value)[1:]
            else:
                argv = None
            try:
                UdsBuildIn[fnc](argv)
                if WaitEvent(UdsAckEvent,500):
                    ClearEvent(UdsAckEvent)
            except:
                print 'Function <%s> is not build-in supported.'%(fnc)
                print traceback.format_exc()
            continue
        elif(value != ''):
            for chr in re.compile(r'\s+').split(value):
                try:
                    if(chr != ''):
                        data.append(int(chr,16))
                except:
                    print 'Error input!'
                    data = [0x3e,0x00]
                    break         
        else:
            data = [0x3e,0x00]
        CanTp_Transmit(data)
        if(True == WaitEvent(UdsAckEvent,5000)): 
            ClearEvent(UdsAckEvent)
        else:
            print "    No Response, Time-out."
""" ----------------------- Function Tanble ------------------------"""   
def biSession(argv):
    data = [0x10]
    if(len(argv) == 1):
        try:
            data.append(int(argv[0]))
        except:
            print 'Session Argument Error!'
            return
    else:
        data.append(0x01) #default session
    CanTp_Transmit(data)
def biSecurity(argv):
    data = [0x27]
    pass
def biSendFF(argv):
    data = [0x10,0x00,0x55,0x55,0x55,0x55,0x55,0x55]
    try:
        size = int(argv[0],16)
        data[0] = 0x10|((size>>8)&0x0F)
        data[1] = size&0xFF
        size = len(argv) -1
        if size > 6:
            size = 6
        for i in range(0,size):
            data[i+2] = int(argv[i],16)
        Can_Write(uds_tx_id,data)
    except:
        print 'SendFF Argument Error!'
        
def biTest(argv):
    global uds_ack
    """ Session Control, Program"""
    data = [0x10,0x02] 
    print '    Send: [0x10,0x02]'
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    level = 10
    """ Security Access, Request Seed"""
    data = [0x27]
    data.append(level*2-1)      
    print '    Send:',data
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    if(uds_ack[2]==0 and uds_ack[3]==0 and uds_ack[4]==0 and uds_ack[5]==0):
        print '    Already UnSecured!'
    else:
        """ Security Access, Send Key"""
        seed = (uds_ack[2]<<24) + (uds_ack[3]<<16) + (uds_ack[4]<<8) + (uds_ack[5])
        key = ((seed/7)<<3) - 111;
        data = [0x27] 
        data.append(level*2)  
        data.append((key>>24)&0xFF)
        data.append((key>>16)&0xFF)
        data.append((key>>8)&0xFF)
        data.append((key)&0xFF)
        print '    Send: ',data
        CanTp_Transmit(data)
        WaitEvent(UdsAckEvent,5000)
        ClearEvent(UdsAckEvent)
#     """ Communication Control:enableRxAndDisableTx"""
#     data = [0x28,0x01,0x02] 
#     print '    Send: [0x28,0x01,0x02] '
#     CanTp_Transmit(data)
#     WaitEvent(UdsAckEvent,5000)
#     ClearEvent(UdsAckEvent)
#     """ RDID : 0xFF01 0xFF09"""
#     data = [0x22,0xFF,0x01,0xFF,0x09] 
#     print '    Send: [0x22,0xFF,0x01,0xFF,0x09] '
#     CanTp_Transmit(data)
#     WaitEvent(UdsAckEvent,5000)
#     ClearEvent(UdsAckEvent)
    """ RDID : 0x010A"""
    data = [0x22,0x01,0x0A] 
    print '    Send: [0x22,0x01,0x0A] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ WDID : 0x010A"""
    data = [0x2E,0x01,0x0A] 
    for i in range(0,128):
        data.append(i)
    print '    Send: [0x2E,0x01,0x0A,.....] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RC Start : 0x020A"""
    data = [0x31,0x01,0x02,0x0A,0x00] 
    print '    Send: [0x31,0x01,0x02,0x0A,0x00]  '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RC RequestResult : 0x020A"""
    data = [0x31,0x03,0x02,0x0A] 
    print '    Send: [0x31,0x03,0x02,0x0A] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)

def biCT(argv):
    if argv is None:
        argv = 100
    while argv > 0:
        biTest(None)    
        argv -= 1
## ==========================  Flash Loader Start ===================
E_OK = 0
E_NOT_OK = -1
def FL_Session():
    """ Session Control, Program"""
    global uds_ack,uds_ack_len
    data = [0x10,0x02] 
    CanTp_Transmit(data)
    if( False==WaitEvent(UdsAckEvent,5000) ):
        return E_NOT_OK
    ClearEvent(UdsAckEvent)
    if(uds_ack[0]==0x50 and uds_ack[1]==0x02):
        return E_OK
    else:
        return E_NOT_OK
def FL_Security():
    """ Security Access, Request Seed"""
    global uds_ack,uds_ack_len
    data = [0x27]
    level = 10
    data.append(level*2-1)      
    CanTp_Transmit(data)
    if( False==WaitEvent(UdsAckEvent,5000) ):
        return E_NOT_OK
    ClearEvent(UdsAckEvent)
    if(uds_ack[2]==0 and uds_ack[3]==0 and uds_ack[4]==0 and uds_ack[5]==0):
        # Already UnSecured!
        return E_OK
    elif(uds_ack[0]==0x67 and uds_ack[1]==(level*2-1)):
        """ Security Access, Send Key"""
        seed = (uds_ack[2]<<24) + (uds_ack[3]<<16) + (uds_ack[4]<<8) + (uds_ack[5])
        key = ((seed/7)<<3) - 111;
        data = [0x27] 
        data.append(level*2)  
        data.append((key>>24)&0xFF)
        data.append((key>>16)&0xFF)
        data.append((key>>8)&0xFF)
        data.append((key)&0xFF)
        CanTp_Transmit(data)
        if( False==WaitEvent(UdsAckEvent,5000) ):
            return E_NOT_OK
        ClearEvent(UdsAckEvent)
        if(uds_ack[0]==0x67 and uds_ack[1]==level*2):
            return E_OK
        else:
            return E_NOT_OK
    else:
        return E_NOT_OK

def FL_EraseFlash():
    """ RC Start : 0x020A"""
    global uds_ack,uds_ack_len
    BlockIdMask = 0x01 # just Erase Block 0
    data = [0x31,0x01,0x02,0x0A,BlockIdMask] 
    CanTp_Transmit(data)
    if( False==WaitEvent(UdsAckEvent,5000) ):
        return E_NOT_OK
    ClearEvent(UdsAckEvent)
    if(uds_ack[0]==0x71 and uds_ack[1]==0x01):
        pass
    elif(uds_ack[0]==0x7f and uds_ack[2]==0x78):
        """ response pending"""
        times = 0
        while(uds_ack[0]==0x7f and uds_ack[2]==0x78):
            if( False==WaitEvent(UdsAckEvent,5000) ):
                return E_NOT_OK
            ClearEvent(UdsAckEvent)
            times += 1
            if(times > 10):
                return E_NOT_OK
        if(uds_ack[0]==0x71 and uds_ack[1]==0x01):
            pass
        else:
            return E_NOT_OK
    else:
        return E_NOT_OK
    """ RC RequestResult : 0x020A"""
    data = [0x31,0x03,0x02,0x0A] 
    CanTp_Transmit(data)
    if( False==WaitEvent(UdsAckEvent,5000) ):
        return E_NOT_OK
    ClearEvent(UdsAckEvent)
    if(uds_ack_len==5 and uds_ack[0]==0x71 and uds_ack[1]==0x03 and uds_ack[2]==0x02 \
        and uds_ack[3]==0x0A and uds_ack[4]==0x00):
        return E_OK
    else:
        return E_NOT_OK

""" For test purpose """
FL_MemoryAddress = 0x00000
FL_MemorySize    = 0x04000 # 16 K

def FL_RequestDownload():
    global uds_ack,uds_ack_len
    data = [0x34,0x00,0x44] 
    memoryAddress = FL_MemoryAddress
    memorySize    = FL_MemorySize

    data.append((memoryAddress>>24)&0xFF)
    data.append((memoryAddress>>16)&0xFF)
    data.append((memoryAddress>>8 )&0xFF)
    data.append((memoryAddress>>0 )&0xFF)

    data.append((memorySize>>24)&0xFF)
    data.append((memorySize>>16)&0xFF)
    data.append((memorySize>>8 )&0xFF)
    data.append((memorySize>>0 )&0xFF)

    CanTp_Transmit(data)
    if( False==WaitEvent(UdsAckEvent,5000) ):
        return E_NOT_OK
    ClearEvent(UdsAckEvent)
    if(uds_ack[0]==0x74):
        return E_OK
    else:
        return E_NOT_OK

def FL_TransferData():
    global uds_ack,uds_ack_len
    blockSequenceCounter = 1
    memoryAddress = FL_MemoryAddress
    memorySize    = FL_MemorySize
    while memorySize > 0:
        data = [0x36]
        data.append(blockSequenceCounter)
        blockSequenceCounter += 1
        if(blockSequenceCounter>0xFF):
            blockSequenceCounter = 0x00
        length = 128
        if(memorySize<length):
            length = memorySize
        for i in range(0,length):
            data.append(memorySize-i)
        memorySize -= length
        CanTp_Transmit(data)
        if( False==WaitEvent(UdsAckEvent,5000) ):
            return E_NOT_OK
        ClearEvent(UdsAckEvent)
        if(uds_ack[0]==0x76):
            continue
        else:
            return E_NOT_OK
    return E_OK

def FL_RequestTransferExit():
    global uds_ack,uds_ack_len
    data = [0x37] 
    CanTp_Transmit(data)
    if( False==WaitEvent(UdsAckEvent,5000) ):
        return E_NOT_OK
    ClearEvent(UdsAckEvent)
    if(uds_ack[0]==0x77):
        return E_OK
    else:
        return E_NOT_OK

def biFlashLoader(argv):
    if(E_OK == FL_Session()):
        print 'Enter Program Session OK!'
    else:
        print 'Enter Program Session Failed!'
        return

    if(E_OK == FL_Security()):
        print 'Security Access OK!'
    else:
        print 'Security Access Failed!'
        return

    if(E_OK == FL_EraseFlash()):
        print 'Erase Flash OK!'
    else:
        print 'Erase Flash Failed!'
        return

    if(E_OK == FL_RequestDownload()):
        print 'Request Download OK!'
    else:
        print 'Request Download Failed!'
        return

    if(E_OK == FL_TransferData()):
        print 'Transfer Data OK!'
    else:
        print 'Transfer Data Failed!'
        return

    if(E_OK == FL_RequestTransferExit()):
        print 'Request Transfer Exit OK!'
    else:
        print 'Request Transfer Exit Failed!'
        return

## ==========================  Flash Loader End   ===================
UdsBuildIn = {
    'Session':biSession,
    'Security':biSecurity,
    'SendFF':biSendFF,
    'Test':biTest,
    'CT':biCT,
    'FL':biFlashLoader
}
def main(argc,argv):
    global uds_tx_id,uds_rx_id
    if(argc != 5):
        UdsOnCanUsage()
        return
    if(argv[1] == '--port'):
        uds_tx_id = int(argv[3],16)
        uds_rx_id = int(argv[4],16)
        UdsOnCanClient(int(argv[2]))
       
if __name__ == '__main__': 
    import sys 
    main(len(sys.argv),sys.argv);
