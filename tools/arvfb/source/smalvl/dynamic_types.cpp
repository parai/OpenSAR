#include "dynamic_types.h"

const char* dt::type_to_string(TYPE t)
{
    switch(t)
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
    if(!strcasecmp(s, "STRING"))
        return STRING;
    else if(!strcasecmp(s, "FLOATPOINT"))
        return FLOATPOINT;
    else if(!strcasecmp(s, "INTEGER"))
        return INTEGER;
    else if(!strcasecmp(s, "ARRAY"))
        return ARRAY;
    else if(!strcasecmp(s, "..."))
        return VA;
    else if(!strcasecmp(s, "NONE"))
        return NONE;
    else if(!strcasecmp(s, "BOOL"))
        return BOOL;
    else 
      return NONE;
}
