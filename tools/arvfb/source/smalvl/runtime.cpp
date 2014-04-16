#include "runtime.h"
#include <typeinfo>

runtime_t::runtime_t() :
		_memmanager(heap_manager_t::get_instance())
{
	RUNTIME_DEBUG("Runtime instantiated");
}

/* Signleton implementation */
runtime_t* runtime_t::self = 0x0;

runtime_t* runtime_t::get_instance()
{
	if (self == 0x0)
	{
		self = new runtime_t();
	}
	return self;
}

function_declaration_t* runtime_t::get_function_declaration(std::string name)
		throw (runtime_exception_t)
{
	try
	{
		return _function_declarations.at(name);
	}
	catch (std::out_of_range e)
	{
		RUNTIME_DEBUG("Function '%s' don't exist in the global scope",
				name.c_str());
		throw runtime_exception_t(FUNCTION_NOT_DECL);
	}
}

void runtime_t::add_function_declaration(function_declaration_t* fd)
		throw (runtime_exception_t)
{
	RUNTIME_DEBUG("Added function declaration: %s", fd->get_name().c_str());
	std::pair<std::string, function_declaration_t*> entry(fd->get_name(), fd);
	std::pair<fun_map_t::iterator, bool> status = _function_declarations.insert(
			entry);
	if (status.second == false)
	{
		std::cerr << "Semantic error: Function " << fd->get_name()
				<< " redeclaration" << std::endl;
	}
}

void runtime_t::set_main_block(block_t* block)
{
	_main_block = block;
}

block_t* runtime_t::get_main_block()
{
	return _main_block;
}

ref_t runtime_t::run_function(function_call_t* fc, frame_stack_t& fs)
		throw (runtime_exception_t)
{
	RUNTIME_DEBUG("'%s' function call", fc->get_name().c_str());
	try
	{
		function_declaration_t* fun = get_function_declaration(fc->get_name());
		/* Function arguments scope */
		fs.push_back(new var_scope_t());
		/* Declaration arguments iterator */
		std::list<expr_t*>::iterator decl_args = fun->get_args().begin();
		/* Call function arguments iterator */
		std::list<expr_t*>::iterator call_args = fc->get_args().begin();

		for (;
				decl_args != fun->get_args().end()
						&& call_args != fc->get_args().end();
				decl_args++, call_args++)
		{
			ref_t ref = compute_expression(*call_args, fs);
			var_t* decl_var = dynamic_cast<var_t*>(*decl_args);
			assign_var(decl_var, ref, fs);
		}

		ref_t result_ref = run(fun->get_instructions(), fs);
		pop_frame_stack(fs);
		return result_ref;
	} catch (runtime_exception_t e)
	{
		if (e.getType() == FUNCTION_NOT_DECL)
		{
			ref_t result;
			bool rv = call_build_in_function(fc, fs, &result);
			if (rv)
			{
				return result;
			}
			else
			{
				return call_external_function(fc, fs);
			}
		}
		else
		{
			throw e;
		}
	}
}
static void HelloWorld(std::vector<object_t*>& objs)
{
	if (objs[0]->get_type() == STRING)
	{
		printf("$$: Hello World! %s\n", objs[0]->s->c_str());
	}
	else
	{

	}
}

bool runtime_t::call_build_in_function(function_call_t* fc, frame_stack_t& fs,
		ref_t* result) throw (runtime_exception_t)
{
	std::list<expr_t*>::iterator i = fc->get_args().begin();
	std::vector<object_t*> arg_objs;
	const char* name = fc->get_name().c_str();
	bool rv = true;

	RUNTIME_DEBUG("Preparing %d arguments", (int)fc->get_args().size());
	//coping function call arguments to ...
	for (; i != fc->get_args().end(); i++)
	{
		ref_t object_ref = compute_expression((*i), fs);

		arg_objs.push_back(_memmanager->get_object(object_ref));
	}RUNTIME_DEBUG("Try to call function from buildin libs");
	if (!strcasecmp(name, "HelloWorld"))
	{
		HelloWorld(arg_objs);
		*result = 0;
	}
	else
	{
		rv = false;
	}

	return rv;
}

