from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import xml.etree.ElementTree as ET

__all__ = ['easyOsGui']            

class easyCounterCfgTree(QTreeWidget):
    counterid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root = parent
        list = ['Counter Name','Max Allowed Value','Ticks Per Base','Min Cycle']
        self.setHeaderLabels(QStringList(list))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def getCounterNameList(self):
        list = []
        for i in range(0,self.topLevelItemCount()):
            treeI = self.topLevelItem(i)
            list.append(self.itemWidget(treeI,0).text()) 
        return list 

    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            name = self.itemWidget(treeItem,0).text()
            # update Action 
            self.root.qAction1.setText('Delete Counter <%s>'%(name))
            self.root.qAction1.setStatusTip('Delete This Counter <%s> Object, Be Careful as this action is dangerous.'%(name))
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setDisabled(True)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def toXML(self):
        List = ET.Element('CounterList')
        for i in range(0,self.topLevelItemCount()):
            Counter = ET.Element('Counter')
            tree = self.topLevelItem(i)
            Counter.attrib['name'] = str(self.itemWidget(tree,0).text())
            Counter.attrib['maxallowedvalue'] = str(self.itemWidget(tree,1).value())
            Counter.attrib['ticksperbase'] = str(self.itemWidget(tree,2).value())
            Counter.attrib['mincycle'] = str(self.itemWidget(tree,3).value())
            List.append(Counter)
        return List
    def loadXML(self,OSROOT):
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        CounterList = OSROOT.find('CounterList')
        if(CounterList == None):
            return
        for Counter in CounterList:
            self.addCounter(Counter)
    def deleteCounter(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))
    def addCounter(self,Counter=None):
        if(Counter != None):
            sName = Counter.attrib['name']
            sMax  = int(Counter.attrib['maxallowedvalue'])
            sTick = int(Counter.attrib['ticksperbase'])
            sMin  = int(Counter.attrib['mincycle'])
        else:
            sName = 'Counter%s'%(self.counterid)
            sMax  = 65535
            sTick = 1
            sMin  = 1
        name = QLineEdit(sName)
        self.counterid += 1
        max = QSpinBox()
        max.setRange(0,0xFFFFFFF)
        max.setValue(sMax)
        tick = QSpinBox()
        tick.setRange(1,0xFFFFFFF)
        tick.setValue(sTick)
        min = QSpinBox()
        min.setRange(0,0xFFFFFFF)
        min.setValue(sMin)
        min.setMaximumWidth(200)
        # add
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem)
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,max)
        self.setItemWidget(treeItem,2,tick)
        self.setItemWidget(treeItem,3,min)
        
