#!/usr/bin/env python
import math

from PyQt4 import QtCore, QtGui, Qt
from PyQt4.pyqtconfig import QtGuiModuleMakefile

# Configuration
BackGround = '../res/Gauge.jpg'
# [x,y,Offset,Length,HeadWidth,TailWidth,Color,StartAngle,Range]
iX          = 0
iY          = 1
iOffset     = 2
iLength     = 3
iHeadWidth  = 4
iTailWidth  = 5
iColor      = 6
iStart      = 7
iRange      = 8
PointerList = [     \
    [195,140,0, 100,10,4,0x7453A2,310,285],
    [473,157,0, 70 ,10,4,0xD63441,322,257],
]

class Pointer(QtGui.QGraphicsItem):
    Degree = 0  # unit in 0.01 -- 
    def __init__(self,Parent,cId):
        super(Pointer, self).__init__()
        self.cId = cId   
        #self.setFlag(QtGui.QGraphicsItem.ItemIsMovable)
        self.setFlag(QtGui.QGraphicsItem.ItemSendsGeometryChanges)
        self.setCacheMode(QtGui.QGraphicsItem.DeviceCoordinateCache)
        self.setZValue(1)

    def boundingRect(self):
        # x,y,width,height
        cfg = PointerList[self.cId]
        return QtCore.QRectF(-cfg[iLength]-cfg[iOffset],-cfg[iLength]-cfg[iOffset],(cfg[iLength]+cfg[iOffset])*2,(cfg[iLength]+cfg[iOffset])*2)
    def getPos(self):
        return self.Degree
    def step(self,steps,clockwise = True):
        cfg = PointerList[self.cId]
        if(clockwise == True):
            self.Degree += steps
        else:
            self.Degree -= steps
        if(self.Degree < 0):
            self.Degree = 0 
        degree = self.Degree/100
        if(degree > cfg[iRange]):
            degree = cfg[iRange]
            self.Degree = cfg[iRange]*100
        self.setRotation(degree+cfg[iStart])

    def paint(self, painter, option, widget):
        cfg = PointerList[self.cId]
        painter.setPen(QtCore.Qt.NoPen)
        painter.setBrush(QtGui.QBrush(QtGui.QColor((cfg[iColor]>>16)&0xFF,(cfg[iColor]>>8)&0xFF,(cfg[iColor]>>0)&0xFF)))
        points = QtGui.QPolygon([
            QtCore.QPoint(-cfg[iOffset],                 cfg[iHeadWidth]/2),
            QtCore.QPoint(-cfg[iOffset]-cfg[iLength],    cfg[iTailWidth]/2),
            QtCore.QPoint(-cfg[iOffset]-cfg[iLength],   -cfg[iTailWidth]/2),
            QtCore.QPoint(-cfg[iOffset],                -cfg[iHeadWidth]/2),
        ])  
        painter.drawConvexPolygon(points);


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
        
        for cId in range(0,len(PointerList)):
            pInd = Pointer(self,cId)
            cfg = PointerList[cId]
            scene.addItem(pInd)
            pInd.setPos(cfg[iX],cfg[iY])
            self.PIndList.append(pInd)
        self.startTimer(1)
        
        self.setWindowTitle("Dial Enjoy by parai")

    def drawBackground(self,painter,rect ):
        Image = QtGui.QImage(BackGround)   
        self.scene().setSceneRect(0, 0, Image.size().width(), Image.size().height())
        painter.drawImage(0,0,Image); 
 
    def keyPressEvent(self, event):
        pass

    def timerEvent(self, event):
        cId = 0
        for pInd in self.PIndList:
            cfg = PointerList[cId]
            pInd.step(10,self.clockwise) 
            if(cId == 0):
                if( pInd.getPos() == 100*cfg[iRange]):
                    self.clockwise = False
                elif(pInd.getPos() == 0):
                    self.clockwise = True
            cId += 1


if __name__ == '__main__':

    import sys

    app = QtGui.QApplication(sys.argv)
    QtCore.qsrand(QtCore.QTime(0,0,0).secsTo(QtCore.QTime.currentTime()))

    widget = GaugeWidget()
    widget.show()

    sys.exit(app.exec_())
