from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os,re
import xml.etree.ElementTree as ET
import traceback
from Arxml import *

__all__ = ['ArgModule','ArgAction']

class ArgTextEdit(QLineEdit):
    def __init__(self,attrib,arxml,root):
        assert(isinstance(root,ArgModule))
        self.attrib = attrib
        self.arxml = arxml
        self.root  = root
        
        super(QLineEdit,self).__init__(self.arxml.attrib(self.attrib))
        self.connect(self, SIGNAL('textChanged(QString)'),self.onTextChanged)
    def onTextChanged(self,text):
        self.arxml.attrib(self.attrib,text)
        
        if(self.attrib == 'Name'):
            self.root.onObjectNameChanged(text)

class ArgAction(QAction):
    def __init__(self,text,parent): 
        super(QAction,self).__init__(text,parent) 
        self.root =  parent
        self.connect(self,SIGNAL('triggered()'),self.onAction)
 
    def onAction(self):
        self.root.onAction(self.text())

class ArgObject(QTreeWidgetItem):
    def __init__(self,arxml,root,parent=None):  
        assert(isinstance(root,ArgModule))
        super(QTreeWidgetItem,self).__init__(parent)
        self.root =  root
        self.arxml = arxml
        self.setText(0,'%s %s'%(self.arxml.tag,self.arxml.attrib('Name')))

    def onObjectNameChanged(self,text):
        assert(text == self.arxml.attrib('Name'))
        self.setText(0,'%s %s'%(self.arxml.tag,self.arxml.attrib('Name')))
    def onItemSelectionChanged(self):
        Index = 0
        for Descriptor in self.arxml.childDescriptors():
            already = False
            if(IsArxmlList(self.arxml)):
                # ok, it is a list things
                for I in range(0,self.childCount()):
                    tree = self.child(I)
                    if(tree.text(0) == Descriptor.tag):
                        already = True
                        break
            if(already == False): 
                self.root.actions[Index].setText('Add %s'%(Descriptor.tag))
                self.root.actions[Index].setDisabled(False)
                Index += 1
        if(self.parent() != None): # if parent is None, then it is top object, cann't be deleted
            self.root.actions[Index].setText('Delete %s'%(self.arxml.tag))
            self.root.actions[Index].setDisabled(False)
            Index += 1
        for i in range(Index,4):
            self.root.actions[i].setDisabled(True)
            
        self.root.showConfig(self.arxml)
        
    def onAction_Add(self,what,arxml=None):
        for Descriptor in self.arxml.childDescriptors():
            if(Descriptor.tag == what):
                if(IsArxmlList(self.arxml)):
                    max = self.arxml.getMaxChildAllowed()
                    if(max > self.childCount()):
                        self.addChild(ArgObject(Arxml(Descriptor),self.root,self))
                    else:
                        print 'Error:Maximum %s for %s is %s!'%(what,self.arxml.tag,max)
                else:
                    # ok, by default this is list things
                    already = False
                    for I in range(0,self.childCount()):
                        tree = self.child(I)
                        if(tree.text(0) == Descriptor.tag):
                            already = True
                            break
                    if(already == False):
                        self.addChild(ArgObject(Arxml(Descriptor),self.root,self))
                    else:
                        print 'Info: Only 1 %s is allowed for %s.'%(Descriptor.tag,self.self.arxml.tag)
                self.setExpanded(True)        

class ArgObjectTree(QTreeWidget):
    def __init__(self,arxml,parent=None):  
        assert(isinstance(parent,ArgModule))
        super(QTreeWidget,self).__init__(parent) 
        self.root  = parent
        self.arxml = arxml
        self.setHeaderLabel('%s'%(self.arxml.tag))
        for arxml in self.arxml.childArxmls():
            self.addTopLevelItem(ArgObject(arxml,self.root))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
        self.setMaximumWidth(300)
        
    def onAction_Delete(self,what):
        object = self.currentItem()
        assert(isinstance(object,ArgObject))
        assert(object.arxml.tag == what)
        if(self.indexOfTopLevelItem(object) != -1):
            self.takeTopLevelItem(self.indexOfTopLevelItem(object))
        else:
            pObj = object.parent()
            pObj.takeChild(pObj.indexOfChild(object))

    def onAction(self,text):
        reAction = re.compile(r'(Add|Delete) (\w+)')
        action = reAction.search(text).groups()
        if(action[0] == 'Add'):
            object = self.currentItem()
            assert(isinstance(object,ArgObject))
            object.onAction_Add(action[1])
        elif(action[0] == 'Delete'):
            self.onAction_Delete(action[1])
       
    def itemSelectionChanged(self):
        object = self.currentItem()
        assert(isinstance(object,ArgObject))
        object.onItemSelectionChanged()
    
    def onObjectNameChanged(self,text):
        object = self.currentItem()
        assert(isinstance(object,ArgObject))
        object.onObjectNameChanged(text)

class ArgModule(QMainWindow):
    actions = []
    def __init__(self,arxml,parent=None):
        super(QMainWindow,self).__init__(parent)
        self.tag = arxml.tag
        
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        
        self.objectTree = ArgObjectTree(arxml,self)
        self.wConfig  = QMainWindow()
        self.qSplitter.insertWidget(0,self.objectTree)
        self.qSplitter.insertWidget(1,self.wConfig)
        
        self.setCentralWidget(self.qSplitter)
        self.creActions()

    def showConfig(self,arxml):
        try:
            print 'Show:',arxml
        except:
            pass
        self.grid = QGridLayout()
        self.frame = QFrame()
        self.frame.setLayout(self.grid)
        if(arxml != None and IsArxmlList(arxml)==False):
            Row = 0
            for [key,value] in arxml.configuration.items():
                K = QLabel(key)
                V = ArgTextEdit(key,arxml,self)
                self.grid.addWidget(K,Row,0)
                self.grid.addWidget(V,Row,1)
                Row += 1
        self.wConfig.setCentralWidget(self.frame)
    def creActions(self):
        #  create 4 action
        for i in range(0,4):
            qAction=ArgAction(self.tr('Action%s'%(i)),self) 
            self.menuBar().addAction(qAction)
            qAction.setDisabled(True)
            self.actions.append(qAction)
            
    def onAction(self,text):
        self.objectTree.onAction(text)
    
    def onObjectNameChanged(self,text):
        self.objectTree.onObjectNameChanged(text)

if __name__ == '__main__':
    qtApp = QtGui.QApplication(sys.argv)
    if(os.name == 'nt'):
        qtApp.setFont(QFont('Consolas', 12)) 
    elif(os.name == 'posix'):
        qtApp.setFont(QFont('Monospace', 12))
    else:
        print('unKnown platform.')
    qtGui = ArgModule(Arxml(ET.parse('./easySAR.arxml').getroot().find('OS')))
    qtGui.show()
    qtApp.exec_()  