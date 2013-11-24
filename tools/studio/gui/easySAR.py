from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
from easyOs import easyOsGui

__all__ = ['easySAR']

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
    easyOs  = None
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowTitle('easy OpenSAR Studio( parai@foxmail.com ^_^)');
        self.resize(800,600)
        self.creMenu()
        self.creStatusBar()
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
        # Tool
        tMenu=self.menuBar().addMenu(self.tr('Tool'))
        ## easyOs
        sItem=QAction(self.tr('easyOs'),self) 
        self.connect(sItem,SIGNAL('triggered()'),self.measyOs) 
        sItem.setStatusTip('Open easyOs console.') 
        tMenu.addAction(sItem)   
    def mOpen(self):
        pass
    def mSave(self):
        pass
    
    def measyOs(self):
        if(self.easyOs==None or self.easyOs.isClosed==True):
            self.easyOs = easyDockWidget('easyOs', self)  
            self.easyOs.setWidget(easyOsGui())  
            self.addDockWidget(QtCore.Qt.RightDockWidgetArea, self.easyOs)
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