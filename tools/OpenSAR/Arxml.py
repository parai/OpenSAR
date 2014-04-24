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
 * Source Open At: https://github.com/parai/OpenSAR/
 */
"""
import xml.etree.ElementTree as ET
import re

__all__ = ['Arxml','IsArxmlList','ArxmlGetURL']

def IsArxmlList(arxml):
    if(isinstance(arxml, Arxml)):
        try:
            max = arxml.descriptor.attrib['Max']
            return True
        except:
            return False
    elif(isinstance(arxml, ET.Element)):
        try:
            max = arxml.attrib['Max']
            return True
        except:
            return False
    else:
        print 'Arxml: IsArxmlList(%s) type Error'%(arxml)
    
class Arxml():
    """
        For easy understanding and implementation, standard AUTOSAR Arxml rule is not adopted.
    More information about standard AUTOSAR Arxml, please see <AUTOSAR_ModelPersistenceRulesforXML.pdf>
    
        The rule of this version Arxml is defined by parai from the view of application and parai's 
    understanding of AUTOSAR.
    """
    version = 0.01 # initialize 
    isNewWithConfig = False
    def __init__(self,descriptor,configuration=None):
        # save input parameter
        assert(isinstance(descriptor, ET.Element))
        self.descriptor = descriptor
        self.tag  = self.descriptor.tag
        # Load configuration
        if(configuration != None):
            self.configuration = configuration
            self.__checkConfiguration()
            isNewWithConfig = True
            #print('Arxml: new %s([%s])with validate configuration'%(self.tag,configuration.attrib))
        # New configuration according to descriptor
        else:
            self.__newConfiguration()
    def toArxml(self):
        """return a copy of configuration"""
        arxml = ET.Element(self.descriptor.tag)
        for [key,type] in self.descriptor.items():
            arxml.attrib[key] = self.attrib(key)
        return arxml
    def __checkConfiguration(self):
        reDeafult = re.compile(r'\s+Default=([^\s=\(\)]+)')
        for [key,type] in self.descriptor.items():
            try:
                var = self.configuration.attrib[key]
            except:
                # no found attribute, maybe descriptor has been updated
                var = 'TBD'
                if(reDeafult.search(type)):
                    var = reDeafult.search(type).groups()[0]
                self.configuration.attrib[key] = var
                
    def __newConfiguration(self): # private
        reDeafult = re.compile(r'\s+Default=([^\s=\(\)]+)')
        self.configuration = ET.Element(self.descriptor.tag)
        for [key,type] in self.descriptor.items():
            var = 'TBD'
            if(reDeafult.search(type)):
                var = reDeafult.search(type).groups()[0]
            self.configuration.attrib[key] = var
    
    def getMaxChildAllowed(self):
        assert(IsArxmlList(self))
        
        return int(self.descriptor.attrib['Max'],10)
    
    def getKeyDescriptor(self,key):
        for [key1,type1] in self.descriptor.items():
            if(key == key1):
                return type1

    def attrib(self,key,value=None):
        """ if value is none, return attrib, else set attrib"""
        isValidKey = False
        for [key1,type1] in self.descriptor.items():
            if(key == key1):
                isValidKey = True
                break
        if(isValidKey):
            if(value == None):
                return self.configuration.attrib[key]
            else:
                self.configuration.attrib[key] = str(value)
        else:
            if(self.tag=='General'):
                pass # General Info, no sub, no name attribute
            elif(IsArxmlList(self)==False):
                print 'Arxml: Error (key,value)=(%s,%s) for %s'%(key,value,self.descriptor.tag)
            if(value==None):
                return ''
    
    def __str__(self):
        wfxml = 'tracelog.arxml'
        ROOT = ET.Element('ROOT')
        
        ROOT.append(self.configuration)
        tree = ET.ElementTree(ROOT)
        tree.write(wfxml, encoding="utf-8", xml_declaration=True);
        fp = open(wfxml,'r')
        cstr = ''
        for el in fp.readlines():
            cstr += el
        fp.close()
        return cstr

    def childArxmls(self):
        arxmls = []
        for descriptor in self.descriptor:
            tcfg  = None
            for configuration in self.configuration:
                if(configuration.tag == descriptor.tag):
                    tcfg = configuration
                    break
            arxmls.append(Arxml(descriptor,tcfg))
        return arxmls

    def childArxmls2(self):
        # only validate configurations
        arxmls = []
        for descriptor in self.descriptor:
            tcfg  = None
            for configuration in self.configuration:
                if(configuration.tag == descriptor.tag):
                    tcfg = configuration
                    arxmls.append(Arxml(descriptor,tcfg))
        return arxmls 
    
    def childDescriptors(self):
        Descriptors = []
        for descriptor in self.descriptor:
            Descriptors.append(descriptor)
        return Descriptors
    
def ArxmlDynURL(ArxmlRoot,url):
    reTarget=re.compile(r'\[([^\s]+)\]')
    refL = url.split('->')
    arxml = None
    for L in refL:
        if(refL.index(L)==0):
            # Get Module
            arxml = ArxmlRoot.find(L)
            if(arxml==None):
                break
        elif(reTarget.search(L)):
            for target in arxml:
                url2   = url.replace('->','.').replace(L,target.attrib['Name'])
                if(ArxmlGetURL(ArxmlRoot,url2) != None):
                    return target.attrib[reTarget.search(L).groups()[0].split('-')[1]]
        else:
            for arx in arxml:
                if(IsArxmlList(arx)):
                    if(arx.tag == L):
                        arxml = arx
                        break
                    else:
                        continue
                elif(arx.tag=='General'):
                    continue
                elif(arx.attrib['Name'] == L):
                    arxml = arx
                    break
    return None  

def ArxmlGetURL(ArxmlRoot,url):
    reDyn=re.compile(r'\(([^\s]+)\)')
    refL = url.split('.')
    arxml = None
    for L in refL:
        if(refL.index(L)==0):
            # Get Module
            arxml = ArxmlRoot.find(L)
            if(arxml==None):
                break
#         elif(reDyn.search(L)):
#             # USED: CanIf->ChannelList->[Channel-ControllerRef]->HrhList->(Self.Name)
#             L = ArxmlDynURL(ArxmlRoot,reDyn.search(L).groups()[0])
#             if(L==None):
#                 break
        # normal loop search
        for arx in arxml:
            if(IsArxmlList(arx) or IsArxmlList(arxml)==False):
                if(arx.tag == L):
                    arxml = arx
                    break
                else:
                    continue
            elif(arx.tag=='General'):
                continue
            elif(arx.attrib['Name'] == L):
                arxml = arx
                break
    if(arxml!=None and arxml.tag==L):
        return arxml
    else:
        try:
            if(arxml!=None and arxml.attrib['Name']==L):
                return arxml
        except:
            return None