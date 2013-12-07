from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import xml.etree.ElementTree as ET

__all__ = ['easyCanIfGui']

class easyCanIfGeneral(QFrame):
    def __init__(self):
        QFrame.__init__(self, None)
        self.det = QCheckBox('Development Error Detection')  
        self.verinfo = QCheckBox('Version Info API') 
        self.dlcchk = QCheckBox('DLC Check Support')  
        ly = QHBoxLayout()
        ly.addWidget(self.det)
        ly.addWidget(self.verinfo)
        ly.addWidget(self.dlcchk)
        self.setLayout(ly) 
    def toXML(self):
        General = ET.Element('General')
        General.attrib['det'] = str(self.det.isChecked())
        General.attrib['verinfo'] = str(self.verinfo.isChecked())
        General.attrib['dlcchk'] = str(self.dlcchk.isChecked())
        return General
    def loadXML(self,ROOT):
        Node = ROOT.find('General')
        if(Node == None):
            print 'no configuration for CanIf General.'
            return
        self.det.setChecked(bool(Node.attrib['det']))
        self.verinfo.setChecked(bool(Node.attrib['verinfo']))
        self.dlcchk.setChecked(bool(Node.attrib['dlcchk']))
        
class easyCanIfCfgTree(QTreeWidget):
    channelid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent)  
        self.root = parent
        self.setHeaderLabels(QStringList(['Name','1','2','3','4','5']))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
        self.setColumnWidth(0,200)
        self.setColumnWidth(1,200)
        self.setColumnWidth(2,200)
        self.setColumnWidth(3,200)
        self.setColumnWidth(4,200)
        self.setColumnWidth(5,200)
    def itemSelectionChanged3(self):
        pass

    def itemSelectionChanged2(self):
        # sub item clicked
        cTree = self.currentItem()
        pTree = cTree.parent()
        if(self.indexOfTopLevelItem(pTree) != -1):
            name = self.itemWidget(cTree,0).text()
            self.root.qAction3.setText('Delete HOH<%s>'%(name))
            self.root.qAction3.setStatusTip('Delete This HOH <%s> Object, Be Careful as this action is dangerous.'%(name))
            self.root.qAction3.setDisabled(False)
            self.root.qAction2.setText('Add Channel')
            self.root.qAction2.setStatusTip('Add Channel.')
            self.root.qAction2.setDisabled(False)
            self.root.qAction1.setText('Add PDU For HOH<%s>'%(name))
            self.root.qAction1.setStatusTip('Add PDU For HOH<%s>'%(name))
            self.root.qAction1.setDisabled(False)
            # update this HOH
            sctrl = self.itemWidget(pTree,1).currentText()
            ref = self.itemWidget(cTree,2)
            sref=ref.currentText()
            ref.clear()
            # TODO , get from CAN, but now I don't support CAN
            if(self.itemWidget(cTree,3) != None):
                ref.addItem('%s_HRH'%(sctrl))
            else:
                ref.addItem('%s_HTH'%(sctrl))
            ref.setCurrentIndex(ref.findText(sref)) 
        else:
            self.itemSelectionChanged3() 
    def itemSelectionChanged(self):
        pTree = self.currentItem()
        if(self.indexOfTopLevelItem(pTree) != -1):
            name = self.itemWidget(pTree,0).text()
            self.root.qAction3.setText('Delete Channel<%s>'%(name))
            self.root.qAction3.setStatusTip('Delete This Channel <%s> Object, Be Careful as this action is dangerous.'%(name))
            self.root.qAction3.setDisabled(False)
            self.root.qAction2.setText('Add HTH For Channel<%s>'%(name))
            self.root.qAction2.setStatusTip('Add HTH For Channel<%s>.'%(name))
            self.root.qAction2.setDisabled(False)
            self.root.qAction1.setText('Add HRH For Channel<%s>'%(name))
            self.root.qAction1.setStatusTip('Add HRH For Channel<%s>.'%(name))
            self.root.qAction1.setDisabled(False)
        else:
            self.itemSelectionChanged2()

    def addHRH(self,Node=None):
        pTree = self.currentItem()
        treeItem = QTreeWidgetItem()
        pTree.addChild(treeItem)
        if(None != Node):
            sname = Node.attrib['name']
            stype = Node.attrib['type']
            sref = Node.attrib['ref']
            sfilter = Node.attrib['filter']
        else:
            sname = '%s_HRH%s'%(self.itemWidget(pTree,0).text(),pTree.childCount())
            stype = 'BASIC'
            sref = ''
            sfilter = 'False'
        
        name = QLineEdit(sname)
        type = QComboBox()
        type.addItems(QStringList(['BASIC','FULL']))
        type.setCurrentIndex(type.findText(stype))
        
        ref = QComboBox()
        list = []
        if(sref != ''):
            list.append(sref)
        ref.addItems(QStringList(list))
        ref.setCurrentIndex(ref.findText(sref))
        
        filter = QCheckBox('Software Filter')
        filter.setChecked(bool(sfilter))
        
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,type)
        self.setItemWidget(treeItem,2,ref)
        self.setItemWidget(treeItem,3,filter)
        pTree.setExpanded(True)
        
    def addHTH(self,Node=None):
        pTree = self.currentItem()
        treeItem = QTreeWidgetItem()
        pTree.addChild(treeItem)
        if(None != Node):
            sname = Node.attrib['name']
            stype = Node.attrib['type']
            sref = Node.attrib['ref']
        else:
            sname = '%s_HTH%s'%(self.itemWidget(pTree,0).text(),pTree.childCount())
            stype = 'BASIC'
            sref = ''
        
        name = QLineEdit(sname)
        type = QComboBox()
        type.addItems(QStringList(['BASIC','FULL']))
        type.setCurrentIndex(type.findText(stype))
        
        ref = QComboBox()
        list = []
        if(sref != ''):
            list.append(sref)
        ref.addItems(QStringList(list))
        ref.setCurrentIndex(ref.findText(sref))
       
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,type)
        self.setItemWidget(treeItem,2,ref)   
        pTree.setExpanded(True)   
    
    def deleteHOH(self):     
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove HOH.'    
    def deleteChannel(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))

    def addChannel(self,Node=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem) 
        if(Node != None):
            sname = Node.attrib['name']
            sctrl = Node.attrib['ctrl']
        else:  
            sname = 'CAN_CHL_%s'%(self.channelid)
            sctrl = ''   
        name = QLineEdit(sname)
        ctrl = QComboBox()
        ctrl.addItems(QStringList(['CAN_CTRL_0','CAN_CTRL_1','CAN_CTRL_2','CAN_CTRL_3','CAN_CTRL_4']))
        ctrl.setCurrentIndex(ctrl.findText(sctrl))
        ctrl.setToolTip('CanIf HW Controller Reference')
        ctrl.setStatusTip('CanIf HW Controller Reference')
        self.setItemWidget(treeItem,0,name)    
        self.setItemWidget(treeItem,1,ctrl) 
        self.channelid += 1
        self.setCurrentItem(treeItem)
        if(None == Node.find('HOHList')):
            return
        for Node2 in Node.find('HOHList'):
            try:
                sfilter = Node2.attrib['filter']
                self.addHRH(Node2)
            except:
                self.addHTH(Node2)

    def toXML(self):
        List = ET.Element('ChannelList')
        for i in range(0,self.topLevelItemCount()):
            Node = ET.Element('Channel')
            tree = self.topLevelItem(i)
            Node.attrib['name'] = str(self.itemWidget(tree,0).text())
            Node.attrib['ctrl'] = str(self.itemWidget(tree,1).currentText())
            List2 = ET.Element('HOHList')
            for j in range(0,tree.childCount()):
                tree2 = tree.child(j)
                if(self.itemWidget(tree2, 3) != None):
                    Node2 = ET.Element('HRH')
                    Node2.attrib['filter']  = str(self.itemWidget(tree2,3).isChecked())
                else:
                    Node2 = ET.Element('HTH')  
                Node2.attrib['name'] = str(self.itemWidget(tree2,0).text())
                Node2.attrib['type'] = str(self.itemWidget(tree2,1).currentText())
                Node2.attrib['ref']  = str(self.itemWidget(tree2,2).currentText())
                List2.append(Node2)
                Node.append(List2)
            List.append(Node)  
        return List

    def loadXML(self,ROOT):
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        List = ROOT.find('ChannelList')
        if(List == None):
            return
        for Node in List:
            self.addChannel(Node)

