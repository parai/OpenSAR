#include "arvfb.h"
#include <math.h>
// ============================= [ LOCAL TYPES  ] =======================================
typedef struct
{
	int                       IsInit;
	TAILQ_HEAD(head,ArsObj_t) ObjList;
}ArScriptArchType;

typedef double (*fDouble)(double);

// ================================ [ LOCAL VARIANTS ] ==================================
static ArScriptArchType sArch = {.IsInit = FALSE,};

// ================================ [ LOCAL FUNCTIONS ] =================================
// change hex ascii to integer
static int htoi(char* string)
{
	int rv=0;
	++string;++string; // skip the prefix 0x or 0X
	while(*string != '\0')
	{
		if(isdigit(*string))
		{
			rv = rv*16 + (*string) - '0';
		}
		else if(isalnum(*string))
		{
			if(isupper(*string))
			{
				rv = rv*16 + (*string) - 'A' + 10;
			}
			else
			{
				rv = rv*16 + (*string) - 'a' + 10;
			}
		}
		else
		{
			assert(0);
		}

		++string;

	}

	return rv;
}

static ArsObjType* AddObject(char* Name,const ArsValueType* Value)
{
	ArsObjType* obj = malloc(sizeof(ArsObjType));

	obj->Name = strdup(Name);
	switch(Value->Type)
	{
		case ARS_STRING:
			obj->Value.Var.String = strdup(Value->Var.String);
			free(Value->Var.String);
			break;
		case ARS_INTEGER:
			obj->Value.Var.Integer = Value->Var.Integer;
			break;
		case ARS_DOUBLE:
			obj->Value.Var.Double = Value->Var.Double;
			break;
		case ARS_FUNCTION:
			obj->Value.Var.Function = Value->Var.Function;
			break;
		case ARS_UNDEFINE:
			// Type is currently not known, process further
			break;
		default:
			assert(0);
			break;
	}
	obj->Value.Type = Value->Type;
	TAILQ_INSERT_TAIL(&sArch.ObjList,obj,Entry);

	return obj;
}

static  void  SetObjectValue(ArsObjType* obj,const ArsValueType* Value)
{
	obj->Value.Type = Value->Type;
	switch(obj->Value.Type = Value->Type)
	{
		case ARS_STRING:
			obj->Value.Var.String = strdup(Value->Var.String);
			free(Value->Var.String);
			break;
		case ARS_INTEGER:
			obj->Value.Var.Integer = Value->Var.Integer;
			break;
		case ARS_DOUBLE:
			obj->Value.Var.Double = Value->Var.Double;
			break;
		case ARS_FUNCTION:
			obj->Value.Var.Function = Value->Var.Function;
			break;
		default:
			assert(0);
			break;
	}
}

static ArsObjType* GetObject(char* Name)
{
	ArsObjType* obj;
	TAILQ_FOREACH(obj,&sArch.ObjList,Entry)
	{
		if(0==strcmp(obj->Name,Name))
		{
			return obj;
		}
		else
		{
			continue;
		}
	}
	return NULL;
}


