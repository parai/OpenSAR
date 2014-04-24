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
#include "dynamic_types.h"

const char* dt::type_to_string(TYPE t)
{
	switch (t)
	{
		case INTEGER:
			return "INTEGER";
		case FLOATPOINT:
			return "FLOATPOINT";
		case ARRAY:
			return "ARRAY";
		case BOOL:
			return "BOOL";
		case STRING:
			return "STRING";
		case NONE:
			return "NONE";
		case VA:
			return "...";
		default:
			return "NONE";
	}
}

TYPE dt::string_to_type(const char* s)
{
	if (!strcasecmp(s, "STRING"))
		return STRING;
	else if (!strcasecmp(s, "FLOATPOINT"))
		return FLOATPOINT;
	else if (!strcasecmp(s, "INTEGER"))
		return INTEGER;
	else if (!strcasecmp(s, "ARRAY"))
		return ARRAY;
	else if (!strcasecmp(s, "..."))
		return VA;
	else if (!strcasecmp(s, "NONE"))
		return NONE;
	else if (!strcasecmp(s, "BOOL"))
		return BOOL;
	else
		return NONE;
}
