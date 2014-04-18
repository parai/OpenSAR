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
	bool open_extension(const char* name) throw (nativelib_exception_t);
	meta_info_t parse_meta(const char* metaline) throw(nativelib_exception_t);
	virtual ~funcaller_t();
	ref_t call_function(function_call_t*, std::vector<object_t*>& obj)
			throw(runtime_exception_t);
	void print_meta(meta_info_t& meta);
	// ===================== [ BUILD-IN ] ============================
	bool call_buildin(function_call_t*, std::vector<object_t*>& objs,ref_t* result)
			throw (runtime_exception_t);

	void Print(std::vector<object_t*>& objs,ref_t* result);
};

#endif // FUNCALLER_H
