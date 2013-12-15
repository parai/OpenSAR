import sys,os
import xml.etree.ElementTree as ET
from GenCanIf import GenCanIf

__all__ = ['GenCom']

def GenCom(wfxml):
    GenCanIf(wfxml)

