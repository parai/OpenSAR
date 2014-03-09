from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import xml.etree.ElementTree as ET

__all__ = ['easyComGui']

class easyComTree(QTreeWidget):
    signalid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Signal Name','Start Bit','Bit Size','Msg','Bus','Format','Init Value','Comment']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(3,200)
        self.setColumnWidth(4,140)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            pTree = self.currentItem()
            name = self.itemWidget(pTree,0).text()
            # update Action 
            self.root.qAction1.setText('Delete Signal <%s>'%(name))
            self.root.qAction1.setStatusTip('Delete This Signal <%s> Object, BeCareful as this action is dangerous.'%(name))
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setDisabled(True)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def loadXML(self,ROOT):
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        SignalList = ROOT.find('SignalList')
        if(SignalList == None):
            return
        for Signal in SignalList:
            self.addSignal(Signal)

    def toXML(self):
        List = ET.Element('SignalList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('Signal')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['start'] = str(self.itemWidget(tree,1).value())
            Node.attrib['size'] = str(self.itemWidget(tree,2).value())
            Node.attrib['msg'] = str(self.itemWidget(tree,3).text())
            Node.attrib['bus'] = str(self.itemWidget(tree,4).currentText())
            Node.attrib['format'] = str(self.itemWidget(tree,5).currentText())
            Node.attrib['init'] = str(self.itemWidget(tree,6).text())
            Node.attrib['comment'] = str(self.itemWidget(tree,7).text())
            List.append(Node)
        return List
    def moveSignalUp(self,isUp):
        tree = self.currentItem()
        Node = ET.Element('Signal')
        Node.attrib['name'] = str(self.itemWidget(tree,0).text())
        Node.attrib['start'] = str(self.itemWidget(tree,1).value())
        Node.attrib['size'] = str(self.itemWidget(tree,2).value())
        Node.attrib['msg'] = str(self.itemWidget(tree,3).text())
        Node.attrib['bus'] = str(self.itemWidget(tree,4).currentText())
        Node.attrib['format'] = str(self.itemWidget(tree,5).currentText())
        Node.attrib['init'] = str(self.itemWidget(tree,6).text())
        Node.attrib['comment'] = str(self.itemWidget(tree,7).text())
        pIndex = self.indexOfTopLevelItem(tree)
        if((pIndex+1) != self.topLevelItemCount() and pIndex != 0):
            if(isUp):
                Index = pIndex - 1
            else:
                Index = pIndex + 1
        elif((pIndex+1) == self.topLevelItemCount()):
            if(isUp):
                Index = pIndex - 1
            else:
                return
        elif(pIndex == 0):
            if(not isUp):
                Index = pIndex + 1
            else:
                return
        self.takeTopLevelItem(pIndex)
        self.addSignal(Node,Index)
            
    def deleteSignal(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))
    def addSignal(self,Node=None,Index=None):
        treeItem = QTreeWidgetItem()
        if(Index == None):
            self.addTopLevelItem(treeItem) 
        else:
            self.insertTopLevelItem(Index,treeItem)
        if(Node != None):
            sname = Node.attrib['name']
            sstart = int(Node.attrib['start'])
            ssize = int(Node.attrib['size'])
            smsg = Node.attrib['msg']
            sbus = Node.attrib['bus']
            sformat = Node.attrib['format']
            sinit = Node.attrib['init']
            scomment = Node.attrib['comment']
        else:  
            sname =  'Signal%s'%(self.signalid)
            sstart = 0
            ssize  = 8  
            smsg = 'RX=MsgName(CAN ID)' 
            sbus = 'CANIF_CHL_LS'
            sformat = 'Motorola'
            sinit = '0x00'
            scomment = ''
        name = QLineEdit(sname)
        start = QSpinBox()
        start.setToolTip("""
| Map    | BIT7 | BIT6 | BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0 |
| Byte 0 |    7 |    6 |    5 |    4 |    3 |    2 |    1 |    0 |
| Byte 1 |   15 |   14 |   13 |   12 |   11 |   10 |    9 |    8 |
| Byte 2 |   23 |   22 |   21 |   20 |   19 |   18 |   17 |   16 |
| Byte 3 |   31 |   30 |   29 |   28 |   27 |   26 |   25 |   24 |
| Byte 4 |   39 |   38 |   37 |   36 |   35 |   34 |   33 |   32 |
| Byte 5 |   47 |   46 |   45 |   44 |   43 |   42 |   41 |   40 |
| Byte 6 |   55 |   54 |   53 |   52 |   51 |   50 |   49 |   48 |
| Byte 7 |   63 |   62 |   61 |   60 |   59 |   58 |   57 |   56 |""")
        start.setRange(0,63)
        start.setValue(sstart)
        size = QSpinBox()
        size.setRange(0,32)  
        size.setValue(ssize)
        msg = QLineEdit(smsg)
        msg.setToolTip("""Format:
    RX=MsgName(CAN ID)
    TX=MsgName(CAN ID)""")
        bus = QComboBox()
        bus.addItems(QStringList(['CANIF_CHL_HS','CANIF_CHL_LS']))
        bus.setCurrentIndex(bus.findText(sbus))
        format = QComboBox()
        format.addItems(QStringList(['Motorola']))
        format.setCurrentIndex(format.findText(sformat))
        init = QLineEdit(sinit)
        comment = QLineEdit(scomment)
        self.setItemWidget(treeItem,0,name)    
        self.setItemWidget(treeItem,1,start) 
        self.setItemWidget(treeItem,2,size) 
        self.setItemWidget(treeItem,3,msg) 
        self.setItemWidget(treeItem,4,bus)
        self.setItemWidget(treeItem,5,format) 
        self.setItemWidget(treeItem,6,init) 
        self.setItemWidget(treeItem,7,comment) 
        self.signalid += 1

