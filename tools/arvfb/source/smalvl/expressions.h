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
#ifndef _EXPRESIONS_H
#define _EXPRESIONS_H

#include "base_instructions.h"
#include "acs.h"
#include <cstring>

/* Expresions */
class value_t : public expr_t {
    std::string value;
    TYPE		type;
public:
    value_t(const std::string text,const TYPE type): value(text),type(type) {
		SEMANTIC_NODE("Value %s node created", acs::type_to_string(type).c_str());
    }

    value_t() {}

    virtual void print() {
    }

    std::string get_value()
    {
        return value;
    }

    TYPE get_type()
	{
		return type;
	}

    virtual ~value_t() {}
};

class type_t : public expr_t {
	std::string value;
public:
    type_t(const std::string text) : value(text) {}

    virtual void print(void) {
    	std::cout<<value;
    }

    TYPE get_value()
	{
		return dt::string_to_type(value.c_str());
	}

    virtual std::string get_decl()
    {
      return std::string("base type");
    }

    ~type_t() {}
};

class declare_t : public expr_t
{
	expr_t*	type;
	std::string name;
	std::list<expr_t*> args;
public:
	declare_t(expr_t* type,const std::string& name,std::list<expr_t*> args) :
		type(type), name(name), args(args)
	{
		SEMANTIC_NODE("Declare '%s' call node created. %d function arguments", name.data(), (int)args.size());
	}

	virtual void print()
	{
		type->print();
		std::cout<<" "<<name<<"(";
		for(std::list<expr_t*>::iterator i=args.begin(); i!=args.end(); i++)
		{
			if (i!=args.begin())
				std::cout<<", ";
			(*i)->print();
		}
		std::cout<<");\n";
	}

	virtual void print(int indent)
	{
		//TODO: implement it!
	}

	expr_t*	get_type()
	{
		return type;
	}

	virtual std::list<expr_t*>& get_args()
	{
		return args;
	}

	virtual std::string get_decl()
	{
		return std::string("unary operation");
	}

	std::string get_name()
	{
		return name;
	}

	virtual ~declare_t() {
		for(std::list<expr_t*>::iterator i=args.begin(); i!=args.end(); i++)
		{
			delete *i;
		}
	}
};

class var_t: public expr_t {
protected:
    std::string name;
    ref_t reference;
public:
    var_t(const std::string& _name): name(_name)
	{
    	reference = REF_IS_VOID;
        SEMANTIC_NODE("VAR $%s created", name.c_str());
	}

    virtual void print()
    {
        std::cout << "$" << name;
    }

    virtual std::string get_name()
    {
        return name;
    }
};

class array_t: public var_t {
    expr_t* index;
public:
    array_t(const std::string& _name, expr_t* index): var_t(_name), index(index) {
        SEMANTIC_NODE("ARRAY $%s created", name.c_str());
    }

    virtual void print()
    {
        std::cout << "$" << name;
    }

    expr_t* get_index() const {
        return index;
    }
};

class array_init_t: public value_t {
    int _init_size;
public:
    array_init_t(int init_size=0x0): value_t(), _init_size(init_size)
    {

    }

    int get_size() const {
        return _init_size;
    }
};

class binary_t : public expr_t {
    const char* op;
    expr_t *arg1, *arg2;
public:
    binary_t(const char* _op, expr_t *_arg1, expr_t *_arg2) :
        op(_op), arg1(_arg1), arg2(_arg2) {
        SEMANTIC_NODE("Binary operation %p %s %p node created", _arg1, op, _arg2);}

    binary_t(const std::string& str, expr_t *_arg1, expr_t *_arg2) :
        op(str.data()), arg1(_arg1), arg2(_arg2) {
        SEMANTIC_NODE(
		"Binary operation %p '%s' %p node created", _arg1, op, _arg2);
    }

    virtual void print() {
        std::cout << "BINARY_OPERATION ";
        arg1->print();
        std::cout << op;
        arg2->print();
        std::cout << ";\n";
    }

    const char* get_operator() const {
        return op;
    }

    expr_t* get_operand(int i=0) {
        if(i)
            return arg2;
        else
            return arg1;
    }

    virtual ~binary_t() {
        delete op;
        delete arg1;
        delete arg2;
    }
};

class unary_t : public expr_t, public oper_t {
    const char* op;
    var_t* var;
public:
    unary_t(const char* op, expr_t* arg) :
        op(op), var(dynamic_cast<var_t*>(arg)) {
        SEMANTIC_NODE("Unary operation '%s' node created", op);}

    unary_t(const std::string& str, expr_t* arg) :
        op(str.data()), var(dynamic_cast<var_t*>(arg)) {
        SEMANTIC_NODE(
		"Unary operation '%s' node created", str.c_str());
	}

	void print(int indent)
	{
		print();
	}

	void print()
	{
		var->print();
		std::cout << op << ";";
	}

	var_t* get_value()
	{
		return var;
	}
	const char* get_operator() const
	{
		return op;
	}
	virtual std::string get_decl()
	{
		return std::string("unary operation");
	}
	~unary_t()
	{
		delete var;
		delete op;
	}
};

class function_call_t : public expr_t, public oper_t
{
	std::string name;
	std::list<expr_t*> args;
public:
	function_call_t(const std::string& name,const std::list<expr_t*>& args) :
			name(name), args(args)
	{
		SEMANTIC_NODE("Function '%s' call node created. %d function arguments", name.data(), (int)args.size());
	}

	virtual void print()
	{
		std::cout<<name<<"(";
		for(std::list<expr_t*>::iterator i=args.begin(); i!=args.end(); i++)
		{
			if (i!=args.begin())
			std::cout<<", ";
			(*i)->print();
		}
		std::cout<<");\n";
	}

	virtual void print(int indent)
	{
		//TODO: implement it!
	}

	virtual std::list<expr_t*>& get_args()
	{
		return args;
	}

	virtual std::string get_decl()
	{
		return std::string("unary operation");
	}

	std::string get_name()
	{
		return name;
	}

	virtual ~function_call_t()
	{}
};

#endif
