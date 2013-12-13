from PyQt4 import QtCore, QtGui
from PyQt4.QtGui import *
from PyQt4.QtCore import *
import sys,os
__all__ = ['DioWidget']

class DioWidget(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowTitle("Dio Simulator")