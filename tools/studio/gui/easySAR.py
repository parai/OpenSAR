from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
from easyOs     import easyOsGui
from easyCom  import easyComGui

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
    easyCan = None
    easyCom = None
    easyOs  = None
    pdir = ''
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowTitle('easy OpenSAR Studio( parai@foxmail.com ^_^)');
        self.showMaximized()
        self.creMenu()
        self.creStatusBar()
        self.easyOsCfg = easyOsGui()
        self.easyComCfg = easyComGui()
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
        ## easyOs
        sItem=QAction(self.tr('easyOs'),self) 
        self.connect(sItem,SIGNAL('triggered()'),self.measyOs) 
        sItem.setStatusTip('Open easyOs console.') 
        tMenu.addAction(sItem) 
        ## easyCom
        sItem=QAction(self.tr('easyCom'),self) 
        self.connect(sItem,SIGNAL('triggered()'),self.measyCom) 
        sItem.setStatusTip('Open easyCom console.') 
        tMenu.addAction(sItem)   
    def mOpen(self):
        self.pdir = QFileDialog.getExistingDirectory(None,'Open OpenSAR Config',gDefault_GEN,QFileDialog.DontResolveSymlinks)
        if(self.pdir == ''):
            return
        self.easyOsCfg.mOpen(self.pdir)
        self.easyComCfg.mOpen(self.pdir)
        self.setWindowTitle('easy OpenSAR Studio( parai@foxmail.com ^_^) Workspace=%s'%(self.pdir));
        self.measyOs()
        self.measyCom()
        QMessageBox(QMessageBox.Information, 'Info', 
                        'Open OpenSAR Configuration xml Successfully !').exec_();
    def mSave(self):
        if(self.pdir == ''):
            self.pdir = QFileDialog.getExistingDirectory(None,'Save OpenSAR Config',gDefault_GEN,QFileDialog.DontResolveSymlinks)
        if(self.pdir == ''):
            return
        self.easyOsCfg.mSave(self.pdir)
        self.easyComCfg.mSave(self.pdir)
        self.setWindowTitle('easy OpenSAR Studio( parai@foxmail.com ^_^) Workspace=%s'%(self.pdir));
        QMessageBox(QMessageBox.Information, 'Info', 
                        'Save OpenSAR Configuration xml Successfully !').exec_();
    def mGen(self):
        if(self.pdir == ''):
            QMessageBox(QMessageBox.Information, 'Info', 
                        'Open or Configure a Workspace first !').exec_();
        self.easyOsCfg.mGen(self.pdir)
        self.easyComCfg.mGen(self.pdir)
        QMessageBox(QMessageBox.Information, 'Info', 
                        'Generate OpenSAR Configuration C Code Successfully !').exec_();
    def measyOs(self):
        if(self.easyOs==None):
            self.easyOs = easyDockWidget('easyOs', self)  
            self.easyOs.setWidget(self.easyOsCfg)  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.easyOs)
        elif(self.easyOs.isClosed==True):
            self.easyOs = easyDockWidget('easyOs', self)  
            self.easyOs.setWidget(self.easyOsCfg)  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.easyOs)
        else:
            print('easyOs already started.')
    
    def measyCom(self):
        if(self.easyCom==None):
            self.easyCom = easyDockWidget('easyCom', self)  
            self.easyCom.setWidget(self.easyComCfg)  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.easyCom)
        elif(self.easyCom.isClosed==True):
            self.easyCom = easyDockWidget('easyCom', self)  
            self.easyCom.setWidget(self.easyComCfg)  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.easyCom)
        else:
            print('easyOs already started.')

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