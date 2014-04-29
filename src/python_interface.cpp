
#include "python_interface.hpp"

#include "mCellTest.h"
#include "support/average.h"
#include "support/config.h"
/*
python_data_table::python_data_table()
	: super()
	{
	}

python_data_table::python_data_table(const super & t)
	: super(t)
	{
	}

python_data_table::python_data_table(const super * t)
	: super(t)
	{
	}
python_data_table::python_data_table(const python_data_table & t)
	: super(t)
	{
	}

table_slice python_data_table::slice_from_slice(boost::python::slice & s, int max)
{
	table_slice ret;
		ret.start = 0;
		ret.stop = max;
		ret.step = 1;

		if(max == 0)
		{
			return ret;
		}

				if(boost::python::extract<long>(s.start()).check())
				{
					ret.start = boost::python::extract<long>(s.start()) % ret.stop;
				}
				if(boost::python::extract<long>(s.step()).check())
				{
					ret.step = boost::python::extract<long>(s.step()) % ret.stop;;
				}
				if(boost::python::extract<long>(s.stop()).check())
				{
					ret.stop = boost::python::extract<long>(s.stop()) % ret.stop;
				}

		return ret;
	}

	python_data_table python_data_table::get_slice(boost::python::slice & s)
	{
		return select_column(slice_from_slice(s, no_columns_selected()));
	}

	template<__op op>
	expression python_data_table::compare_python_table(const python_data_table & t)
	{
		return compare_table(op, t);
	}

BOOST_PYTHON_MODULE(data_tables)
{
	 using namespace boost::python;
	 typedef python_data_table data_table_type;
	 typedef test_instance<data_table_type> test_type;


class_<expression, shared_ptr<expression> >("rxn_expression", no_init)
		.def("truth", &expression::get_bool)
		.def("__bool__", &expression::get_bool)
		.def("__nonzero__", &expression::get_bool)
	;

	struct local
	{
		template<typename U, typename V>
		U & add_comparisions(U & u)
		{
			u.def("__eq__", &U::get_expression<__eq__, V>)
			 .def("__ne__", &U::get_expression<__ne__, V>)
			 .def("__lt__", &U::get_expression<__lt__, V>)
			 .def("__le__", &U::get_expression<__le__, V>)
			 .def("__gt__", &U::get_expression<__gt__, V>)
			 .def("__ge__", &U::get_expression<__ge__, V>)
			 ;

			return u;
		}

		template<typename self, typename it, typename return_type>
		void add_arithmatic(self & u)
		{
#define add_transitive_operator(token, use) \
			 def( "__"#token"__", &self::use<it, return_type, true>)\
			.def("__r"#token"__", &self::use<it, return_type, true>)

#define add_intransitive_operator(token, use) \
			 def( "__"#token"__", &self::use<it, return_type>)\
			.def("__r"#token"__", &self:: r##use <it, return_type>)

			u.add_transitive_operator(add, add)
			 .add_intransitive_operator(sub, sub)
			 .add_transitive_operator(mul, mul)
			 .add_intransitive_operator(div, div)
			 .add_intransitive_operator(div, truediv)
			 .add_intransitive_operator(floordiv, floordiv)
			 .def("__neg__", &U::__neg__)
			 .def("__pos__", &U::__pos__)
			 .def("__abs__", &U::__abs__)
			 ;
#undef add_transitive_operator
#undef add_intransitive_operator
		}

		template<typename self, typename it, typename return_type>
		void add_same_type_arithmatic(U & u)
		{
			add_arithmatic<U,V>(u);
			u.def("__rpow__", &U::__pow__<V>)
			 .def("__rmod__", &U::__mod__<V>)
			 .def("__rlshift__", &U::__lshift__<V>)
			 .def("__rrshift__", &U::__rshift__<V>)
			 ;
		}

		template<typename U>
		void def_data_column(U & u)
		{
			u.def(vector_indexing_suite<U>());
			add_arithmatic<U, int>(u);
			add_arithmatic<U, double>(u);
			add_arithmatic<data_column<average>, average>(u);
			add_same_type_arithmatic<U, data_column<double> >(u);
			add_same_type_arithmatic<data_column<average>, data_column<average> >(u);
		}

	};
#define comparison_dec(op, type) \
	.def( #op , &data_table_type::compare< op , type > )

#define comparison_declaration(type) \
	comparison_dec(__eq__ , type) \
	comparison_dec(__ne__ , type) \
	comparison_dec(__lt__ , type) \
	comparison_dec(__le__ , type) \
	comparison_dec(__gt__ , type) \
	comparison_dec(__ge__ , type)

#define comp_exp( op ) \
	.def( #op , &data_table_type::py_compare_list< op > )\
	.def( #op , &data_table_type::compare_python_table< op > )


class_<data_column<T>, shared_ptr<data_column<T> > > & column_definition
= class_<data_column<T>, shared_ptr<data_column<T> > >(class_name, no_init)






class_<data_table_type, shared_ptr<data_table_type> >("data_table", no_init)
		.add_property("min_time", &data_table_type::get_min_time, &data_table_type::apply_min_time)
		.add_property("max_time", &data_table_type::get_max_time, &data_table_type::apply_max_time )
		.add_property("min_row", &data_table_type::get_min_row, &data_table_type::set_min_row)
		.add_property("max_row", &data_table_type::get_max_row, &data_table_type::set_max_row )
		.add_property("base_time", &data_table_type::get_base_time, &data_table_type::apply_base_time )
		.add_property("total_columns", &data_table_type::total_columns)
		.add_property("selected_columns", &data_table_type::no_columns_selected)
		.add_property("total_rows", &data_table_type::total_rows)
		.add_property("selected_rows", &data_table_type::no_rows_selected)
		.def("rate", &data_table_type::set_base_time)
		.def("to", &data_table_type::set_max_time )
		.def("before", &data_table_type::set_max_time )
		.def("from", &data_table_type::set_min_time )
		.def("after", &data_table_type::set_min_time )
		.def("__getitem__", &data_table_type::get_slice)
		.def("__getitem__", &data_table_type::select_column<string>)
		.def("__getitem__", &data_table_type::select_column<int>)
		comparison_types
		comp_exp(__eq__)
		comp_exp(__ne__)
		comp_exp(__lt__)
		comp_exp(__le__)
		comp_exp(__gt__)
		comp_exp(__ge__)
		.def("__add__", &data_table_type::add )
		.def("__sub__", &data_table_type::sub )
		.def("__mul__", &data_table_type::multiply )
		.def("__div__", &data_table_type::divide )
		.def("__truediv__", &data_table_type::divide )
		.def("__rmul__", &data_table_type::multiply )
		.def("__rdiv__", &data_table_type::divide )
		.def("__rtruediv__", &data_table_type::divide )
	;

#undef comp_exp
#undef data_table_type

FILE * get_stdout();
FILE * get_stderr();



class_<test_type, shared_ptr<test_type> >("McellTest", init<std::string>(args("name")))
		.def(init<std::string, bool>(args("name", "quiet")))
		.add_property("exit_code", &test_type::get_exit_code)
		.add_property("signal", &test_type::get_signal)
		.add_property("name", &test_type::get_name)
		.add_property("stdout", &test_type::get_stdout)
		.add_property("stderr", &test_type::get_stderr)
		.def(init<std::string, bool>(args("name", "quiet")))
		.def(init<std::string, int>(args("name", "iterations")))
		.def(init<std::string, bool, int>(args("name", "quiet", "iterations")))
		.def("__getitem__", &test_type::GetReactionOutput, return_value_policy<reference_existing_object>())
		.def("reaction_output", &test_type::GetReactionOutput, return_value_policy<reference_existing_object>())
		.def("trigger_output", &test_type::GetTriggerOutput, return_value_policy<reference_existing_object>())
		;

class_<equilibrium, shared_ptr<equilibrium> >("equilibrium", init<double, double>())
		;

class_<average, shared_ptr<average> >("average", init<double, double>())
		;
};

python::python(int argc, const char * argv[])
{
try {
	Py_SetProgramName(const_cast<char*>(argv[0]));
	Py_Initialize();
	PySys_SetArgv(argc, const_cast<char**>(argv));
	PyImport_AppendInittab((char*)"data_tables", initdata_tables);
	main_module = boost::python::import("__main__");
	global = boost::python::extract<boost::python::dict>(main_module.attr("__dict__"));
	sys_path = PySys_GetObject(const_cast<char *>("path"));
	PyObject * data_tables = PyImport_ImportModule("data_tables");
	global["rxn_expression"] = boost::python::handle<>(PyObject_GetAttrString(data_tables, "rxn_expression"));
	global["data_table"] = boost::python::handle<>(PyObject_GetAttrString(data_tables, "data_table"));
	global["McellTest"] = boost::python::handle<>(PyObject_GetAttrString(data_tables, "McellTest"));
	global["equilibrium"] = boost::python::handle<>(PyObject_GetAttrString(data_tables, "equilibrium"));
	global["average"] = boost::python::handle<>(PyObject_GetAttrString(data_tables, "average"));
} catch (boost::python::error_already_set& e) {
	PyErr_PrintEx(0);
}

if (PyErr_Occurred())
{
	PyErr_Print();
	exit(-1);
}
}

python::~python()
{
	Py_Finalize();
}

void python::run_script(const char * path)
{
	FILE * py = fopen(path, "r");

	if(py != NULL)
	{
		PyRun_SimpleFile(py, path);
		fclose(py);
	}
}

void python::insert_path(const char * path)
{
	PyList_Insert(sys_path, 0, PyString_FromString(path));
}

*/
