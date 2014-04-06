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
from Gen.GenDcm import GenDcm
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
    elif(arxml.tag == 'Dcm'):
        GenDcm(arxml,dir)
        

from Gen.GCF import *
from Arxml  import *

def ArvfbGetCanIf(arxml,pdu):
    url = 'CanIf.ChannelList'
    for chl in ArxmlGetURL(arxml,url):
        for hth in GLGet(chl,'HthList'):
            for pdu2 in GLGet(hth,'PduList'):
                if(GAGet(pdu2,'EcuCPduRef') == GAGet(pdu,'Name')):
                    return [pdu2,GAGet(chl,'ControllerRef')]
        for hrh in GLGet(chl,'HrhList'):
            for pdu2 in GLGet(hrh,'PduList'):
                if(GAGet(pdu2,'EcuCPduRef') == GAGet(pdu,'Name')):
                    return [pdu2,GAGet(chl,'ControllerRef')]
    return None

def ArvfbGetCom(arxml,pdu):
    url = 'Com.IPduList'
    for pdu2 in ArxmlGetURL(arxml,url):
        if(GAGet(pdu2,'PduRef') == GAGet(pdu,'Name')):
                return pdu2
    return None
def ArvfbGen(arxml,dir):
    fp = open('%s/arvfb.config'%(dir),'w')
    url = 'EcuC.PduList'
    for pdu in ArxmlGetURL(arxml,url):
        pduIf = ArvfbGetCanIf(arxml,pdu)
        pduCom = ArvfbGetCom(arxml,pdu)
        if(pduIf != None and pduCom!=None):
            Name = GAGet(pdu,'Name')
            Id   = GAGet(pduIf[0],'Identifier')
            BusId = pduIf[1][-1:]
            isTxE = (GAGet(pduCom,'Direction')=='RECEIVE')
            Period = GAGet(pduCom,'TimePeriodFactor')
            fp.write('\n//  %-16s %-12s %-4s %-8s %-8s\n'%('Name','Id','BusId','isTxE','Period'))
            fp.write('Pdu %-16s %-12s %-4s %-8s %-8s\n'%(Name,Id,BusId,isTxE,Period))
            fp.write('//     %-16s %-8s %-8s %-8s\n'%('Name','StartBit','Size','InitialValue'))
            for gsig in GLGet(pduCom,'GroupSignalList'):
                for sig in GLGet(gsig,'SignalList'):
                    fp.write('Signal %-16s %-8s %-8s %-8s\n'%(GAGet(sig,'Name'),GAGet(sig,'StartBit'),
                                                              GAGet(sig,'Size'),GAGet(sig,'InitialValue')))
            for sig in GLGet(pduCom,'SignalList'):
                fp.write('Signal %-16s %-8s %-8s %-8s\n'%(GAGet(sig,'Name'),GAGet(sig,'StartBit'),
                                                          GAGet(sig,'Size'),GAGet(sig,'InitialValue')))
    fp.write('\n\nexit\n\n')
    fp.close()
    print '>>> Gen Arvfb <<<'
           
gDefault_GEN = '../../app/config/GEN'
if __name__ == '__main__':
    import xml.etree.ElementTree as ET
    wfxml = '%s/AutosarConfig.arxml'%(gDefault_GEN)
    if(os.path.exists(wfxml)):
        root = ET.parse(wfxml).getroot();
        for arxml in root:
            ArGen(arxml,gDefault_GEN)
        ArvfbGen(root,gDefault_GEN)