#include "funcaller.h"
#include <fstream>

funcaller_t::funcaller_t()
{
}

funcaller_t::~funcaller_t()
{
    extensions_list_t::iterator i;
    for(i=extensions.begin(); i!= extensions.end(); i++)
    {
        //TODO:
    }
}

bool funcaller_t::open_extension(const char* name) throw (nativelib_exception_t)
{
    std::ifstream fs;
    fs.open( name );

    if(!fs.is_open())
    {
        return false;
    }

    lib_extension_t ext;
    std::getline(fs, ext.lib_name);

    RUNTIME_DEBUG("Loading native extension %s", ext.lib_name.data());
    void* handle = dlopen(ext.lib_name.data(), RTLD_LAZY);

    if(handle == NULL) {
        std::cerr << dlerror() << std::endl;
        return false;
    }

    ext.handler = handle;

    while( !fs.eof() ) {
        std::string buffer;
        std::getline(fs, buffer);

        if(buffer.empty())
            continue;
        if(buffer.find_first_of("//") == 0)
            continue;

        meta_info_t meta = this->parse_meta(buffer.data());
        //this->print_meta(meta);
        ext.functions_list.push_back(meta);
    }

    extensions.push_back(ext);
}

meta_info_t funcaller_t::parse_meta(const char* line) throw(nativelib_exception_t)
{
    int b;
    bool token_start = false;
    char token[128];
    int t=0;
    meta_info_t meta;

    for(int i=0; line[i]; i++)
    {
        if(!token_start) {
            if(std::isalnum(line[i]) || line[i]=='.') {
                token_start = true;
                b=i;
                continue;
            }
        }
        else {
            if( !std::isalnum(line[i]) && line[i]!='.')
            {
                token_start = false;
                /* Result token */
                memset(token, 0, sizeof(token));
                memcpy(token, line+b, i-b);
                TYPE type = dt::string_to_type(token);

                if(t == 0)
                {
                    meta.return_type = type;
                }
                else if(t==1)
                {
                    meta.name = std::string(token);
                }
                else
                    meta.argument_types.push_back(type);

                t++;

                continue;
            }
        }
    }
    return meta;
}

