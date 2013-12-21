from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys
import socket

import xml.etree.ElementTree as ET

__all__ = ['CanWidget']

# Config 

CanMatrix = '../../app/config/GEN/com.wfxml'

__root = ET.parse(CanMatrix).getroot()

####
def tInt(strnum):
    if(strnum.find('0x')!=-1 or strnum.find('0X')!=-1):
        return int(strnum,16)
    else:
        return int(strnum,10)
    
def GAGet(what,which):
    # what must be a pdu = [ Name, Id, Bus]
    # or a Signal Node
    try:
        if(which == 'name'):
            return what[0]
        elif(which == 'id'):
            return tInt(what[1])
        elif(which == 'bus'):
            return what[2]
        else:
            raise Exception('No Found')
    except:
        import re
        # (Type=RX/TX ) (Name) (Id)
        reMsg = re.compile(r'(\w+)\s*=\s*(\w+)\s*\(\s*(\w+)\s*\)')
        msg = what.attrib['msg']
        msg = reMsg.search(msg).groups()
        if(which == 'msgname'):
            return msg[1]
        elif(which == 'msgtype'):
            return msg[0]
        elif(which == 'type'):
            size = tInt(what.attrib['size'])
            if(size <= 8):
                return 'UINT8'
            elif(size <= 16):
                return 'UINT16'
            elif(size <= 32):
                 return 'UINT32'
            else:
                return 'UINT8_N'
        else:
            return what.attrib[which]

def GLGet(what,which = None):
    """ Gen Get List
        Get A Special List []
    """
    global __root
    import re
    # (Type=RX/TX ) (Name) (Id)
    reMsg = re.compile(r'(\w+)\s*=\s*(\w+)\s*\(\s*(\w+)\s*\)')
    if(which!=None and which=='SignalRefs'):
        # what is a pdu = [ Name, Id, Bus]
        rList = []
        for sig in __root.find('SignalList'):
            import re
            # (Type=RX/TX ) (Name) (Id)
            reMsg = re.compile(r'(\w+)\s*=\s*(\w+)\s*\(\s*(\w+)\s*\)')
            msg = sig.attrib['msg']
            msg = reMsg.search(msg).groups()
            if(what[0]==msg[1] and what[1]==msg[2] and what[2]==sig.attrib['bus']):
                rList.append(sig)
        return rList
    if(what == 'Signal'):
        return __root.find('SignalList')
    rlist =[]
    if(what == 'TxPdu'):
        for Signal in __root.find('SignalList'):
            msg = Signal.attrib['msg']
            try:
                msg = reMsg.search(msg).groups()
                if(msg[0] == 'TX'):
                   flag = False
                   for pdu in rlist:
                       # If has same Id and Bus
                       if(msg[2] == pdu[1] and Signal.attrib['bus'] == pdu[2]):
                           if(msg[1]) != pdu[0]:
                               raise Exception('Name must be the same if the message has the same Id on the same Bus.')
                           flag = True
                   if(False == flag):
                       pdu = []
                       pdu.append(msg[1])
                       pdu.append(msg[2])
                       pdu.append(Signal.attrib['bus'])
                       rlist.append(pdu)
            except:
                print traceback.format_exc()
                print 'PduR: Error Message Configured for %s'%(Signal.attrib['name'])
    elif(what == 'RxPdu'):
        for Signal in __root.find('SignalList'):
            msg = Signal.attrib['msg']
            try:
                msg = reMsg.search(msg).groups()
                if(msg[0] == 'RX'):
                   flag = False
                   for pdu in rlist:
                       if(msg[2] == pdu[1] and Signal.attrib['bus'] == pdu[2]):
                           if(msg[1]) != pdu[0]:
                               raise Exception('Name must be the same if the message has the same Id on the same Bus.')
                           flag = True
                   if(False == flag):
                       pdu = []
                       pdu.append(msg[1])
                       pdu.append(msg[2])
                       pdu.append(Signal.attrib['bus'])
                       rlist.append(pdu)
            except:
                print traceback.format_exc()
                print 'PduR: Error Message Configured for %s'%(Signal.attrib['name'])
    return rlist
