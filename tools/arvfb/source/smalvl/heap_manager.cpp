#include "heap_manager.h"
#include <ctime>

heap_manager_t* heap_manager_t::_self = 0x0;

heap_manager_t* heap_manager_t::get_instance()
{
  if( _self == 0x0 )
    _self = new heap_manager_t();
  
  return _self;
}


heap_manager_t::heap_manager_t()
{
  
  _self = this;
  link_in_use = 0;
}

heap_manager_t::~heap_manager_t()
{

}

ref_t heap_manager_t::add_object(object_t* object) throw(runtime_exception_t)
{
  ref_t _ref =  link_in_use++;
  if(object == 0x0)
    object = new object_t(NONE);
  
  //RUNTIME_DEBUG("Heap manager create new reference %p", _ref);
  std::pair<ref_t, object_t*> _pair(_ref, object);
  std::pair<heap_t::iterator, bool> status = _heap.insert(_pair);
  if(!status.second)
    throw runtime_exception_t(REF_EXIST);
  //RUNTIME_DEBUG("Object type %s added %p to heap", dt::type_to_string( object->get_type() ), _ref);
  return _ref;
}

ref_t heap_manager_t::add_object(TYPE type) throw (runtime_exception_t)
{
    object_t* obj = new object_t(type);
    return add_object(obj);
}

void heap_manager_t::remove_object(ref_t ref) throw (runtime_exception_t)
{
    delete this->get_object(ref);
    
    if(_heap.erase(ref) < 1) {
      runtime_exception_t e(REF_INVALID);
      e.get_message() += std::string(" ")+  std::string(__FILE__) +":" + std::to_string(__LINE__);
      throw e;      
    }
    RUNTIME_DEBUG("Object ref=%p deleted from heap", ref);
}

ref_t heap_manager_t::set_object(ref_t _ref, object_t* object) throw(runtime_exception_t)
{
  /* Delete old object */
  // delete this->get_object(_ref);
  /* Creating new pair*/
  std::pair<ref_t, object_t*> _pair(_ref, object);
  /*Set up new object*/
  _heap.erase(_ref);
  std::pair<heap_t::iterator, bool> status = _heap.insert(_pair);
  RUNTIME_DEBUG("Set up ref=%p to object type=%s, value=%s", _ref, 
		dt::type_to_string(object->get_type()), 
	       ( (const char*) *object));
  return _ref; 
}

object_t* heap_manager_t::get_object(ref_t ref) throw (runtime_exception_t)
{
  try { 
    return _heap.at(ref);
  } 
  catch (std::out_of_range)
  {
    runtime_exception_t e(REF_INVALID);
    e.get_message() += std::string(" ")+  std::string(__FILE__) +":" + std::to_string(__LINE__);
    throw e;
  }
}

void heap_manager_t::remove_object(std::map<std::string, ref_t>* scope) throw(runtime_exception_t)
{
  int count = 0;
  std::map<std::string, ref_t>::iterator i;
  for(i=scope->begin(); i!=scope->end(); i++)
  {
    this->remove_object((*i).second);
    count++;
  }
  
  RUNTIME_DEBUG("*\tGarbage collector! Clean up %d objects!\t*", count);
}

void heap_manager_t::increment_link_count(ref_t& ref)
{
  object_t* obj = this->get_object(ref);
  obj->link_count++;
  
  //std::cout << "Increment link count for '" << (const char*) (*obj) << "' link_count=" << obj->link_count << std::endl;
}

void heap_manager_t::decrement_link_count(ref_t& ref)
{
  object_t* obj = this->get_object(ref);
  obj->link_count--;
}

void heap_manager_t::gc()
{
  heap_t::iterator i=_heap.begin();
  for(; i!=_heap.end(); i++)
  {
    if( (*i).second->link_count == 0 )
    {
      //std::cout << "\t delete object '" << (const char*) (* (*i).second) << "' with reference " << (*i).first << std::endl;
      delete (*i).second;
      _heap.erase(i);
    }
  }
}