class easyAlarmCfgTree(QTreeWidget):
    alarmid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root = parent
        list = ['Alarm Name','Owner Counter','Autostart','App Mode',
                'Start Time','Cycle','Action','Task/Counter/Cbk','Event']
        self.setHeaderLabels(QStringList(list))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def toXML(self):
        List = ET.Element('AlarmList')
        for i in range(0,self.topLevelItemCount()):
            Alarm = ET.Element('Alarm')
            tree = self.topLevelItem(i)
            Alarm.attrib['name'] = str(self.itemWidget(tree,0).text())
            Alarm.attrib['owner'] = str(self.itemWidget(tree,1).currentText())
            Alarm.attrib['autostart'] = str(self.itemWidget(tree,2).currentText())
            Alarm.attrib['appmode'] = str(self.itemWidget(tree,3).currentText())
            Alarm.attrib['starttime'] = str(self.itemWidget(tree,4).value())
            Alarm.attrib['cycle'] = str(self.itemWidget(tree,5).value())
            Alarm.attrib['action'] = str(self.itemWidget(tree,6).currentText())
            if( Alarm.attrib['action'] == 'AlarmCallBack'):
                 Alarm.attrib['tcc'] = str(self.itemWidget(tree,7).text())
            else:
                Alarm.attrib['tcc'] = str(self.itemWidget(tree,7).currentText())
            if(Alarm.attrib['action'] == 'SetEvent'):
                Alarm.attrib['event'] = str(self.itemWidget(tree,8).currentText())
            List.append(Alarm)
        return List
    def loadXML(self,OSROOT):
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        AlarmList = OSROOT.find('AlarmList')
        if(AlarmList == None):
            return
        for Alarm in AlarmList:
            self.addAlarm(Alarm)
    def currentIndexChanged_Task(self,Task):
        """ Only when Action is SetEvent"""
        treeItem = self.currentItem()
        try:
            currentEvent = self.itemWidget(treeItem,8).currentText()
        except:
            currentEvent = ''
        event = QComboBox()
        event.setMaximumWidth(300)
        self.setItemWidget(treeItem,8,event)
        event.addItems(QStringList(self.root.easyTaskTree.getTaskEventList(str(Task))))
        event.setCurrentIndex(event.findText(currentEvent))
    def currentIndexChanged_Action(self,Action):
        """ Call Back for Action"""
        treeItem = self.currentItem()
        if('ActivateTask' == Action):
            try:
                currentTask = self.itemWidget(treeItem,7).currentText()
            except:
                currentTask = ''
            task = QComboBox()
            self.setItemWidget(treeItem,7,task)
            self.setItemWidget(treeItem,8,None)
            task.addItems(QStringList(self.root.easyTaskTree.getTaskNameList()))
            task.setCurrentIndex(task.findText(currentTask))
        elif('SetEvent' == Action):
            try:
                currentTask = self.itemWidget(treeItem,7).currentText()
            except:
                currentTask = ''
            try:
                currentEvent = self.itemWidget(treeItem,8).currentText()
            except:
                currentEvent = ''
            #print currentTask,currentEvent
            task = QComboBox()
            event = QComboBox()
            event.setMaximumWidth(300)
            self.setItemWidget(treeItem,7,task)
            self.setItemWidget(treeItem,8,event)
            event.addItems(QStringList(self.root.easyTaskTree.getTaskEventList(currentTask)))
            event.setCurrentIndex(event.findText(currentEvent))
            task.addItems(QStringList(self.root.easyTaskTree.getTaskNameList('Event')))
            task.setCurrentIndex(task.findText(currentTask))
            self.currentIndexChanged_Task(currentTask)
            self.connect(task,SIGNAL('currentIndexChanged(QString)'),self.currentIndexChanged_Task)
        elif('IncrementCounter' == Action):
            try:
                currentCounter = self.itemWidget(treeItem,7).currentText()
            except:
                currentCounter = ''
            counter = QComboBox()
            self.setItemWidget(treeItem,7,counter)
            self.setItemWidget(treeItem,8,None)
            counter.addItems(QStringList(self.root.easyCounterTree.getCounterNameList()))
            counter.setCurrentIndex(counter.findText(currentCounter))
        elif('AlarmCallBack' == Action):
            try:
                currentCbk = self.itemWidget(treeItem,7).text()
            except:
                currentCbk = ''
            cbk = QLineEdit('%s_Cbk'%(self.itemWidget(treeItem,0).text()))
            self.setItemWidget(treeItem,7,cbk)
            self.setItemWidget(treeItem,8,None)
    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            alarm = self.itemWidget(treeItem,0).text()
            # update Menu Action 
            self.root.qAction1.setText('Delete Alarm<%s>'%(alarm))
            self.root.qAction1.setStatusTip('Delete This Alarm <%s> Object, Be Careful as this action is dangerous.'%(alarm))
            self.root.qAction1.setDisabled(False)
            self.root.qAction2.setDisabled(True)
            # update Action
            self.currentIndexChanged_Action(self.itemWidget(treeItem,6).currentText())
            # update Appmode
            
            # update Counter
            counter = self.itemWidget(treeItem,1)
            currentCounter = counter.currentText()
            counter.clear()
            counter.addItems(QStringList(self.root.easyCounterTree.getCounterNameList()))
            counter.setCurrentIndex(counter.findText(currentCounter))
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
        
    def deleteAlarm(self):
        self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem())) 
        
    def addAlarm(self,Alarm=None):
        if(Alarm != None):
            sname = Alarm.attrib['name']
            sowner = Alarm.attrib['owner']
            sautostart = Alarm.attrib['autostart']
            sappmode = Alarm.attrib['appmode']
            sstarttime = int(Alarm.attrib['starttime'])
            scycle = int(Alarm.attrib['cycle'])
            saction = Alarm.attrib['action']
            stcc = Alarm.attrib['tcc']
            if(saction == 'SetEvent'):
                sevent = Alarm.attrib['event']
            else:
                sevent = ''
        else:
            sname = 'Alarm%s'%(self.alarmid)
            sowner = ''
            sautostart = 'False'
            sappmode = 'OSDEFAULTAPPMODE'
            sstarttime = 0
            scycle = 0
            saction = 'ActivateTask'
            stcc = ''
            sevent = ''
        name = QLineEdit(sname)
        self.alarmid += 1
        owner = QComboBox()
        if(sowner != ''):
            owner.addItem(sowner)
        owner.setCurrentIndex(owner.findText(sowner))
        autostart = QComboBox()
        autostart.addItems(QStringList(['False','Relative','Absolute']))
        autostart.setCurrentIndex(autostart.findText(sautostart))
        appmode = QComboBox()
        appmode.addItems(QStringList(['OSDEFAULTAPPMODE']))
        if(sappmode != 'OSDEFAULTAPPMODE' and sappmode != ''):
            appmode.addItem(sappmode)
        appmode.setCurrentIndex(appmode.findText(sappmode))
        start = QSpinBox()
        start.setRange(0,65535)
        start.setValue(sstarttime)
        cycle = QSpinBox()
        cycle.setRange(0,65535)
        cycle.setValue(scycle)
        action = QComboBox()
        action.addItems(QStringList(['ActivateTask','SetEvent','IncrementCounter','AlarmCallBack']))
        self.connect(action,SIGNAL('currentIndexChanged(QString)'),self.currentIndexChanged_Action)
        action.setStatusTip('Note: Selected the item firstly, and then do a action!')
        action.setCurrentIndex(action.findText(saction))
        if(saction == 'AlarmCallBack'):
            if(tcc != ''):
                tcc = QLineEdit(stcc)
            else:
                tcc = QLineEdit('%s_Cbk'%(sname))
        else:
            tcc = QComboBox() # update it when the item clicked
            if(tcc != ''):
                tcc.addItem(stcc)
        # add 
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem)
        self.setItemWidget(treeItem,0,name)
        self.setItemWidget(treeItem,1,owner)
        self.setItemWidget(treeItem,2,autostart)
        self.setItemWidget(treeItem,3,appmode)
        self.setItemWidget(treeItem,4,start)
        self.setItemWidget(treeItem,5,cycle)
        self.setItemWidget(treeItem,6,action)
        self.setItemWidget(treeItem,7,tcc)
        if(saction == 'SetEvent'):
            event =  QComboBox()
            if(sevent != ''):
                event.addItem(sevent)
            self.setItemWidget(treeItem,8,event)
        