# ================================================= [ DCM ] ==============================================         
class easyDcmDefaultTree(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Not Supported','Not Supported']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
    def updateAction(self):
        self.root.qAction1.setText('')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(True)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)
    def onAction(self,action):
        pass      
    def loadXML(self,ROOT):
        pass
    def toXML(self):
        return None  

def tBool(strr):
    if(strr.upper() == 'TRUE'):
        return True
    else:
        return False
    
# tw = Tree Widget
class twSession(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Identifier','P2P','P2A','Comment']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(2,40)
        self.setColumnWidth(3,40)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            pTree = self.currentItem()
            name = self.itemWidget(pTree,0).text()
            # update Action 
            self.root.qAction1.setText('Delete Session <%s>'%(name))
            self.root.qAction1.setStatusTip('Delete This Session <%s> Object, BeCareful as this action is dangerous.'%(name))
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setDisabled(True)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('Add Session')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(False)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)        
    def onAction(self,action):
        if(action == 'Add Session'):
            self.addSession()
        elif(action.find('Delete Session')!=-1):
            self.deleteSession()
            
    def loadXML(self,ROOT):
        print "## Load SessionControl!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        SessionList = ROOT.find('SessionList')
        if(SessionList == None):
            return
        for Session in SessionList:
            self.addSession(Session)

    def toXML(self):
        List = ET.Element('SessionList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('Session')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['identifier'] = str(self.itemWidget(tree,1).text())
            Node.attrib['p2p'] = str(self.itemWidget(tree,2).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,3).isChecked())
            Node.attrib['comment'] = str(self.itemWidget(tree,4).text())
            List.append(Node)
        return List 
    
    def GetList(self,what):
        List = []
        if(what == 'Session'):
            for i in range(0,self.topLevelItemCount()):
                tree = self.topLevelItem(i)
                name = str(self.itemWidget(tree,0).text())
                List.append('Ses_'+name)
        return List 
        
    def deleteSession(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))    
    def addSession(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            sname = Node.attrib['name']
            sidentifier = Node.attrib['identifier']
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
        else:  
            sname =  'SessionTBD'
            sidentifier = 'TBD'
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
        name = QLineEdit(sname)
        identifier = QLineEdit(sidentifier)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment)
        self.setItemWidget(treeItem,0,name)    
        self.setItemWidget(treeItem,1,identifier)  
        self.setItemWidget(treeItem,2,p2p) 
        self.setItemWidget(treeItem,3,p2a)    
        self.setItemWidget(treeItem,4,comment)   
class twSecurity(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Identifier','P2P','P2A','KeySize','SeedSize','Comment']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(2,40)
        self.setColumnWidth(3,40)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            pTree = self.currentItem()
            name = self.itemWidget(pTree,0).text()
            # update Action 
            self.root.qAction1.setText('Delete Security <%s>'%(name))
            self.root.qAction1.setStatusTip('Delete This Security <%s> Object, BeCareful as this action is dangerous.'%(name))
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setDisabled(True)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('Add Security')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(False)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)        
    def onAction(self,action):
        if(action == 'Add Security'):
            self.addSecurity()
        elif(action.find('Delete Security')!=-1):
            self.deleteSecurity()
            
    def loadXML(self,ROOT):
        print "## Load SecurityAccess!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        SecurityList = ROOT.find('SecurityList')
        if(SecurityList == None):
            return
        for Security in SecurityList:
            self.addSecurity(Security)

    def toXML(self):
        List = ET.Element('SecurityList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('Security')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['identifier'] = str(self.itemWidget(tree,1).text())
            Node.attrib['p2p'] = str(self.itemWidget(tree,2).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,3).isChecked())
            Node.attrib['keySize'] = str(self.itemWidget(tree,4).text())
            Node.attrib['seedSize'] = str(self.itemWidget(tree,5).text())
            Node.attrib['comment'] = str(self.itemWidget(tree,6).text())
            List.append(Node)
        return List 
    def GetList(self,what):
        List = []
        if(what == 'Security'):
            for i in range(0,self.topLevelItemCount()):
                tree = self.topLevelItem(i)
                name = str(self.itemWidget(tree,0).text())
                List.append('Sec_'+name)
        return List     
    def deleteSecurity(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))    
    def addSecurity(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            sname = Node.attrib['name']
            sidentifier = Node.attrib['identifier']
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
            skeysize = Node.attrib['keySize']
            sseedsize = Node.attrib['seedSize']
        else:  
            sname =  'SecurityTBD'
            sidentifier = 'TBD'
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
            skeysize = '4'
            sseedsize = '4'
        name = QLineEdit(sname)
        identifier = QLineEdit(sidentifier)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment)
        keysize = QLineEdit(skeysize)
        seedsize = QLineEdit(sseedsize)
        self.setItemWidget(treeItem,0,name)    
        self.setItemWidget(treeItem,1,identifier)  
        self.setItemWidget(treeItem,2,p2p) 
        self.setItemWidget(treeItem,3,p2a)  
        self.setItemWidget(treeItem,4,keysize) 
        self.setItemWidget(treeItem,5,seedsize)  
        self.setItemWidget(treeItem,6,comment)   