ref_t runtime_t::call_external_function(function_call_t* fc, frame_stack_t& fs)
		throw (runtime_exception_t)
{
	std::list<expr_t*>::iterator i = fc->get_args().begin();
	std::vector<object_t*> arg_objs;
	RUNTIME_DEBUG("Preparing %d arguments", (int)fc->get_args().size());
	//coping function call arguments to ...
	for (; i != fc->get_args().end(); i++)
	{
		ref_t object_ref = compute_expression((*i), fs);

		arg_objs.push_back(_memmanager->get_object(object_ref));
	}RUNTIME_DEBUG("Try to call function from native libs");
	return _funcaller.call_function(fc, arg_objs);
}

bool runtime_t::semantic_analys()
{
	return true;
}

void runtime_t::interpretate()
{
	frame_stack_t fs;

	try
	{
		runtime_t::get_instance()->run(this->_main_block, fs);
	} catch (runtime_exception_t e)
	{
		std::cerr << "Fatal rutime error: ";
		e.print();
	}
}

ref_t runtime_t::run(block_t* block, frame_stack_t& fs)
		throw (runtime_exception_t)
{
	RUNTIME_DEBUG("Runing block: instuctions count is %d", block->size());
	fs.push_back(new var_scope_t());

	instructions_list_t* instructions = block->get_operators();
	//print_frame_stack(fs);
	for (instructions_list_t::iterator i = instructions->begin();
			i != instructions->end(); i++)
	{
		//TODO: return
		oper_t* instruction = (*i);
		run_instruction(instruction, fs);
	}
	//cleaning stack
	pop_frame_stack(fs);
	return 0x0;
}

ref_t runtime_t::run_instruction(oper_t* instruction, frame_stack_t& fs)
		throw (runtime_exception_t)
{
#define ti typeid(*instruction)
	if ( ti == typeid(block_t))
	{
		return run(dynamic_cast<block_t*>(instruction), fs);
	}
	else if (ti == typeid(function_call_t))
	{
		function_call_t *fc = reinterpret_cast<function_call_t*>(instruction);
		return run_function(fc, fs);
	}
	else if (ti == typeid(for_op_t))
	{
		for_op_t* for_op = dynamic_cast<for_op_t*>(instruction);

		run_instruction(for_op->get_init_op(), fs);
		object_t* condition_obj = _memmanager->get_object(
				compute_expression(for_op->get_condition(), fs));

		for (; (bool) (*condition_obj);
				condition_obj = _memmanager->get_object(
						compute_expression(for_op->get_condition(), fs)))
		{
			RUNTIME_DEBUG("While ( %s ) cycle", ((const char*) *condition_obj));
			ref_t ref = run(for_op->get_block(), fs);
			if (ref != REF_IS_VOID)
				return ref;
			run_instruction(for_op->get_iter_op(), fs);
		}

		return REF_IS_VOID;
	}
	else if ( ti == typeid(while_op_t))
	{

		while_op_t* while_op = dynamic_cast<while_op_t*>(instruction);
		object_t* condition_obj = _memmanager->get_object(
				compute_expression(while_op->get_condition(), fs));

		for (; (bool) (*condition_obj);
				condition_obj = _memmanager->get_object(
						compute_expression(while_op->get_condition(), fs)))
		{
			RUNTIME_DEBUG("While ( %s ) cycle", ((const char*) *condition_obj));
			run(while_op->get_block(), fs);
		}
	}
	else if ( ti == typeid(if_op_t))
	{
		if_op_t* if_op = dynamic_cast<if_op_t*>(instruction);
		object_t* condition_obj = _memmanager->get_object(
				compute_expression(if_op->get_condition(), fs));
		RUNTIME_DEBUG("If/else ( %s ) statement",
				((const char*) *condition_obj));
		if ((bool) (*condition_obj))
		{
			return run(if_op->get_then_instr(), fs);
		}
		else if (if_op->get_else_instr() != 0x0)
			return run(if_op->get_else_instr(), fs);

	}
	else if ( ti == typeid(require_t))
	{
		require_t* r = dynamic_cast<require_t*>(instruction);
		ref_t ref = compute_expression(r->get_value(), fs);
		_funcaller.open_extension((const char*) *_memmanager->get_object(ref));
	}
	else if ( ti == typeid(break_op_t))
	{
		return 0x0;
	}
	else if ( ti == typeid(return_op_t))
	{
		RUNTIME_DEBUG("Return value");
		return_op_t* ret = dynamic_cast<return_op_t*>(instruction);
		ref_t ref = compute_expression(ret->get_value(), fs);
		pop_frame_stack(fs);
		return ref;
	}
	else if ( ti == typeid(function_declaration_t))
	{
		/* Nothing to do */
	}
	else if ( ti == typeid(include_t))
	{
		include_t* r = dynamic_cast<include_t*>(instruction);
		(void) r;
	}
	else if ( ti == typeid(assign_t))
	{
		assign_t *a = dynamic_cast<assign_t*>(instruction);
		var_t* var = a->get_var();
		expr_t* value = a->get_value();
		assign_var(var, value, fs);
	}
	else if ( ti == typeid(unary_t))
	{
		unary_t* expr = reinterpret_cast<unary_t*>(instruction);
		compute_unary_expression(expr, fs);
	}
	else if ( ti == typeid(unset_t))
	{
		unset_t* unset = dynamic_cast<unset_t*>(instruction);
		unset_var(unset->get_var(), fs);

	}
	return REF_IS_VOID;
}

