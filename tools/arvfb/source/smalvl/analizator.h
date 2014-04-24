/* Copyright(C) 2013, OpenSAR by Fan Wang(parai). All rights reserved.
 *
 * This file is part of OpenSAR.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Source Open At: https://github.com/parai/OpenSAR/
 */
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