class twEcuReset(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','P2P','P2A','Comment','']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(1,150)
        self.setColumnWidth(2,40)
        self.setColumnWidth(3,600)
        self.addRoot()
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        pTree = self.currentItem()
        name = self.itemWidget(pTree,0).text()
        # update Action 
        if(name=='ROOT'):
            self.root.qAction1.setText('Add SessionRef')
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('Add SecurityRef')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(False)
        else:
            ref = str(self.itemWidget(pTree,1).currentText())
            self.itemWidget(pTree,1).clear()
            self.itemWidget(pTree,1).addItems(QStringList(self.root.GetList(name)))
            self.itemWidget(pTree,1).setCurrentIndex(self.itemWidget(pTree,1).findText(ref))
            
            self.root.qAction1.setText('Delete %sRef <%s>'%(name,ref))
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(True)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)        

    def onAction(self,action):
        if(action == 'Add SessionRef'):
            self.addSubRef('Session')
        elif(action == 'Add SecurityRef'):
            self.addSubRef('Security')
        elif(action.find('Delete SecurityRef')!=-1):
            self.deleteSubRef('Security')
        elif(action.find('Delete SessionRef')!=-1):
            self.deleteSubRef('Security')
            
    def deleteSubRef(self,what=None):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
    def addSubRef(self,RefName,Node=None):
        pItem = self.currentItem()
        Item = QTreeWidgetItem()
        if Node != None:
            sName = Node.attrib['name']
            sRef = Node.attrib['ref']
            #scomment = Node.attrib['comment']
            scomment = ''
        else:
            sName = RefName
            sRef = ''
            scomment = ''
        if(sName == 'Session'):
            pItem.insertChild(0,Item)
        else:
            pItem.addChild(Item)
        name = QLineEdit(sName)
        name.setDisabled(True)
        ref = QComboBox()
        ref.addItems(QStringList(self.root.GetList(sName)))
        ref.setCurrentIndex(ref.findText(sRef))
        comment =QLineEdit(scomment)
        self.setItemWidget(Item,0,name)
        self.setItemWidget(Item,1,ref)
        self.setItemWidget(Item,3,comment)
        pItem.setExpanded(True)
 
    def loadXML(self,ROOT):
        print "## Load EcuReset!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        List = ROOT.find('EcuResetList')
        if(List != None):
            for Node in List:
                self.addRoot(Node)
        else:
            self.addRoot()

    def toXML(self):
        List = ET.Element('EcuResetList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('EcuReset')
            tree = self.topLevelItem(i)
            Node.attrib['p2p'] = str(self.itemWidget(tree,1).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,2).isChecked())
            Node.attrib['comment'] = str(self.itemWidget(tree,3).text())
            sList = ET.Element('SSRefList') #SS = Session && Security
            for j in range(0,tree.childCount()):
                sNode = ET.Element('SS')
                stree = tree.child(j)
                sNode.attrib['name'] = str(self.itemWidget(stree,0).text())
                sNode.attrib['ref'] = str(self.itemWidget(stree,1).currentText())
                sNode.attrib['comment'] = str(self.itemWidget(stree,3).text())
                sList.append(sNode)
            Node.append(sList)
            List.append(Node)
        return List 
    def deleteRoot(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem())) 
    def addRoot(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
        else:  
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
        name = QLineEdit('ROOT')
        name.setDisabled(True)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment) 
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,p2p) 
        self.setItemWidget(treeItem,2,p2a)    
        self.setItemWidget(treeItem,3,comment)  
        if(Node != None and Node.find('SSRefList') != None):
            self.setCurrentItem(treeItem)
            for sNode in Node.find('SSRefList'):
                self.addSubRef(None, sNode)                      
