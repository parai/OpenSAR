
import xml.etree.ElementTree as ET

__all__ = ['Arxml','IsArxmlList']

def IsArxmlList(arxml):
    assert(isinstance(arxml, Arxml))
    try:
        max = arxml.descriptor.attrib['Max']
        return True
    except:
        return False
    
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
    def __newConfiguration(self): # private
        self.configuration = ET.Element(self.descriptor.tag)
        for [key,type] in self.descriptor.items():
            self.configuration.attrib[key] = 'TBD'
    
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
            if(IsArxmlList(self)==False):
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