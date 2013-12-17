import sys,os
import xml.etree.ElementTree as ET
from GenCanIf import GenCanIf
from GenCanTp import GenCanTp

__all__ = ['GenCom']

def GenCom(wfxml):
    GenCanIf(wfxml)
    GenCanTp(wfxml)

