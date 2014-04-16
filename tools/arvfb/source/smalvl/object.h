#ifndef _OBJECT_H
#define _OBJECT_H

#include <cmath>
#include "dynamic_types.h"
#include "debug.h"
#include "acs.h"

class object_t;
typedef std::map<ref_t, object_t*> heap_t;
typedef std::map<std::string, ref_t> array_obj_t;

class object_t
{
private:
	TYPE _dynamic_type;

public:
	int link_count;

	union
	{
		std::string* s;
		array_obj_t* a;
		int i;
		double *d;
		bool b;
	};

	object_t(TYPE _type): _dynamic_type(_type), link_count(0)
	{
		s=0x0;
		i=0x0;
		d=0x0;
		b=0x0;
		a=0x0;
	}

	~object_t()
	{
		RUNTIME_DEBUG("Destructing object %p, value=%s", this, ((const char*) *this));
		/* TODO: why a declaration of function increment_link_count here, but without use it.
		 * Or am I not understand C++ grammar ?
		 */
		//void increment_link_count(ref_t& ref);
		switch(_dynamic_type)
		{
			case INTEGER:
			/*Nothing to do */
			break;
			case FLOATPOINT:
			if(d) delete d;
			break;
			case ARRAY:
			if(a) delete a;
			break;
			case STRING:
			if(s) delete s;
			break;
			case BOOL:
			/* Nothing to do */
			break;
			default:
			break;	// supress warning as not process VA ...
		}
	}
	/*
	 * OK, most of the key word of C++ is not understand by me.
	 * But who cares, if it works OK.
	 * We just need to know, a ha, do something like what in python
	 * bool('True'); int('0x789') ..., but only explicit
	 */
	explicit operator bool() const throw(runtime_exception_t, std::invalid_argument)
	{
		if(this->get_type() == STRING)
		{
			return acs::get_logical_value( *(this->s) );
		}
		else if(this->get_type()==INTEGER || this->get_type() == FLOATPOINT)
		{
			return (this->i!=0);
		}
		else if(this->get_type() == BOOL)
		{
			return this->b;
		}
		else
		{
			throw runtime_exception_t(INVALID_COVERSION);
		}
	}

	explicit operator int() const throw(runtime_exception_t, std::invalid_argument)
	{
		if(this->get_type() == STRING)
		{
			return std::stoi( *(this->s) );
		}
		else if(this->get_type()== FLOATPOINT)
		{
			return std::trunc( *(this->d) );
		}
		else if(this->get_type() == BOOL)
		{
			return (this->b)?(1):(0);
		}
		else if(this->get_type() == INTEGER)
		{
			return this->i;
		}
		else if(this->get_type() == ARRAY)
		{
			return a->size();
		}
		else
		{
			throw runtime_exception_t(INVALID_COVERSION);
		}
	}

	explicit operator double() const throw(runtime_exception_t, std::invalid_argument)
	{
		if(this->get_type() == STRING)
		{
			return std::stod( *(this->s) );
		}
		else if(this->get_type() == INTEGER)
		{
			return (double) this->i;
		}
		else if(this->get_type() == BOOL)
		{
			return (this->b)?(1.0):(0.0);
		}
		else if(this->get_type() == ARRAY)
		{
			return a->size();
		}
		else
		{
			throw runtime_exception_t(INVALID_COVERSION);
		}
	}

	explicit operator std::string() const throw (std::invalid_argument)
	{
		if(this->get_type() == FLOATPOINT)
		return std::string( std::to_string(*(this->d)) );
		else if(this->get_type() == INTEGER)
		return std::string( std::to_string(this->i) );
		else if(this->get_type() == BOOL)
		return std::string( (this->b)?("true"):("false") );
		else if( this->get_type() == STRING )
		return *(this->s);
		else if( this->get_type() == NONE )
		return "NONE";
		else if( this->get_type() == ARRAY )
		return std::string("Array with ") +std::to_string(a->size()) +" elements";
		else
		throw runtime_exception_t(INVALID_COVERSION);
	}

	explicit operator const char*() const throw (std::invalid_argument)
	{
		return ((std::string)(*this)).data();
	}

	bool operator < (const object_t& obj)throw (runtime_exception_t)
	{
		if( this->get_type() == STRING || obj.get_type()== STRING )
		{
			return ( (std::string) *this) < ( (std::string) obj );
		}
		else if( this->get_type() == BOOL || obj.get_type() == BOOL )
		{
			return ( (bool) (*this) ) < ( ( bool ) obj);
		}
		else if( this->get_type() == INTEGER || this->get_type() == FLOATPOINT)
		{
			return ( (double) *this) < ( (double) obj);
		}
		else
		{
			throw runtime_exception_t(UNCOMPARABLE_TYPES);
		}

	}

	bool operator >(const object_t& obj) throw (runtime_exception_t)
	{
		if( this->get_type() == STRING || obj.get_type()== STRING )
		{
			return ( (std::string) *this) > ( (std::string) obj );
		}
		else if( this->get_type() == BOOL || obj.get_type() == BOOL )
		{
			return ( (bool) (*this) ) > ( ( bool ) obj);
		}
		else if( this->get_type() == INTEGER || this->get_type() == FLOATPOINT)
		{
			return ( (double) *this) > ( (double) obj);
		}
		else
		{
			throw runtime_exception_t(UNCOMPARABLE_TYPES);
		}
	}

	bool operator >= (const object_t& obj) throw (runtime_exception_t)
	{
		if( this->get_type() == STRING || obj.get_type()== STRING )
		{
			return ( (std::string) *this) <= ( (std::string) obj );
		}
		else if( this->get_type() == BOOL || obj.get_type() == BOOL )
		{
			return ( (bool) (*this) ) <= ( ( bool ) obj);
		}
		else if( this->get_type() == INTEGER || this->get_type() == FLOATPOINT)
		{
			return ( (double) *this) <= ( (double) obj);
		}
		else
		throw runtime_exception_t(UNCOMPARABLE_TYPES);
	}

	bool operator <= (const object_t& obj) throw (runtime_exception_t)
	{
		if( this->get_type() == STRING || obj.get_type()== STRING )
		{
			return ( (std::string) *this) <= ( (std::string) obj );
		}
		else if( this->get_type() == BOOL || obj.get_type() == BOOL )
		{
			return ( (bool) (*this) ) <= ( ( bool ) obj);
		}
		else if( this->get_type() == INTEGER || this->get_type() == FLOATPOINT)
		{
			return ( (double) *this) <= ( (double) obj);
		}
		else
		throw runtime_exception_t(UNCOMPARABLE_TYPES);
	}

	bool operator == (const object_t& obj) throw (runtime_exception_t)
	{
		if( this->get_type() == STRING || obj.get_type()== STRING )
		{
			return ( (std::string) *this) == ( (std::string) obj );
		}
		else if( this->get_type() == BOOL || obj.get_type() == BOOL )
		{
			return ( (bool) (*this) ) == ( ( bool ) obj);
		}
		else if( this->get_type() == INTEGER || this->get_type() == FLOATPOINT)
		{
			return ( (double) *this) == ( (double) obj);
		}
		else
		throw runtime_exception_t(UNCOMPARABLE_TYPES);
	}

	bool operator != (const object_t& obj)
	{
		return !( (*this) == obj );
	}

	TYPE get_type() const
	{
		return _dynamic_type;
	}

};
#endif