class twCommunicationControl(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','P2P','P2A','Comment','']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(1,150)
        self.setColumnWidth(2,40)
        self.setColumnWidth(3,600)
        self.addRoot()
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        pTree = self.currentItem()
        name = self.itemWidget(pTree,0).text()
        # update Action 
        if(name=='ROOT'):
            self.root.qAction1.setText('Add SessionRef')
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('Add SecurityRef')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(False)
        else:
            ref = str(self.itemWidget(pTree,1).currentText())
            self.itemWidget(pTree,1).clear()
            self.itemWidget(pTree,1).addItems(QStringList(self.root.GetList(name)))
            self.itemWidget(pTree,1).setCurrentIndex(self.itemWidget(pTree,1).findText(ref))
            
            self.root.qAction1.setText('Delete %sRef <%s>'%(name,ref))
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(True)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)        

    def onAction(self,action):
        if(action == 'Add SessionRef'):
            self.addSubRef('Session')
        elif(action == 'Add SecurityRef'):
            self.addSubRef('Security')
        elif(action.find('Delete SecurityRef')!=-1):
            self.deleteSubRef('Security')
        elif(action.find('Delete SessionRef')!=-1):
            self.deleteSubRef('Security')
            
    def deleteSubRef(self,what=None):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
    def addSubRef(self,RefName,Node=None):
        pItem = self.currentItem()
        Item = QTreeWidgetItem()
        if Node != None:
            sName = Node.attrib['name']
            sRef = Node.attrib['ref']
            #scomment = Node.attrib['comment']
            scomment = ''
        else:
            sName = RefName
            sRef = ''
            scomment = ''
        if(sName == 'Session'):
            pItem.insertChild(0,Item)
        else:
            pItem.addChild(Item)
        name = QLineEdit(sName)
        name.setDisabled(True)
        ref = QComboBox()
        ref.addItems(QStringList(self.root.GetList(sName)))
        ref.setCurrentIndex(ref.findText(sRef))
        comment =QLineEdit(scomment)
        self.setItemWidget(Item,0,name)
        self.setItemWidget(Item,1,ref)
        self.setItemWidget(Item,3,comment)
        pItem.setExpanded(True)
 
    def loadXML(self,ROOT):
        print "## Load CommunicationControl!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        List = ROOT.find('CommunicationControlList')
        if(List != None):
            for Node in List:
                self.addRoot(Node)
        else:
            self.addRoot()

    def toXML(self):
        List = ET.Element('CommunicationControlList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('CommunicationControl')
            tree = self.topLevelItem(i)
            Node.attrib['p2p'] = str(self.itemWidget(tree,1).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,2).isChecked())
            Node.attrib['comment'] = str(self.itemWidget(tree,3).text())
            sList = ET.Element('SSRefList') #SS = Session && Security
            for j in range(0,tree.childCount()):
                sNode = ET.Element('SS')
                stree = tree.child(j)
                sNode.attrib['name'] = str(self.itemWidget(stree,0).text())
                sNode.attrib['ref'] = str(self.itemWidget(stree,1).currentText())
                sNode.attrib['comment'] = str(self.itemWidget(stree,3).text())
                sList.append(sNode)
            Node.append(sList)
            List.append(Node)
        return List 
    def deleteRoot(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem())) 
    def addRoot(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
        else:  
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
        name = QLineEdit('ROOT')
        name.setDisabled(True)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment) 
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,p2p) 
        self.setItemWidget(treeItem,2,p2a)    
        self.setItemWidget(treeItem,3,comment)  
        if(Node != None and Node.find('SSRefList') != None):
            self.setCurrentItem(treeItem)
            for sNode in Node.find('SSRefList'):
                self.addSubRef(None, sNode)
class twRWDID(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Identifier','Attribute','P2P','P2A','Comment','']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(1,150)
        self.setColumnWidth(3,40)
        self.setColumnWidth(4,40)
        self.setColumnWidth(5,600)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        pTree = self.currentItem()
        name = self.itemWidget(pTree,0).text()
        if(self.indexOfTopLevelItem(pTree) != -1):
            self.root.qAction1.setText('Add SessionRef')
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('Add SecurityRef')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(False)
            self.root.qAction3.setText('Delete DataIdentifier <%s>'%(name))
            self.root.qAction3.setStatusTip('')
            self.root.qAction3.setDisabled(False)
        else:
            ref = str(self.itemWidget(pTree,1).currentText())
            self.itemWidget(pTree,1).clear()
            self.itemWidget(pTree,1).addItems(QStringList(self.root.GetList(name)))
            self.itemWidget(pTree,1).setCurrentIndex(self.itemWidget(pTree,1).findText(ref))
            
            self.root.qAction1.setText('Delete %sRef <%s>'%(name,ref))
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(True)
            self.root.qAction3.setText('')
            self.root.qAction3.setStatusTip('')
            self.root.qAction3.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('Add DataIdentifier')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(False)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)

    def onAction(self,action):
        if(action == 'Add SessionRef'):
            self.addSubRef('Session')
        elif(action == 'Add SecurityRef'):
            self.addSubRef('Security')
        elif(action.find('Delete SecurityRef')!=-1):
            self.deleteSubRef('Security')
        elif(action.find('Delete SessionRef')!=-1):
            self.deleteSubRef('Security')
        elif(action == 'Add DataIdentifier'):
            self.addRoot()
        elif(action.find('Delete DataIdentifier')!= -1):
            self.deleteRoot()
            
    def deleteSubRef(self,what=None):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
    def addSubRef(self,RefName,Node=None):
        pItem = self.currentItem()
        Item = QTreeWidgetItem()
        if Node != None:
            sName = Node.attrib['name']
            sRef = Node.attrib['ref']
            #scomment = Node.attrib['comment']
            scomment = ''
        else:
            sName = RefName
            sRef = ''
            scomment = ''
        if(sName == 'Session'):
            pItem.insertChild(0,Item)
        else:
            pItem.addChild(Item)
        name = QLineEdit(sName)
        name.setDisabled(True)
        ref = QComboBox()
        ref.addItems(QStringList(self.root.GetList(sName)))
        ref.setCurrentIndex(ref.findText(sRef))
        comment =QLineEdit(scomment)
        self.setItemWidget(Item,0,name)
        self.setItemWidget(Item,1,ref)
        self.setItemWidget(Item,5,comment)
        pItem.setExpanded(True)
 
    def loadXML(self,ROOT):
        print "## Load ReadWriteByIdentifier!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        List = ROOT.find('RWDIDList')
        if(List != None):
            for Node in List:
                self.addRoot(Node)

    def toXML(self):
        List = ET.Element('RWDIDList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('RWDID')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['identifier'] = str(self.itemWidget(tree,1).text())
            Node.attrib['attribute'] = str(self.itemWidget(tree,2).currentText())
            Node.attrib['p2p'] = str(self.itemWidget(tree,3).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,4).isChecked())
            Node.attrib['comment'] = str(self.itemWidget(tree,5).text())
            sList = ET.Element('SSRefList') #SS = Session && Security
            for j in range(0,tree.childCount()):
                sNode = ET.Element('SS')
                stree = tree.child(j)
                sNode.attrib['name'] = str(self.itemWidget(stree,0).text())
                sNode.attrib['ref'] = str(self.itemWidget(stree,1).currentText())
                sNode.attrib['comment'] = str(self.itemWidget(stree,5).text())
                sList.append(sNode)
            Node.append(sList)
            List.append(Node)
        return List 
    def deleteRoot(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem())) 
    def addRoot(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            sname = Node.attrib['name']
            sidentifier = Node.attrib['identifier']
            sattribute = Node.attrib['attribute']
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
        else:  
            sname = 'TBD'
            sidentifier = 'TBD'
            sattribute = 'rw'
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
        name = QLineEdit(sname)
        identifier = QLineEdit(sidentifier)
        identifier.setToolTip("""
Identifier is 16 bits long for Normal DataIdentifier.
For PeriodDataIdentifier,Identifier is 8bits long, the high byte is fixed to 0xF2.
P.S: For DDD, request Identifier is either 0xF2** or 0xF3**, so normally, Identifier 
     shouldn\'t be 0xF3**.
        """)
        attribute = QComboBox()
        attribute.addItems(QStringList(['r','rw','rp','rwp','rd','rwd','rpd','rwpd','w']))
        attribute.setCurrentIndex(attribute.findText(sattribute))
        attribute.setToolTip("""
r=readable
w=writable    
p=can be periodic  
d=can be dynamic defined
        """)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment) 
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,identifier) 
        self.setItemWidget(treeItem,2,attribute)    
        self.setItemWidget(treeItem,3,p2p) 
        self.setItemWidget(treeItem,4,p2a)
        self.setItemWidget(treeItem,5,comment) 
        if(Node != None and Node.find('SSRefList') != None):
            self.setCurrentItem(treeItem)
            for sNode in Node.find('SSRefList'):
                self.addSubRef(None, sNode)               
