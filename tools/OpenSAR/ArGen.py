"""/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */
"""
import os

__all__=['ArGen']
from Gen.GenOS import GenOS
from Gen.GenCan import GenCan
from Gen.GenCanIf import GenCanIf
from Gen.GenPduR import GenPduR
from Gen.GenCanTp import GenCanTp
from Gen.GenCom import GenCom
def ArGen(arxml,dir):
    if(arxml.tag == 'OS'):
        GenOS(arxml,dir)
    elif(arxml.tag == 'Can'):
        GenCan(arxml,dir)
    elif(arxml.tag == 'CanIf'):
        GenCanIf(arxml,dir)
    elif(arxml.tag == 'PduR'):
        GenPduR(arxml,dir)
    elif(arxml.tag == 'CanTp'):
        GenCanTp(arxml,dir)
    elif(arxml.tag == 'Com'):
        GenCom(arxml,dir)
        
gDefault_GEN = '../../app/config/GEN'
if __name__ == '__main__':
    import xml.etree.ElementTree as ET
    wfxml = '%s/AutosarConfig.arxml'%(gDefault_GEN)
    if(os.path.exists(wfxml)):
        root = ET.parse(wfxml).getroot();
        for arxml in root:
            ArGen(arxml,gDefault_GEN)