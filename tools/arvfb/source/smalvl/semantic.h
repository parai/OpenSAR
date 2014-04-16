#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <list>
#include <string>
#include <typeinfo>
#include <cstdio>
#include <cstdarg>
#include <iostream>

#include "base_instructions.h"
#include "expressions.h"
#include "operators.h"

typedef struct _YYSTYPE
{
	std::string str;
	oper_t* oper;
	expr_t* expr;
std::list<expr_t*> args;
} YYSTYPE;

#define YYSTYPE _YYSTYPE

#endif
