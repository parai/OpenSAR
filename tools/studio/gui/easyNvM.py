from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import xml.etree.ElementTree as ET

__all__ = ['easyNvMGui']            
       
class easyEaCfgTree(QTreeWidget):
    blockid = 0
    dataid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Type','Size','Default Value','Comment']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            if(self.indexOfTopLevelItem(treeItem) != -1):
                name = self.itemWidget(treeItem,0).text()
                self.root.qAction2.setText('Delete NvM Block <%s>'%(name))
                self.root.qAction2.setStatusTip('Delete This NvM Block <%s> Object, Be Careful as this action is dangerous.'%(name))
                self.root.qAction2.setDisabled(False)
                self.root.qAction1.setText('Add Data For NvM Block<%s>'%(name))
                self.root.qAction1.setStatusTip('Add Data For NvM Block<%s>.'%(name))
                self.root.qAction1.setDisabled(False)
            else:
                # Sub Data Item
                name = self.itemWidget(treeItem.parent(),0).text()
                self.root.qAction2.setText('Delete NvM Block <%s>'%(name))
                self.root.qAction2.setStatusTip('Delete This NvM Block <%s> Object, Be Careful as this action is dangerous.'%(name))
                self.root.qAction2.setDisabled(False)
                name = self.itemWidget(treeItem,0).text()
                self.root.qAction1.setText('Delete Data <%s>'%(name))
                self.root.qAction1.setStatusTip('Delete This Data <%s> Object, Be Careful as this action is dangerous.'%(name))
                self.root.qAction1.setDisabled(False)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def toXML(self):
        BlockList = ET.Element('EaBlockList')
        for i in range(0,self.topLevelItemCount()):
            Block = ET.Element('Block')
            treeBlock = self.topLevelItem(i)
            Block.attrib['name'] = str(self.itemWidget(treeBlock,0).text())
            DataList = ET.Element('DataList')
            for j in range(0,treeBlock.childCount()):
                Data = ET.Element('Data')
                treeData = treeBlock.child(j)
                Data.attrib['name'] = str(self.itemWidget(treeData,0).text())
                Data.attrib['type'] = str(self.itemWidget(treeData,1).currentText())
                Data.attrib['size'] = str(self.itemWidget(treeData,2).text())
                Data.attrib['default'] = str(self.itemWidget(treeData,3).text())
                Data.attrib['comment'] = str(self.itemWidget(treeData,4).text())
                DataList.append(Data)
            Block.append(DataList)
            BlockList.append(Block)
        return BlockList
    def loadXML(self,ROOT):
        """TaskList is in the format of ET"""
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        if(ROOT.find('EaBlockList') == None):
            return
        for Block in ROOT.find('EaBlockList'):
            self.addNvMBlock(Block)

    def addNvMBlock(self,Block=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem)
        if(Block != None):
            sName = Block.attrib['name']
        else:
            sName = 'Block%s'%(self.blockid)
        # task name 
        Name = QLineEdit(sName)
        self.setItemWidget(treeItem,0,Name)    
        self.blockid += 1 
        if(Block != None):
            self.setCurrentItem(treeItem)
            for Data in Block.find('DataList'):
                self.addNvMData(Data)
          
    def addNvMData(self,Data=None):
        blockItem = self.currentItem()
        dataItem = QTreeWidgetItem()
        blockItem.addChild(dataItem)
        if Data != None:
            sName = Data.attrib['name']
            sType = Data.attrib['type']
            sSize = Data.attrib['size']
            sDefault = Data.attrib['default']
            sComment  = Data.attrib['comment']
        else:
            sName = 'Data%s'%(self.dataid)
            sType = 'uint32'
            sSize = '4'
            sDefault = '0'
            sComment = ''
        dataName = QLineEdit(sName)
        dataName.setStatusTip('Name For Data, each Data must has a unique name.')
        dataType = QComboBox()
        dataType.addItems(QStringList(['uint32','uint16','uint8','uint32_n','uint16_n','uint8_n']))
        dataType.setCurrentIndex(dataType.findText(sType))
        dataSize = QLineEdit(sSize)
        dataSize.setToolTip('Only valid for type [uint32_n,uint16_n,uint8_n]')
        dataDefault = QLineEdit(sDefault)
        dataDefault.setToolTip(('If Array, use comma to seperate each value,such as "0,1,2"'))
        Comment = QLineEdit(sComment)      
        self.setItemWidget(dataItem,0,dataName)
        self.setItemWidget(dataItem,1,dataType)
        self.setItemWidget(dataItem,2,dataSize)
        self.setItemWidget(dataItem,3,dataDefault)
        self.setItemWidget(dataItem,4,Comment)
        blockItem.setExpanded(True)
        self.dataid += 1
            
    def deleteData(self):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
            
    def deleteBlock(self):
        if(self.indexOfTopLevelItem(self.currentItem()) != -1):
            self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))
        else:
            self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem().parent()))
        