ref_t runtime_t::compute_expression(expr_t* expr, frame_stack_t& fs)
		throw (runtime_exception_t)
{
	if (typeid(*expr) == typeid(binary_t))
	{
		/* Math operations */
		binary_t* bin = (binary_t*) (expr);
		object_t* arg1 = _memmanager->get_object(
				compute_expression(bin->get_operand(0), fs));
		object_t* arg2 = _memmanager->get_object(
				compute_expression(bin->get_operand(1), fs));
		return compute_math_expression(arg1, bin->get_operator(), arg2);
	}
	else if (typeid(*expr) == typeid(unary_t))
	{
		unary_t* unar_op = reinterpret_cast<unary_t*>(expr);
		return compute_unary_expression(unar_op, fs);
	}
	else if (typeid(*expr) == typeid(function_call_t))
	{
		function_call_t *fc = reinterpret_cast<function_call_t*>(expr);
		return run_function(fc, fs);
	}
	else if (typeid(*expr) == typeid(var_t))
	{
		var_t* var = dynamic_cast<var_t*>(expr);
		return get_var_ref(var, fs);
	}
	else if (typeid(*expr) == typeid(array_t))
	{
		var_t* var = dynamic_cast<var_t*>(expr);
		return get_var_ref(var, fs);
	}
	else if (typeid(*expr) == typeid(array_init_t))
	{
		ref_t obj_ref = _memmanager->add_object(ARRAY);
		object_t *obj = _memmanager->get_object(obj_ref);
		obj->a = new array_obj_t();
		return obj_ref;
	}
	else if (typeid(*expr) == typeid(value_t))
	{
		value_t* value = dynamic_cast<value_t*>(expr);
		//RUNTIME_DEBUG("value_t  %s", ( (const char*) *value->get_value()));
		/* Placing object in heap */
		object_t* explicit_obj = acs::create_object(value->get_value());
		/* Add new object to heap */
		ref_t ref = _memmanager->add_object(explicit_obj);
		/* Increment link count. Will be const count=1 */
		_memmanager->increment_link_count(ref);
		return ref;
	}

	return 0x0;
}

