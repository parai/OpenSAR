# MicroSAR Implement Language Lexer

from ply import *

keywords = (
    'OS',   'STATUS', 'ERRORHOOK', 'PRETASKHOOK', 'POSTTASKHOOK', 'SHUTDOWNHOOK',
    'TASK', 'SCHEDULE', 'PRIORITY', 'ACTIVATION', 'AUTOSTART', 'APPMODE',
    'ALARM', 'ACTION', 'ACTIVATETASK', 'SETEVENT', 'ALARMCALLBACK',
    'EVENT', 'MASK',
    'AUTO','TRUE','FALSE','STANDARD','EXTENDED',
    'NON','FULL','MIXED'
)

tokens = keywords + (
     'EQUALS','PLUS','MINUS','TIMES','DIVIDE','POWER',  # =, +, -, *, /, ^
     'LPAREN','RPAREN','LBRACE','RBRACE','LBK','RBK',   # (, ), {, }, [, ],
     'LT','LE','GT','GE','NE',                          # <, <=, >, >=, !=
     'COMMA','SEMI', 'INTEGER','FLOAT', 'STRING',       # :, ,, ;,
     'ID'
)

t_ignore = ' \t'

def t_COMMENT(t):
    r'// .*\n'

def t_ID(t):
    r'[_A-Za-z][_A-Za-z0-9]*'
    if t.value in keywords:
        t.type = t.value
    return t
    
t_EQUALS  = r'='
t_PLUS    = r'\+'
t_MINUS   = r'-'
t_TIMES   = r'\*'
t_POWER   = r'\^'
t_DIVIDE  = r'/'
t_LPAREN  = r'\('
t_RPAREN  = r'\)'
t_LBRACE  = r'\{'
t_RBRACE  = r'\}'
t_LBK     = r'\['   # bracket
t_RBK     = r'\]'
t_LT      = r'<'
t_LE      = r'<='
t_GT      = r'>'
t_GE      = r'>='
t_NE      = r'<>'
t_COMMA   = r'\,'
t_SEMI    = r';'
t_INTEGER = r'\d+'    
t_FLOAT   = r'((\d*\.\d+)(E[\+-]?\d+)?|([1-9]\d*E[\+-]?\d+))'
t_STRING  = r'\".*?\"'

def t_newline(t):
    r'\n'
    t.lexer.lineno += 1

def t_error(t):
    print("Illegal character %s" % t.value[0])
    t.lexer.skip(1)

lex.lex(debug=0)