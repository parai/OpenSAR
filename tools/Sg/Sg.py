# Static Gui tool for AUTOSAR
import sys,os
from PyQt4 import QtGui
from PyQt4.QtGui import *

qtApp = QtGui.QApplication(sys.argv)

class Sg():
    def __init__(self,file,option=None):
        self.file = file
        self.option = option
    def toU8Dot(self,fp):
        name = os.path.basename(self.file).replace('.','_')
        aname = os.path.abspath(self.file)
        fp.write('static const uint8 %s_dots[] = \n{'%(name))
        IM = QImage(self.file)
        fp.write('\n\t/* size(%s,%s) */'%(IM.size().width(),IM.size().height()))
        for y in range(0,IM.size().height()):
            fp.write('\n\t')
            DOT = B = 0
            for x in range(0,IM.size().width()):
                rgb = IM.pixel(x,y)
                r = (rgb>>16)&0xFF
                g = (rgb>>8)&0xFF
                b = (rgb>>0)&0xFF
                # only output the black one
                if(r<5 and g<5 and b<5):
                    DOT = DOT|(1<<B);    
                B += 1;
                if(B == 8):
                    fp.write('0x%-2X,'%(DOT))
                    DOT = 0;B = 0;
            if(B>0):
                fp.write('0x%-2s,'%(DOT))
        fp.write('\n};\n')
    def toU8Pixel(self,fp):
        name = os.path.basename(self.file).replace('.','_')
        aname = os.path.abspath(self.file)
        fp.write('static const uint8 %s_pixels[] = \n{'%(name))
        IM = QImage(self.file)
        for y in range(0,IM.size().height()):
            fp.write('\n\t')
            for x in range(0,IM.size().width()):
                rgb = IM.pixel(x,y)
                fp.write('%-3s,%-3s,%-3s,  '%((rgb>>16)&0xFF,(rgb>>8)&0xFF,(rgb>>0)&0xFF))
        fp.write('\n};\n')
        fp.write('static const SGIMPImage %s_image=\n'%(name))
        fp.write('{  /* %s */\n'%(aname))
        fp.write('\t.dwWidth=%s,\n'%(IM.size().width()))
        fp.write('\t.dwHeight=%s,\n'%(IM.size().height()))
        fp.write('\t.dwBytes_per_pixel=3,\n')
        fp.write('\t.pucPixel_data=%s_pixels\n};\n'%(name))

def GetSgImage(IML=[],fp=None):
    for image in IML:
        if(fp != None):
            Sg(image).toU8Pixel(fp)

def GetSgFont(IML=[],fp=None):
    for image in IML:
        if(fp != None):
            Sg(image).toU8Dot(fp)

if __name__ == '__main__':    
    fp = open('../../app/config/Lcd_Res/Lcd_Res.c','w')    
    IML = ['../../app/config/Lcd_Res/SpeedG.bmp',
           '../../app/config/Lcd_Res/TachoG.bmp']
    GetSgImage(IML,fp)

    IML = ['../../app/config/Lcd_Res/guo.bmp']
    GetSgFont(IML,fp)
    
    fp.close()
    
    print '>>> DONE <<<'