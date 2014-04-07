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
typedef enum
{
	ARS_STRING,
	ARS_INTEGER,
	ARS_DOUBLE,
	ARS_FUNCTION,
	ARS_UNDEFINE
}ArsObjTypeType;

typedef void (*ArsFpType)(void); // Function Pointer Type


typedef struct
{
	ArsObjTypeType  Type;
	union
	{
		char*   	String;
		int     	Integer;
		double  	Double;
		ArsFpType   Function;
	}Var;
}ArsValueType;

typedef struct ArsObj_t
{
	char* 			Name;
	ArsValueType    Value;
	TAILQ_ENTRY(ArsObj_t) Entry;
}ArsObjType;

// ============================ [ FUNCTIONS ] =============================================
void 		arso_init(void);
ArsObjType* arso_add(char* Name,const ArsValueType* Value);
void  		arso_write(ArsObjType* obj,const ArsValueType* Value);
void  		arso_read(const ArsObjType* obj,ArsValueType* Value);
ArsObjType* arso_get(char* Name);
char*       arso_strdup(const char* string);
void        arso_strfree(char* string);

#endif /* ARSOBJ_H_ */
