from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys
import threading
import socket
import time

__all__ = ['easyOsGui']

class easyAlarmCfgTbl(QTableWidget):
    time = None
    def __init__(self,parent=None):  
        super(QTableWidget,self).__init__(parent)  
        self.setColumnCount(9)  
        self.setRowCount(0)  
        self.setHorizontalHeaderLabels(QStringList(['Alarm Name','Owner Counter','Autostart','APPMODE',
                                    'Start Time','Cycle','Action Type','Task','Event']))
        self.setMinimumWidth(800); 

class easyTaskCfgTbl(QTableWidget):
    time = None
    def __init__(self,parent=None):  
        super(QTableWidget,self).__init__(parent)  
        self.setColumnCount(5)  
        self.setRowCount(0)  
        self.setHorizontalHeaderLabels(QStringList(['Task Name','Stack Size','Priority','Autostart','APPMODE']))
        self.setMinimumWidth(800); 
    def addCanMsg(self,msg=None):
            index = self.rowCount()
            self.setRowCount(self.rowCount()+1) 
            item0 = QTableWidgetItem(self.tr('bTask1'))
            item0.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
            item1 = QTableWidgetItem(self.tr('512'))
            item1.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
            item2 = QTableWidgetItem(self.tr('5'))
            item2.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
            item3 = QTableWidgetItem(self.tr('True'))
            item3.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
            item4 = QTableWidgetItem(self.tr('OSDEFAULTMODE'))
            item4.setFlags(Qt.ItemIsSelectable|Qt.ItemIsEnabled)
            self.setItem(index,0,item0)    
            self.setItem(index,1,item1)  
            self.setItem(index,2,item2)  
            self.setItem(index,3,item3)  
            self.setItem(index,4,item4) 

class easyOsCfgTree(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent)
        self.setHeaderLabel('easyOs')
        self.addTopLevelItem(QTreeWidgetItem(QStringList('General')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Application')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Task')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Counter')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Alarm')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Event')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('ISR')))
        self.setMaximumWidth(400);
        self.setMinimumWidth(200); 

class easyOsGui(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.easyTree = easyOsCfgTree()
        self.easyTaskTbl = easyTaskCfgTbl()
        self.easyAlarmTbl = easyAlarmCfgTbl()
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        #self.creToolbar()
        self.creGui()
        self.creMenu()
    def creMenu(self):
        sItem=QAction(self.tr('Open'),self) 
        sItem.setStatusTip('Open a OpenSAR Os configure file.')
        self.connect(sItem,SIGNAL('triggered()'),self.mOpen)   
        self.menuBar().addAction(sItem) 
        sItem=QAction(self.tr('Save'),self) 
        sItem.setStatusTip('Save the OpenSAR Os configure file.')
        self.connect(sItem,SIGNAL('triggered()'),self.mSave)  
        self.menuBar().addAction(sItem)  
        
        self.menuBar().addAction(sItem) 
        sItem=QAction(self.tr('Generate'),self) 
        sItem.setStatusTip('Generate OpenSAR Os configure C file.')
        self.connect(sItem,SIGNAL('triggered()'),self.mGen)  
        self.menuBar().addAction(sItem)
    def creGui(self):
        self.qSplitter.insertWidget(0,self.easyTree)
        self.qSplitter.insertWidget(1,self.easyTaskTbl)
        self.qSplitter.insertWidget(1,self.easyAlarmTbl)
        self.setCentralWidget(self.qSplitter)
        self.connect(self.easyTree,SIGNAL('itemClicked(QTreeWidgetItem*, int)'),self.easyTreeClicked)  
    def showTableWidget(self,widget):
        if(self.easyTaskTbl == widget):
            self.easyTaskTbl.setVisible(True);
        else:
            self.easyTaskTbl.setVisible(False);
        if(self.easyAlarmTbl == widget):
            self.easyAlarmTbl.setVisible(True);
        else:
            self.easyAlarmTbl.setVisible(False);
    def easyTreeClicked(self,item,column):
        if(item.text(0) == 'Alarm'):
            self.showTableWidget(self.easyAlarmTbl)
        elif(item.text(0) == 'Task'):
            self.showTableWidget(self.easyTaskTbl)
    def mOpen(self):
        pass
    def mSave(self):
        pass
    def mGen(self):
        pass