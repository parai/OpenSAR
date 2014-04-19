#include "acs.h"
#include <cstdio>
#include "object.h"

bool acs::is_double(std::string src)
{
	const char *src_data = src.c_str();
	for (int i = 0; src_data[i]; i++)
	{
		if (!std::isdigit(src_data[i]) || src_data[i] != '.')
		{
			return false;
		}
	}

	return true;
}

double acs::get_double(std::string str) throw (runtime_exception_t)
{
	double value;
	if (acs::is_double(str))
	{
		sscanf(str.c_str(), "%lf", &value);
		return value;
	}
	else
		throw runtime_exception_t(NOT_NUMBER);
}

bool acs::get_logical_value(std::string str) throw (runtime_exception_t)
{
	if (str == std::string("true"))
		return true;
	else if (str == std::string("false"))
		return false;
	else
		throw runtime_exception_t(NOT_LOGICAL);
}

TYPE acs::determine_type(std::string src)
{
	const char* data = src.c_str();
	int i = 0;
	TYPE type = INTEGER;

	for (i = 0; data[i]; i++)
	{
		if (!std::isdigit(data[i]))
		{
			type = FLOATPOINT;
			break;
		}
	}

	if (data[i] == 0x0)
		return type;

	for (; data[i]; i++)
	{
		if (!std::isdigit(data[i]) && data[i] != '.')
		{
			break;
		}
	}

	if (data[i] == 0x0)
		return type;

	if (src == "false" || src == "true")
	{
		return BOOL;
	}
	else
	{
		return STRING;
	}
}

object_t* acs::create_object(std::string v)
{
	TYPE type = determine_type(v);
	object_t* object = new object_t(type);
	switch (type)
	{
		case INTEGER:
			sscanf(v.c_str(), "%d", &(object->i));
			break;
		case FLOATPOINT:
			object->d = new double(0);
			sscanf(v.c_str(), "%lf", (object->d));
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
