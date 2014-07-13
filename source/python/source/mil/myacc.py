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

def p_Os(p):
    '''Os : OS ID LBRACE OsDeclareList RBRACE SEMI '''
    p[0] = ('OS',p[2],p[4])
    cstr = 'OS %s {\n'%(p[2])
    for attr,value in p[4].items():
        cstr += '\t%s = %s;\n'%(attr,value)
    cstr += '};\n\n'
    print cstr
def p_OsDeclareList(p):
    '''OsDeclareList : OsDeclareList OsDeclare
                    | OsDeclare '''
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
    
def p_OsDeclare(p):
    '''OsDeclare :  STATUS EQUALS STANDARD SEMI
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
    '''Task : TASK ID LBRACE DeclareList RBRACE SEMI '''
    pass
def p_Alarm(p):
    '''Alarm : ALARM ID LBRACE DeclareList RBRACE SEMI '''
    pass
def p_Event(p):
    '''Event : EVENT ID LBRACE DeclareList RBRACE SEMI '''
    pass  

def p_DeclareList(p):
    '''DeclareList : DeclareList Declare
                    | Declare '''
    pass
    
def p_Declare(p):
    '''Declare :  STATUS EQUALS STANDARD SEMI
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