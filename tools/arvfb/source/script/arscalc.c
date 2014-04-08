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
 * but WITHOUT ANY WARRANTY; withrvar even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email: parai@foxmail.com
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */

#include "arvfb.h"

// ==================================== [ MACROS    ] ==========================================
#define is_string(var)   			(YVAR_STRING ==(var)->type)
#define is_integer(var)  			(YVAR_INTEGER==(var)->type)
#define is_double(var)   			(YVAR_DOUBLE==(var)->type)
#define is_char(var)     			(YVAR_CHAR==(var)->type)
#define is_function(var)     		(YVAR_FUNCTION==(var)->type)

#define is_both_integer(var1,var2) ( is_integer(var1) && is_integer(var2) )
#define is_both_char(var1,var2)    ( is_char(var1) && is_char(var2) )

// ==================================== [ TYPES     ] ==========================================
typedef double (*feval1_t)(double);  // Eval which with 1 parameter

// ==================================== [ LOCAL FUNCTIONS ] ====================================
static double get_value(const yvar_t* from)
{
	double rv = -1;

	switch(from->type)
	{
		case YVAR_CHAR:
			rv = from->u.chr;
			break;
		case YVAR_INTEGER:
			rv = from->u.integer;
			break;
		case YVAR_DOUBLE:
			rv = from->u.dvar;
			break;
		default:
			assert(0);
			break;
	}
	return rv;
}

static boolean is_evaluable(const yvar_t* me)
{
	boolean rv = FALSE;

	switch(me->type)
	{
		case YVAR_CHAR:
		case YVAR_INTEGER:
		case YVAR_DOUBLE:
			rv = TRUE;
			break;
		default:
			break;
	}
	return rv;
}

static void arsc_error(void)
{	// TODO it better
	puts("arsc error: TODO.\n");
}
// ==================================== [ FUNCTIONS ] ==========================================
void arsc_copy(yvar_t* to,yvar_t* from)
{
	memcpy(to,from,sizeof(yvar_t));
}

void arsc_add(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2)
{
	if( (is_evaluable(var1)) &&
		(is_evaluable(var2)))
	{
		if(is_both_char(var1,var2))
		{
			rvar->type = YVAR_CHAR;
			rvar->u.chr = var1->u.chr + var2->u.chr;
		}
		else if(is_both_integer(var1,var2))
		{
			rvar->type = YVAR_INTEGER;
			rvar->u.integer = var1->u.integer + var2->u.integer;
		}
		else
		{
			rvar->type = YVAR_DOUBLE;
			rvar->u.dvar = get_value(var1) + get_value(var2);
		}
	}
	else if(is_string(var1))
	{
		char temp[64];
		int  length;
		rvar->type = YVAR_STRING;
		rvar->u.string = arso_strdup(var1->u.string);
		switch(var2->type)
		{
			case YVAR_STRING:
				rvar->u.string = realloc(rvar->u.string,
						strlen(rvar->u.string)+strlen(var2->u.string)+1);
				rvar->u.string = strcat(rvar->u.string,var2->u.string);
				break;
			case YVAR_INTEGER:
				length = sprintf(temp,"%d",var2->u.integer);
				rvar->u.string = realloc(rvar->u.string,
						strlen(rvar->u.string)+length+1);
				rvar->u.string = strcat(rvar->u.string,temp);
				break;
			case YVAR_DOUBLE:
				length = sprintf(temp,"%g",var2->u.dvar);
				rvar->u.string = realloc(rvar->u.string,
						strlen(rvar->u.string)+length+1);
				rvar->u.string = strcat(rvar->u.string,temp);
				break;
			default:
				assert(0);
				break;
		}
	}
	else
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;
	}
}

void arsc_sub(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2)
{
	if( (is_evaluable(var1)) &&
		(is_evaluable(var2)))
	{
		if(is_both_char(var1,var2))
		{
			rvar->type = YVAR_CHAR;
			rvar->u.chr = var1->u.chr - var2->u.chr;
		}
		else if(is_both_integer(var1,var2))
		{
			rvar->type = YVAR_INTEGER;
			rvar->u.integer = var1->u.integer - var2->u.integer;
		}
		else
		{
			rvar->type = YVAR_DOUBLE;
			rvar->u.dvar = get_value(var1) - get_value(var2);
		}
	}
	else
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;
	}
}

