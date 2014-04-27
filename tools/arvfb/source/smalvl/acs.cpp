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
#include "acs.h"
#include <cstdio>
#include "object.h"

bool acs::get_logical_value(std::string str) throw (runtime_exception_t)
{
	if (str == std::string("true"))
		return true;
	else if (str == std::string("false"))
		return false;
	else
		throw runtime_exception_t(NOT_LOGICAL);
}

object_t* acs::create_object(std::string v,TYPE type)
{
	object_t* object = new object_t(type);
	switch (type)
	{
		case INTEGER:
			if((v.c_str()[0] == '0' )&&((v.c_str()[1] == 'x' )||(v.c_str()[1] == 'X')))
			{
				sscanf(&(v.c_str()[2]), "%x", &(object->i));
			}
			else
			{
				sscanf(v.c_str(), "%d", &(object->i));
			}
			break;
		case FLOATPOINT:
			sscanf(v.c_str(), "%lf", &(object->d));
			break;
		case BOOL:
			object->b = acs::get_logical_value(v);
			break;
		case STRING:
			object->s = new std::string(v);
			break;
		default:
			break;
	}
	return object;
}

std::string acs::get_string(double value)
{
	char buffer[128];
	sprintf(buffer, "%lf", value);
	return std::string(buffer);
}

std::string acs::type_to_string(TYPE t)
{
	switch (t)
	{
		case INTEGER:
			return "Integer";
		case FLOATPOINT:
			return "FloatPoint";
		case STRING:
			return "String";
		case ARRAY:
			return "Array";
		case BOOL:
			return "Boolean";
		default:
			break;
	}
	return "None";	// TODO
}