ref_t runtime_t::compute_math_expression(object_t* obj1, const char* op,
		object_t* obj2) throw (runtime_exception_t, std::invalid_argument)
{

	RUNTIME_DEBUG("Binary operation %s computing, ojects types %s %s", op,
			dt::type_to_string(obj1->get_type()),
			dt::type_to_string(obj2->get_type()));

	if (op[0] == '.')
	{
		RUNTIME_DEBUG("\tstring '%s' and '%s' concetate",
				((std::string) *obj1).data(), ((std::string) *obj2).data());
		ref_t ref = _memmanager->add_object(STRING);
		object_t* object = _memmanager->get_object(ref);
		object->s = new std::string((std::string) *obj1 + (std::string) *obj2);
		return ref;
	}
	/*
	 * Perform operations only on INTEGERs and FLOATPOINTs types
	 */
	else if ((strchr("+-/%*", op[0]) != NULL)
			&& (obj1->get_type() == INTEGER || obj1->get_type() == FLOATPOINT)
			&& (obj2->get_type() == INTEGER || obj2->get_type() == FLOATPOINT))
	{
		TYPE target_type = FLOATPOINT;

		if (obj1->get_type() != FLOATPOINT && obj2->get_type() != FLOATPOINT)
			target_type = INTEGER;

		ref_t ref = _memmanager->add_object(target_type);
		object_t* object = _memmanager->get_object(ref);

		int ir;
		double dr;

		switch (op[0])
		{
			case '+':
				if (target_type == INTEGER)
					ir = (int) *obj1 + (int) *obj2;
				else
					dr = (double) *obj1 + (double) *obj2;
				break;
			case '-':
				if (target_type == INTEGER)
					ir = (int) *obj1 - (int) *obj2;
				else
					dr = (double) *obj1 - (double) *obj2;
				break;
			case '*':
				if (target_type == INTEGER)
					ir = (int) *obj1 * (int) *obj2;
				else
					dr = (double) *obj1 * (double) *obj2;
				break;
			case '/':
				if (target_type == INTEGER)
					ir = (int) *obj1 / (int) *obj2;
				else
					dr = (double) *obj1 / (double) *obj2;
				break;
			case '%':
				ir = (int) *obj1 + (int) *obj2;
				dr = ir;
				break;
			default:
				assert(0);
				break;
		}
		/* Fill object union*/
		if (target_type == INTEGER)
		{
			object->i = ir;
			RUNTIME_DEBUG("Binary operation result = %d", ir);
		}
		else
		{
			object->d = new double(dr);
			RUNTIME_DEBUG("Binary operation result = %lf", dr);
		}
		return ref;

	}
	else if (strchr("<>!=", op[0]) != NULL)
	{
		ref_t ref = _memmanager->add_object(BOOL);
		object_t* obj = _memmanager->get_object(ref);

		if (!strcmp("<", op))
		{
			obj->b = (*obj1) < (*obj2);
		}
		else if (!strcmp("<=", op))
		{
			obj->b = (*obj1) <= (*obj2);
		}
		else if (!strcmp(">", op))
		{
			obj->b = (*obj1) > (*obj2);
		}
		else if (!strcmp(">=", op))
		{
			obj->b = (*obj1) >= (*obj2);
		}
		else if (!strcmp("!=", op))
		{
			obj->b = (*obj1) != (*obj2);
		}
		else if (!strcmp("==", op))
		{
			obj->b = (*obj1) == (*obj2);
		}
		return ref;
	}

	throw runtime_exception_t(INVALID_COVERSION);
}