class easyTaskCfgTree(QTreeWidget):
    taskid = 0
    eventid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Task Name','Stack Size','Priority','Activation','Autostart','Schedule']
        self.setHeaderLabels(QStringList(list))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            if(self.indexOfTopLevelItem(treeItem) != -1):
                task = self.itemWidget(treeItem,0).text()
                self.root.qAction2.setText('Delete Task<%s>'%(task))
                self.root.qAction2.setStatusTip('Delete This Task <%s> Object, Be Careful as this action is dangerous.'%(task))
                self.root.qAction2.setDisabled(False)
                self.root.qAction1.setText('Add Event For Task<%s>'%(task))
                self.root.qAction1.setStatusTip('Add Event For Task<%s>.'%(task))
                self.root.qAction1.setDisabled(False)
            else:
                # Sub Event Item
                task = self.itemWidget(treeItem.parent(),0).text()
                self.root.qAction2.setText('Delete Task<%s>'%(task))
                self.root.qAction2.setStatusTip('Delete This Task <%s> Object, Be Careful as this action is dangerous.'%(task))
                self.root.qAction2.setDisabled(False)
                event = self.itemWidget(treeItem,0).text()
                self.root.qAction1.setText('Delete Event <%s>'%(event))
                self.root.qAction1.setStatusTip('Delete This Event <%s> Object, Be Careful as this action is dangerous.'%(event))
                self.root.qAction1.setDisabled(False)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def toXML(self):
        TaskList = ET.Element('TaskList')
        for i in range(0,self.topLevelItemCount()):
            Task = ET.Element('Task')
            treeTask = self.topLevelItem(i)
            Task.attrib['name'] = str(self.itemWidget(treeTask,0).text())
            Task.attrib['stacksz'] = str(self.itemWidget(treeTask,1).value())
            Task.attrib['priority'] = str(self.itemWidget(treeTask,2).value())
            Task.attrib['activation'] = str(self.itemWidget(treeTask,3).value())
            Task.attrib['autostart'] = str(self.itemWidget(treeTask,4).currentText())
            Task.attrib['schedule'] = str(self.itemWidget(treeTask,5).currentText())
            EventList = ET.Element('EventList')
            for j in range(0,treeTask.childCount()):
                Event = ET.Element('Event')
                treeEvent = treeTask.child(j)
                Event.attrib['name'] = str(self.itemWidget(treeEvent,0).text())
                Event.attrib['mask'] = str(self.itemWidget(treeEvent,1).text())
                EventList.append(Event)
            Task.append(EventList)
            TaskList.append(Task)
        return TaskList
    def loadXML(self,OSROOT):
        """TaskList is in the format of ET"""
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        if(OSROOT.find('TaskList') == None):
            return
        for Task in OSROOT.find('TaskList'):
            self.addTask(Task)

    def getTaskNameList(self,option = None):
        list = []
        if(option == None):
            for i in range(0,self.topLevelItemCount()):
                treeTask = self.topLevelItem(i)
                list.append(self.itemWidget(treeTask,0).text())
        elif(option == 'Event'):
            for i in range(0,self.topLevelItemCount()):
                treeTask = self.topLevelItem(i)
                if(treeTask.childCount() > 0):
                    list.append(self.itemWidget(treeTask,0).text())
        return list
    def getTaskEventList(self,name):
        list = []
        for i in range(0,self.topLevelItemCount()):
            treeTask = self.topLevelItem(i)
            if(self.itemWidget(treeTask,0).text() == name):
                for j in range(0,treeTask.childCount()):
                    treeEvent = treeTask.child(j)
                    list.append(self.itemWidget(treeEvent,0).text())
                break
        return list
    def addTask(self,Task=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem)
        if(Task != None):
            sName = Task.attrib['name']
            sStackSz = int(Task.attrib['stacksz'])
            sPriority = int(Task.attrib['priority'])
            sActivation = int(Task.attrib['activation'])
            sAutostart = Task.attrib['autostart']
            sSchedule = Task.attrib['schedule']
        else:
            sName = 'Task%s'%(self.taskid)
            sStackSz = 512
            sPriority = self.taskid
            sActivation = 1
            sAutostart = 'True'
            sSchedule = 'FULL'
        # task name 
        taskName = QLineEdit(sName)
        # stack size
        stackSize = QSpinBox()      
        stackSize.setRange(64,0xFFFFF)
        stackSize.setValue(sStackSz) 
        # Priority
        taskPrio = QSpinBox()
        taskPrio.setRange(0,256)
        taskPrio.setValue(sPriority) 
        # Activation
        Activation = QSpinBox()
        Activation.setRange(1,256)
        Activation.setValue(sActivation)
        # Autostart
        autostart = QComboBox()
        autostart.addItems(QStringList(['True','False']))
        autostart.setCurrentIndex(autostart.findText(sAutostart))
        # Schedule
        Schedule = QComboBox()
        Schedule.addItems(QStringList(['NON','FULL'])) 
        Schedule.setCurrentIndex(Schedule.findText(sSchedule))
        Schedule.setMaximumWidth(100)
        self.setItemWidget(treeItem,0,taskName)    
        self.setItemWidget(treeItem,1,stackSize)  
        self.setItemWidget(treeItem,2,taskPrio) 
        self.setItemWidget(treeItem,3,Activation) 
        self.setItemWidget(treeItem,4,autostart)  
        self.setItemWidget(treeItem,5,Schedule)
        self.taskid += 1 
        if(Task != None):
            self.setCurrentItem(treeItem)
            for Event in Task.find('EventList'):
                self.addEvent(Event)
          
    def addEvent(self,Event=None):
        taskItem = self.currentItem()
        eventItem = QTreeWidgetItem()
        taskItem.addChild(eventItem)
        if Event != None:
            sName = Event.attrib['name']
            sMask = Event.attrib['mask']
        else:
            sName = 'Event%s'%(self.eventid)
            sMask = '0x%X'%((1<<(self.eventid%32)))
        eventName = QLineEdit(sName)
        eventName.setStatusTip('Name For Event, each Event must has a unique name.')
        eventMask = QLineEdit(sMask)
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
        self.addTopLevelItem(QTreeWidgetItem(QStringList('ISR')))
        self.setMaximumWidth(200); 