ref_t funcaller_t::call(void* faddr,  const meta_info_t& meta, std::vector<object_t*>& objs)
throw(runtime_exception_t)
{
    if(objs.size() != meta.argument_types.size() && meta.argument_types.back()!= VA )
        throw runtime_exception_t(std::string("Function ") +meta.name +" invalid argumets count", FUNCTION__CALL_INVALID_ARGUMENTS);

    /* Arguments count = 0 */
    if(objs.size() == 0)
    {
        if(meta.return_type == NONE)
        {
            void (*fun)() = (void (*)()) faddr;
            fun();
            return 0x0;
        }
        if(meta.return_type == INTEGER )
        {
            int(*fun)() = (int (*)()) faddr;
            ref_t ref  = heap_manager_t::get_instance()->add_object(INTEGER);
            object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	    obj->i = fun();
            return ref;
        }
        else if(meta.return_type == STRING)
        {
            const char* (*fun)() = (const char* (*)()) faddr;
            ref_t ref = heap_manager_t::get_instance()->add_object(STRING);
	    object_t* obj = heap_manager_t::get_instance()->get_object(ref);
            obj->s = new std::string(fun());
            return ref;
        }
        else if(meta.return_type == FLOATPOINT)
        {
            double(*fun)() = (double (*)()) faddr;
            ref_t ref = heap_manager_t::get_instance()->add_object(FLOATPOINT);
            object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	    obj->d = new double(fun());
            return ref;
        }
    }
    /* Arguments count = 1 */
    else if(objs.size() == 1)
    {
	//fun(const char*)
        if(meta.argument_types[0] == STRING)
        {
            if(meta.return_type == INTEGER )
            {
                int(*fun)(const char*) = (int (*)(const char*)) faddr;
                ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
                object_t* obj = heap_manager_t::get_instance()->get_object(ref);
		obj->i = fun( ((const char*) *objs[0]) );
                return ref;
            }
            else if(meta.return_type == NONE) {
	      void (*fun)(const char*) = (void (*)(const char*)) faddr;
	      fun( (const char*) *objs[0]);
	      return 0x0;
	    }
	    else if( meta.return_type == STRING )
	    {
	      char buffer[BUFFER_SIZE];
	      memset(buffer, 0x0, sizeof(buffer));
	      const char* src = (const char*) *objs[0];
	      memcpy(buffer, src, strlen(src));
	      char* (*fun)(char*) = (char* (*)(char*)) faddr;
	      ref_t ref = heap_manager_t::get_instance()->add_object(STRING);
	      object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	      obj->s = new std::string( fun(buffer) );
	      return ref;
	    }
        }
        //fun(int)
        else if(meta.argument_types[0] == INTEGER)
        {
            if(meta.return_type == INTEGER )
            {
	        int(*fun)(int) = (int (*)(int)) faddr;
                ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
                object_t* obj = heap_manager_t::get_instance()->get_object(ref);
		obj->i = fun( (int) *objs[0] );
                return ref;
            }
        }
    }
    /*Arguments count == 2 */
    else if(objs.size() == 2)
    {
      if(meta.return_type == INTEGER)
      {
	if(meta.argument_types[0] == STRING && meta.argument_types[1] == STRING)
	{
	  int (*fun)(const char*, const char*) = ( int (*)(const char*, const char*) ) faddr;
	  ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
	  object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	  RUNTIME_DEBUG("Call function %s(\"%s\", \"%s\")", meta.name.data(), (const char*) *objs[1], (const char*) *objs[0]);
	  obj->i = fun( (const char*) *objs[1], (const char*) *objs[0]);
	  return ref;
	}
	else if(meta.argument_types[0] == STRING && meta.argument_types[1] == INTEGER)
	{
	  int (*fun)(const char*, int) = ( int (*)(const char*, int) ) faddr;
	  ref_t ref = heap_manager_t::get_instance()->add_object(INTEGER);
	  object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	  RUNTIME_DEBUG("Call function %s(\"%s\", \"%p\")", meta.name.data(), (const char*) *objs[1], (int) *objs[0]);
	  obj->i = fun( (const char*) *objs[1], (int) *objs[0]);
	  return ref;
	}
      }
    }
    /* Arguments count == 3 */
    else if(objs.size() == 3)
    {
      if(meta.return_type == STRING)
      {
	if(meta.argument_types[0] == STRING && meta.argument_types[1] == INTEGER && meta.argument_types[2] == INTEGER)
	{
	  const char* (*fun)(const char*, int, int) = ( const char* (*)(const char*, int, int) ) faddr;
	  int size = (int) *objs[1];
	  char buffer[size];
	  memset(buffer, 0x0, size);
	  fun( buffer, size, (int) *objs[0]);
	  //TODO: memcontrol
	  ref_t ref = heap_manager_t::get_instance()->add_object(STRING);
	  object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	  obj->s = new std::string(buffer);
	  return ref;
	}
      }
    }
    /* Arguments count = 4 */
    else if(objs.size() == 4)
    {
	if(meta.return_type == INTEGER )
	{
	  if(meta.argument_types[0] == STRING && meta.argument_types[1] == INTEGER && 
	    meta.argument_types[2] == INTEGER && meta.argument_types[3] == INTEGER )
	  {
	    int (*fun)(const char*, int, int, int) = (int (*)(const char*, int, int, int)) faddr;
	    ref_t ref = heap_manager_t::get_instance()->add_object(meta.return_type);
	    object_t* obj = heap_manager_t::get_instance()->get_object(ref);
	    obj->i = fun( (const char*) *objs[0], (int) *objs[1], (int) *objs[2], (int) *objs[3]);
	    return ref;
	  }
	}
    }

    //TODO:
    runtime_exception_t e(FUNCTION__CALL_INVALID_ARGUMENTS);
    e.get_message() += std::string(" ") +std::string(__FILE__) +std::string("::") + std::to_string(__LINE__) +"::  ";
    e.get_message() += dt::type_to_string(meta.return_type);
    e.get_message() += " ";
    e.get_message() += meta.name;
    e.get_message() += " (";
      for(int i=0; i<objs.size(); i++)
      {
	e.get_message() += dt::type_to_string(meta.argument_types[i]);
	e.get_message() += " ";
      }
    e.get_message() += ") ";
    throw e;
}


ref_t funcaller_t::call_function(function_call_t* fc, std::vector<object_t*>& objs) throw(runtime_exception_t)
{
    /* Clear any existing error*/

    std::list<meta_info_t>::iterator fun_meta;

    for( extensions_list_t::iterator i=extensions.begin(); i!= extensions.end(); i++)
    {
        for(fun_meta = (*i).functions_list.begin(); fun_meta != (*i).functions_list.end(); fun_meta++)
        {
            //std::cout << "*\t" << "looking " << (*fun_meta).name << std::endl;

            if( (*fun_meta).name == fc->get_name())
            {
                dlerror();
                void* handle = (*i).handler;
                void* fun_addr = dlsym(handle, (*fun_meta).name.c_str());

                if( dlerror() == NULL )
                {
                    RUNTIME_DEBUG("Call function %s from native lib", (*fun_meta).name.c_str());
                    return call(fun_addr, (*fun_meta), objs);
                }
            }
        }
    }
    /*Preparing exception message */
    runtime_exception_t e(FUNCTION_NOT_DECL);
    e.get_message() += std::string(" '") +fc->get_name() +std::string("' ");
    throw e;
}

void funcaller_t::print_meta(meta_info_t& meta) {
    std::cout << "*\tPrinting meta information for function:" << meta.name << std::endl;
    std::vector<TYPE>::iterator i =meta.argument_types.begin();

    std::cout << "*\tfunction return type " << dt::type_to_string(meta.return_type) << std::endl;
    std::cout << "*\targuments type" << std::endl;

    for(; i!= meta.argument_types.end(); i++)
    {
        std::cout << "*\t\t" << dt::type_to_string((*i)) << std::endl;
    }

    std::cout << std::endl;
}