ref_t runtime_t::compute_unary_expression(unary_t* expr, frame_stack_t& fs)
		throw (runtime_exception_t, std::invalid_argument)
{
	const char* op = expr->get_operator();

	if (!strcmp(op, "++"))
	{
		//TODO: safe delete object
		//TODO: do all actions in heap
		assign_t *assign = new assign_t(expr->get_value(),
				new binary_t("+", expr->get_value(), new value_t("1")));
		run_instruction(assign, fs);
		return compute_expression(expr->get_value(), fs);
	}
	else if (!strcmp(op, "--"))
	{
		//TODO: safe delete object
		//TODO: do all actions in heap
		assign_t *assign = new assign_t(expr->get_value(),
				new binary_t("-", expr->get_value(), new value_t("1")));
		run_instruction(assign, fs);
		return compute_expression(expr->get_value(), fs);
	}
	else
	{
		assert(0);
	}
	return REF_IS_VOID;
}

ref_t runtime_t::create_var(var_t* var, ref_t& ref, frame_stack_t& fs)
{
	/* Get current local var scope */
	var_scope_t* var_scope = fs.back();
	/* Inset in local scope var*/
	std::pair<std::string, ref_t> _pair(var->get_name(), ref);
	var_scope->insert(_pair);
	RUNTIME_DEBUG("Insert var $%s refer to %p to scope", var->get_name().data(),
			ref);
	return ref;
}

ref_t& runtime_t::get_var_ref(var_t* var, frame_stack_t& fs, ref_t default_ref)
		throw (runtime_exception_t)
{
	frame_stack_t::iterator i = fs.begin();
	int indent = 0;

	for (; i != fs.end(); i++)
	{
		var_scope_t* var_scope = (*i);
		try
		{
			ref_t ref = var_scope->at(var->get_name());
			//RUNTIME_DEBUG("Var $%s reference found in frame stack at %d level", var->get_name().data(), indent);
			object_t* obj = _memmanager->get_object(ref);
			/* We work with array */
			if (typeid(*var) == typeid(array_t))
			{
				array_t *array = dynamic_cast<array_t*>(var);
				array_obj_t* array_map = obj->a;
				ref_t key_value = compute_expression(array->get_index(), fs);
				std::string key = (std::string) (*_memmanager->get_object(
						key_value));

				//in case if in array does't exist key
				if (array_map->find(key) == array_map->end()
						&& default_ref != 0x0)
				{
					std::pair<std::string, ref_t> _pair(key, default_ref);
					array_map->insert(_pair);
				}
				else if (array_map->find(key) == array_map->end())
				{
					runtime_exception_t e(VAR_NOT_DECL);
					e.get_message() = "Array $";
					e.get_message() += var->get_name();
					e.get_message() += ". Out of bound exception. Key=" + key;
					throw e;
				}
				return (array_map->at(key));
			}
			else
				return var_scope->at(var->get_name());

		}
		catch (std::out_of_range e)
		{

		}
		indent++;
	}

	//RUNTIME_DEBUG("Var $%s reference not found in frame stack at all %d levels", var->get_name().data(), indent);
	runtime_exception_t e(VAR_NOT_DECL);
	e.get_message() = "Var $";
	e.get_message() += var->get_name();
	e.get_message() += " not declared in var scope";
	throw e;
}

object_t* runtime_t::get_var_value(var_t* var, frame_stack_t& fs)
		throw (runtime_exception_t)
{
	ref_t ref = get_var_ref(var, fs);
	object_t* obj = _memmanager->get_object(ref);

	if (typeid(*var) == typeid(array_t))
	{
		ref_t ref = get_var_ref(var, fs);
//        RUNTIME_DEBUG("* get_var_value( $%s[%s]) type = %s, value = %s ", array->get_name().data(),
//                      key.data(),
//                      dt::type_to_string(array_item->get_type()), ((const char*) *array_item));
		/* array_item point to element in to array map */
		obj = _memmanager->get_object(ref);
	}

	RUNTIME_DEBUG("* get_var_value( $%s ), type = %s, value = %s ",
			var->get_name().data(), dt::type_to_string(obj->get_type()),
			((const char*) *obj));
	return obj;
}

