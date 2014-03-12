from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os,re
import xml.etree.ElementTree as ET
import traceback

__all__ = ['easyModule']

class easyAction(QAction):
    def __init__(self,text,parent):
        assert(isinstance(parent,easyModule))  
        super(QAction,self).__init__(text,parent) 
        self.root =  parent
        self.connect(self,SIGNAL('triggered()'),self.onAction)
 
    def onAction(self):
        self.root.onAction(self.text())

class twiObj(QTreeWidgetItem):
    def __init__(self,arxmlDescriptor,root,parent=None):  
        assert(isinstance(root,easyModule))
        super(QTreeWidgetItem,self).__init__(parent)
        self.root =  root
        self.arxmlDescriptor = arxmlDescriptor
        self.isTop = (parent==None)
        self.gui = self.creGui()
        try:
            # just to see it has a name attribute or not, if not a exception will be raised
            if('Text' == arxmlDescriptor.attrib['Name']):
                assert(isinstance(self.parent(),twiObj))
                self.setText(0,'%s%s'%(arxmlDescriptor.tag,self.parent().childCount()))
        except:
            #print traceback.format_exc()
            self.setText(0,arxmlDescriptor.tag)

    def creGui(self):
        pass
    
    def onItemSelectionChanged(self):
        Index = 0
        for Descriptor in self.arxmlDescriptor:
            already = False
            try:
                max = Descriptor.attrib['Max']
                # ok, it is a list things
                for I in range(0,self.childCount()):
                    tree = self.child(I)
                    if(tree.text(0) == Descriptor.tag):
                        already = True
                        break
            except:
                pass
            if(already == False): 
                self.root.actions[Index].setText('Add %s'%(Descriptor.tag))
                self.root.actions[Index].setDisabled(False)
                Index += 1
        if(self.isTop != True):
            self.root.actions[Index].setText('Delete %s'%(self.arxmlDescriptor.tag))
            self.root.actions[Index].setDisabled(False)
            Index += 1
        for i in range(Index,4):
            self.root.actions[i].setDisabled(True)
    
    def onAction_Add(self,what):
        for Descriptor in self.arxmlDescriptor:
            if(Descriptor.tag == what):
                try:
                    max = int(self.arxmlDescriptor.attrib['Max'],10)
                    if(max > self.childCount()):
                        self.addChild(twiObj(Descriptor,self.root,self))
                    else:
                        print 'Error:Maximum %s for %s is %s!'%(what,self.arxmlDescriptor.tag,max)
                except:
                    # ok, by default this is list things
                    already = False
                    for I in range(0,self.childCount()):
                        tree = self.child(I)
                        if(tree.text(0) == Descriptor.tag):
                            already = True
                            break
                    if(already == False):
                        self.addChild(twiObj(Descriptor,self.root,self))
                    else:
                        print 'Info: Only 1 %s is allowed for %s.'%(Descriptor.tag,self.arxmlDescriptor.tag)
                self.setExpanded(True)

class easyMenu(QTreeWidget):
    def __init__(self,arxmlDescriptor,parent):  
        assert(isinstance(parent,easyModule))
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        self.arxmlDescriptor = arxmlDescriptor
        self.setHeaderLabel('easy%s'%(arxmlDescriptor.tag))
        for Descriptor in arxmlDescriptor:
            self.addTopLevelItem(twiObj(Descriptor,self.root))
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
        self.setMaximumWidth(300)
        
    def onAction_Delete(self,what):
        treeItem = self.currentItem()
        assert(isinstance(treeItem,twiObj))
        assert(treeItem.arxmlDescriptor.tag == what)
        if(treeItem.isTop):
            self.takeTopLevelItem(self.indexOfTopLevelItem(treeItem))
        else:
            pTree = treeItem.parent()
            pTree.takeChild(pTree.indexOfChild(treeItem))

    def onAction(self,text):
        reAction = re.compile(r'(Add|Delete) (\w+)')
        action = reAction.search(text).groups()
        if(action[0] == 'Add'):
            treeItem = self.currentItem()
            assert(isinstance(treeItem,twiObj))
            treeItem.onAction_Add(action[1])
        elif(action[0] == 'Delete'):
            self.onAction_Delete(action[1])
       
    def itemSelectionChanged(self):
        treeItem = self.currentItem()
        assert(isinstance(treeItem,twiObj))
        treeItem.onItemSelectionChanged()

class easyModule(QMainWindow):
    actions = []
    def __init__(self,arxmlDescriptor,parent=None):
        super(QMainWindow,self).__init__(parent)
        self.root = parent
        self.arxmlDescriptor = arxmlDescriptor
        self.setWindowTitle('%s of AUTOSAR'%(arxmlDescriptor.tag))
        self.menuTree = easyMenu(arxmlDescriptor,self)
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        
        self.qSplitter.insertWidget(0,self.menuTree)
        
        self.setCentralWidget(self.qSplitter)
        self.creMenu()
        self.showMaximized()

    def creMenu(self):
        #  create 4 action
        for i in range(0,4):
            qAction=easyAction(self.tr('Action%s'%(i)),self) 
            self.menuBar().addAction(qAction)
            qAction.setDisabled(True)
            self.actions.append(qAction)
            
    def onAction(self,text):
        self.menuTree.onAction(text)
        
if __name__ == '__main__':
    qtApp = QtGui.QApplication(sys.argv)
    if(os.name == 'nt'):
        qtApp.setFont(QFont('Consolas', 12)) 
    elif(os.name == 'posix'):
        qtApp.setFont(QFont('Monospace', 12))
    else:
        print('unKnown platform.')
    qtGui = easyModule(ET.parse('./easySAR.arxml').getroot().find('OS'))
    qtGui.show()
    qtApp.exec_()        