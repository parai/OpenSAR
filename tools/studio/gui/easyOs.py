from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys
import threading
import socket
import time

__all__ = ['easyOsGui']

class easyAlarmCfgTbl(QTableWidget):
    parent = None
    def __init__(self,parent):  
        super(QTableWidget,self).__init__(parent)  
        self.parent = parent
        self.setColumnCount(9)  
        self.setRowCount(0)  
        self.setHorizontalHeaderLabels(QStringList(['Alarm Name','Owner Counter','Autostart','APPMODE',
                        'Start Time','Cycle','Action Type','Task','Event']))
        self.setMinimumWidth(800); 

class easyTaskCfgTbl(QTableWidget):
    parent = None
    taskid = 0
    def __init__(self,parent):  
        super(QTableWidget,self).__init__(parent)  
        self.parent = parent
        self.setColumnCount(5)  
        self.setRowCount(0)  
        self.setHorizontalHeaderLabels(QStringList(['Task Name','Stack Size','Priority',
                    'Autostart','APPMODE']))
        self.setMinimumWidth(800); 
        self.setColumnWidth(0,200)
        self.setColumnWidth(4,200)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            task = self.cellWidget(self.currentRow(),0).text()
            self.parent.qAction1.setText('Delete Task<%s>'%(task))
            self.parent.qAction1.setStatusTip('Delete This Task <%s> Object, Be Careful as this action is dangerous.'
                                        %(task))
            self.parent.qAction1.setDisabled(False)
        except:
            self.parent.qAction1.setDisabled(True)
    def addTask(self):
            index = self.rowCount()
            self.setRowCount(self.rowCount()+1) 
            item0 = QLineEdit(self.tr('Task%d'%(self.taskid)))
            item1 = QSpinBox()      
            item2 = QSpinBox()
            item3 = QComboBox()
            item3.addItems(QStringList(['True','False']))
            item4 = QLineEdit(self.tr('OSDEFAULTMODE'))  
            self.setCellWidget(index,0,item0)    
            self.setCellWidget(index,1,item1)  
            self.setCellWidget(index,2,item2)  
            self.setCellWidget(index,3,item3)  
            self.setCellWidget(index,4,item4)
            self.taskid += 1
    def deleteTask(self):
        self.removeRow(self.currentRow())
        if(0 == self.rowCount()):
            self.parent.qAction1.setDisabled(True)

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
        self.easyTree = easyOsCfgTree(self)
        self.easyTaskTbl = easyTaskCfgTbl(self)
        self.easyAlarmTbl = easyAlarmCfgTbl(self)
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
         
        sItem=QAction(self.tr('Generate'),self) 
        sItem.setStatusTip('Generate OpenSAR Os configure C file.')
        self.connect(sItem,SIGNAL('triggered()'),self.mGen)  
        self.menuBar().addAction(sItem)
        
        sItem=QAction(self.tr('       '),self) 
        self.menuBar().addAction(sItem)
        sItem.setDisabled(True)
        
        #  create Three three Action
        self.qAction1=QAction(self.tr('Action1'),self) 
        self.connect(self.qAction1,SIGNAL('triggered()'),self.mqAction1) 
        self.menuBar().addAction(self.qAction1)
        self.qAction1.setDisabled(True)
    def mqAction1(self):
        if(self.qAction1.text() == 'Add Task'):
            self.easyTaskTbl.addTask()
        elif(str(self.qAction1.text()).find('Delete Task') != -1):
            self.easyTaskTbl.deleteTask()
    def creGui(self):
        self.qSplitter.insertWidget(0,self.easyTree)
        self.qSplitter.insertWidget(1,self.easyTaskTbl)
        self.qSplitter.insertWidget(1,self.easyAlarmTbl)
        self.showTableWidget(self.easyAlarmTbl)
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
        if(item.text(0) == 'Task'):
            self.qAction1.setText('Add Task')
            self.qAction1.setStatusTip('Add a Task Object.')
            self.qAction1.setDisabled(False)
        else:
            self.qAction1.setDisabled(True)
    def mOpen(self):
        pass
    def mSave(self):
        pass
    def mGen(self):
        pass