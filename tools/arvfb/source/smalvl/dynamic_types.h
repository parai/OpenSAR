#ifndef _DYNAMIC_TYPES_H
#define _DYNAMIC_TYPES_H

#include <cstring>
#include <map>
#include "debug.h"

typedef unsigned int ref_t;
enum TYPE { INTEGER, FLOATPOINT, ARRAY, BOOL, STRING, NONE, VA };

namespace dt {
  const char* type_to_string(TYPE t);
  TYPE string_to_type(const char* s);
};

#endif
