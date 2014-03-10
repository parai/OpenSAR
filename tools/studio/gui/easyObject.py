from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import xml.etree.ElementTree as ET

class easyAction(QAction):
    def __init__(self,text,parent):  
        super(QAction,self).__init__(text,parent) 
        self.root =  parent
        self.connect(self,SIGNAL('triggered()'),self.onAction)
 
    def onAction(self):
        self.root.onAction(self.text())
    
class easyMenu(QTreeWidget):
    def __init__(self,arxml,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        self.arxml = arxml
        self.setHeaderLabel('easy%s'%(arxml.tag))
        for sub in arxml:
            self.addTopLevelItem(QTreeWidgetItem(QStringList('%s'%(sub.tag))))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
        self.setMaximumWidth(300)
    def GetNode(self,text):
        for sub in self.arxml:
            if(sub.tag == text):
                return sub
        return None

    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            if(self.indexOfTopLevelItem(treeItem) != -1):
                self.root.showTarget(treeItem.text(0))
                Node = self.GetNode(treeItem.text(0))
                if(Node != None):
                    Index = 0
                    for sub in Node:
                        self.root.actions[Index].setText('Add %s'%(sub.tag))
                        self.root.actions[Index].setDisabled(False)
                        Index += 1
                    for i in range(Index,4):
                        self.root.actions[i].setDisabled(True)
            else:
                # Sub Item
                pass
        except:
            for i in range(0,4):
                self.root.actions[i].setDisabled(True)
                
class easyObj(QTreeWidget):
    def __init__(self,arxml,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root = parent
        self.arxml = arxml
        self.setHeaderLabel('%s'%(arxml.tag))
    
    def showTarget(self,text):
        if(self.arxml.tag == text):
            self.setVisible(True)
        else:
            self.setVisible(False)
              
class easyObject(QMainWindow):
    objects = []
    actions = []
    def __init__(self,arxml,parent=None):
        super(QMainWindow,self).__init__(parent)
        self.root = parent
        self.arxml = arxml
        self.setWindowTitle('%s of AUTOSAR'%(arxml.tag))
        self.menuTree = easyMenu(arxml,self)
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        
        self.qSplitter.insertWidget(0,self.menuTree)
        
        self.setCentralWidget(self.qSplitter)
        self.creObjs()
        self.creMenu()
        self.showMaximized()
        
    def creObjs(self):
        for sub in self.arxml:
            obj = easyObj(sub,self)
            self.objects.append(obj)
            self.qSplitter.insertWidget(1,obj)
        self.showTarget('None')
    
    def showTarget(self,text):
        for obj in self.objects:
            obj.showTarget(text)

    def creMenu(self):
        #  create 4 action
        for i in range(0,4):
            qAction=easyAction(self.tr('Action%s'%(i)),self) 
            self.menuBar().addAction(qAction)
            qAction.setDisabled(True)
            self.actions.append(qAction)
            
    def onAction(self,text):
        print text
        
if __name__ == '__main__':
    qtApp = QtGui.QApplication(sys.argv)
    if(os.name == 'nt'):
        qtApp.setFont(QFont('Consolas', 12)) 
    elif(os.name == 'posix'):
        qtApp.setFont(QFont('Monospace', 12))
    else:
        print('unKnown platform.')
    qtGui = easyObject(ET.parse('./easySAR.arxml').getroot().find('OS'))
    qtGui.show()
    qtApp.exec_()        