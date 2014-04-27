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
#ifndef FUNCALLER_H
#define FUNCALLER_H

#include <list>
#include <string>
#include <vector>
#include "dynamic_types.h"
#include "heap_manager.h"
#include "exceptions.h"
#include "expressions.h"
#include "dlfcn.h"
#include "operators.h"

#define BUFFER_SIZE 1024

struct meta_info_t
{
	TYPE return_type;
	std::vector<TYPE> argument_types;
	std::string name;
};

struct lib_extension_t
{
	std::list<meta_info_t> functions_list;
	std::string lib_name;
	void* handler;
};

class funcaller_t
{
	typedef std::list<lib_extension_t> extensions_list_t;
	extensions_list_t extensions;

	ref_t call(void* faddr, const meta_info_t& meta, std::vector<object_t*>& obj)
			throw(runtime_exception_t);
public:
	funcaller_t();
	bool open_extension(require_t* require) throw (nativelib_exception_t);
	virtual ~funcaller_t();
	ref_t call_function(function_call_t*, std::vector<object_t*>& obj)
			throw(runtime_exception_t);
	void print_meta(meta_info_t& meta);
	// ===================== [ BUILD-IN ] ============================
	bool call_buildin(function_call_t*, std::vector<object_t*>& objs,ref_t* result)
			throw (runtime_exception_t);

	void Print(std::vector<object_t*>& objs,ref_t* result);
	void Sleep(std::vector<object_t*>& objs,ref_t* result);
	void Write(std::vector<object_t*>& objs,ref_t* result)	throw (runtime_exception_t);
	void Read(std::vector<object_t*>& objs,ref_t* result)	throw (runtime_exception_t);
};

#endif // FUNCALLER_H
