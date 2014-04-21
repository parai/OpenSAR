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
				printf("####: Create from hex(%s)=%d\n",v.c_str(),object->i);
			}
			else
			{
				sscanf(v.c_str(), "%d", &(object->i));
				printf("####: Create from int(%s)=%d\n",v.c_str(),object->i);
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
