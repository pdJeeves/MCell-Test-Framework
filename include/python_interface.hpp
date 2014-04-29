#ifndef _python_interface_h_
#define _python_interface_h_
#include <boost/python.hpp>
#include <boost/python/slice.hpp>
#include "script_manager.h"

#define SCRIPTING_ENGINE python

class python : public virtual script_interface
{
public:
	python(int argc, const char * argv[]);
	~python();
	boost::python::object main_module;
	boost::python::object global;
	PyObject * sys_path;

	void run_script(const char * path);
	void import_class_(PyObject *, const char * name);
	void insert_path(const char * path);
};


#endif
