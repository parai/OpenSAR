"""/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
"""
from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os,re
import xml.etree.ElementTree as ET
import traceback
from Arxml import *

__all__ = ['ArgModule','ArgAction']

cCharWidth=14
cActionNumber=8

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

def IsEnabled(key,arobj):
    arxml=arobj.arxml
    reEnabled = re.compile(r'Enabled=\((.*)\)')
    reSpilt  = re.compile(r'\s+')
    reSelf   = re.compile(r'Self\.([^\s\(\)]+)(==|!=)(\w+)')
    reUrl    = re.compile(r'([^\s]+)(==|!=)(\w+)')
    reUrlSelf= re.compile(r'Self\.([^\s\(\)]+)')
    descriptor = arxml.getKeyDescriptor(key)
    Enabled = True
    if(reEnabled.search(descriptor)):
        list = reEnabled.search(descriptor).groups()[0]
        list = re.split(reSpilt, list)
        isAnd = True # '&&"
        isEnabled = True
        for cond in list:
            if(cond == '||' or cond == 'or'):
                isAnd = False
            elif(cond == '&&' or cond == 'and'):
                isAnd = True
            else:
                if(reSelf.search(cond)):
                    keyL = reSelf.search(cond).groups()[0] 
                    tarobj = arobj
                    for key1 in keyL.split('.'):
                        if(key1=='Parent'):
                            tarobj = tarobj.parent()
                        else:
                            break
                    operater = reSelf.search(cond).groups()[1]
                    value = reSelf.search(cond).groups()[2]
                    if(operater=='==' and tarobj.arxml.attrib(key1)==value):
                        isEnabled = True
                    elif(operater=='!=' and tarobj.arxml.attrib(key1)!=value):
                        isEnabled = True
                    else:
                        isEnabled = False
                elif(reUrlSelf.search(cond)):
                    keyL = reUrlSelf.search(cond).groups()[0] 
                    tarobj = arobj
                    for key1 in keyL.split('.'):
                        if(key1=='Parent'):
                            tarobj = tarobj.parent()
                        else:
                            break
                    selfV = tarobj.arxml.attrib(key1)
                    url=reUrl.search(cond).groups()[0]
                    url=url.replace('(Self.%s)'%(keyL),selfV)
                    operater = reUrl.search(cond).groups()[1]
                    value = reUrl.search(cond).groups()[2]
                    if(value=='None'):
                        rV=arobj.root.getURL(url)
                        if(operater=='==' and rV==None):
                            isEnabled = True
                        elif(operater=='!=' and rV!=None):
                            isEnabled = True
                        else:
                            isEnabled = False
                    else:
                        urlL=url.split('.')
                        key1=urlL[len(urlL)-1]
                        back=len(key1)+1
                        url1=url[:-back]
                        rV=arobj.root.getURL(url1)
                        if(rV != None):
                            if(operater=='==' and rV.attrib[key1]==value):
                                isEnabled = True
                            elif(operater=='!=' and rV.attrib[key1]!=value):
                                isEnabled = True
                            else:
                                isEnabled = False
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
    
class ArgInfoInput(QTextEdit):
    def __init__(self,key,arobj,root,IsNotInTableWidget=True):
        assert(isinstance(root,ArgModule))
        self.key = key
        self.arobj = arobj
        super(QTextEdit,self).__init__(self.arobj.arxml.attrib(self.key))
        self.setToolTip(self.arobj.arxml.getKeyDescriptor(self.key).replace('\\n','\n'))
        self.connect(self, SIGNAL('textChanged()'),self.onTextChanged)
    def onTextChanged(self):
        self.arobj.arxml.attrib(self.key,self.toPlainText())  
    
class ArgInput(QLineEdit):
    def __init__(self,key,arobj,root,IsNotInTableWidget):
        assert(isinstance(root,ArgModule))
        self.key = key
        self.arobj = arobj
        self.root  = root
        self.IsNotInTableWidget=IsNotInTableWidget
        super(QLineEdit,self).__init__(self.arobj.arxml.attrib(self.key))
        self.setEnabled(IsEnabled(key, arobj))
        if(IsEnabled(key, arobj)==False):
            # if disabled set it to default
            reDeafult = re.compile(r'\s+Default=([^\s=\(\)]+)')
            var = 'TBD'
            descriptor = self.arobj.arxml.getKeyDescriptor(self.key)
            if(reDeafult.search(descriptor)):
                var = reDeafult.search(descriptor).groups()[0]
            self.arobj.arxml.attrib(self.key,var) 
        self.setToolTip(self.arobj.arxml.getKeyDescriptor(self.key).replace('\\n','\n'))
        self.connect(self, SIGNAL('textChanged(QString)'),self.onTextChanged)
        
    def onTextChanged(self,text):
        reInput = re.compile(r'^(Text|Integer)')
        descriptor = self.arobj.arxml.getKeyDescriptor(self.key)
        
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
            self.arobj.arxml.attrib(self.key,text)
            if(self.key == 'Name'):
                self.arobj.onObjectNameChanged(text)
        else:
            self.setText(self.arobj.arxml.attrib(self.key))