####
#
class tSignal():
    id = 0  # used to ref pdu
    name = ''
    start = 0
    size = 0
    init = 0
    comment = ''

cTxPdu = 0
cRxPdu = 1

class PduType():    
    def __init__(self,msg):
        self.id = 0
        self.type = cRxPdu
        self.SignalRefs = []
        self.data = [0,0,0,0,0,0,0,0]  
        self.id = GAGet(msg,'id')
        for sig in GLGet(msg,'SignalRefs'):
            Signal = tSignal()
            Signal.name = GAGet(sig,'name')
            Signal.start = tInt(GAGet(sig,'start'))
            Signal.size = tInt(GAGet(sig,'size'))
            Signal.init = tInt(GAGet(sig,'init'))
            Signal.comment = GAGet(sig,'comment')
            if(GAGet(sig,'msgtype') == 'RX'):
                self.type = cRxPdu
            else:
                self.type = cTxPdu
            self.add(Signal)

    def __str__(self):
        cstr = 'Id = %s ['%(self.id)
        for d in self.data:
            cstr += '%-2s,'%(d)
        cstr +=']\n'
        for sig in self.SignalRefs:
            cstr += '%s,'%(sig.name)
        return cstr
        
    def setValue(self,start,size,init):
        BA = 0
        bitsize = size
        for i in range(0,(size+7)/8):
            start   += BA     # bit accessed in this cycle
            bitsize -= BA
            pos = start/8
            offset = start%8 
            if((8-offset) > bitsize):
                BA =  bitsize
            else:
                BA = (8-offset)
            BM = ((1<<BA)-1)<<offset
            self.data[pos] &=  ~BM
            self.data[pos] |=  BM&((init>>(bitsize-BA))<<offset)
            init = init>>offset
            
    def add(self,Signal):
        self.SignalRefs.append(Signal)
        self.setValue(Signal.start,Signal.size,Signal.init)


class SignalTable(QTableWidget):
    def __init__(self,parent=None):  
        super(QTableWidget,self).__init__(parent) 
        self.PduList = [] 
        self.setColumnCount(3)  
        self.setRowCount(0)  
        self.setHorizontalHeaderLabels(QStringList(['Name','Value','Comment']))
        self.setColumnWidth(2,400)
        self.initSignals()
        self.connect(self, SIGNAL('cellChanged(int,int)'),self.on_cellChanged)
    
    def on_cellChanged(self,row,column):
        signame = self.item(row,0).text()
        value = str(self.item(row,column).text())
        for pdu in self.PduList:
            for sig in pdu.SignalRefs:
                if(sig.name == signame):
                    pdu.setValue(sig.start,sig.size,tInt(value))
                    print pdu
                    break
        
    def parse(self):
        for msg in GLGet('RxPdu')+GLGet('TxPdu'):
            self.PduList.append(PduType(msg))
            
    def initSignals(self):
        self.parse()
        for pdu in self.PduList:
            for sig in pdu.SignalRefs:
                if(pdu.type == cTxPdu):
                    self.addSignal(sig,True)
                else:
                    self.addSignal(sig,False)                 

    def addSignal(self,Signal,isTx):
        index = self.rowCount()
        self.setRowCount(self.rowCount()+1) 
        item0 = QTableWidgetItem(self.tr('%s'%(Signal.name)))
        item0.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
        item1 = QTableWidgetItem(self.tr('%s'%(Signal.init)))
        if(isTx==True):
            item1.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
        else:
            pass
        item2 = QTableWidgetItem(self.tr('%s'%(Signal.comment)))
        item2.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
        self.setItem(index,0,item0)    
        self.setItem(index,1,item1)  
        self.setItem(index,2,item2)  

class CanWidget(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowIcon(QtGui.QIcon("./res/Can.bmp"))
        self.setWindowTitle("Can Simulator")
        self.setCentralWidget(SignalTable())

