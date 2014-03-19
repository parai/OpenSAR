from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os,re
import xml.etree.ElementTree as ET
import traceback
from Arxml import *

__all__ = ['ArgModule','ArgAction']

def Integer(cstr):
    cstr = str(cstr)
    try:
        if(cstr.find('0x')!=-1 or cstr.find('0X')!=-1):
            return int(cstr,16)
        else:
            return int(cstr,10)
    except:
        #print traceback.format_exc()
        return None

def IsEnabled(key,trItem):
    arxml=trItem.arxml
    reEnabled = re.compile(r'Enabled=\((.*)\)')
    reSpilt  = re.compile(r'\s+')
    reSelf   = re.compile(r'Self\.([^\s]+)(==|!=)(\w+)')
    descriptor = arxml.getKeyDescriptor(key)
    Enabled = True
    if(reEnabled.search(descriptor)):
        list = reEnabled.search(descriptor).groups()[0]
        list = re.split(reSpilt, list)
        isAnd = True # '&&"
        isEnabled = True
        for cond in list:
            if(cond == '||'):
                isAnd = False
            elif(cond == '&&'):
                isAnd = True
            else:
                if(reSelf.search(cond)):
                    keyL = reSelf.search(cond).groups()[0] 
                    tarTrItem = trItem
                    for key1 in keyL.split('.'):
                        if(key1=='Parent'):
                            tarTrItem = tarTrItem.parent()
                        else:
                            break
                    operater = reSelf.search(cond).groups()[1]
                    value = reSelf.search(cond).groups()[2]
                    if(operater=='==' and tarTrItem.arxml.attrib(key1)==value):
                        isEnabled = True
                    elif(operater=='!=' and tarTrItem.arxml.attrib(key1)!=value):
                        isEnabled = True
                    else:
                        isEnabled = False
                else:
                    if(cond=='False'):
                        isEnabled = False
                    else:
                        isEnabled = True
                                        
                if(isAnd):
                    Enabled = (Enabled and isEnabled)
                else:
                    Enabled = (Enabled or isEnabled)
    return Enabled
    
    
    
class ArgInput(QLineEdit):
    def __init__(self,key,trItem,root):
        assert(isinstance(root,ArgModule))
        self.key = key
        self.trItem = trItem
        self.root  = root
        super(QLineEdit,self).__init__(self.trItem.arxml.attrib(self.key))
        self.setEnabled(IsEnabled(key, trItem))
        self.setToolTip(self.trItem.arxml.getKeyDescriptor(self.key).replace('\\n','\n'))
        self.connect(self, SIGNAL('textChanged(QString)'),self.onTextChanged)
    def onTextChanged(self,text):
        reInput = re.compile(r'^(Text|Integer)')
        descriptor = self.trItem.arxml.getKeyDescriptor(self.key)
        
        type = reInput.search(descriptor).groups()[0]
        
        doUpdate = False
        if(text == 'TBD' or text == 'TB' or text == 'T' or text == '' or
           text == '0x' or text == '0X' or text == '0'):
            doUpdate = True
        elif(type == 'Text'):
            doUpdate = True
        elif(type == 'Integer'):
            reRange = re.compile(r'Range=(\w*)~(\w*)')
            if(reRange.search(descriptor)):
                min = Integer(reRange.search(descriptor).groups()[0])
                max = Integer(reRange.search(descriptor).groups()[1])
                var = Integer(text)
                if(var!=None and var>=min and var<=max):
                    doUpdate = True
            elif(Integer(text) != None):
                doUpdate = True        
        if(doUpdate):
            self.trItem.arxml.attrib(self.key,text)
            if(self.key == 'Name'):
                self.root.onObjectNameChanged(text)
        else:
            self.setText(self.trItem.arxml.attrib(self.key))