class ArgSelect(QComboBox):
    def __init__(self,key,arobj,root,IsNotInTableWidget):
        assert(isinstance(root,ArgModule))
        self.key = key
        self.arobj = arobj
        self.root  = root
        self.IsNotInTableWidget=IsNotInTableWidget
        super(QComboBox,self).__init__()
        self.setEnabled(IsEnabled(key, arobj))
        self.initItems()
        self.setToolTip(self.arobj.arxml.getKeyDescriptor(self.key).replace('\\n','\n'))
        self.connect(self, SIGNAL('currentIndexChanged(QString)'),self.onTextChanged)
        
    def initItems(self):
        if(self.isEnabled()==False):
            # if disabled set it to default
            reDeafult = re.compile(r'\s+Default=([^\s=\(\)]+)')
            var = 'TBD'
            descriptor = self.arobj.arxml.getKeyDescriptor(self.key)
            if(reDeafult.search(descriptor)):
                var = reDeafult.search(descriptor).groups()[0]
            self.arobj.arxml.attrib(self.key,var)  
            return
        reSelect = re.compile(r'^(EnumRef|Enum|Boolean)')
        descriptor = self.arobj.arxml.getKeyDescriptor(self.key)
        type = reSelect.search(descriptor).groups()[0]
        if(type == 'Enum'):
            reList = re.compile(r'^Enum=\(([^\s]+)\)')
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
                tarobj = self.arobj
                for key in keyL.split('.'):
                    if(key=='Parent'):
                        tarobj = tarobj.parent()
                    else:
                        break
                ref = ref.replace('(Self.%s)'%(keyL),tarobj.arxml.attrib(key))
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
        self.setCurrentIndex(self.findText(self.arobj.arxml.attrib(self.key)))
    def onTextChanged(self,text):
        self.arobj.arxml.attrib(self.key,text)        
        if(self.key == 'Name'):
            self.arobj.onObjectNameChanged(text)
        elif(self.IsNotInTableWidget==True):
            self.root.showConfig(self.arobj)
        else:
            # if IsNotInTableWidget == False, the in table show, update my parent
            self.root.showConfig(self.arobj.parent())
        

