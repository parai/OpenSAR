from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys
import threading
import socket
import time

__all__ = ['easyOsGui']            

class easyAlarmCfgTbl(QTreeWidget):
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root = parent
        list = ['Alarm Name','Owner Counter','Autostart','App Mode',
                'Start Time','Cycle','Action','Task','Event']
        self.setHeaderLabels(QStringList(list))

class easyTaskCfgTbl(QTreeWidget):
    taskid = 0
    eventid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Task Name','Stack Size','Priority','Autostart','Schedule']
        self.setHeaderLabels(QStringList(list))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
        self.connect(self, SIGNAL('itemClicked(QTreeWidgetItem,int)'),self.itemClicked)
    def itemClicked(self,item,column):
        self.setCurrentItem()
        print 'fff'
    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            if(self.indexOfTopLevelItem(treeItem) != -1):
                task = self.itemWidget(treeItem,0).text()
                self.root.qAction1.setText('Delete Task<%s>'%(task))
                self.root.qAction1.setStatusTip('Delete This Task <%s> Object, Be Careful as this action is dangerous.'%(task))
                self.root.qAction1.setDisabled(False)
                self.root.qAction2.setText('Add Event For Task<%s>'%(task))
                self.root.qAction2.setStatusTip('Add Event For Task<%s>.'%(task))
                self.root.qAction2.setDisabled(False)
            else:
                # Sub Event Item
                task = self.itemWidget(treeItem.parent(),0).text()
                self.root.qAction1.setText('Delete Task<%s>'%(task))
                self.root.qAction1.setStatusTip('Delete This Task <%s> Object, Be Careful as this action is dangerous.'%(task))
                self.root.qAction1.setDisabled(False)
                event = self.itemWidget(treeItem,0).text()
                self.root.qAction2.setText('Delete Event <%s>'%(event))
                self.root.qAction2.setStatusTip('Delete This Event <%s> Object, Be Careful as this action is dangerous.'%(event))
                self.root.qAction2.setDisabled(False)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def addTask(self):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem)
        # task name 
        taskName = QLineEdit('Task%s'%(self.taskid))
        # stack size
        stackSize = QSpinBox()      
        stackSize.setRange(64,0xFFFFF)
        stackSize.setValue(512) 
        # Priority
        taskPrio = QSpinBox()
        taskPrio.setRange(0,256)
        taskPrio.setValue(8) 
        # Autostart
        autostart = QComboBox()
        autostart.addItems(QStringList(['True','False']))
        # Schedule
        Schedule = QComboBox()
        Schedule.addItems(QStringList(['NON','FULL'])) 
        # Resource
        Resource = QLineEdit(self.tr(''))
        self.setItemWidget(treeItem,0,taskName)    
        self.setItemWidget(treeItem,1,stackSize)  
        self.setItemWidget(treeItem,2,taskPrio)  
        self.setItemWidget(treeItem,3,autostart)  
        self.setItemWidget(treeItem,4,Schedule)
        self.taskid += 1 
          
    def addEvent(self):
        taskItem = self.currentItem()
        eventItem = QTreeWidgetItem()
        taskItem.addChild(eventItem)
        eventName = QLineEdit('Event%s'%(self.eventid%32))
        eventName.setStatusTip('Name For Event, each Event must has a unique name.')
        eventMask = QLineEdit('0x%X'%(self.eventid))
        eventMask.setStatusTip('Mask For Event, 32 bits long.')
        self.setItemWidget(eventItem,0,eventName)
        self.setItemWidget(eventItem,1,eventMask)
        taskItem.setExpanded(True)
        self.eventid += 1
            
    def deleteEvent(self):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
            
    def deleteTask(self):
        if(self.indexOfTopLevelItem(self.currentItem()) != -1):
            self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))
        else:
            self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem().parent()))

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

# this is the root configuration of OpenSAR os
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
        
        self.qAction2=QAction(self.tr('Action2'),self) 
        self.connect(self.qAction2,SIGNAL('triggered()'),self.mqAction2) 
        self.menuBar().addAction(self.qAction2)
        self.qAction2.setDisabled(True)
    def mqAction1(self):
        if(self.qAction1.text() == 'Add Task'):
            self.easyTaskTbl.addTask()
        elif(str(self.qAction1.text()).find('Delete Task') != -1):
            self.easyTaskTbl.deleteTask()
    def mqAction2(self):
        if(str(self.qAction2.text()).find('Add Event For Task') != -1):
            self.easyTaskTbl.addEvent()
        elif(str(self.qAction2.text()).find('Delete Event') != -1):
            self.easyTaskTbl.deleteEvent()
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