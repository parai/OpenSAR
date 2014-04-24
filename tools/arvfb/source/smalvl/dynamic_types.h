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
#ifndef _DYNAMIC_TYPES_H
#define _DYNAMIC_TYPES_H

#include <cstring>
#include <map>
#include "debug.h"

#define REF_IS_VOID		((ref_t)0)	// reference type is void
typedef unsigned int ref_t;
enum TYPE { INTEGER, FLOATPOINT, ARRAY, BOOL, STRING, NONE, VA };

namespace dt {
  const char* type_to_string(TYPE t);
  TYPE string_to_type(const char* s);
};

#endif