class ArgSelect(QComboBox):
    def __init__(self,key,trItem,root):
        assert(isinstance(root,ArgModule))
        self.key = key
        self.trItem = trItem
        self.root  = root
        super(QComboBox,self).__init__()
        self.setEnabled(IsEnabled(key, trItem))
        self.initItems()
        self.setToolTip(self.trItem.arxml.getKeyDescriptor(self.key).replace('\\n','\n'))
        self.connect(self, SIGNAL('currentIndexChanged(QString)'),self.onTextChanged)
        
    def initItems(self):
        if(self.isEnabled()==False):
            return
        reSelect = re.compile(r'^(EnumRef|Enum|Boolean)')
        descriptor = self.trItem.arxml.getKeyDescriptor(self.key)
        type = reSelect.search(descriptor).groups()[0]
        if(type == 'Enum'):
            reList = re.compile(r'^Enum=\((.*)\)')
            list = reList.search(descriptor).groups()[0].split(',')
            self.addItems(QStringList(list))
        elif(type == 'Boolean'):
            self.addItems(QStringList(['True','False']))
        elif(type=='EnumRef'):
            reRef = re.compile(r'^EnumRef=([^\s]+)\s*')
            reFilter = re.compile(r'Filter=\((\w+)(==|!=)(\w+)\)')
            ref = reRef.search(descriptor).groups()[0]
            reSelf = re.compile(r'\(Self\.([^\s]+)\)')
            if(reSelf.search(ref)):
                keyL = reSelf.search(ref).groups()[0] 
                tarTrItem = self.trItem
                for key in keyL.split('.'):
                    if(key=='Parent'):
                        tarTrItem = tarTrItem.parent()
                    else:
                        break
                ref = ref.replace('(Self.%s)'%(keyL),tarTrItem.arxml.attrib(key))
            list = []
            url=self.root.getURL(ref)
            if(url!=None):
                for uu in url:
                    if(reFilter.search(descriptor)):
                        grp = reFilter.search(descriptor).groups()
                        if(grp[1]=='==' and uu.attrib[grp[0]]==grp[2]):
                            list.append(uu.attrib['Name'])
                        elif(grp[1]=='!=' and uu.attrib[grp[0]]!=grp[2]):
                            list.append(uu.attrib['Name'])
                    else:
                        list.append(uu.attrib['Name'])
            self.addItems(QStringList(list))
        self.setCurrentIndex(self.findText(self.trItem.arxml.attrib(self.key)))
    def onTextChanged(self,text):
        self.trItem.arxml.attrib(self.key,text)        
        if(self.key == 'Name'):
            self.root.onObjectNameChanged(text)
        else:
            self.root.showConfig(self.trItem)

def ArgWidget(key,trItem,root):
    reInput = re.compile(r'^Text|Integer')
    reSelect = re.compile(r'^EnumRef|Enum|Boolean')
    descriptor = trItem.arxml.getKeyDescriptor(key)
    if(reInput.search(descriptor)):
        return ArgInput(key,trItem,root)
    elif(reSelect.search(descriptor)):
        return ArgSelect(key,trItem,root)
    else:
        return QLineEdit('Type Error for %s'%(trItem.arxml.tag))

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
        
        for arx in self.arxml.childArxmls2():
            self.addChildArobj(ArgObject(arx,self.root,self))
            self.setExpanded(True)
        
    def toArxml(self):
        arxml = self.arxml.toArxml()
        for i in range(0,self.childCount()):
            arobj = self.child(i)
            assert(isinstance(arobj, ArgObject))
            arxml.append(arobj.toArxml())
        return arxml

    def addChildArobj(self,arobj):
        assert(isinstance(arobj, ArgObject))
        self.addChild(arobj)
        if(IsArxmlList(arobj.arxml)):
            pass
        else:
            if('TBD' == arobj.arxml.attrib('Name')):
                arobj.arxml.attrib('Name','%s%s'%(arobj.arxml.tag,self.childCount()))
                arobj.onObjectNameChanged(arobj.arxml.attrib('Name'))
        
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
        if(self.parent() != None): # if parent is None, then it is top arobj, cann't be deleted
            self.root.actions[Index].setText('Delete %s'%(self.arxml.tag))
            self.root.actions[Index].setDisabled(False)
            Index += 1
        for i in range(Index,4):
            self.root.actions[i].setDisabled(True)
            self.root.actions[i].setText('')
            
        self.root.showConfig(self)
        
    def onAction_Add(self,what,arxml=None):
        for Descriptor in self.arxml.childDescriptors():
            if(Descriptor.tag == what):
                if(IsArxmlList(self.arxml)):
                    max = self.arxml.getMaxChildAllowed()
                    if(max > self.childCount()):
                        self.addChildArobj(ArgObject(Arxml(Descriptor),self.root,self))
                    else:
                        print 'Error:Maximum %s for %s is %s!'%(what,self.arxml.tag,max)
                else:
                    # ok, by default this is list things
                    already = False
                    for I in range(0,self.childCount()):
                        arobj = self.child(I)
                        if(arobj.arxml.tag == Descriptor.tag):
                            already = True
                            break
                    if(already == False):
                        self.addChildArobj(ArgObject(Arxml(Descriptor),self.root,self))
                    else:
                        print 'Info: Only 1 %s is allowed for %s.'%(Descriptor.tag,self.arxml.tag)
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
        arobj = self.currentItem()
        assert(isinstance(arobj,ArgObject))
        assert(arobj.arxml.tag == what)
        if(self.indexOfTopLevelItem(arobj) != -1):
            self.takeTopLevelItem(self.indexOfTopLevelItem(arobj))
        else:
            pObj = arobj.parent()
            pObj.takeChild(pObj.indexOfChild(arobj))
    def reloadArxml(self,arxml):
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        self.arxml = arxml
        for arxml in self.arxml.childArxmls2():
            arobj =ArgObject(arxml,self.root) 
            self.addTopLevelItem(arobj)
        # bug fix for arxml update
        for arxml in self.arxml.childArxmls():
            already = False
            for arxml1 in self.arxml.childArxmls2():
                if(arxml.tag==arxml1.tag):
                    already=True
            if(already == False):
                self.addTopLevelItem(ArgObject(arxml,self.root))
        # expand it
        for i in range(0,self.topLevelItemCount()):
            trItem = self.topLevelItem(i)
            trItem.setExpanded(True)
            # expand it
            for j in range(0,trItem.childCount()):
                trItem2 = trItem.child(j)
                trItem2.setExpanded(True)
                # expand it
                for k in range(0,trItem2.childCount()):
                    trItem3 = trItem.child(k)
                    trItem3.setExpanded(True)
                    # expand it
                    for n in range(0,trItem3.childCount()):
                        trItem4 = trItem.child(n)
                        trItem4.setExpanded(True)

    def toArxml(self):
        arxml = self.arxml.toArxml()
        for i in range(0,self.topLevelItemCount()):
            arobj = self.topLevelItem(i)
            assert(isinstance(arobj, ArgObject))
            arxml.append(arobj.toArxml())
        return arxml
    
    def onAction(self,text):
        reAction = re.compile(r'(Add|Delete) (\w+)')
        action = reAction.search(text).groups()
        if(action[0] == 'Add'):
            arobj = self.currentItem()
            assert(isinstance(arobj,ArgObject))
            arobj.onAction_Add(action[1])
        elif(action[0] == 'Delete'):
            self.onAction_Delete(action[1])
       
    def itemSelectionChanged(self):
        arobj = self.currentItem()
        assert(isinstance(arobj,ArgObject))
        arobj.onItemSelectionChanged()
    
    def onObjectNameChanged(self,text):
        arobj = self.currentItem()
        assert(isinstance(arobj,ArgObject))
        arobj.onObjectNameChanged(text)

