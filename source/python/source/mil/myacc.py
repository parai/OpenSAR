# MicroSAR Implement Language Yacc
from ply import *
import mlex

tokens = mlex.tokens

precedence = (
               ('left', 'PLUS','MINUS'),
               ('left', 'TIMES','DIVIDE'),
               ('left', 'POWER'),
               #('right','UMINUS')
)

def obj2str(o):
    cstr = '%s %s {\n'%(o[0],o[1])
    for attr,value in o[2].items():
        cstr += '\t%s = %s;\n'%(attr,value)
    cstr += '};\n\n'
    return cstr

#### A MIL file is a series of objects. 

def p_mil(p):
    '''mil : mil object
               | object'''
    if len(p) == 2 and p[1]:
       p[0] = []
       p[0].append(p[1])
    elif len(p) ==3:
       p[0] = p[1]
       if not p[0]: p[0] = []
       if p[2]:
           p[0].append(p[1])

#### Format of all MIL objects. 

def p_object(p):
    '''object :  Os
            |    Task
            |    Alarm
            |    Event'''
    p[0] = p[1]

def __AttribList(p):
    if len(p) == 2 and p[1]:
        p[0] = { }
        (attr,oper,value) = p[1]
        p[0][attr] = value
    elif len(p) ==3:
        p[0] = p[1]
        if not p[0]: p[0] = { }
        if p[2]:
            (attr,oper,value) = p[2]
            p[0][attr] = value
def p_Os(p):
    '''Os : OS ID LBRACE OsAttribList RBRACE SEMI '''
    p[0] = ('OS',p[2],p[4])
    print obj2str(p[0])
def p_OsAttribList(p):
    '''OsAttribList : OsAttribList OsAttrib
                    | OsAttrib '''
    __AttribList(p)
    
def p_OsAttrib(p):
    '''OsAttrib :  STATUS EQUALS STANDARD SEMI
                | STATUS EQUALS EXTENDED SEMI
                | ERRORHOOK EQUALS TRUE SEMI
                | ERRORHOOK EQUALS FALSE SEMI
                | PRETASKHOOK EQUALS TRUE SEMI
                | PRETASKHOOK EQUALS FALSE SEMI
                | POSTTASKHOOK EQUALS TRUE SEMI
                | POSTTASKHOOK EQUALS FALSE SEMI
                | SHUTDOWNHOOK EQUALS TRUE SEMI
                | SHUTDOWNHOOK EQUALS FALSE SEMI '''
    p[0] = (p[1],p[2],p[3])
def p_Task(p):
    '''Task : TASK ID LBRACE TaskAttribList RBRACE SEMI '''
    p[0] = ('TASK',p[2],p[4])
    print obj2str(p[0])
    
def p_TaskAttribList(p):
    '''TaskAttribList : TaskAttribList TaskAttrib
                    | TaskAttrib '''
    __AttribList(p)
def p_TaskAttrib(p):
    '''TaskAttrib :  SCHEDULE EQUALS NON SEMI
                | SCHEDULE EQUALS FULL SEMI
                | SCHEDULE EQUALS MIXED SEMI
                | PRIORITY EQUALS INTEGER SEMI
                | ACTIVATION EQUALS INTEGER SEMI
                | AUTOSTART EQUALS FALSE SEMI
                | AUTOSTART EQUALS TRUE LBRACE AppModeList RBRACE SEMI '''
    if len(p) == 5:
        p[0] = (p[1],p[2],p[3])
    elif len(p) == 8:
        p[0] = (p[1],p[2],(p[3],p[5]))

def p_AppModeList(p):
    ''' AppModeList : AppModeList AppMode
                    | AppMode'''
    if len(p) == 2:
       p[0] = '%s'%(p[1])
    elif len(p) ==3:
       p[0] = '%s | %s'%(p[1],p[2])
def p_AppMode(p):
    ''' AppMode : APPMODE EQUALS ID SEMI'''
    p[0] = p[3]
    
def p_Alarm(p):
    '''Alarm : ALARM ID LBRACE AlarmAttribList RBRACE SEMI '''
    p[0] = ('ALARM',p[2],p[4])
    print obj2str(p[0])

def p_AlarmAttribList(p):
    '''AlarmAttribList : AlarmAttribList AlarmAttrib
                    | AlarmAttrib '''
    __AttribList(p)
def p_AlarmAttrib(p):
    '''AlarmAttrib :  ACTION EQUALS ACTIVATETASK LBRACE TASK EQUALS ID SEMI RBRACE SEMI
                | ACTION EQUALS ALARMCALLBACK LBRACE ALARMCALLBACKNAME EQUALS ID SEMI RBRACE SEMI
                | ACTION EQUALS SETEVENT LBRACE TASK EQUALS ID SEMI EVENT EQUALS ID SEMI RBRACE SEMI
                | ACTION EQUALS SETEVENT LBRACE TASK EQUALS ID SEMI EVENT EQUALS INTEGER SEMI RBRACE SEMI
                | ACTION EQUALS SETEVENT LBRACE EVENT EQUALS ID SEMI TASK EQUALS ID SEMI RBRACE SEMI
                | ACTION EQUALS SETEVENT LBRACE EVENT EQUALS INTEGER SEMI TASK EQUALS ID SEMI RBRACE SEMI
                | AUTOSTART EQUALS FALSE SEMI
                | AUTOSTART EQUALS TRUE LBRACE AppModeList RBRACE SEMI '''
    if len(p) == 5:
        p[0] = (p[1],p[2],p[3])
    elif len(p) == 11:
        p[0] = (p[1],p[2],(p[3],'%s=%s'%(p[5],p[7])))
def p_Event(p):
    '''Event : EVENT ID LBRACE AttribList RBRACE SEMI '''
    pass  

def p_AttribList(p):
    '''AttribList : AttribList Attrib
                    | Attrib '''
    pass
    
def p_Attrib(p):
    '''Attrib :  STATUS EQUALS STANDARD SEMI
                | ERRORHOOK EQUALS FALSE SEMI
                | PRETASKHOOK EQUALS FALSE SEMI
                | POSTTASKHOOK EQUALS FALSE SEMI
                | SCHEDULE EQUALS NON SEMI
                    '''
    print p[1:]   
        
#### Empty
   
def p_empty(p):
    '''empty : '''

#### Catastrophic error handler
def p_error(p):
    if not p:
        print("SYNTAX ERROR AT EOF")
    else:
        try:
            print("SYNTAX ERROR AT LINE(%s) %s"%(p.lineno,p))
        except:
            print("SYNTAX ERROR AT LINE(%s) %s"%(int(p),p))

bparser = yacc.yacc()

def parse(data,debug=0):
    bparser.error = 0
    p = bparser.parse(data,debug=debug)
    if bparser.error: return None
    return p    