# this is the root configuration of OpenSAR os
class easyOsGui(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.easyTree = easyOsCfgTree(self)
        self.easyTaskTree = easyTaskCfgTree(self)
        self.easyCounterTree = easyCounterCfgTree(self)
        self.easyAlarmTree = easyAlarmCfgTree(self)
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
            self.easyTaskTree.addTask()
        elif(self.qAction1.text() == 'Add Counter'):
            self.easyCounterTree.addCounter()
        elif(str(self.qAction1.text()).find('Delete Counter') != -1):
            self.easyCounterTree.deleteCounter()
        elif(self.qAction1.text() == 'Add Alarm'):
            self.easyAlarmTree.addAlarm()
        elif(str(self.qAction1.text()).find('Delete Alarm') != -1):
            self.easyAlarmTree.deleteAlarm()
        elif(str(self.qAction1.text()).find('Add Event For Task') != -1):
            self.easyTaskTree.addEvent()
        elif(str(self.qAction1.text()).find('Delete Event') != -1):
            self.easyTaskTree.deleteEvent()

    def mqAction2(self):
        if(str(self.qAction2.text()).find('Delete Task') != -1):
            self.easyTaskTree.deleteTask()
    def creGui(self):
        self.qSplitter.insertWidget(0,self.easyTree)
        self.qSplitter.insertWidget(1,self.easyTaskTree)
        self.qSplitter.insertWidget(1,self.easyAlarmTree)
        self.qSplitter.insertWidget(1,self.easyCounterTree)
        self.showTableWidget(self.easyAlarmTree)
        self.setCentralWidget(self.qSplitter)
        self.connect(self.easyTree,SIGNAL('itemClicked(QTreeWidgetItem*, int)'),self.easyTreeClicked)  
    def showTableWidget(self,widget):
        if(self.easyTaskTree == widget):
            self.easyTaskTree.setVisible(True);
        else:
            self.easyTaskTree.setVisible(False);
        if(self.easyAlarmTree == widget):
            self.easyAlarmTree.setVisible(True);
        else:
            self.easyAlarmTree.setVisible(False);
        if(self.easyCounterTree == widget):
            self.easyCounterTree.setVisible(True);
        else:
            self.easyCounterTree.setVisible(False);
    def easyTreeClicked(self,item,column):
        if(item.text(0) == 'Alarm'):
            self.showTableWidget(self.easyAlarmTree)
        elif(item.text(0) == 'Task'):
            self.showTableWidget(self.easyTaskTree)
        elif(item.text(0) == 'Counter'):
            self.showTableWidget(self.easyCounterTree)
        if(item.text(0) == 'Task'):
            self.qAction1.setText('Add Task')
            self.qAction1.setStatusTip('Add a Task Object.')
            self.qAction1.setDisabled(False)
            self.qAction2.setDisabled(True)
        elif(item.text(0) == 'Counter'):
            self.qAction1.setText('Add Counter')
            self.qAction1.setStatusTip('Add a Counter Object.')
            self.qAction1.setDisabled(False)
            self.qAction2.setDisabled(True)
        elif(item.text(0) == 'Alarm'):
            self.qAction1.setText('Add Alarm')
            self.qAction1.setStatusTip('Add a Alarm Object.')
            self.qAction1.setDisabled(False)
            self.qAction2.setDisabled(True)
        else:
            self.qAction1.setDisabled(True)
    def mOpen(self):
        wfxml=QFileDialog.getOpenFileName(self, 'Open OpenSAR OS Configuration File.', 
                '../../app/config/os.wfxml', 'OpenSAR OS(*.wfxml)');
        if(wfxml==''):
            return
        root = ET.parse(wfxml).getroot();
        self.easyTaskTree.loadXML(root)
        self.easyCounterTree.loadXML(root)
        self.easyAlarmTree.loadXML(root)
        self.qAction1.setDisabled(True)
        self.qAction2.setDisabled(True)
    def mSave(self):
        wfxml=QFileDialog.getSaveFileName(self, 'Save OpenSAR OS Configuration File.', 
            '../../app/config/os.wfxml', 'OpenSAR OS(*.wfxml)');
        if(wfxml==''):
            return
        OSROOT = ET.Element('OSROOT')
        OSROOT.append(self.easyTaskTree.toXML())
        OSROOT.append(self.easyCounterTree.toXML())
        OSROOT.append(self.easyAlarmTree.toXML())
        tree = ET.ElementTree(OSROOT)
        tree.write(wfxml, encoding="utf-8", xml_declaration=True);
    def mGen(self):
        from gen.GenOS import GenOS
        wfxml=QFileDialog.getOpenFileName(self, 'Get OpenSAR OS Configuration File.', 
                '../../app/config/os.wfxml', 'OpenSAR OS(*.wfxml)');
        if(wfxml==''):
            return
        GenOS(str(wfxml))