def ArgWidget(key,arobj,root,IsNotInTableWidget=True):
    reInfo  = re.compile(r'^TextArea')
    reInput = re.compile(r'^Text|Integer')
    reSelect = re.compile(r'^EnumRef|Enum|Boolean')
    descriptor = arobj.arxml.getKeyDescriptor(key)
    if(reInfo.search(descriptor)):
        return ArgInfoInput(key,arobj,root,IsNotInTableWidget)
    elif(reInput.search(descriptor)):
        return ArgInput(key,arobj,root,IsNotInTableWidget)
    elif(reSelect.search(descriptor)):
        return ArgSelect(key,arobj,root,IsNotInTableWidget)
    else:
        return QLineEdit('Type Error for %s'%(arobj.arxml.tag))

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
        if(IsArxmlList(self.arxml) or self.arxml.tag=='General' or
           IsArxmlList(self.parent().arxml)==False):
            self.setText(0,'%s'%(self.arxml.tag))
        else:
            self.setText(0,'%s'%(self.arxml.attrib('Name')))
        
        for arx in self.arxml.childArxmls2():
            self.addChildArobj(ArgObject(arx,self.root,self))
            self.setExpanded(True)
        #self.sortChildren(0,Qt.AscendingOrder)
        
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
        self.onItemSelectionChanged() # trigger refresh
        if(IsArxmlList(arobj.arxml) or IsArxmlList(arobj.parent().arxml)==False):
            pass
        else:
            if('TBD' == arobj.arxml.attrib('Name')):
                arobj.arxml.attrib('Name','%s%s'%(arobj.arxml.tag,self.childCount()))
                arobj.onObjectNameChanged(arobj.arxml.attrib('Name'))
        
    def onObjectNameChanged(self,text):
        assert(text == self.arxml.attrib('Name'))
        if(IsArxmlList(self.arxml) or self.arxml.tag=='General' or
           IsArxmlList(self.parent().arxml)==False):
            self.setText(0,'%s'%(self.arxml.tag))
        else:
            self.setText(0,'%s'%(self.arxml.attrib('Name')))

    def onItemSelectionChanged(self):
        Index = 0
        for Descriptor in self.arxml.childDescriptors():
            already = False
            if(IsArxmlList(Descriptor) or IsArxmlList(self.arxml)==False):
                # ok, it is a list things or its parent is not a list
                for I in range(0,self.childCount()):
                    arobj = self.child(I)
                    if(arobj.arxml.tag==Descriptor.tag):
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
        for i in range(Index,cActionNumber):
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
                    # ok, by default this is list things or its parent is not a list
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
        self.setMaximumWidth(600)
        
        
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
        try:
            for i in range(0,self.topLevelItemCount()):
                arobj = self.topLevelItem(i)
                arobj.setExpanded(True)
                # expand it
                for j in range(0,arobj.childCount()):
                    arobj2 = arobj.child(j)
                    arobj2.setExpanded(True)
                    # expand it
                    for k in range(0,arobj2.childCount()):
                        arobj3 = arobj.child(k)
                        arobj3.setExpanded(True)
                        # expand it
                        for n in range(0,arobj3.childCount()):
                            arobj4 = arobj.child(n)
                            arobj4.setExpanded(True)
        except:
            #print traceback.format_exc()
            pass

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
    
    def showConfig(self,arobj):
        assert(isinstance(arobj, ArgObject))
        if(IsArxmlList(arobj.arxml)==False):
            self.frame = QFrame()
            self.frame.setMinimumWidth(self.width()*3/4)
            self.grid = QGridLayout()
            self.frame.setLayout(self.grid)
            for Column in range(0,len(arobj.arxml.descriptor.items())):
                rePos = re.compile(r'PosGUI=(\d+)')
                for [key,value] in arobj.arxml.descriptor.items():
                    posGui = int(rePos.search(value).groups()[0],10)
                    if(posGui == Column):
                        K = QLabel(key)
                        V = ArgWidget(key,arobj,self)
                        self.grid.addWidget(K,Column,0)
                        self.grid.addWidget(V,Column,1)
            self.wConfig.setCentralWidget(self.frame)
        else:
            self.table = QTableWidget()
            headers = []
            widths=[]
            for i in range(0,arobj.childCount()):
                arobj1 = arobj.child(i)
                for Column in range(0,len(arobj1.arxml.descriptor.items())):
                    rePos = re.compile(r'PosGUI=(\d+)')
                    for [key,value] in arobj1.arxml.descriptor.items():
                        posGui = int(rePos.search(value).groups()[0],10)
                        if(posGui == Column):
                            headers.append(key)
                            widths.append(len(key)*cCharWidth)
                            break
                break
            self.table.setColumnCount(len(headers))  
            self.table.setHorizontalHeaderLabels(QStringList(headers))
            for i in range(0,arobj.childCount()):
                arobj1 = arobj.child(i)
                index = self.table.rowCount()
                self.table.setRowCount(index+1)
                for Column in range(0,len(arobj1.arxml.descriptor.items())):
                    rePos = re.compile(r'PosGUI=(\d+)')
                    for [key,value] in arobj1.arxml.descriptor.items():
                        posGui = int(rePos.search(value).groups()[0],10)
                        if(posGui == Column):
                            if((len(arobj1.arxml.attrib(key))+1)*cCharWidth>widths[Column]):
                                widths[Column] = (len(arobj1.arxml.attrib(key))+1)*cCharWidth
                            V = ArgWidget(key,arobj1,self,False)
                            self.table.setCellWidget(index,Column,V) 
            try:
                for Column in range(0,len(arobj1.arxml.descriptor.items())):
                    self.table.setColumnWidth(Column,widths[Column])
            except:
                pass
            self.table.setMinimumWidth(self.width()*3/4)                  
            self.wConfig.setCentralWidget(self.table)
    def creActions(self):
        #  create cActionNumber action
        for i in range(0,cActionNumber):
            qAction=ArgAction(self.tr(''),self) 
            self.menuBar().addAction(qAction)
            qAction.setDisabled(True)
            self.actions.append(qAction)
            
    def onAction(self,text):
        self.arobjTree.onAction(text)
    
    def getURL(self,ref):
        return self.main.getURL(ref)

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