class twIOConrol(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Identifier','Info','P2P','P2A','Comment','']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(1,150)
        self.setColumnWidth(3,40)
        self.setColumnWidth(4,40)
        self.setColumnWidth(5,600)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        pTree = self.currentItem()
        name = self.itemWidget(pTree,0).text()
        if(self.indexOfTopLevelItem(pTree) != -1):
            self.root.qAction1.setText('Add SessionRef')
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('Add SecurityRef')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(False)
            self.root.qAction3.setText('Delete IOControl <%s>'%(name))
            self.root.qAction3.setStatusTip('')
            self.root.qAction3.setDisabled(False)
        else:
            ref = str(self.itemWidget(pTree,1).currentText())
            self.itemWidget(pTree,1).clear()
            self.itemWidget(pTree,1).addItems(QStringList(self.root.GetList(name)))
            self.itemWidget(pTree,1).setCurrentIndex(self.itemWidget(pTree,1).findText(ref))
            
            self.root.qAction1.setText('Delete %sRef <%s>'%(name,ref))
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(True)
            self.root.qAction3.setText('')
            self.root.qAction3.setStatusTip('')
            self.root.qAction3.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('Add IOControl')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(False)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)

    def onAction(self,action):
        if(action == 'Add SessionRef'):
            self.addSubRef('Session')
        elif(action == 'Add SecurityRef'):
            self.addSubRef('Security')
        elif(action.find('Delete SecurityRef')!=-1):
            self.deleteSubRef('Security')
        elif(action.find('Delete SessionRef')!=-1):
            self.deleteSubRef('Security')
        elif(action == 'Add IOControl'):
            self.addRoot()
        elif(action.find('Delete IOControl')!= -1):
            self.deleteRoot()
            
    def deleteSubRef(self,what=None):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
    def addSubRef(self,RefName,Node=None):
        pItem = self.currentItem()
        Item = QTreeWidgetItem()
        if Node != None:
            sName = Node.attrib['name']
            sRef = Node.attrib['ref']
            #scomment = Node.attrib['comment']
            scomment = ''
        else:
            sName = RefName
            sRef = ''
            scomment = ''
        if(sName == 'Session'):
            pItem.insertChild(0,Item)
        else:
            pItem.addChild(Item)
        name = QLineEdit(sName)
        name.setDisabled(True)
        ref = QComboBox()
        ref.addItems(QStringList(self.root.GetList(sName)))
        ref.setCurrentIndex(ref.findText(sRef))
        comment =QLineEdit(scomment)
        self.setItemWidget(Item,0,name)
        self.setItemWidget(Item,1,ref)
        self.setItemWidget(Item,5,comment)
        pItem.setExpanded(True)
 
    def loadXML(self,ROOT):
        print "## Load InputOutputControl!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        List = ROOT.find('IOControlList')
        if(List != None):
            for Node in List:
                self.addRoot(Node)

    def toXML(self):
        List = ET.Element('IOControlList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('IOControl')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['identifier'] = str(self.itemWidget(tree,1).text())
            Node.attrib['info'] = str(self.itemWidget(tree,2).text())
            Node.attrib['p2p'] = str(self.itemWidget(tree,3).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,4).isChecked())
            Node.attrib['comment'] = str(self.itemWidget(tree,5).text())
            sList = ET.Element('SSRefList') #SS = Session && Security
            for j in range(0,tree.childCount()):
                sNode = ET.Element('SS')
                stree = tree.child(j)
                sNode.attrib['name'] = str(self.itemWidget(stree,0).text())
                sNode.attrib['ref'] = str(self.itemWidget(stree,1).currentText())
                sNode.attrib['comment'] = str(self.itemWidget(stree,5).text())
                sList.append(sNode)
            Node.append(sList)
            List.append(Node)
        return List 
    def deleteRoot(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem())) 
    def addRoot(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            sname = Node.attrib['name']
            sidentifier = Node.attrib['identifier']
            sinfo = Node.attrib['info']
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
        else:  
            sname = 'TBD'
            sidentifier = 'TBD'
            sinfo = '(STA,Op=1,EnM=0,Sts=1);(RTD,Op=0,EnM=0,Sts=1);(RCTE,Op=0,EnM=0,Sts=1);(FCS,Op=0,EnM=0,Sts=1);'
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
        name = QLineEdit(sname)
        identifier = QLineEdit(sidentifier)
        info = QLineEdit(sinfo)
        info.setToolTip("""
Function:
    STA=Short Term Adjustment
    RTD=Reset To Default 
    RCTE=Return Control To Ecu     
    FCS=Freeze Current State 
    Note: If any of the function above is not configured, then the function is disabled.
Size:
    Op=Option Record Size
    EnM=Enable Mask Record Size
    Sts=Status Record Size(Answer)
 
        """)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment) 
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,identifier) 
        self.setItemWidget(treeItem,2,info)    
        self.setItemWidget(treeItem,3,p2p) 
        self.setItemWidget(treeItem,4,p2a)
        self.setItemWidget(treeItem,5,comment) 
        if(Node != None and Node.find('SSRefList') != None):
            self.setCurrentItem(treeItem)
            for sNode in Node.find('SSRefList'):
                self.addSubRef(None, sNode)  
