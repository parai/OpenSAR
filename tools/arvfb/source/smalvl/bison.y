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
%{
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "runtime.h"
#include  "analizator.h"
#include <typeinfo>

void yyerror(const char *str)
{
	fprintf(stderr,"Parse error\tline: %d, %s (yytext=%s)\n", yylineno, str, yytext);
	Arch_Trace("Parse error\tline: %d, %s (yytext=%s)\n", yylineno, str, yytext);
}
 
%}

%token FUNCTION ARRAY_DECLARATION
%token IDENTIFIER TK_INTEGER TK_FLOATPOINT TK_STRING 
%token LIB_NAME TK_TYPE
%token MORE_OR_EQUAL LESS_OR_EQUAL NOT_EQUAL EQUAL
%token RETURN INCLUDE
%token IF ELSE
%token TRUE FALSE
%token INC DEC
%token WHILE FOR
%token BREAK
%token UNSET

%right '+'
%right '-'
%right '*'
%right '/'
%right '.'

%expect 11

%type<str> IDENTIFIER  
%type<str> TK_INTEGER TK_FLOATPOINT
%type<str> TK_STRING  
%type<str> LIB_NAME TK_TYPE
%type<str> MORE_OR_EQUAL LESS_OR_EQUAL NOT_EQUAL EQUAL
%type<str> TRUE FALSE
%type<str> DEC INC

%type<oper> return_value
%type<oper> break
%type<oper> loop_for
%type<oper> loop_while
%type<oper> if_stmt
%type<oper> else_stmt
%type<oper> function_declaration
%type<oper> block
%type<oper> condition_statement
%type<oper> command
%type<oper> instructions
%type<oper> instruction
%type<oper> top_level_cmd
%type<oper> body
%type<oper> assign_value
%type<oper> unset
%type<oper> require
%type<args> declare_list
%type<expr> declare
%type<args> arg_type_list

%type<expr> value
%type<expr> string_expr
%type<expr> math_hight_expr
%type<expr> math_expr
%type<expr> expresion
%type<expr> function_call
%type<expr> array
%type<expr> unar_op
%type<expr> explicit_value
%type<expr> var
%type<expr> type

%type<args> function_call_arguments
%type<args> function_declaration_arguments
%%
/* Top level rules */
program: body
  {
   runtime_t::get_instance()->set_main_block((block_t*) $1);
   runtime_t::get_instance()->semantic_analys();
   //runtime_t::get_instance()->interpretate();
  }
  ;

body:
  top_level_cmd
  |
  body top_level_cmd {    
      $$ = new block_t($1, $2);
  }
  ;

/* Avalilable statements in source body*/
top_level_cmd: 
  function_declaration
  |
  require
  |
  instructions { $$ = $1; }
  ;
 
instructions:
  instructions instruction { $$ = new block_t($1, $2); }
  |
  instruction { }
  ;
  
instruction:
   command ';' { $$ = $1; }
   |
   condition_statement { $$ = $1; }
   ;
	
command:
  unar_op {
    $$ =  (oper_t*)($1); 
  }
  |
  function_call {
    /* Function call is expr_t and oper_t child */
      $$ =  (oper_t*)($1); 
    }
  | 
  assign_value
  |
  return_value
  |
  unset
  |
  break
  |
  error { yyerrok; yyclearin;  std::cerr <<"Error in command running statement\n"; }
  ;

condition_statement:
  loop_for
  |
  loop_while
  |
  if_stmt
  ;

block: 
  '{' instructions '}' { $$ = new block_t($2); };

function_declaration:
  FUNCTION IDENTIFIER '(' function_declaration_arguments ')' block
  {
    function_declaration_t* fd = new function_declaration_t($2, $4, $6);
    $$ = fd;
    runtime_t::get_instance()->add_function_declaration(fd);
  }
  |
  error block{
    yyerrok;
    yyclearin;
    std::cerr << "Function declaration error!\n";
  };

function_declaration_arguments: /* empty */ 
  %empty	{}
  |
  var ',' function_declaration_arguments {
    $$ = std::list<expr_t*>($3);
    $$.push_back($1);
  }
  | 
  var {  
     $$ = std::list<expr_t*>();
     $$.push_back($1);
  }
  ;
  
function_call_arguments: /* empty */
  %empty	{}
  |
  value ',' function_call_arguments {
    $$ = std::list<expr_t*>($3);
    $$.push_back($1);
  }
  | value
  {  
     $$ = std::list<expr_t*>();
     $$.push_back($1);
  }
  ;  

value:
  expresion { $$ = $1; }
  ;
