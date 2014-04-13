#ifndef HEAP_MANAGER_H
#define HEAP_MANAGER_H

#include <map>
#include <cmath>
#include "acs.h"
#include "exceptions.h"
#include "dynamic_types.h"
#include "object.h"
#include "debug.h"

class heap_manager_t
{
    /* Memory managment scope */
    static heap_manager_t* _self;
    heap_t _heap;
    ref_t link_in_use;
    heap_manager_t();
public:
    static heap_manager_t* get_instance();
    virtual ~heap_manager_t();
    
    ref_t add_object(object_t* object=0x0) throw(runtime_exception_t);
    ref_t add_object(TYPE type) throw (runtime_exception_t);
    ref_t set_object(ref_t, object_t*) throw(runtime_exception_t);
    void remove_object(ref_t) throw (runtime_exception_t);
    void remove_object(std::map<std::string, ref_t>* scope) throw(runtime_exception_t);
    object_t* get_object(ref_t) throw (runtime_exception_t);

    void increment_link_count(ref_t& ref);
    void decrement_link_count(ref_t& ref);
    
    void gc();
    
};

#endif // HEAP_MANAGER_H