void arsc_plus(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2)
{
	if( (is_evaluable(var1)) &&
		(is_evaluable(var2)))
	{
		if(is_both_char(var1,var2))
		{
			rvar->type = YVAR_INTEGER;
			rvar->u.chr = var1->u.chr * var2->u.chr;
		}
		else if(is_both_integer(var1,var2))
		{
			rvar->type = YVAR_INTEGER;
			rvar->u.integer = var1->u.integer * var2->u.integer;
		}
		else
		{
			rvar->type = YVAR_DOUBLE;
			rvar->u.dvar = get_value(var1) * get_value(var2);
		}
	}
	else
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;
	}
}

void arsc_div(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2)
{
	if( (is_evaluable(var1)) &&
			(is_evaluable(var2)))
	{	// TODO: if diveder is 0
		if(is_both_char(var1,var2))
		{
			rvar->type = YVAR_INTEGER;
			rvar->u.chr = var1->u.chr / var2->u.chr;
		}
		else if(is_both_integer(var1,var2))
		{
			rvar->type = YVAR_INTEGER;
			rvar->u.integer = var1->u.integer / var2->u.integer;
		}
		else
		{
			rvar->type = YVAR_DOUBLE;
			rvar->u.dvar = get_value(var1) / get_value(var2);
		}
	}
	else
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;
	}
}

void arsc_neg(yvar_t* rvar,const yvar_t* var1)
{
	if( (YVAR_CHAR == var1->type))
	{
		rvar->type = YVAR_CHAR;
		rvar->u.chr = - var1->u.chr;
	}
	else if( (YVAR_INTEGER == var1->type))
	{
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = - var1->u.integer;
	}
	else if( (YVAR_DOUBLE == var1->type))
	{
		rvar->type = YVAR_DOUBLE;
		rvar->u.dvar = - var1->u.dvar;
	}
}

void arsc_pow(yvar_t* rvar,const yvar_t* var1,const yvar_t* var2)
{
	if( (is_evaluable(var1)) &&
		(is_evaluable(var2)))
	{
		rvar->type = YVAR_DOUBLE;
		rvar->u.dvar = pow(get_value(var1), get_value(var2));
	}
	else
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;
	}
}

void arsc_eval(yvar_t* rvar,const yvar_t* fnc,const yvar_t* param)
{
	yobj_t* obj = arso_get(fnc->u.string);
	if( (NULL == obj) || (YVAR_FUNCTION != obj->var.type) ||
		(!is_evaluable(param)))
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;	// indicate error
	}
	else
	{
		rvar->type = YVAR_DOUBLE;
		rvar->u.dvar = ((feval1_t)obj->var.u.function)(get_value(param));
	}
}

void arsc_read(yvar_t* rvar,const yvar_t* ref)
{
	yobj_t* obj = arso_get(ref->u.string);
	if(NULL != obj)
	{
		arsc_copy(rvar,&obj->var);
	}
	else
	{
		arsc_error();
		rvar->type = YVAR_INTEGER;
		rvar->u.integer = -1;	// indicate error
	}
}

void arsc_print(yvar_t* var)
{
	switch(var->type)
	{
//		case YVAR_STRING:
//		{
//				yobj_t* obj = arso_get(var->u.name);
//				if(obj != NULL)
//				{
//					if(is_string(& obj->var))
//					{
//						printf ("\t%s = %s\n", var->u.name,obj->var.u.string);
//					}
//					else if(is_double(& obj->var))
//					{
//						printf ("\t%s = %g\n", var->u.name,obj->var.u.dvar);
//					}
//					else if(is_integer(& obj->var))
//					{
//						printf ("\t%s = %d\n", var->u.name,obj->var.u.integer);
//					}
//					else if(is_char(& obj->var))
//					{
//						printf ("\t%s = '%c'\n", var->u.name,obj->var.u.chr);
//					}
//					else if(is_function(& obj->var))
//					{
//						printf ("\t%s = (yfp_t)%p\n", var->u.name,obj->var.u.function);
//					}
//					else
//					{
//						assert(0);
//					}
//				}
//				else
//				{
//					printf ("\tERROR:var <%s> is not defined.\n", var->u.name);
//				}
//				break;
//		}
		case YVAR_STRING:
			printf ("\t(String)\"%s\"\n", var->u.string);
			break;
		case YVAR_INTEGER:
			printf ("\t(Integer)%d\n", var->u.integer);
			break;
		case YVAR_DOUBLE:
			printf ("\t(Double)%g\n", var->u.dvar);
			break;
		case YVAR_FUNCTION:
			printf ("\t(Function)%p\n", var->u.function);
			break;
		default:
			assert(0);
			break;
	}
}
