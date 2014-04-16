#ifndef ANALIZATOR_H
#define ANALIZATOR_H

#include <cstdio>
#include <iostream>
#include "semantic.h"
#include "bison.tab.h"

extern int yyparse(void);
extern int yylex(void);
extern void yyerror(const char *str);

extern char *yytext;
extern YYSTYPE yylval;

extern int yy_flex_debug;
extern FILE *yyin, *yyout;

extern int yylineno;
extern int yydebug;

#endif
