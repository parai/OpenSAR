
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


typedef std::map<std::string, ref_t> var_scope_t;
typedef std::vector<var_scope_t*> frame_stack_t;

class runtime_t
{
private:
    /* Function declaration scope */
    typedef std::map<std::string, function_declaration_t*> fun_map_t;
    fun_map_t _function_declarations;

    /* Heap */
    heap_manager_t *_memmanager;
    /* Native function caller */
    funcaller_t _funcaller;
    /* Singletone realization */
    static runtime_t* self;
    block_t* _main_block;
    std::string run_instruction_name;
    runtime_t();

public:
    static runtime_t* get_instance();

    void gc(frame_stack_t& fs) throw (runtime_exception_t);

    /*Function declaration operations*/
    function_declaration_t* get_function_declaration(std::string name) throw (runtime_exception_t);
    void add_function_declaration(function_declaration_t* fd) throw (runtime_exception_t);

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
    ref_t& get_var_ref(var_t* var, frame_stack_t& fs, ref_t default_ref=0x0) throw (runtime_exception_t);
    void unset_var(std::string name, frame_stack_t& fs);
    void unset_var(var_t* var, frame_stack_t& fs);
    void assign_var(var_t* var, ref_t& ref, frame_stack_t& fs);
    void assign_var(var_t* var, expr_t* expr, frame_stack_t& fs);
    
    /* Run code */
    ref_t run(block_t* block, frame_stack_t& fs) throw(runtime_exception_t);
    /* Run single instruction, not a block of instructions */
    ref_t run_instruction(oper_t* instruction, frame_stack_t& fs) throw (runtime_exception_t);
    /*Call function*/
    ref_t run_function(function_call_t*, frame_stack_t& fs) throw(runtime_exception_t);
    ref_t call_external_function(function_call_t*, frame_stack_t& fs) throw(runtime_exception_t);
    bool  call_build_in_function(function_call_t*, frame_stack_t& fs,ref_t*) throw(runtime_exception_t);
    ref_t compute_expression(expr_t *expr, frame_stack_t& fs)  throw (runtime_exception_t);
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