class ArgModule(QMainWindow):
    # TODO: actions = []
    # members declare here seems to be static 
    # that is the members can be accessed by all instance of class ArgModule
    # SO BETTER TO DECALRE MEMEBERS in __init__()
    def __init__(self,arxml,main):
        self.actions = []
        super(QMainWindow,self).__init__()
        self.tag = arxml.tag
        self.main=main
        
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        
        self.arobjTree = ArgObjectTree(arxml,self)
        self.wConfig  = QMainWindow()
        self.qSplitter.insertWidget(0,self.arobjTree)
        self.qSplitter.insertWidget(1,self.wConfig)
        
        self.setCentralWidget(self.qSplitter)
        self.creActions()
    
    def reloadArxml(self,arxml):
        self.arobjTree.reloadArxml(arxml)
    
    def toArxml(self):
        return self.arobjTree.toArxml()
    
    def showConfig(self,trItem):
        assert(isinstance(trItem, ArgObject))
        self.grid = QGridLayout()
        self.frame = QFrame()
        self.frame.setLayout(self.grid)
        if(IsArxmlList(trItem.arxml)==False):
            for Row in range(0,len(trItem.arxml.descriptor.items())):
                rePos = re.compile(r'PosGUI=(\d+)')
                for [key,value] in trItem.arxml.descriptor.items():
                    posGui = int(rePos.search(value).groups()[0],10)
                    if(posGui == Row):
                        K = QLabel(key)
                        V = ArgWidget(key,trItem,self)
                        self.grid.addWidget(K,Row,0)
                        self.grid.addWidget(V,Row,1)
        self.wConfig.setCentralWidget(self.frame)
    def creActions(self):
        #  create 4 action
        for i in range(0,4):
            qAction=ArgAction(self.tr(''),self) 
            self.menuBar().addAction(qAction)
            qAction.setDisabled(True)
            self.actions.append(qAction)
            
    def onAction(self,text):
        self.arobjTree.onAction(text)
    
    def getURL(self,ref):
        return self.main.getURL(ref)
    
    def onObjectNameChanged(self,text):
        self.arobjTree.onObjectNameChanged(text)

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