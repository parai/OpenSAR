
__all__=['ArGen']
from Gen.GenOS import GenOS

def ArGen(arxml,dir):
    print('>>> Generate %s...<<<'%(arxml.tag))
    if(arxml.tag == 'OS'):
        GenOS(arxml,dir)