static void Calc_Add(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
{
	if( (ARS_INTEGER == in1->Type) &&
		(ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = in1->Var.Integer + in2->Var.Integer;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			 (ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double + in2->Var.Integer;
	}
	else if( (ARS_INTEGER == in1->Type) &&
			 (ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Integer + in2->Var.Double;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			(ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double + in2->Var.Double;
	}
}

static void Calc_Sub(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
{
	if( (ARS_INTEGER == in1->Type) &&
		(ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = in1->Var.Integer - in2->Var.Integer;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			 (ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double - in2->Var.Integer;
	}
	else if( (ARS_INTEGER == in1->Type) &&
			 (ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Integer - in2->Var.Double;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			(ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double - in2->Var.Double;
	}
}

static void Calc_Plus(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
{
	if( (ARS_INTEGER == in1->Type) &&
		(ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = in1->Var.Integer * in2->Var.Integer;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			 (ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double * in2->Var.Integer;
	}
	else if( (ARS_INTEGER == in1->Type) &&
			 (ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Integer * in2->Var.Double;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			(ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double * in2->Var.Double;
	}
}

static void Calc_Div(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
{
	if( (ARS_INTEGER == in1->Type) &&
		(ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = in1->Var.Integer / in2->Var.Integer;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			 (ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double / in2->Var.Integer;
	}
	else if( (ARS_INTEGER == in1->Type) &&
			 (ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Integer / in2->Var.Double;
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			(ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = in1->Var.Double / in2->Var.Double;
	}
}

static void Calc_Neg(ArsValueType* out,const ArsValueType* in1)
{
	if( (ARS_INTEGER == in1->Type))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = - in1->Var.Integer;
	}
	else if( (ARS_DOUBLE == in1->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = - in1->Var.Double;
	}
}

static void Calc_Pow(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
{
	if( (ARS_INTEGER == in1->Type) &&
		(ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = pow((double)in1->Var.Integer, (double)in2->Var.Integer);
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			 (ARS_INTEGER == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = pow(in1->Var.Double, (double)in2->Var.Integer);
	}
	else if( (ARS_INTEGER == in1->Type) &&
			 (ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = pow((double)in1->Var.Integer, in2->Var.Double);
	}
	else if( (ARS_DOUBLE == in1->Type) &&
			(ARS_DOUBLE == in2->Type))
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = pow(in1->Var.Double, in2->Var.Double);
	}
}

static void Execute(ArsValueType* out,const ArsValueType* fnc,const ArsValueType* param)
{
	assert(ARS_STRING==fnc->Type);
	ArsObjType* obj = GetObject(fnc->Var.String);
	if( (NULL == obj))
	{
		out->Type = ARS_INTEGER;
		out->Var.Integer = -1;	// indicate error
	}
	else
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = ((fDouble)obj->Value.Var.Function)((double)(param->Type==ARS_INTEGER ?
					param->Var.Integer:param->Var.Double));
	}
}
// ========================== [ IMPORTS ] ====================================
#define YYERROR_VERBOSE
#define DEBUG_FLEX(...)  //printf(__VA_ARGS__)
#define DEBUG_BISON(...) //printf(__VA_ARGS__)
int  yylex    (void);
void yyerror  (char const *);
#include "script.yc"
#include "script.fc"

// ================================ [ FUNCTIONS ] ========================================
void yyerror  (char const * errMsg)
{
	assert(errMsg!=NULL);
	printf("Script: %s\n",errMsg);
}

int yywrap (void )
{
	return 0;
}

void ArScript_Init(void)
{
	if(FALSE == sArch.IsInit)
	{
		sArch.IsInit = TRUE;
	}
	else
	{	// re-init
		// free memory firstly
		ArsObjType* obj;
		TAILQ_FOREACH(obj,&sArch.ObjList,Entry)
		{
			if(ARS_STRING == obj->Value.Type)
			{
				free(obj->Value.Var.String);
			}
			free(obj->Name);
			TAILQ_REMOVE(&sArch.ObjList,obj,Entry);
			free(obj);
		}
	}

	TAILQ_INIT(& sArch.ObjList);

	ArsValueType arsValue;

	arsValue.Type = ARS_FUNCTION;

	arsValue.Var.Function = (ArsFpType)sin;
	AddObject("sin",	&arsValue);
	arsValue.Var.Function = (ArsFpType)cos;
	AddObject("cos",	&arsValue);
	arsValue.Var.Function = (ArsFpType)atan;
	AddObject("atan",	&arsValue);
	arsValue.Var.Function = (ArsFpType)exp;
	AddObject("exp",	&arsValue);
	arsValue.Var.Function = (ArsFpType)log;
	AddObject("ln",		&arsValue);
	arsValue.Var.Function = (ArsFpType)sqrt;
	AddObject("sqrt",	&arsValue);
}
void ArScript(int argc, char* argv[])
{
	--argc;++argv;	// skip the Program name

	puts("Test:Calc\n");
	yyparse();

}



