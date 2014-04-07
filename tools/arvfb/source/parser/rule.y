/* require bison 3.0.2 */

%{
  #include <stdio.h>  /* For DEBUG_BISON, etc. */
  #include <math.h>   /* For pow, used in the grammar.  */
  #include "arparser.h"
%}


%define api.value.type union  
%token <double> 	yDouble
%token <int>      	yInteger
%token <int> 		yExit
%token <char*> 		yString
%token <int> 		yPdu ySignal	/* value is not cared */
%type  <double>  	exp


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
%printer { fprintf (yyoutput, "%s", $$); } <char*>;
%printer { fprintf (yyoutput, "%d", $$); } <int>;
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
	yPdu yString yInteger yInteger yInteger yInteger         { 
		DEBUG_BISON("Pdu:Name=%s,Identifier=0x%x,BusId=%d,IsTxEnabled=%s,Period=%d\n",
				$2,$3,$4,$5?"TRUE":"FALSE",$6);
		ArCom_DefinePdu($2,$3,$4,$5,$6); 
		free($2);  // in couple with strdup
	}
	/*      Name StartBit BitSize DefaultValue */
|	ySignal yString yInteger yInteger yInteger      { 
		DEBUG_BISON("Signal:Name=%s,StartBit=%d,BitSize=%d,DefaultValue=%d\n",
				$2,$3,$4,$5);
		ArCom_DefineSignal($2,$3,$4,$5);
		free($2);
	}
|	yExit	{
		return 0;
	}
;

/* End of grammar.  */
%%