#!/usr/bin/env python
import math

from PyQt4 import QtCore, QtGui, Qt
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from PyQt4.pyqtconfig import QtGuiModuleMakefile

__all__ = ['GaugeWidget']

# Configuration
BackGround = './res/Gauge.jpg'
# [x,y,Offset,Length,HeadWidth,TailWidth,Color,Start,Range]
iX          = 0
iY          = 1
iOffset     = 2
iLength     = 3
iHeadWidth  = 4
iTailWidth  = 5
iColor      = 6
iStart      = 7     # Start Degree
iRange      = 8     # Span Range Degree
StepMotorList = [     \
    [195,140,0, 100,10,4,0x7453A2,310,285],
    [473,157,20,70 ,10,4,0xD63441,322,250],
]

cMechanicalZero = -1000 # relative to iStart, unit in 0.01 degree

class StepMotor(QtGui.QGraphicsItem):
    Degree = 0  # unit in 0.01 -- 
    def __init__(self,Parent,cId):
        super(StepMotor, self).__init__()
        self.cId = cId   
        #self.setFlag(QtGui.QGraphicsItem.ItemIsMovable)
        self.setFlag(QtGui.QGraphicsItem.ItemSendsGeometryChanges)
        self.setCacheMode(QtGui.QGraphicsItem.DeviceCoordinateCache)
        self.setZValue(1)

    def boundingRect(self):
        # x,y,width,height
        cfg = StepMotorList[self.cId]
        return QtCore.QRectF(-cfg[iLength]-cfg[iOffset],-cfg[iLength]-cfg[iOffset],(cfg[iLength]+cfg[iOffset])*2,(cfg[iLength]+cfg[iOffset])*2)
    
    def getPosDegree(self):
        return self.Degree
    
    def setPosDegree(self,Degree):
        cfg = StepMotorList[self.cId]
        if(Degree <= cfg[iRange]*100 and Degree >= cMechanicalZero):
            self.Degree = Degree
        else:
            print 'StepMotor:Wrong Degree Value from AUTOSAR Client.'
        # Set Degree 
        self.setRotation(self.Degree/100+cfg[iStart])
    
    def step(self,steps,clockwise = True):
        """ 1 step == 0.01 degree """
        cfg = StepMotorList[self.cId]
        if(clockwise == True):
            self.Degree += steps
        else:
            self.Degree -= steps
        if(self.Degree < cMechanicalZero):
            self.Degree = cMechanicalZero 
        # { Operation below should be removed
        degree = self.Degree/100
        if(degree > cfg[iRange]):
            degree = cfg[iRange]
            self.Degree = cfg[iRange]*100
        # }
        self.setRotation(degree+cfg[iStart])

    def paint(self, painter, option, widget):
        cfg = StepMotorList[self.cId]
        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(QtGui.QBrush(QtGui.QColor((cfg[iColor]>>16)&0xFF,(cfg[iColor]>>8)&0xFF,(cfg[iColor]>>0)&0xFF)))
        points = QtGui.QPolygon([
            QtCore.QPoint(-cfg[iOffset],                 cfg[iHeadWidth]/2),
            QtCore.QPoint(-cfg[iOffset]-cfg[iLength],    cfg[iTailWidth]/2),
            QtCore.QPoint(-cfg[iOffset]-cfg[iLength],   -cfg[iTailWidth]/2),
            QtCore.QPoint(-cfg[iOffset],                -cfg[iHeadWidth]/2),
        ])  
        painter.drawConvexPolygon(points);
        if(cfg[iOffset] < 0):
            radius = -cfg[iOffset]
        elif(cfg[iOffset] > 0):
            radius = cfg[iOffset]
        else:
            radius = cfg[iHeadWidth]
        radius += 2
        painter.drawEllipse(-radius,-radius,radius*2,radius*2)
        painter.setBrush(QtGui.QBrush(QtGui.QColor(0,0,0))) #black
        radius2 = radius*2/3
        if(radius2 > cfg[iHeadWidth]):
            radius2 = radius-cfg[iHeadWidth]*2/3
        painter.drawEllipse(-radius2,-radius2,radius2*2,radius2*2)
        
            


class GaugeWidget(QtGui.QGraphicsView):
    angle = 0
    clockwise = True
    def __init__(self):
        super(GaugeWidget, self).__init__()
        self.PIndList = []
        scene = QtGui.QGraphicsScene(self)
        scene.setItemIndexMethod(QtGui.QGraphicsScene.NoIndex)
        self.setScene(scene)
        self.setCacheMode(QtGui.QGraphicsView.CacheBackground)
        self.setViewportUpdateMode(QtGui.QGraphicsView.BoundingRectViewportUpdate)
        self.setRenderHint(QtGui.QPainter.Antialiasing)
        self.setTransformationAnchor(QtGui.QGraphicsView.AnchorUnderMouse)
        self.setResizeAnchor(QtGui.QGraphicsView.AnchorViewCenter)
        
        for cId in range(0,len(StepMotorList)):
            pInd = StepMotor(self,cId)
            cfg = StepMotorList[cId]
            scene.addItem(pInd)
            pInd.setPos(cfg[iX],cfg[iY])
            self.PIndList.append(pInd)
            pInd.setPosDegree(cMechanicalZero)
        self.startTimer(10)
        
        self.resize(650, 310)
        self.setWindowIcon(QtGui.QIcon("./res/StepMotor.bmp"))
        self.setWindowTitle("Step Motor Simulator")

    def drawBackground(self,painter,rect ):
        Image = QtGui.QImage(BackGround)   
        self.scene().setSceneRect(0, 0, Image.size().width(), Image.size().height())
        painter.drawImage(0,0,Image); 
        
    def keyPressEvent(self, event):
        pass

    def timerEvent(self, event):
        cId = 0
        for pInd in self.PIndList:
            cfg = StepMotorList[cId]
            pInd.step(300,self.clockwise) 
            if(cId == 0):
                if( pInd.getPosDegree() == 100*cfg[iRange]):
                    self.clockwise = False
                elif(pInd.getPosDegree() == cMechanicalZero):
                    self.clockwise = True
            cId += 1


if __name__ == '__main__':

    import sys

    app = QtGui.QApplication(sys.argv)
    QtCore.qsrand(QtCore.QTime(0,0,0).secsTo(QtCore.QTime.currentTime()))

    widget = GaugeWidget()
    widget.show()

    sys.exit(app.exec_())
