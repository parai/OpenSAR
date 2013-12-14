from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
import traceback

from component.StepMotor import GaugeWidget
from component.Dio       import DioWidget
class MainConsole(QMainWindow):
    Gauge = None
    Dio   = None
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowIcon(QtGui.QIcon("./res/WinIcon.bmp"))
        self.setWindowTitle('AUTOSAR SIMULATOR ( ^_^ parai@foxmail.com ^_^ )');
        self.resize(500,20)
        self.preOpen()
        self.creToolBar()
    
    def OpenModule(self,cfg):
        if(cfg[0] == 'Gauge'):
            self.Gauge = GaugeWidget()
            self.Gauge.setGeometry(50+int(cfg[1]),50+int(cfg[2]),int(cfg[3]),int(cfg[4]))
            self.Gauge.show()
        elif(cfg[0] == 'Dio'):
            self.Dio = DioWidget()
            self.Dio.setGeometry(50+int(cfg[1]),50+int(cfg[2]),int(cfg[3]),int(cfg[4]))
            self.Dio.show()
        elif(cfg[0] == 'Main'):
            self.setGeometry(50+int(cfg[1]),50+int(cfg[2]),int(cfg[3]),int(cfg[4]))

    def preOpen(self):
        import re
        fp = open('Simulator.cfg','r')
        for el in fp.readlines():
            try:
                grp =  re.compile(r'(\w+) x=(\d+) y=(\d+) width=(\d+) height=(\d+)').search(el).groups()  
                self.OpenModule(grp)
            except:
                print traceback.format_exc()
    def closeEvent(self,event):
        if(self.Gauge != None and self.Gauge.isVisible()):
            self.Gauge.close()
        if(self.Dio != None and self.Dio.isVisible()):
            pos = self.Dio.close()
        self.close()
        
    def on_SimList_activated(self,text):
        self.on_SimList_currentIndexChanged(text)
        
    def on_SimList_currentIndexChanged(self,text):
        if(text == 'Step Motor'):
            if(self.Gauge == None):
                self.Gauge = GaugeWidget()
            self.Gauge.show()
        elif(text == 'Dio'):
            if(self.Dio == None):
                self.Dio = DioWidget()
            self.Dio.show()
            
    def on_Save_triggered(self):
        fp = open('Simulator.cfg','w')
        
        pos = self.pos()
        size = self.size()
        cstr = 'Main x=%s y=%s width=%s height=%s\n'%(pos.x(),pos.y(),size.width(),size.height())
        fp.write(cstr)
        if(self.Gauge != None and self.Gauge.isVisible()):
            pos = self.Gauge.pos()
            size = self.Gauge.size()
            cstr = 'Gauge x=%s y=%s width=%s height=%s\n'%(pos.x(),pos.y(),size.width(),size.height())
            fp.write(cstr)
        if(self.Dio != None and self.Dio.isVisible()):
            pos = self.Dio.pos()
            size = self.Dio.size()
            cstr = 'Dio x=%s y=%s width=%s height=%s\n'%(pos.x(),pos.y(),size.width(),size.height())
            fp.write(cstr)
        fp.close()

    def on_Link_clicked(self):
        import webbrowser
        webbrowser.open('http://www.autosar.org/')
        webbrowser.open('https://github.com/parai/OpenSAR')

    def creToolBar(self):
        self.toolbar = self.addToolBar('Console')
        #
        action = QAction(self)
        action.setIcon(QIcon('./res/isave.bmp'))
        self.connect(action,SIGNAL('triggered()'),self.on_Save_triggered) 
        self.toolbar.addAction(action)
        action.setToolTip('Save Simulator Information.')
        # /
        self.toolbar.addSeparator()
        #
        Link = QPushButton()
        Link.setIcon(QIcon('./res/Autosar.bmp'))
        Link.setToolTip('Open Autosar Reference URL')
        Width = 150
        Heigth = QImage('./res/Autosar.bmp').size().height()*Width/QImage('./res/Autosar.bmp').size().width()
        Link.setIconSize(QSize(Width,Heigth))
        Link.setFlat(True)
        self.toolbar.addWidget(Link)
        self.connect(Link,SIGNAL('clicked()'),self.on_Link_clicked) 
        #  
        SimList = QComboBox()
        SimList.addItem(QIcon('./res/Dio.bmp'),'Dio')
        SimList.addItem(QIcon('./res/StepMotor.bmp'),'Step Motor') 
        self.toolbar.addWidget(SimList)
        self.toolbar.setFloatable(False)
        self.toolbar.setMovable(False)
        self.connect(SimList,SIGNAL('currentIndexChanged(QString)'),self.on_SimList_currentIndexChanged) 
        self.connect(SimList,SIGNAL('activated(QString)'),self.on_SimList_activated) 
        
def Main():
    qtApp = QtGui.QApplication(sys.argv)
    if(os.name == 'nt'):
        qtApp.setFont(QFont('Consolas', 12)) 
    elif(os.name == 'posix'):
        qtApp.setFont(QFont('Monospace', 12))
    else:
        print('unKnown platform.')
    qtGui = MainConsole()
    qtGui.show()
    qtApp.exec_()

if __name__ == '__main__':
    Main()    