void runtime_t::pop_frame_stack(frame_stack_t& fs)
{
	clean_varscope(fs.back());
	gc(fs);
	delete fs.back();
	fs.pop_back();
}

void runtime_t::gc(frame_stack_t& fs) throw (runtime_exception_t)
{
	_memmanager->gc();
}

void runtime_t::assign_var(var_t* var, expr_t* value, frame_stack_t& fs)
{
	ref_t ref = compute_expression(value, fs);
	assign_var(var, ref, fs);
}

void runtime_t::assign_var(var_t* var, ref_t& value_ref, frame_stack_t& fs)
{
	//if var is indexed array item
	if (typeid(*var) == typeid(array_t))
	{
		try
		{
			array_t* array = dynamic_cast<array_t*>(var);
			(void) array; // TODO
			try
			{
				_memmanager->decrement_link_count(get_var_ref(var, fs));
			} catch (runtime_exception_t re)
			{
				/* index don't exist in array. */
				_memmanager->increment_link_count(value_ref);
				get_var_ref(var, fs, value_ref);
				return;
			}
			_memmanager->increment_link_count(value_ref);
			get_var_ref(var, fs) = value_ref;
		} catch (runtime_exception_t e)
		{
			/* If not exist, create in local scope */
			if (e.getType() == VAR_NOT_DECL)
			{
				//TODO: init array;
				throw e;
			}
			/* Else another unexpected exception */
			else
			{
				throw e;
			}
		} catch (std::out_of_range e1)
		{
			throw std::out_of_range("Assigned uninit array element");
		}

	}
	else
	{
//        RUNTIME_DEBUG("Assign $%s type=%s, value=%s", var->get_name().data(),
//                      dt::type_to_string(obj->get_type()),
//                      (const char*) *obj);
		/* Search var in frame stack */
		try
		{
			_memmanager->decrement_link_count(get_var_ref(var, fs));
			get_var_ref(var, fs) = value_ref;
			_memmanager->increment_link_count(value_ref);
		}
		/* Adding var in frame stack */
		catch (runtime_exception_t e)
		{
			/* If not exist, create in local toplevel scope */
			if (e.getType() == VAR_NOT_DECL)
			{
				this->create_var(var, value_ref, fs);
				_memmanager->increment_link_count(value_ref);
				/* And add object to heap*/
			}
			/* Else another unexpected exception */
			else
				throw e;
		}
	}
}

void runtime_t::print_frame_stack(frame_stack_t& fs)
{
	frame_stack_t::iterator i = fs.begin();
	int indent = 0;

	std::cout << "Printing var scope" << std::endl;

	for (; i != fs.end(); i++)
	{
		var_scope_t::iterator var_iterator = (*i)->begin();
		for (; var_iterator != (*i)->end(); var_iterator++)
		{
			for (int j = 0; j < indent; j++)
				std::cout << " ";

			std::cout << (*var_iterator).first << "="
					<< (const char*) (*_memmanager->get_object(
							(*var_iterator).second)) << "\t"
					<< (*var_iterator).second << std::endl;
		}

		indent++;
	}
}

void runtime_t::clean_varscope(var_scope_t* vs)
{
	var_scope_t::iterator i = vs->begin();
	for (; i != vs->end(); i++)
	{
		_memmanager->decrement_link_count((*i).second);
	}
}

void runtime_t::unset_var(std::string name, frame_stack_t& fs)
{
	frame_stack_t::iterator i = fs.begin();
	for (; i != fs.end(); i++)
	{
		var_scope_t::iterator var_iterator = (*i)->begin();
		for (; var_iterator != (*i)->end(); var_iterator++)
		{
			if (name == (*var_iterator).first)
			{
				//decrease link count to object
				_memmanager->decrement_link_count((*var_iterator).second);
				//delete from var scope map
				(*i)->erase(var_iterator);
			}
		}
	}
}

void runtime_t::unset_var(var_t* var, frame_stack_t& fs)
{
	unset_var(var->get_name(), fs);
}

