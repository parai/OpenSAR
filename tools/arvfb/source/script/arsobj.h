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
#ifndef ARSOBJ_H_
#define ARSOBJ_H_
#include <sys/queue.h>
// ========================== [ TYPEs ]======================================
// here is the thing, for the reason to simple typing, all use small case.
typedef enum
{
	YVAR_CHAR,
	YVAR_STRING,
	YVAR_INTEGER,
	YVAR_DOUBLE,
	YVAR_FUNCTION,
	YVAR_ERROR, // for arscript error report only
	YVAR_UNDEFINE
}yvar_type_t;

typedef void (*yfp_t)(void); // Function Pointer Type


typedef struct
{
	yvar_type_t     type;
	union
	{
		char*   	string;
		int     	integer;
		double  	dvar;
		char        chr;
		yfp_t       function;
	}u;
}yvar_t;

typedef struct yobj_s
{
	char* 			name;
	yvar_t    		var;
	TAILQ_ENTRY(yobj_s) entry;
}yobj_t;

// ============================ [ FUNCTIONS ] =============================================
void 		arso_init(void);
yobj_t* 	arso_new(char* name,const yvar_t* Value);
void  		arso_write(yobj_t* obj,const yvar_t* Value);
void  		arso_read(const yobj_t* obj,yvar_t* Value);
yobj_t* 	arso_get(char* name);
char*       arso_strdup(const char* string);
void        arso_strfree(char* string);

#endif /* ARSOBJ_H_ */
