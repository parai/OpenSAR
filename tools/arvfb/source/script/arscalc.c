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
 * Sourrce Open At: https://github.com/parai/OpenSAR/
 */

#include "arvfb.h"


// ==================================== [ TYPES     ] ==========================================
typedef double (*feval1_t)(double);  // Eval which with 1 parameter

// ==================================== [ LOCAL FUNCTIONS ] ====================================
static double get_value(const ArsValueType* from)
{
	double rv = -1;

	switch(from->Type)
	{
		case ARS_INTEGER:
			rv = from->Var.Integer;
			break;
		case ARS_DOUBLE:
			rv = from->Var.Double;
			break;
		default:
			assert(0);
			break;
	}
	return rv;
}
// ==================================== [ FUNCTIONS ] ==========================================
void arsc_copy(ArsValueType* to,ArsValueType* from)
{
	memcpy(to,from,sizeof(ArsValueType));
}

void arsc_add(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
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

void arsc_sub(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
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

void arsc_plus(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
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

void arsc_div(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
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

void arsc_neg(ArsValueType* out,const ArsValueType* in1)
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

void arsc_pow(ArsValueType* out,const ArsValueType* in1,const ArsValueType* in2)
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

void arsc_eval(ArsValueType* out,const ArsValueType* fnc,const ArsValueType* param)
{
	assert(ARS_STRING==fnc->Type);
	ArsObjType* obj = arso_get(fnc->Var.String);
	if( (NULL == obj) || (ARS_FUNCTION != obj->Value.Type) ||
		((ARS_INTEGER != param->Type)&&(ARS_DOUBLE != param->Type)))
	{
		printf("arsc error: %s(%g) is not evaluable.\n",fnc->Var.String,get_value(param));
		out->Type = ARS_INTEGER;
		out->Var.Integer = -1;	// indicate error
	}
	else
	{
		out->Type = ARS_DOUBLE;
		out->Var.Double = ((feval1_t)obj->Value.Var.Function)(get_value(param));
	}
}
