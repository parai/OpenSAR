#ifndef _ACS_H
#define _ACS_H

#include <string>
#include "exceptions.h"
#include "dynamic_types.h"

class object_t;

namespace acs {
  bool is_double(std::string src);
  TYPE determine_type(std::string);
  std::string type_to_string(TYPE);
  object_t* create_object(std::string);
  double get_double(std::string str) throw (runtime_exception_t);
  bool get_logical_value(std::string str) throw (runtime_exception_t);
  
  std::string get_string(double value);
};

#endif
