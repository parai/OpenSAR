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
// ===================== [ LOCAL MACROS    ] ================================

// ===================== [ LOCAL TYPES     ] ================================
typedef struct
{
	boolean IsInitialised;

	TAILQ_HEAD(head,ArsObj_t) ObjList;

}ArsObjArchType;
// ===================== [ LOCAL VARIANTS  ] ================================
static ArsObjArchType sArch = {.IsInitialised = FALSE,};


// ===================== [ LOCAL FUNCTIONS ] ================================

// ===================== [    FUNCTIONS    ] ================================

char*       arso_strdup(const char* string)
{
	//printf("##:new %s\n",string);
	return strdup(string);
}
void        arso_strfree(char* string)
{
	//printf("##:del %s\n",string);
	free(string);
}
ArsObjType* arso_add(char* Name,const ArsValueType* Value)
{
	ArsObjType* obj = malloc(sizeof(ArsObjType));

	obj->Name = arso_strdup(Name);
	switch(Value->Type)
	{
		case ARS_STRING:
			obj->Value.Var.String = arso_strdup(Value->Var.String);
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

void  arso_write(ArsObjType* obj,const ArsValueType* Value)
{
	obj->Value.Type = Value->Type;
	switch(obj->Value.Type)
	{
		case ARS_STRING:
			obj->Value.Var.String = arso_strdup(Value->Var.String);
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

void  arso_read(const ArsObjType* obj,ArsValueType* Value)
{
	Value->Type = obj->Value.Type;
	switch(obj->Value.Type)
	{
		case ARS_STRING:
			Value->Var.String = arso_strdup(obj->Value.Var.String);
			break;
		case ARS_INTEGER:
			Value->Var.Integer = obj->Value.Var.Integer;
			break;
		case ARS_DOUBLE:
			Value->Var.Double = obj->Value.Var.Double;
			break;
		case ARS_FUNCTION:
			Value->Var.Function = obj->Value.Var.Function;
			break;
		default:
			assert(0);
			break;
	}
}

ArsObjType* arso_get(char* Name)
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


void arso_init(void)
{
	if(FALSE == sArch.IsInitialised)
	{
		sArch.IsInitialised = TRUE;
	}
	else
	{	// re-init
		// free memory firstly
		ArsObjType* obj;
		ArsObjType objCopy;
		TAILQ_FOREACH(obj,&sArch.ObjList,Entry)
		{
			if(ARS_STRING == obj->Value.Type)
			{
				free(obj->Value.Var.String);
			}
			free(obj->Name);
			TAILQ_REMOVE(&sArch.ObjList,obj,Entry);
			memcpy(&objCopy,obj,sizeof(ArsObjType));
			free(obj);
			obj = &objCopy;
		}
	}

	TAILQ_INIT(& sArch.ObjList);
}