class easyFeeCfgTree(QTreeWidget):
    blockid = 0
    dataid = 0
    def __init__(self,parent):  
        super(QTreeWidget,self).__init__(parent) 
        self.root =  parent
        list = ['Name','Type','Size','Default Value','Comment']
        self.setHeaderLabels(QStringList(list))
        self.setColumnWidth(0,150)
        self.connect(self, SIGNAL('itemSelectionChanged()'),self.itemSelectionChanged)
    def itemSelectionChanged(self):
        try:
            treeItem = self.currentItem()
            if(self.indexOfTopLevelItem(treeItem) != -1):
                name = self.itemWidget(treeItem,0).text()
                self.root.qAction2.setText('Delete NvM Block <%s>'%(name))
                self.root.qAction2.setStatusTip('Delete This NvM Block <%s> Object, Be Careful as this action is dangerous.'%(name))
                self.root.qAction2.setDisabled(False)
                self.root.qAction1.setText('Add Data For NvM Block<%s>'%(name))
                self.root.qAction1.setStatusTip('Add Data For NvM Block<%s>.'%(name))
                self.root.qAction1.setDisabled(False)
            else:
                # Sub Data Item
                name = self.itemWidget(treeItem.parent(),0).text()
                self.root.qAction2.setText('Delete NvM Block <%s>'%(name))
                self.root.qAction2.setStatusTip('Delete This NvM Block <%s> Object, Be Careful as this action is dangerous.'%(name))
                self.root.qAction2.setDisabled(False)
                name = self.itemWidget(treeItem,0).text()
                self.root.qAction1.setText('Delete Data <%s>'%(name))
                self.root.qAction1.setStatusTip('Delete This Data <%s> Object, Be Careful as this action is dangerous.'%(name))
                self.root.qAction1.setDisabled(False)
        except:
            self.root.qAction1.setDisabled(True)
            self.root.qAction2.setDisabled(True)
    def toXML(self):
        BlockList = ET.Element('FeeBlockList')
        for i in range(0,self.topLevelItemCount()):
            Block = ET.Element('Block')
            treeBlock = self.topLevelItem(i)
            Block.attrib['name'] = str(self.itemWidget(treeBlock,0).text())
            DataList = ET.Element('DataList')
            for j in range(0,treeBlock.childCount()):
                Data = ET.Element('Data')
                treeData = treeBlock.child(j)
                Data.attrib['name'] = str(self.itemWidget(treeData,0).text())
                Data.attrib['type'] = str(self.itemWidget(treeData,1).currentText())
                Data.attrib['size'] = str(self.itemWidget(treeData,2).text())
                Data.attrib['default'] = str(self.itemWidget(treeData,3).text())
                Data.attrib['comment'] = str(self.itemWidget(treeData,4).text())
                DataList.append(Data)
            Block.append(DataList)
            BlockList.append(Block)
        return BlockList
    def loadXML(self,ROOT):
        """TaskList is in the format of ET"""
        # delete all as reload
        for i in range(0,self.topLevelItemCount()):
            self.takeTopLevelItem(0)
        # reload
        if(ROOT.find('FeeBlockList') == None):
            return
        for Block in ROOT.find('FeeBlockList'):
            self.addNvMBlock(Block)

    def addNvMBlock(self,Block=None):
        treeItem = QTreeWidgetItem()
        self.addTopLevelItem(treeItem)
        if(Block != None):
            sName = Block.attrib['name']
        else:
            sName = 'Block%s'%(self.blockid)
        # task name 
        Name = QLineEdit(sName)
        self.setItemWidget(treeItem,0,Name)    
        self.blockid += 1 
        if(Block != None):
            self.setCurrentItem(treeItem)
            for Data in Block.find('DataList'):
                self.addNvMData(Data)
          
    def addNvMData(self,Data=None):
        blockItem = self.currentItem()
        dataItem = QTreeWidgetItem()
        blockItem.addChild(dataItem)
        if Data != None:
            sName = Data.attrib['name']
            sType = Data.attrib['type']
            sSize = Data.attrib['size']
            sDefault = Data.attrib['default']
            sComment  = Data.attrib['comment']
        else:
            sName = 'Data%s'%(self.dataid)
            sType = 'uint32'
            sSize = '4'
            sDefault = '0'
            sComment = ''
        dataName = QLineEdit(sName)
        dataName.setStatusTip('Name For Data, each Data must has a unique name.')
        dataType = QComboBox()
        dataType.addItems(QStringList(['uint32','uint16','uint8','uint32_n','uint16_n','uint8_n']))
        dataType.setCurrentIndex(dataType.findText(sType))
        dataSize = QLineEdit(sSize)
        dataSize.setToolTip('Only valid for type [uint32_n,uint16_n,uint8_n]')
        dataDefault = QLineEdit(sDefault)
        dataDefault.setToolTip(('If Array, use comma to seperate each value,such as "0,1,2"'))
        Comment = QLineEdit(sComment)      
        self.setItemWidget(dataItem,0,dataName)
        self.setItemWidget(dataItem,1,dataType)
        self.setItemWidget(dataItem,2,dataSize)
        self.setItemWidget(dataItem,3,dataDefault)
        self.setItemWidget(dataItem,4,Comment)
        blockItem.setExpanded(True)
        self.dataid += 1
            
    def deleteData(self):
        if(self.indexOfTopLevelItem(self.currentItem()) == -1):
            pTree = self.currentItem().parent()
            index = pTree.indexOfChild(self.currentItem())
            pTree.takeChild(index)
        else:
            print 'system error when remove event.'
            
    def deleteBlock(self):
        if(self.indexOfTopLevelItem(self.currentItem()) != -1):
            self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem()))
        else:
            self.takeTopLevelItem(self.indexOfTopLevelItem(self.currentItem().parent()))

