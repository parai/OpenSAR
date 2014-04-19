#ifndef _OPERATORS_H
#define _OPERATORS_H

#include "base_instructions.h"
#include <map>

typedef enum
{	// Control Option In
	CTRL_MAIN_BLOCK,
	CTRL_FUNCTION,
	CTRL_FOR_LOOP,
	CTRL_WHILE_LOOP,
	// Control Option Out
	CTRL_RETURN,
	CTRL_BREAK,
	CTRL_NONE
}ctrl_t;

//represent expression operator
// expresion ';' will be represented by this node
class expr_op_t : public oper_t {
    expr_t* expr;
public:
    expr_op_t(expr_t* expr) : expr(expr) {}
    virtual void print(int indent=0) {
        expr->print();
        std::cout << ";" << std::endl;
    }

    virtual std::string get_decl()
    {
      return std::string("base instruction");
    }
    
    virtual ~expr_op_t() {
        delete expr;
    }
};
//function declaration node
class function_declaration_t: public oper_t {
    block_t implementation;
    std::list<expr_t*> args;
    std::string name;
public:
    function_declaration_t(const std::string &_name,std::list<expr_t*> _args, oper_t* _impl):
        implementation(_impl), args(_args), name(_name)
	{
        SEMANTIC_NODE("Function %s declaration %p", name.c_str(), this);
	}
    
    std::string get_name() { return name; }
    
    block_t* get_instructions() { return &implementation; }
    
    virtual void print(int indent=0)
    {
        std::cout << "function " << name << "(";
        for(std::list<expr_t*>::iterator i=args.begin(); i!=args.end(); i++) {
            //if (i!=args.begin())
                std::cout<<", ";
            (*i)->print();
        }
        std::cout<<")\n{";

        implementation.print(indent+1);
        std::cout << "\n}\n";
    }

    virtual std::list<expr_t*>& get_args() { return args; }
    
    virtual std::string get_decl()
    {
    	return std::string("function ") +name +"()";
    }
    
    virtual ~function_declaration_t() {}
};
//for loop node
class for_op_t: public oper_t {
    expr_t* condition;
    oper_t *init, *itop;
    block_t block;

public:
    for_op_t(oper_t* _init, expr_t* _condition, oper_t* _itop, oper_t* _block):
    	condition(_condition), init(_init), itop(_itop), block(_block) {
        SEMANTIC_NODE("For cycle node created");
    }

    virtual void print(int indent=0)
    {
        std::cout << "For cycle (";
        init->print(indent+1);
        std::cout << ";";
        condition->print();
        std::cout<<";";
        itop->print(indent+1);
        std::cout <<"\n {\n";
        block.print(indent+1);
        std::cout << "\n}\n";
    }
    virtual std::string get_decl()
    {
      return std::string("for cycle");
    }
    
    expr_t* get_condition() { return condition; }
    block_t* get_block() { return &block; }
    oper_t* get_init_op() { return init; }
    oper_t* get_iter_op() { return itop; }
};
//if statement node
class if_op_t : public oper_t {
    expr_t* cond;
    block_t thenops, elseops;
public:
    if_op_t(expr_t* _cond, oper_t* _thenops, oper_t* _elseops) :
        cond(_cond), thenops(_thenops) {
	  
	SEMANTIC_NODE("If statement:");if( _elseops != 0x0)
	  //TODO: Write normal code for elseops initialization
	  elseops.append_safe(_elseops);
        SEMANTIC_NODE(
		"\t\tThen ops %p. Else ops %p. Condition expr %p", _thenops, _elseops,
		_cond);
    }
   
    void print(int indent=0) {
        std::cout << "IF statement (";
	cond->print();
        std::cout << ")";
        std::cout << "{";
        thenops.print(indent+1);

        if(elseops.size())
        {
            std::cout << "}\n{";
            elseops.print(indent+1);
        }
        std::cout << "}";
    }
    
