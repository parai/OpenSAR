#ifndef _EXPRESIONS_H
#define _EXPRESIONS_H

#include "base_instructions.h"
#include "acs.h"
#include <cstring>



/* Expresions */
class value_t : public expr_t {
    std::string _value;
public:
    value_t(const std::string _text): _value( _text) {
        const char* type_str = acs::type_to_string(acs::determine_type(_text)).data();
        SEMANTIC_NODE("Value %s node created", type_str);
    }

    value_t() {}

    virtual void print() {
    }

    std::string get_value()
    {
        return _value;
    }

    virtual ~value_t() {}
};

class var_t: public expr_t {
protected:
    std::string name;
    ref_t reference;
public:
    var_t(const std::string& _name): name(_name) {
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
        SEMANTIC_NODE("Binary operation %p %s %p node created", _arg1, op, _arg2);
    }

    binary_t(const std::string& str, expr_t *_arg1, expr_t *_arg2) :
        op(str.data()), arg1(_arg1), arg2(_arg2) {
        SEMANTIC_NODE("Binary operation %p '%s' %p node created", _arg1, op, _arg2);
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
        SEMANTIC_NODE("Unary operation '%s' node created", op);
    }

    unary_t(const std::string& str, expr_t* arg) :
        op(str.data()), var(dynamic_cast<var_t*>(arg)) {
        SEMANTIC_NODE("Unary operation '%s' node created", str.c_str());
    }

    void print(int indent) {
        print();
    }

    void print() {
        var->print();
        std::cout << op << ";";
    }

    var_t* get_value() {
        return var;
    }
    const char* get_operator() const {
        return op;
    }
    virtual std::string get_decl() {
        return std::string("unary operation");
    }
    ~unary_t() {
        delete var;
        delete op;
    }
};

class function_call_t : public expr_t, public oper_t {
    std::string name;
    std::list<expr_t*> args;
public:
    function_call_t(const std::string& name,
                    const std::list<expr_t*>& args) :
        name(name), args(args) {
        SEMANTIC_NODE("Function '%s' call node created. %d function arguments", name.data(), args.size());
    }

    virtual void print() {
        std::cout<<name<<"(";
        for(std::list<expr_t*>::iterator i=args.begin(); i!=args.end(); i++) {
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

    virtual std::list<expr_t*>& get_args() {
        return args;
    }

    virtual std::string get_decl() {
      return std::string("unary operation");
    }
    
    std::string get_name() {
        return name;
    }

    virtual ~function_call_t() {}
};

#endif
