#!/usr/bin/env python
import math

from PyQt4 import QtCore, QtGui, Qt
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import socket

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
    [195,140,0, 100,10,4,0x7453A2,310,285], # Speed
    [473,157,20,70 ,10,4,0xD63441,322,250], # Tacho
]

cMechanicalZero = 1000 # relative to iStart, unit in 0.01 degree

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
        if(Degree <= (cfg[iRange]*100+cMechanicalZero) and Degree >= 0):
            self.Degree = Degree
        else:
            print 'StepMotor:Wrong Degree Value from AUTOSAR Client.',self.cId,Degree
            return
        # Set Degree 
        self.setRotation((self.Degree-cMechanicalZero)/100+cfg[iStart])

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
            pInd.setPosDegree(0)
        self.startTimer(10)
        
        self.resize(650, 310)
        self.setWindowIcon(QtGui.QIcon("./res/StepMotor.bmp"))
        self.setWindowTitle("Step Motor Simulator")
        
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.settimeout(10)
            self.sock.connect(('127.0.0.1', 60002)) 
        except:
            print 'StepMotor: AUTOSAR Client isnot started.'
        self.sock.settimeout(0)

    def drawBackground(self,painter,rect ):
        Image = QtGui.QImage(BackGround)   
        self.scene().setSceneRect(0, 0, Image.size().width(), Image.size().height())
        painter.drawImage(0,0,Image); 
        
    def keyPressEvent(self, event):
        pass
    
    def setMatrix(self,matrix):
        cId = 0
        if(len(matrix) == len(StepMotorList)*2):
            for pInd in self.PIndList:
                degree = (ord(matrix[cId*2])<<8) + (ord(matrix[cId*2+1]))
                pInd.setPosDegree(degree)
                cId += 1
    def timerEvent(self, event):
        try: 
            matrix = self.sock.recv(len(StepMotorList)*2)
            self.setMatrix(matrix)
            #self.sock.send(self.getMatrix())
        except socket.error as e:
#             if e.errno == errno.WSAENOBUFS:
#                 pass
            #print traceback.format_exc()
            pass


if __name__ == '__main__':

    import sys

    app = QtGui.QApplication(sys.argv)
    QtCore.qsrand(QtCore.QTime(0,0,0).secsTo(QtCore.QTime.currentTime()))

    widget = GaugeWidget()
    widget.show()

    sys.exit(app.exec_())
