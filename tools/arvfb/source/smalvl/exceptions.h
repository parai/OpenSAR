#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include <iostream>
#include <stdexcept>

enum EXCEPTION_TYPE { INTERNAL_EXCEPTION, NOT_NUMBER, NOT_LOGICAL, NULL_REF,
                      FUNCTION_NOT_DECL, FUNCTION_DECL, FUNCTION__CALL_INVALID_ARGUMENTS,
		      VAR_DECL, VAR_NOT_DECL,
                      REF_EXIST, REF_INVALID, INVALID_COVERSION, UNCOMPARABLE_TYPES
                    };

class exception_t
{
protected:
    std::string message;
public:
    exception_t(const std::string &message="Undefined exception"): message(message) {}
    virtual void print()
    {
        std::cout << "EXCEPTION: " << message << std::endl;
    }
    
    virtual void set_message(std::string message)
    {
      this->message = message;
    }
    
    virtual std::string& get_message() {
	    return message;
    }
};

class runtime_exception_t: public exception_t
{
    EXCEPTION_TYPE type;
public:
    runtime_exception_t(const std::string &message): exception_t(message) {}
    runtime_exception_t(const std::string &message, EXCEPTION_TYPE type): exception_t(message), type(type) {}
    runtime_exception_t(EXCEPTION_TYPE type): exception_t(), type(type)
    {
        switch(type) {
        case NOT_NUMBER:
            message = "Value is not a number";
            break;
        case NOT_LOGICAL:
            message = "Value is not logical";
            break;
        case NULL_REF:
            message = "Null regerence exception";
            break;
        case FUNCTION_NOT_DECL:
            message = "Function not declarated in this scope";
            break;
	case FUNCTION__CALL_INVALID_ARGUMENTS:
	    message = "Function call invalid arguments";
	    break;
        case INVALID_COVERSION:
            message = "Dynamic type conversion error";
            break;
        case REF_INVALID:
            message = "Incorrect reference to heap object";
            break;
        case VAR_NOT_DECL:
            message = "Variable don't declarated early and not initalized. First mentioning.";
            break;
	
        case INTERNAL_EXCEPTION:
            message = "Runtime internal exception";
            break;

        case VAR_DECL:
            message = "Variable yet decalared early";
            break;
	    
	case REF_EXIST:
	    message = "Reference exist";
	    break;
	case UNCOMPARABLE_TYPES:
	    message = "I can't compare this values";
	    break;
        default:
            message = "Undefined exception, exception code is " +std::to_string(type);
            break;
        }
    }

    virtual EXCEPTION_TYPE getType() {
        return type;
    }
};

class nativelib_exception_t: public exception_t
{
};

#endif
