from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
from Argui import *
from Arxml import *
from ArGen import *
import xml.etree.ElementTree as ET

__all__ = ['easySAR']

gDefault_GEN = '../../app/config/GEN'

class easyDockWidget(QDockWidget):
    isClosed = False
    def __init__(self,title,parent=None):
        QDockWidget.__init__(self,title,parent)
        self.setAllowedAreas(QtCore.Qt.LeftDockWidgetArea|QtCore.Qt.RightDockWidgetArea)  
        #self.setFeatures(QDockWidget.DockWidgetClosable|QDockWidget.DockWidgetMovable)
    def closeEvent(self,event):
        self.isClosed = True

class easySARGui(QMainWindow):
    def __init__(self):
        self.modules = []
        self.docks   = []
        self.actions = []
        self.pdir = ''
        
        QMainWindow.__init__(self, None)
        self.setWindowTitle('easy OpenSAR Studio( parai@foxmail.com ^_^)');
        self.showMaximized()
        self.setMinimumSize(800, 400)
        
        self.creStatusBar()
        self.systemDescriptor = ET.parse('./easySAR.arxml').getroot()
        self.creMenu()
        
        self.mOpen(gDefault_GEN)

    def creMenu(self):
        # File
        tMenu=self.menuBar().addMenu(self.tr('File'))
        ## Open Ctrl+O  
        sItem=QAction(self.tr('Open'),self) 
        sItem.setShortcut('Ctrl+O'); 
        sItem.setStatusTip('Open a OpenSAR configure file.')
        self.connect(sItem,SIGNAL('triggered()'),self.mOpen)  
        tMenu.addAction(sItem) 
        ## Save Ctrl+S
        sItem=QAction(self.tr('Save'),self) 
        sItem.setShortcut('Ctrl+S'); 
        sItem.setStatusTip('Save the OpenSAR configure file.')
        self.connect(sItem,SIGNAL('triggered()'),self.mSave)  
        tMenu.addAction(sItem)  
        ## Save Ctrl+G
        sItem=QAction(self.tr('Generate'),self) 
        sItem.setShortcut('Ctrl+G'); 
        sItem.setStatusTip('Convert the OpenSAR configure file to C Code.')
        self.connect(sItem,SIGNAL('triggered()'),self.mGen)  
        tMenu.addAction(sItem)
        # easySAR Module
        tMenu=self.menuBar().addMenu(self.tr('Module'))
        for desc in self.systemDescriptor:
            sItem=ArgAction(self.tr(desc.tag),self) 
            sItem.setStatusTip('Open easy%s console.'%(desc.tag)) 
            tMenu.addAction(sItem)
            module = ArgModule(Arxml(desc))
            self.modules.append(module)
            self.docks.append(None)

    def mOpen(self,default=None):
        if(default == None):
            self.pdir = QFileDialog.getExistingDirectory(None,'Open OpenSAR Config',gDefault_GEN,QFileDialog.DontResolveSymlinks)
            if(self.pdir == ''):
                return
        else:
            self.pdir = default
        wfxml = '%s/AutosarConfig.arxml'%(self.pdir)
        if(os.path.exists(wfxml)==False):
            return
        root = ET.parse(wfxml).getroot();
        for module in self.modules:
            if(root.find(module.tag) != None):
                module.reloadArxml(Arxml(self.systemDescriptor.find(module.tag),
                                         root.find(module.tag)))
                self.onAction(module.tag)
        if(default == None):
            QMessageBox(QMessageBox.Information, 'Info', 
                        'Open OpenSAR Configuration arxml Successfully !').exec_();
    def mSave(self):
        if(self.pdir == ''):
            self.pdir = QFileDialog.getExistingDirectory(None,'Save OpenSAR Configuration',gDefault_GEN,QFileDialog.DontResolveSymlinks)
        if(self.pdir == ''):
            return
        wfxml = '%s/AutosarConfig.arxml'%(self.pdir)
        ROOT = ET.Element('ROOT')
        for module in self.modules:
            ROOT.append(module.toArxml())
        tree = ET.ElementTree(ROOT)
        tree.write(wfxml, encoding="utf-8", xml_declaration=True);
        QMessageBox(QMessageBox.Information, 'Info', 
                    'Save OpenSAR Configuration arxml Successfully !').exec_();
    def mGen(self):
        if(self.pdir == ''):
            QMessageBox(QMessageBox.Information, 'Info', 
                        'Open or Configure a Workspace first !').exec_();
            return
        for module in self.modules:
            ArGen(module.toArxml(),self.pdir)
        QMessageBox(QMessageBox.Information, 'Info', 
                        'Generate OpenSAR Configuration C Code Successfully !').exec_();
    def onAction(self,text):
        I = 0
        for module in self.modules:
            if(text == module.tag):
                break
            else:
                I += 1
        if(I >= len(self.modules)):
            return
        
        if(self.docks[I]==None):
            self.docks[I] = easyDockWidget('easy%s'%(self.modules[I].tag), self)  
            self.docks[I].setWidget(self.modules[I])  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.docks[I])
        elif(self.docks[I].isClosed==True):
            self.docks[I] = easyDockWidget('easy%s'%(self.modules[I].tag), self)  
            self.docks[I].setWidget(self.modules[I])  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.docks[I])
        else:
            print('easy%s already started.'%(self.modules[I].tag))    
    def creStatusBar(self):
        self.statusBar = QStatusBar()
        self.setStatusBar(self.statusBar)
        self.statusBar.showMessage('easy OpenSAR Studio Platform',0)
        
def easySAR():
    qtApp = QtGui.QApplication(sys.argv)
    if(os.name == 'nt'):
        qtApp.setFont(QFont('Consolas', 12)) 
    elif(os.name == 'posix'):
        qtApp.setFont(QFont('Monospace', 12))
    else:
        print('unKnown platform.')
    qtGui = easySARGui()
    qtGui.show()
    qtApp.exec_()
    

if __name__ == '__main__':
    easySAR()