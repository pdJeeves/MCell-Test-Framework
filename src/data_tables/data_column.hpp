#ifndef _data_column_hpp_
#define _data_column_hpp_
#include "../support/b_string.h"
#include "expression.hpp"
#include <vector>

template<typename type>
class data_column : public std::vector<type>
{
typedef std::vector<type> super;

	std::string _title;
	std::string _file;
	std::string _path;

public:
	data_column(const std::string & title, const std::string & file, const  std::string & path)
	: _title(title), _file(file), _path(path)
	{
	}

	const std::string & get_name() const
	{
		return _title;
	}
	const std::string & get_filename() const
	{
		return _file;
	}
	const std::string & get_filepath() const
	{
		return _path;
	}

#define transitive(a, b, c)   { a b c }
#define intransitive(a, b, c) { c b a }

#define declare_operation(trans, token, operation)\
template<typename T, typename return_type = type >\
data_column<return_type> basic_##trans##_##token  (T t)\
{\
	auto r_value = data_column<return_type>(_title, _file);\
	\
	for(int i = 0; i < super::size(); ++i)\
	{\
		r_value.push_back(trans(super::at(i), operation, t));\
	}\
	\
	return r_value;\
}\
template<typename T, typename return_type = type >\
data_column<return_type> column_##trans##_##token (const data_column<T> & t)\
{\
	\
	auto r_value = data_column<return_type>(_title, _file);\
	\
	for(int i = 0; i < super::size() && i < t.size(); ++i)\
	{\
		r_value.push_back(trans(super::at(i), operation, t.at(i)));\
	}\
	\
	return r_value;\
}

#define transitive_operation(a, b) declare_operation(transitive, a, b)
#define intransitive_operation(a, b) \
	declare_operation(transitive, a, b)\
	declare_operation(intransitive, a, b)

	transitive_operation(add, +)
	transitive_operation(multiply, *)

	intransitive_operation(subtract, -)
	intransitive_operation(divide, /)

#undef transitive_operation
#undef intransitive_operation
#undef declare_operation

	const data_column<type> & save() const
	{
		return *this;
	}

	data_column<type> & negate()
	{
		for(int i = 0; i < super::size(); ++i)
		{
			super::at(i) = - super::at(i);
		}
		return *this;
	}

	data_column<type> & absolute()
	{
		for(int i = 0; i < super::size(); ++i)
		{
			super::at(i) = super::at(i) < 0? - super::at(i) : super::at(i);
		}
		return *this;
	}

	template<int Operator, typename T>
	expression && get_expression(T t)
	{
		return expression(bind(&data_column<type>::document_comparison<string, T>, *this, Operator, t),
						  bind(&data_column<type>::document_comparison<bool, T>, *this, Operator, t));
	}

	template<typename return_type, typename T>
	return_type document_comparison(int op, T t)
	{
		b_string message;
		message.prefix << "\nIn column " << _title << ", of file " << _file << ":";

		message << internal_comparison(op, t);
		return (return_type) message;
	}

private:
	template<typename T>
	std::string && internal_comparision(int op, T t)
	{
		return "comparison not implemented.";
	}

};

#endif
