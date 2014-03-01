
from gen.GenOS import GenOS
from gen.GenCom import GenCom
from gen.GenNvM import GenNvM

pdir = '../../app/config/GEN'

wfxml = '%s/os.wfxml'%(pdir)
GenOS(str(wfxml))

wfxml = '%s/com.wfxml'%(pdir)
GenCom(str(wfxml))

wfxml = '%s/nvm.wfxml'%(pdir)
GenNvM(str(wfxml))