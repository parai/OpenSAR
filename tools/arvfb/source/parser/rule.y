/* require bison 3.0.2 */

%{
  #include <stdio.h>  /* For DEBUG_BISON, etc. */
  #include <math.h>   /* For pow, used in the grammar.  */
  #include "arparser.h"
%}


%define api.value.type union  
%token <double>      yDouble
%token <int>         yInteger
%token <ArpVarType*> yExit
%token <ArpVarType*> yVar
%token <ArpVarType*> yPdu ySignal
%type  <double>  exp


%precedence '='
%left '-' '+'
%left '*' '/'
%precedence NEG /* negation--unary minus */
%right '^'      /* exponentiation */

/* Generate the parser description file.  */
%verbose
/* Enable run-time traces (yydebug).  */
%define parse.trace

/* Formatting semantic values.  */
%printer { fprintf (yyoutput, "%s", $$->Var.String); } yExit;
%printer { fprintf (yyoutput, "%s", $$->Var.String); } yVar;
%printer { fprintf (yyoutput, "%s", $$->Var.String); } yPdu;
%printer { fprintf (yyoutput, "%s", $$->Var.String); } ySignal;
%printer { fprintf (yyoutput, "%g", $$); } <double>;

%% /* The grammar follows.  */

input:
  %empty
| input line
;



line:
  '\n'
| exp '\n'
| error '\n' { puts("##:"); yyerrok;   }
;



exp:
  	/*  Name Identifier BusID  IsTxEnabled Period */
	yPdu yVar yInteger yInteger yInteger yInteger         { 
		DEBUG_BISON("Pdu:Name=%s,Identifier=0x%x,BusId=%d,IsTxEnabled=%s,Period=%d\n",
				$2->Var.String,$3,$4,$5?"TRUE":"FALSE",$6);
		ArCom_DefinePdu($2->Var.String,$3,$4,$5,$6);          
	}
	/*      Name StartBit BitSize DefaultValue */
|	ySignal yVar yInteger yInteger yInteger      { 
		DEBUG_BISON("Signal:Name=%s,StartBit=%d,BitSize=%d,DefaultValue=%d\n",
				$2->Var.String,$3,$4,$5);
		ArCom_DefineSignal($2->Var.String,$3,$4,$5);          
	}
|	yExit	{
		return 0;
	}
;

/* End of grammar.  */
%%