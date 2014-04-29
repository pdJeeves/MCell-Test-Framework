#include "python_interface.hpp"
#include "double_table_reader.h"
#include "test_interface.h"
#include "average.h"
#include "range.hpp"
#include "data_table.hpp"
#include "config.h"

BOOST_PYTHON_MODULE(data_tables)
{
	 using namespace boost::python;

class_<b_string>("b_string", no_init)
		.def("truth",       &b_string::get_bool)
		.def("__bool__",    &b_string::get_bool)
		.def("__nonzero__", &b_string::get_bool)
		.def("__str__",     &b_string::get_string)
	;

def("Average", &average::list_from_table);
def("Average", &average::create);
def("Average", &average::create_from_col);

class_<average>("average", no_init)
		 .add_property("mean"    , &average::get_mean)
         .add_property("std_dev" , &average::get_std_dev)
		 .add_property("variance", &average::get_variance)
		 .add_property("samples" , &average::get_samples)
		 .def("__eq__", &average::equals<double>)
		 .def("__ne__", &average::not_equals<double>)
		 .def("__lt__", &average::less_than<double>)
		 .def("__le__", &average::less_equal<double>)
		 .def("__gt__", &average::greater_than<double>)
		 .def("__ge__", &average::greater_equal<double>)
		 .def("__eq__", &average::equals<average>)
		 .def("__ne__", &average::not_equals<average>)
		 .def("__lt__", &average::less_than<average>)
		 .def("__le__", &average::less_equal<average>)
		 .def("__gt__", &average::greater_than<average>)
		 .def("__ge__", &average::greater_equal<average>)
		 .def("__add__", &average::add<double>)
		 .def("__radd__", &average::add<double>)
		 .def("__sub__", &average::sub<double>)
		 .def("__add__", &average::add<average>)
		 .def("__sub__", &average::sub<average>)
		 .def("__mul__", &average::mul)
		 .def("__rmul__", &average::mul)
		 .def("__div__", &average::div)
		 .def("__rdiv__", &average::rdiv)
		;

def("Distribution", &TestRange::CreateDistribution);

class_<TestRange>("Range", init<double, double>())
		.def("__contains__"    , &TestRange::does_contain<double, true>)
		.def("__contains__"    , &TestRange::does_contain<average, true>)
		.def("__contains__"    , &TestRange::does_contain_vec<double, true>)
		.def("__contains__"    , &TestRange::does_contain_vec<average, true>)
		.def("not__contains__" , &TestRange::does_contain<double, false>)
		.def("not__contains__" , &TestRange::does_contain<average, false>)
		.def("not__contains__" , &TestRange::does_contain_vec<double, false>)
		.def("not__contains__" , &TestRange::does_contain_vec<average, false>)
		;

////////////////////////////////////////////////////////
//   EXPRESSIONS
////////////////////////////////////////////////////////

#define add_expression(data_type, T, token, tok2, type)\
	def(#token, &data_type<T>::get_##tok2##_expression<token, type>)

#define add_expressions(data_type, T, tok2, type)\
	add_expression(data_type, T, __eq__, tok2, type)\
	.add_expression(data_type, T, __ne__, tok2, type)\
	.add_expression(data_type, T, __lt__, tok2, type)\
	.add_expression(data_type, T, __le__, tok2, type)\
	.add_expression(data_type, T, __gt__, tok2, type)\
	.add_expression(data_type, T, __ge__, tok2, type)

#define basic_expressions(data_type, T)\
	add_expressions(data_type, T, basic, double)\
	.add_expressions(data_type, T, basic, average)

#define column_expressions(data_type, T)\
	basic_expressions(data_type, T)\
	.add_expressions(data_type, T, column, double)\
	.add_expressions(data_type, T, column, average)

#define table_expressions(data_type, T)\
	column_expressions(data_type, T)\
	.add_expressions(data_type, T, table, double)\
	.add_expressions(data_type, T, table, average)

////////////////////////////////////////////////////////
//   ARITHMATIC
////////////////////////////////////////////////////////

#define dbl_operation(data_type, T, tok0, tok1, tok2)\
	def("__" #tok0 "__", &data_type<T>::tok1##_##tok2<double, T>)

#define avg_operation(data_type, T, tok0, tok1, tok2)\
	def("__" #tok0 "__", &data_type<T>::tok1##_##tok2<average, average>)

#define operations(data_type, T, tok)\
	dbl_operation(data_type, T, add, tok, add)\
	.dbl_operation(data_type, T, sub, tok, sub)\
	.dbl_operation(data_type, T, mul, tok, mul)\
	.dbl_operation(data_type, T, div, tok, div)\
	.avg_operation(data_type, T, add, tok, add)\
	.avg_operation(data_type, T, sub, tok, sub)

#define roperations(data_type, T, tok)\
	dbl_operation(data_type, T, radd, tok, add)\
	.dbl_operation(data_type, T, rsub, tok, rsub)\
	.dbl_operation(data_type, T, rmul, tok, mul)\
	.dbl_operation(data_type, T, rdiv, tok, rdiv)\
	.avg_operation(data_type, T, radd, tok, add)\
	.avg_operation(data_type, T, rsub, tok, rsub)

#define basic_operations(data_type, T)\
	def("__abs__", &data_type<T>::absolute)\
	.def("__neg__", &data_type<T>::negate)\
	.def("__pos__", &data_type<T>::save)\
	.operations(data_type, T, basic)\
	.roperations(data_type, T, basic)

#define column_operations(data_type, T)\
	basic_operations(data_type, T)\
	.operations(data_type, T, column)

#define table_operations(data_type, T)\
	column_operations(data_type, T)\
	.roperations(data_type, T, column)\
	.operations(data_type, T, table)

#define declare_column(T)\
	class_<data_column<T> >(#T "_data_column", no_init)\
		.add_property("title", &data_column<T>::get_title)\
		.def("__getitem__",    &data_column<T>::get_slice)\
		.def("__getitem__",    &data_column<T>::get_numbered_item)\
		.def("__len__",        &data_column<T>::size)\
		.column_operations(data_column, T)\
		.column_expressions(data_column, T)

#define declare_table(T)\
	declare_column(T);\
	class_<data_table<T> >(#T "_data_table", no_init)\
		.table_expressions(data_table, T)\
		.table_operations(data_table, T)\
		.add_property("title",    &data_table<T>::get_name)\
		.add_property("path",     &data_table<T>::get_path)\
		.add_property("min_time", &data_table<T>::get_min_time)\
		.def("after",             &data_table<T>::set_min_time)\
		.add_property("max_time", &data_table<T>::get_max_time)\
		.def("before",            &data_table<T>::set_max_time)\
		.add_property("seconds",  &data_table<T>::get_seconds)\
		.def("rate",              &data_table<T>::set_reaction_rate)\
		.def("__getitem__",       &data_table<T>::get_named_item)\
		.def("__getitem__",       &data_table<T>::get_numbered_item)\
		.def("__getitem__",       &data_table<T>::get_slice)\
		.def("__len__",           &data_table<T>::size)


declare_table(double);
def("MCellOutput", &double_table_reader::load_file);

declare_table(average);
def("AveragedMCellOutput", &average_table_reader::load_file);

def("MCellTest", &double_test::create_double_test);
def("MCellTest", &double_test::create_double_test_option);

class_<double_test>("MCellDoubleTest", no_init)
		.add_property("exit_code", &double_test::get_return_code)
		.add_property("signal",    &double_test::get_signal_code)
		.add_property("logfile",   &double_test::get_logfile)
		.add_property("errfile",   &double_test::get_errfile)
		.def("__getitem__",        &double_test::GetOutput)
		;

def("MCellTest", &average_test::create_average_test);
def("MCellTest", &average_test::create_average_test_option);

class_<average_test>("MCellAverageTest", no_init)
		.add_property("exit_code", &average_test::get_return_code)
		.add_property("signal",    &average_test::get_signal_code)
		.add_property("logfile",   &average_test::get_logfile)
		.add_property("errfile",   &average_test::get_errfile)
		.def("__getitem__",        &average_test::GetOutput)
		;
 //*/
};

void python::import_class_(PyObject * module, const char * name)
{
	global[name] = boost::python::handle<>(PyObject_GetAttrString(module, name));
}

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

	import_class_(data_tables, "Average");
	import_class_(data_tables, "Range");
	import_class_(data_tables, "Distribution");
	import_class_(data_tables, "MCellTest");
	import_class_(data_tables, "MCellOutput");
	import_class_(data_tables, "AveragedMCellOutput");

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

//*/