class twRoutineConrol(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Identifier','Info','P2P','P2A','Comment','']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.setColumnWidth(1,150)
        self.setColumnWidth(3,40)
        self.setColumnWidth(4,40)
        self.setColumnWidth(5,600)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        pTree = self.currentItem()
        name = self.itemWidget(pTree,0).text()
        if(self.indexOfTopLevelItem(pTree) != -1):
            self.root.qAction1.setText('Add SessionRef')
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('Add SecurityRef')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(False)
            self.root.qAction3.setText('Delete RoutineControl <%s>'%(name))
            self.root.qAction3.setStatusTip('')
            self.root.qAction3.setDisabled(False)
        else:
            ref = str(self.itemWidget(pTree,1).currentText())
            self.itemWidget(pTree,1).clear()
            self.itemWidget(pTree,1).addItems(QStringList(self.root.GetList(name)))
            self.itemWidget(pTree,1).setCurrentIndex(self.itemWidget(pTree,1).findText(ref))
            
            self.root.qAction1.setText('Delete %sRef <%s>'%(name,ref))
            self.root.qAction1.setStatusTip('')
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setText('')
            self.root.qAction2.setStatusTip('')
            self.root.qAction2.setDisabled(True)
            self.root.qAction3.setText('')
            self.root.qAction3.setStatusTip('')
            self.root.qAction3.setDisabled(True)
    def updateAction(self):
        self.root.qAction1.setText('Add RoutineControl')
        self.root.qAction1.setStatusTip('')
        self.root.qAction1.setDisabled(False)
        self.root.qAction2.setText('')
        self.root.qAction2.setStatusTip('')
        self.root.qAction2.setDisabled(True)
        self.root.qAction3.setText('')
        self.root.qAction3.setStatusTip('')
        self.root.qAction3.setDisabled(True)

    def onAction(self,action):
        if(action == 'Add SessionRef'):
            self.addSubRef('Session')
        elif(action == 'Add SecurityRef'):
            self.addSubRef('Security')
        elif(action.find('Delete SecurityRef')!=-1):
            self.deleteSubRef('Security')
        elif(action.find('Delete SessionRef')!=-1):
            self.deleteSubRef('Security')
        elif(action == 'Add RoutineControl'):
            self.addRoot()
        elif(action.find('Delete RoutineControl')!= -1):
            self.deleteRoot()
            
    def deleteSubRef(self,what=None):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
    def addSubRef(self,RefName,Node=None):
        pItem = self.currentItem()
        Item = QTreeWidgetItem()
        if Node != None:
            sName = Node.attrib['name']
            sRef = Node.attrib['ref']
            #scomment = Node.attrib['comment']
            scomment = ''
        else:
            sName = RefName
            sRef = ''
            scomment = ''
        if(sName == 'Session'):
            pItem.insertChild(0,Item)
        else:
            pItem.addChild(Item)
        name = QLineEdit(sName)
        name.setDisabled(True)
        ref = QComboBox()
        ref.addItems(QStringList(self.root.GetList(sName)))
        ref.setCurrentIndex(ref.findText(sRef))
        comment =QLineEdit(scomment)
        self.setItemWidget(Item,0,name)
        self.setItemWidget(Item,1,ref)
        self.setItemWidget(Item,5,comment)
        pItem.setExpanded(True)
 
    def loadXML(self,ROOT):
        print "## Load RoutineControl!"
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        List = ROOT.find('RoutineControlList')
        if(List != None):
            for Node in List:
                self.addRoot(Node)

    def toXML(self):
        List = ET.Element('RoutineControlList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('RoutineControl')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['identifier'] = str(self.itemWidget(tree,1).text())
            Node.attrib['info'] = str(self.itemWidget(tree,2).text())
            Node.attrib['p2p'] = str(self.itemWidget(tree,3).isChecked())
            Node.attrib['p2a'] = str(self.itemWidget(tree,4).isChecked())
            Node.attrib['comment'] = str(self.itemWidget(tree,5).text())
            sList = ET.Element('SSRefList') #SS = Session && Security
            for j in range(0,tree.childCount()):
                sNode = ET.Element('SS')
                stree = tree.child(j)
                sNode.attrib['name'] = str(self.itemWidget(stree,0).text())
                sNode.attrib['ref'] = str(self.itemWidget(stree,1).currentText())
                sNode.attrib['comment'] = str(self.itemWidget(stree,5).text())
                sList.append(sNode)
            Node.append(sList)
            List.append(Node)
        return List 
    def deleteRoot(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem())) 
    def addRoot(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            sname = Node.attrib['name']
            sidentifier = Node.attrib['identifier']
            sinfo = Node.attrib['info']
            scomment = Node.attrib['comment']
            sp2p = Node.attrib['p2p']
            sp2a = Node.attrib['p2a']
        else:  
            sname = 'TBD'
            sidentifier = 'TBD'
            sinfo = '(Start,Op=1,Sts=1);(Stop,Op=0,Sts=1);(Result,Sts=1);'
            scomment = ''
            sp2p = 'True'
            sp2a = 'False'
        name = QLineEdit(sname)
        identifier = QLineEdit(sidentifier)
        info = QLineEdit(sinfo)
        info.setToolTip("""
Function:
    Start=Start Routine
    Stop=Stop Routine
    Result=Request Routine Result  
    Note: If any of the function above is not configured, then the function is disabled.
Size:
    Op=Option Record Size
    Sts=Status Record Size(Answer)
 
        """)
        p2p = QCheckBox()
        p2p.setChecked(tBool(sp2p))
        p2a = QCheckBox()
        p2a.setChecked(tBool(sp2a))
        comment = QLineEdit(scomment) 
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,identifier) 
        self.setItemWidget(treeItem,2,info)    
        self.setItemWidget(treeItem,3,p2p) 
        self.setItemWidget(treeItem,4,p2a)
        self.setItemWidget(treeItem,5,comment) 
        if(Node != None and Node.find('SSRefList') != None):
            self.setCurrentItem(treeItem)
            for sNode in Node.find('SSRefList'):
                self.addSubRef(None, sNode)                               
dcm_TreeTable =  [
    ['SessionControl',twSession],
    ['SecurityAccess',twSecurity],
    ['EcuReset',twEcuReset],
    ['CommunicationControl',twCommunicationControl],
    ['ReadWriteDataByIdentifier',twRWDID],
    #['ReadDataByPeriodicIdentifier',easyDcmDefaultTree], # processed by twRWDID
    ['ReadWriteMemoryByAddress',easyDcmDefaultTree],
    ['ReadScalingDataByIdentifier',easyDcmDefaultTree],
    #['DynamicallyDefineDataIdentifier',easyDcmDefaultTree], # processed by twRWDID
    ['ClearDiagnosticInformation',easyDcmDefaultTree],
    ['ReadDTCInformation',easyDcmDefaultTree],
    ['InputOutputControlByIdentifier',twIOConrol],
    ['RoutineControl',twRoutineConrol],
    ['RequestDownload',easyDcmDefaultTree],
    ['RequestUpload',easyDcmDefaultTree],
    ['TransferData',easyDcmDefaultTree],
    ['RequestTransferExit',easyDcmDefaultTree],
];

class easyComCfgTree(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent)
        self.setHeaderLabel('easyCom')
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Com')))
        Item = QTreeWidgetItem(QStringList('Dcm')) 
        self.addTopLevelItem(Item)
        for tw in dcm_TreeTable:
            Item.addChild(QTreeWidgetItem(QStringList(tw[0])))
        Item.setExpanded(True)
        self.setMaximumWidth(350); 

class easyComGui(QMainWindow):
    DcmWidgetList = []
    def __init__(self):
        global dcm_TreeTable
        QMainWindow.__init__(self, None)
        self.easyTree = easyComCfgTree(self)
        self.easyComTree = easyComTree(self)
        for tw in dcm_TreeTable:
            self.DcmWidgetList.append([tw[0],tw[1](self)])
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        self.creMenu()
        self.creGui()
    def GetList(self,what):
        if(what == 'Session'):
           for wd in  self.DcmWidgetList:
               if(wd[0] == 'SessionControl'):
                   return wd[1].GetList(what)
        elif(what == 'Security'):
           for wd in  self.DcmWidgetList:
               if(wd[0] == 'SecurityAccess'):
                   return wd[1].GetList(what)
                
    def on_move_up(self):
        if(self.easyComTree.isVisible()):
            self.easyComTree.moveSignalUp(True)
    def on_move_down(self):
        if(self.easyComTree.isVisible()):
            self.easyComTree.moveSignalUp(False)
    def creMenu(self):
        qAction =QAction(self)
        qAction.setIcon(QIcon('./res/move_up.bmp'))
        qAction.setStatusTip('move the Selected signal up')
        self.connect(qAction,SIGNAL('triggered()'),self.on_move_up) 
        self.menuBar().addAction(qAction)
        
        qAction =QAction(self)
        qAction.setIcon(QIcon('./res/move_down.bmp'))
        qAction.setStatusTip('move the Selected signal up')
        self.connect(qAction,SIGNAL('triggered()'),self.on_move_down)
        self.menuBar().addAction(qAction)
        
        self.menuBar().addSeparator()
        #  create Three three Action
        self.qAction1=QAction(self.tr('Action1'),self) 
        self.connect(self.qAction1,SIGNAL('triggered()'),self.mqAction1) 
        self.menuBar().addAction(self.qAction1)
        self.qAction1.setDisabled(True)
        
        self.qAction2=QAction(self.tr('Action2'),self) 
        self.connect(self.qAction2,SIGNAL('triggered()'),self.mqAction2) 
        self.menuBar().addAction(self.qAction2)
        self.qAction2.setDisabled(True)
        
        self.qAction3=QAction(self.tr('Action3'),self) 
        self.connect(self.qAction3,SIGNAL('triggered()'),self.mqAction3) 
        self.menuBar().addAction(self.qAction3)
        self.qAction3.setDisabled(True)
        
    def mqAction1(self):
        if(self.qAction1.text() == 'Add Signal'):
            self.easyComTree.addSignal()
        elif(str(self.qAction1.text()).find('Delete Signal') != -1):
            self.easyComTree.deleteSignal()
        else:
            for wd in self.DcmWidgetList:
                if(wd[1].isVisible()):
                    wd[1].onAction(str(self.qAction1.text()))
    def mqAction2(self):
        for wd in self.DcmWidgetList:
            if(wd[1].isVisible()):
                wd[1].onAction(str(self.qAction2.text()))

    def mqAction3(self):
        for wd in self.DcmWidgetList:
            if(wd[1].isVisible()):
                wd[1].onAction(str(self.qAction3.text()))
    def creGui(self):
        self.qSplitter.insertWidget(0,self.easyTree)
        self.qSplitter.insertWidget(1,self.easyComTree)
        id = 1
        for wd in self.DcmWidgetList:
            self.qSplitter.insertWidget(id,wd[1])
            id += 1
        self.showTableWidget(self.easyComTree)
        self.setCentralWidget(self.qSplitter)
        self.connect(self.easyTree,SIGNAL('itemClicked(QTreeWidgetItem*, int)'),self.easyTreeClicked)  
    def showTableWidget(self,widget):
        for wd in self.DcmWidgetList:
            if(wd[0] == widget):
                wd[1].updateAction()
                wd[1].setVisible(True)
            else:
                wd[1].setVisible(False)
            
        if(self.easyComTree == widget):
            self.qAction1.setText('Add Signal')
            self.qAction1.setStatusTip('Add Signal for COM')
            self.qAction1.setDisabled(False)
            self.qAction2.setDisabled(True)
            self.easyComTree.setVisible(True);
        else:
            self.easyComTree.setVisible(False);
    def easyTreeClicked(self,item,column):
        if(item.text(0) == 'Com'):
            self.showTableWidget(self.easyComTree)
        elif(item.text(0) == 'Dcm'):
            self.showTableWidget(None)
            self.qAction1.setText('')
            self.qAction1.setStatusTip('')
            self.qAction1.setDisabled(False)
            self.qAction2.setText('')
            self.qAction2.setStatusTip('')
            self.qAction2.setDisabled(False)
        else:
            if(item.parent().text(0) == 'Dcm'):
                self.showTableWidget(str(item.text(0)))
    def mOpen(self,pdir):
        wfxml = '%s/com.wfxml'%(pdir)
        root = ET.parse(wfxml).getroot();
        self.easyComTree.loadXML(root)
        for wd in self.DcmWidgetList:
            wd[1].loadXML(root)
        self.qAction1.setDisabled(True)
        self.qAction2.setDisabled(True)
    def mSave(self,pdir):
        wfxml = '%s/com.wfxml'%(pdir)
        ROOT = ET.Element('COMROOT')
        ROOT.append(self.easyComTree.toXML())
        for wd in self.DcmWidgetList:
            if(wd[1].toXML() != None):
                ROOT.append(wd[1].toXML())
        tree = ET.ElementTree(ROOT)
        tree.write(wfxml, encoding="utf-8", xml_declaration=True);
    def mGen(self,pdir):
        from gen.GenCom import GenCom
        wfxml = '%s/com.wfxml'%(pdir)
        GenCom(str(wfxml))
