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
#ifndef _RUNTIME_H
#define _RUNTIME_H

#include "semantic.h"
#include "operators.h"
#include "heap_manager.h"
#include "funcaller.h"
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <vector>

#define RUNTIME_CRITICAL_ENTER()  GDK_THREADS_ENTER()
#define RUNTIME_CRITICAL_LEAVE()  GDK_THREADS_LEAVE()

typedef std::map<std::string, ref_t> var_scope_t;
typedef std::vector<var_scope_t*> frame_stack_t;

class runtime_t
{
private:
	/* Function declaration scope */
	typedef std::map<std::string, function_declaration_t*> fun_map_t;
	fun_map_t _function_declarations;
	fun_map_t _event_declarations;

	/* Heap */
	heap_manager_t*_memmanager;
	/* Native function caller */
	funcaller_t _funcaller;
	/* Singletone realization */
	static runtime_t* self;
	block_t* _main_block;
	std::string run_instruction_name;
	frame_stack_t global_stack;
	runtime_t();

public:
	static runtime_t* get_instance();

	heap_manager_t* get_mem() { return _memmanager; };

	void init(void);

	void gc(frame_stack_t& fs) throw (runtime_exception_t);

	/*Function declaration operations*/
	function_declaration_t* get_function_declaration(std::string name) throw (runtime_exception_t);
	void add_function_declaration(function_declaration_t* fd) throw (runtime_exception_t);
	void add_event_declaration(function_declaration_t* fd) throw (runtime_exception_t);

	/* Main hierarchy block */
	void set_main_block(block_t* block);
	block_t* get_main_block();

	/*Abstract semantic tree analys */
	bool semantic_analys();
	/* Interpretate */
	void interpretate();

	/* Work with var scopes and frame_stack */
	/* Create var in top frame stack var scope*/
	ref_t create_var(var_t* var, ref_t& ref, frame_stack_t& fs);
	object_t* get_var_value(var_t* var, frame_stack_t& fs) throw (runtime_exception_t);
	ref_t& get_var_ref(var_t* var, frame_stack_t& fs, ref_t default_ref=REF_IS_VOID) throw (runtime_exception_t);
	void unset_var(std::string name, frame_stack_t& fs);
	void unset_var(var_t* var, frame_stack_t& fs);
	void assign_var(var_t* var, ref_t& ref, frame_stack_t& fs);
	void assign_var(var_t* var, expr_t* expr, frame_stack_t& fs);

	/* Run code */
	ref_t run(block_t* block, frame_stack_t& fs, ctrl_t* p_ctrl) throw(runtime_exception_t);
	/* Run single instruction, not a block of instructions */
	ref_t run_instruction(oper_t* instruction, frame_stack_t& fs, ctrl_t* p_ctrl) throw (runtime_exception_t);
	/*Call function*/
	ref_t run_function(function_call_t*, frame_stack_t& fs) throw(runtime_exception_t);
	ref_t call_external_function(function_call_t*, frame_stack_t& fs) throw(runtime_exception_t);
	bool  call_build_in_function(function_call_t*, frame_stack_t& fs,ref_t*) throw(runtime_exception_t);
	ref_t compute_expression(expr_t *expr, frame_stack_t& fs) throw (runtime_exception_t);
	/*
	 * Throw std::invalid_argument if try to exercise conversion on object_t;
	 * For instance, mathematical operations will be perfomed on objects converted to
	 * INTEGER or FLOAT, if runtime can't convert source type to INTEGER and
	 * FLOAT throw std::invalid_argument
	 * !!! Warning: this function allocate space for object_t, you mast delete allocated
	 * !!! space after using this object;
	 */
	ref_t compute_math_expression(object_t* obj1, const char* op, object_t* obj2)
	throw (runtime_exception_t, std::invalid_argument);

	ref_t compute_unary_expression(unary_t* expr, frame_stack_t& fs)
	throw (runtime_exception_t, std::invalid_argument);

	/* Framestack operations */
	void pop_frame_stack(frame_stack_t& fs);
	void clean_varscope(var_scope_t* vs);
	void print_frame_stack(frame_stack_t& fs);
};

#endif
