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
    print p

#### Format of all MIL objects. 

def p_object(p):
    '''object :  Os
            |    Task
            |    Alarm
            |    Event
        '''
    print p
def p_Os(p):
    '''Os : OS ID LBRACE DeclareList RBRACE SEMI '''
    print p
def p_Task(p):
    '''Task : TASK ID LBRACE DeclareList RBRACE SEMI '''
    print p[1:]
def p_Alarm(p):
    '''Alarm : ALARM ID LBRACE DeclareList RBRACE SEMI '''
    print p[1:]
def p_Event(p):
    '''Event : EVENT ID LBRACE DeclareList RBRACE SEMI '''
    print p[1:]    

def p_DeclareList(p):
    '''DeclareList : DeclareList Declare
                    | Declare '''
    print p[1:]
    
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
        print("SYNTAX ERROR AT %s"%(p))

bparser = yacc.yacc()

def parse(data,debug=0):
    bparser.error = 0
    p = bparser.parse(data,debug=debug)
    if bparser.error: return None
    return p    