    virtual std::string get_decl()
    {
      return std::string("if statement");
    }
    
    expr_t* get_condition() { return cond; }
    block_t* get_then_instr() { return &thenops; }
    block_t* get_else_instr() { return (elseops.size())?(&elseops):(0x0); }

    virtual ~if_op_t() {
        delete cond;
    }
};
//while loop node
class while_op_t : public oper_t {
    expr_t* cond;
    block_t ops;
public:
    while_op_t(expr_t* cond, oper_t* ops) : cond(cond), ops(ops) {
        SEMANTIC_NODE("While loop node created");
    }

    void print(int indent=0) {
        std::cout << "WHILE loop (";
        cond->print();
        std::cout << ")\n{\n";
        ops.print(indent+1);
        std::cout << "}\n";
    }

    virtual std::string get_decl()
    {
      return std::string("while cycle");
    }
    
    expr_t* get_condition() { return cond; }
    block_t* get_block() { return &ops; }
    
    virtual ~while_op_t() {
        delete cond;
    }
};

class include_t: public oper_t {
    expr_t *file;
public:
    include_t(expr_t* _file) : file(_file) {
        SEMANTIC_NODE("Include instruction node created");
    }

    virtual void print(int indent=0) {
        std::cout << "INCLUDE ";
        file->print();
        std::cout << ";\n";
    }
    
    virtual std::string get_decl()
    {
      return std::string("include statement");
    }

    virtual ~include_t() {
        delete file;
    }
};

class require_t: public oper_t {
    expr_t* file;
public:
    require_t(expr_t* _file): file(_file) {
        SEMANTIC_NODE("Require instruction node created");
    }

    virtual void print(int indent=0) {
        std::cout << "REQUIRE ";
        file->print();
        std::cout << ";\n";
    }
    
    expr_t* get_value() {
       return file; 
    }
    
    virtual std::string get_decl()
    {
      return std::string("require statement");
    }
    
    virtual ~require_t() {
        delete file;
    }
};

class return_op_t: public oper_t {
    expr_t *retvalue;
public:
    return_op_t(expr_t* value):
        retvalue(value) {
        SEMANTIC_NODE("Return instruction node created");
    }

    virtual void print(int indent=0) {
        std::cout << "RETURN ";
        retvalue->print();
        std::cout << ";\n";
    }
    
    expr_t* get_value() { return retvalue; }

    virtual std::string get_decl()
    {
      return std::string("return statement");
    }
    
    virtual ~return_op_t() {
        delete retvalue;
    }
};

class break_op_t: public oper_t {
public:
    break_op_t() {
        SEMANTIC_NODE("Break instruction node created");
    }

    virtual void print(int indent=0) {
        std::cout << "break;\n";
    }
    
    virtual std::string get_decl()
    {
      return std::string("break statement");
    }
};

/* Assign value - operator implementation */
class assign_t : public oper_t {
    expr_t* value;
    var_t* var;
public:
    assign_t(expr_t* _var, expr_t* value) : value(value), var(dynamic_cast<var_t*>(_var))
    {
        SEMANTIC_NODE("Assign instruction for var $%s created",
		var->get_name().c_str());
    }

	virtual void print(int indent)
	{
		var->print(); std::cout << " = "; var->print();
		std::cout << ";\n";
	}

	var_t* get_var() {return var;}
	expr_t* get_value() {return value;}

	virtual std::string get_decl()
	{
		return std::string("var ") +var->get_name() + "assign statement";
	}

	virtual ~assign_t()
	{
		delete var;
	}
};

class unset_t: public oper_t
{
	var_t* var;
public:
	unset_t(expr_t* expr): var(dynamic_cast<var_t*>(expr)){};
	virtual void print(int indent){}
	virtual std::string get_decl()
	{
		return std::string("unset $") +var->get_name();
	}
	virtual var_t* get_var()
	{
		return var;
	}
};
#endif
