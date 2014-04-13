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

	TAILQ_HEAD(obj_head,yobj_s) yobj_list;

}ArsObjArchtype;
// ===================== [ LOCAL VARIANTS  ] ================================
static ArsObjArchtype sArch = {.IsInitialised = FALSE,};


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
yobj_t* arso_new(char* name,const yvar_t* var)
{
	yobj_t* obj = malloc(sizeof(yobj_t));

	obj->name = arso_strdup(name);
	switch(var->type)
	{
		case YVAR_CHAR:
			obj->var.u.chr = var->u.chr;
			break;
		case YVAR_STRING:
			obj->var.u.string = arso_strdup(var->u.string);
			break;
		case YVAR_INTEGER:
			obj->var.u.integer = var->u.integer;
			break;
		case YVAR_DOUBLE:
			obj->var.u.dvar = var->u.dvar;
			break;
		case YVAR_FUNCTION:
			obj->var.u.function = var->u.function;
			break;
		default:
			assert(0);
			break;
	}
	obj->var.type = var->type;
	TAILQ_INSERT_TAIL(&sArch.yobj_list,obj,entry);

	return obj;
}

void  arso_write(yobj_t* obj,const yvar_t* var)
{
	obj->var.type = var->type;
	switch(obj->var.type)
	{
		case YVAR_CHAR:
			obj->var.u.chr = var->u.chr;
			break;
		case YVAR_STRING:
			obj->var.u.string = arso_strdup(var->u.string);
			break;
		case YVAR_INTEGER:
			obj->var.u.integer = var->u.integer;
			break;
		case YVAR_DOUBLE:
			obj->var.u.dvar = var->u.dvar;
			break;
		case YVAR_FUNCTION:
			obj->var.u.function = var->u.function;
			break;
		default:
			assert(0);
			break;
	}
}

void  arso_read(const yobj_t* obj,yvar_t* var)
{
	var->type = obj->var.type;
	switch(obj->var.type)
	{
		case YVAR_CHAR:
			var->u.chr = obj->var.u.chr;
			break;
		case YVAR_STRING:
			var->u.string = arso_strdup(obj->var.u.string);
			break;
		case YVAR_INTEGER:
			var->u.integer = obj->var.u.integer;
			break;
		case YVAR_DOUBLE:
			var->u.dvar = obj->var.u.dvar;
			break;
		case YVAR_FUNCTION:
			var->u.function = obj->var.u.function;
			break;
		default:
			assert(0);
			break;
	}
}

yobj_t* arso_get(char* name)
{
	yobj_t* obj;
	TAILQ_FOREACH(obj,&sArch.yobj_list,entry)
	{
		if(0==strcmp(obj->name,name))
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
		yobj_t* obj;
		yobj_t objCopy;
		TAILQ_FOREACH(obj,&sArch.yobj_list,entry)
		{
			if(YVAR_STRING == obj->var.type)
			{
				free(obj->var.u.string);
			}
			free(obj->name);
			TAILQ_REMOVE(&sArch.yobj_list,obj,entry);
			memcpy(&objCopy,obj,sizeof(yobj_t));
			free(obj);
			obj = &objCopy;
		}
	}

	TAILQ_INIT(& sArch.yobj_list);
}
