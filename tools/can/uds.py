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

uds_tx_id = 0x731
uds_rx_id = 0x732
UdsAckEvent = DeclareEvent()

uds_ack = []

def UdsOnCanUsage():
    print "Usage:"
    print "\t python uds.py --port port"
    print "Example: python uds.py --port 8999"

def UdsConfig():
    global uds_tx_id, uds_rx_id
    print 'Welcome to OpenOSEK UDS client cnter!'
    value = raw_input('Please Input the UDS client Tx CAN ID(default = 0x731):')
    if('' != value):
        uds_tx_id = int(value,16)
    value = raw_input('Please Input the UDS client Rx CAN ID(default = 0x732):')
    if('' != value):
        uds_rx_id = int(value,16)
    print 'Tx = %s, Rx = %s.'%(hex(uds_tx_id),hex(uds_rx_id))

def Uds_RxIndication(data):
    global uds_ack
    uds_ack = []
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
    #UdsConfig()
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
    """ Security Access, Request Seed"""
    data = [0x27,0x01,0x01] 
    print '    Send: [0x27,0x01,0x01]'
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    if(uds_ack[2]==0 and uds_ack[3]==0 and uds_ack[4]==0 and uds_ack[5]==0):
        print 'Already UnSecured!'
    else:
        """ Security Access, Send Key 0xDEADBEEF"""
        data = [0x27,0x02,0xFE,0xEB,0xDA,0xED] 
        print '    Send: [0x27,0x02,0xFE,0xEB,0xDA,0xED] '
        CanTp_Transmit(data)
        WaitEvent(UdsAckEvent,5000)
        ClearEvent(UdsAckEvent)
    """ Communication Control:enableRxAndDisableTx"""
    data = [0x28,0x01,0x02] 
    print '    Send: [0x28,0x01,0x02] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RDID : 0xFF01 0xFF09"""
    data = [0x22,0xFF,0x01,0xFF,0x09] 
    print '    Send: [0x22,0xFF,0x01,0xFF,0x09] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RDID : 0xFF09"""
    data = [0x22,0xFF,0x09] 
    print '    Send: [0x22,0xFF,0x09] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ WDID : 0xFE02"""
    data = [0x2E,0xFE,0x02] 
    for i in range(0,128):
        data.append(i)
    print '    Send: [0x2E,0xFE,0x02,.....] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ WDID : 0xFE09"""
    data = [0x2E,0xFE,0x09] 
    for i in range(0,64):
        data.append(i*2)
    print '    Send: [0x2E,0xFE,0x09,.....] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RC Start : 0xAB11"""
    data = [0x31,0x01,0xAB,0x11] 
    print '    Send: [0x31,0x01,0xAB,0x11] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RC Stop : 0xAB11"""
    data = [0x31,0x02,0xAB,0x11] 
    print '    Send: [0x31,0x02,0xAB,0x11] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)
    """ RC RequestResult : 0xAB11"""
    data = [0x31,0x03,0xAB,0x11] 
    print '    Send: [0x31,0x03,0xAB,0x11] '
    CanTp_Transmit(data)
    WaitEvent(UdsAckEvent,5000)
    ClearEvent(UdsAckEvent)

def biCT(argv):
    if argv is None:
        argv = 100
    while argv > 0:
        biTest(None)    
        argv -= 1
    
UdsBuildIn = {
    'Session':biSession,
    'Security':biSecurity,
    'SendFF':biSendFF,
    'Test':biTest,
    'CT':biCT
}
def main(argc,argv):
    if(argc != 3):
        UdsOnCanUsage()
        return
    if(argv[1] == '--port'):
        UdsOnCanClient(int(argv[2]))
       
if __name__ == '__main__': 
    import sys 
    main(len(sys.argv),sys.argv);