# this is the root configuration of OpenSAR CanIf
class easyCanIfGui(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.general = easyCanIfGeneral()
        self.entry = easyCanIfCfgTree(self)
        self.CreGui()
        self.creMenu()
    def CreGui(self):
        qSplitter = QSplitter(Qt.Vertical,self)
        qSplitter.insertWidget(0,self.general)
        qSplitter.insertWidget(1,self.entry)
        self.setCentralWidget(qSplitter)
    def creMenu(self):
        #  create Actions
        self.qAction1=QAction(self.tr('Add Channel'),self) 
        self.connect(self.qAction1,SIGNAL('triggered()'),self.mqAction1) 
        self.menuBar().addAction(self.qAction1)
        self.qAction1.setDisabled(False)
        
        self.qAction2=QAction(self.tr('Action2'),self) 
        self.connect(self.qAction2,SIGNAL('triggered()'),self.mqAction2) 
        self.menuBar().addAction(self.qAction2)
        self.qAction2.setDisabled(True)
        
        self.qAction3=QAction(self.tr('Action3'),self) 
        self.connect(self.qAction3,SIGNAL('triggered()'),self.mqAction3) 
        self.menuBar().addAction(self.qAction3)
        self.qAction3.setDisabled(True)
    def mqAction1(self):
        if(self.qAction1.text() == 'Add Channel'):
            self.entry.addChannel()
        elif(str(self.qAction1.text()).find('Add HRH For Channel') != -1):
            self.entry.addHRH()
        elif(str(self.qAction1.text()).find('Add PDU For HOH') != -1):
            self.entry.addPDU()
            
    def mqAction2(self):
        if(str(self.qAction2.text()).find('Add HTH For Channel') != -1):
            self.entry.addHTH()
        elif(self.qAction2.text() == 'Add Channel'):
            self.entry.addChannel()
        
    def mqAction3(self):
        if(str(self.qAction3.text()).find('Delete Channel') != -1):
            self.entry.deleteChannel()
        elif(str(self.qAction3.text()).find('Delete HOH') != -1):
            self.entry.deleteHOH()
    def mOpen(self,pdir):
        wfxml = '%s/canif.wfxml'%(pdir)
        root = ET.parse(wfxml).getroot();
        self.general.loadXML(root)
        self.entry.loadXML(root)
        self.qAction1.setText('Add Channel')
        self.qAction1.setDisabled(False)
        self.qAction2.setDisabled(True)
        self.qAction3.setDisabled(True)
    def mSave(self,pdir):
        wfxml = '%s/canif.wfxml'%(pdir)
        ROOT = ET.Element('ROOT')
        ROOT.append(self.general.toXML())
        ROOT.append(self.entry.toXML())
        tree = ET.ElementTree(ROOT)
        tree.write(wfxml, encoding="utf-8", xml_declaration=True);
    def mGen(self,pdir):
        pass    