class easyNvMCfgTree(QTreeWidget):
    def __init__(self,parent=None):  
        super(QTreeWidget,self).__init__(parent)
        self.setHeaderLabel('easyNvM')
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Fee')))
        self.addTopLevelItem(QTreeWidgetItem(QStringList('Ea')))
        self.setMaximumWidth(200); 

# this is the root configuration of OpenSAR NvM
class easyNvMGui(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.easyTree = easyNvMCfgTree(self)
        self.easyFeeTree = easyFeeCfgTree(self)
        self.easyEaTree  = easyEaCfgTree(self)
        self.qSplitter = QSplitter(Qt.Horizontal,self)
        self.creGui()
        self.creMenu()
    def creMenu(self):
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
        if(self.qAction1.text() == 'Add Fee Block'):
            self.easyFeeTree.addNvMBlock()
        elif(self.qAction1.text() == 'Add Ea Blcok'):
            self.easyEaTree.addNvMBlock()
        elif(str(self.qAction1.text()).find('Add Data For NvM Block') != -1):
            if(self.easyFeeTree.isVisible()):
                self.easyFeeTree.addNvMData()
            else:
                self.easyEaTree.addNvMData()
        elif(str(self.qAction1.text()).find('Delete Data') != -1):
            if(self.easyFeeTree.isVisible()):
                self.easyFeeTree.deleteData()
            else:
                self.easyEaTree.deleteData()

    def mqAction2(self):
        if(str(self.qAction2.text()).find('Delete NvM Block') != -1):
            if(self.easyFeeTree.isVisible()):
                self.easyFeeTree.deleteBlock()
            else:
                self.easyEaTree.deleteBlock()

    def creGui(self):
        self.qSplitter.insertWidget(0,self.easyTree)
        self.qSplitter.insertWidget(1,self.easyFeeTree)
        self.qSplitter.insertWidget(1,self.easyEaTree)
        self.setCentralWidget(self.qSplitter)
        self.connect(self.easyTree,SIGNAL('itemClicked(QTreeWidgetItem*, int)'),self.easyTreeClicked) 
        self.showTableWidget(self.easyFeeTree)

    def showTableWidget(self,widget):
        if(self.easyFeeTree == widget):
            self.easyFeeTree.setVisible(True);
        else:
            self.easyFeeTree.setVisible(False);
        if(self.easyEaTree == widget):
            self.easyEaTree.setVisible(True);
        else:
            self.easyEaTree.setVisible(False);

    def easyTreeClicked(self,item,column):
        if(item.text(0) == 'Fee'):
            self.showTableWidget(self.easyFeeTree)
        elif(item.text(0) == 'Ea'):
            self.showTableWidget(self.easyEaTree)
        if(item.text(0) == 'Fee'):
            self.qAction1.setText('Add Fee Block')
            self.qAction1.setStatusTip('Add a Fee Block Object.')
            self.qAction1.setDisabled(False)
            self.qAction2.setDisabled(True)
        elif(item.text(0) == 'Ea'):
            self.qAction1.setText('Add Ea Blcok')
            self.qAction1.setStatusTip('Add a Ea Blcok Object.')
            self.qAction1.setDisabled(False)
            self.qAction2.setDisabled(True)
        else:
            self.qAction1.setDisabled(True)
    def mOpen(self,pdir):
        wfxml = '%s/nvm.wfxml'%(pdir)
        root = ET.parse(wfxml).getroot();
        self.easyFeeTree.loadXML(root)
        self.easyEaTree.loadXML(root)
        self.qAction1.setDisabled(True)
        self.qAction2.setDisabled(True)
    def mSave(self,pdir):
        wfxml = '%s/nvm.wfxml'%(pdir)
        root = ET.Element('NVMROOT')
        root.append(self.easyFeeTree.toXML())
        root.append(self.easyEaTree.toXML())
        tree = ET.ElementTree(root)
        tree.write(wfxml, encoding="utf-8", xml_declaration=True);
    def mGen(self,pdir):
        from gen.GenNvM import GenNvM
        wfxml = '%s/nvm.wfxml'%(pdir)
        GenNvM(str(wfxml))