/* End of top level rules */

explicit_value:
  TRUE { $$ = new value_t($1,BOOL); }
  |
  FALSE { $$ = new value_t($1,BOOL); }
  |
  TK_INTEGER { $$ = new value_t($1,INTEGER); }
  |
  TK_FLOATPOINT { $$ = new value_t($1,FLOATPOINT); }
  |
  TK_STRING { $$ = new value_t($1,STRING); }
  |
  var 
  |
  function_call
  ;
      
break:
  BREAK { $$ = new break_op_t(); }
  ;
  
unar_op:
  var INC { $$ = new unary_t("++", $1); }
  |
  var DEC { $$ = new unary_t("--", $1); }
  |
  DEC var { $$ = new unary_t("--", $2); }
  |
  INC var { $$ = new unary_t("++", $2); };

array:
  ARRAY_DECLARATION { $$ = new array_init_t(); }
  ;

var:
  '$' IDENTIFIER { $$ = new var_t($2); }
  |
  '$' IDENTIFIER '[' value ']' { 
    $$ = new array_t($2, $4); 
  };
  
unset: 
  UNSET var { $$ = new unset_t($2); };
  
function_call:
  IDENTIFIER '(' function_call_arguments ')'  {
    $$ = new function_call_t($1, $3);
  } ;

assign_value:
  var '=' value { $$ = new assign_t($1, $3); }
  | 
  var '=' array { $$ = new assign_t($1, $3); }
  ;
 
return_value:
  RETURN value { $$ = new return_op_t($2); };

loop_for:
  FOR '(' command ';' expresion ';' command ')' block
  {
    $$ = new for_op_t($3, $5, $7, $9);
  };

loop_while:
  WHILE '(' value ')' block {
    $$ = new while_op_t($3, $5); 
  };

if_stmt:
  IF '(' value ')' block else_stmt { $$ = new if_op_t($3, $5, $6); };

else_stmt:
  %empty	{ $$ = NULL; }
  |
  ELSE block { $$ = new block_t($2); };
  |
  ELSE IF '(' value ')' block else_stmt { $$ = new if_op_t($4, $6, $7); };
  
expresion:
  math_expr { $$ = $1; }
  |
  expresion '<' math_expr { $$ = new binary_t("<", $1, $3); }
  |
  expresion '>' math_expr { $$ = new binary_t(">", $1, $3); }
  |
  expresion EQUAL math_expr { $$ = new binary_t("==", $1, $3); }
  |
  expresion MORE_OR_EQUAL math_expr { $$ = new binary_t(">=", $1, $3); }
  |
  expresion LESS_OR_EQUAL math_expr { $$ = new binary_t("<=", $1, $3); }
  |
  expresion NOT_EQUAL math_expr { $$ = new binary_t("!=", $1, $3); }
  |
  '(' expresion ')' { $$ = $2; }
  |
  error  { std::cerr << "Expresion error\n"; }
  ;
 
/* Math operation implementation with priority */
math_expr: 
  math_hight_expr 
  |
  math_expr '+' math_hight_expr { $$ = new binary_t("+", $1, $3); }
  |
  math_expr '-' math_hight_expr { $$ = new binary_t("-", $1, $3); }
  ;
  
math_hight_expr:
  string_expr
  |
  math_hight_expr '*' string_expr { $$ = new binary_t("*", $1, $3); }
  |
  math_hight_expr '/' string_expr { $$ = new binary_t("/", $1, $3); }
  |
  math_hight_expr '%' string_expr { $$ = new binary_t("%", $1, $3); }
  ;


string_expr:	
  explicit_value
  |
  string_expr '.' explicit_value { $$ = new binary_t(".", $1, $3); /* Yes, I change it, to make it more look like class string */}
  ;
 
require:
	LIB_NAME '{' declare_list '}' ';'	{ $$ = new require_t($1,$3);}
	;

declare_list:
	declare	{
		$$ = std::list<expr_t*>();
		$$.push_back($1);
	}
	|
	declare declare_list {
		$$ = std::list<expr_t*>($2);
		$$.push_back($1);
	}
	;

declare:
	type IDENTIFIER '(' arg_type_list ')' ';'	{ $$ = new declare_t($1,$2,$4);}
	;

arg_type_list:
	%empty	{}
	|
	type {  
		$$ = std::list<expr_t*>();
		$$.push_back($1);
	}
	|
	type ',' arg_type_list {
		$$ = std::list<expr_t*>($3);
		$$.push_back($1);
	}
	;

type:
	TK_TYPE	{ $$ = new type_t($1) ; }


