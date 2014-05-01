#ifndef _data_table_h_
#define _data_table_h_
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include "data_column.hpp"
#include <map>

using boost::shared_ptr;

template<typename type>
class data_table : public std::vector<data_column<type> >
{
typedef std::vector<data_column<type> >               super;
typedef std::multimap<double, std::string>			  comment_map;
typedef comment_map::iterator						  comment_iterator;
typedef std::pair<comment_iterator, comment_iterator> comment_range;
public:
	shared_ptr<comment_map> _comments;
	std::vector<double>     _time;
	std::string             _stem;
	std::string				_path;

	data_table(const boost::filesystem::path & path)
	: _stem(path.stem().native()), _path(path.native()) 
	{ 
	};

	template<typename T>
	data_table(const data_table<T> * it)
	: _stem(it->_stem), _path(it->_path), _time(it->_time), _comments(it->_comments) { };

	template<typename U>
	data_table(int row_count,
			   int col_count,
			   const boost::filesystem::path & path,
			   const std::vector<std::string> & _header,
			   const U & vec,
			   const shared_ptr<comment_map> & comments)
		: _comments(comments)
		,  _stem(path.stem().native())
		,  _path(path.native()) 
    {
		if(vec.empty())
		{
			return;
		}

		super::reserve(col_count);
		_time.reserve(row_count);

		int offset = _header.size() - col_count;

		for(int i = 0; i < col_count; ++i)
		{
			if(offset >= 0)
			{
				super::push_back(data_column<type>(_header[offset+i], _stem));
			}
			else
			{
				char buffer[16];
				sprintf(buffer, "column[%d]", i);
				super::push_back(data_column<type>(std::string(buffer), _stem));
			}

			super::back().reserve(row_count);
		}

		insert_data(vec);
	}

	template<typename U>
	void insert_data(const U & vec)
	{
	}

	std::string get_name() const
	{
		return _stem;
	}

	std::string get_path() const
	{
		return _path;
	}

	data_column<type> get_named_item(std::string name)
	{
		for(int i = 0; i < super::size(); ++i)
		{
			if(name.compare(super::at(i).get_title()) == 0)
			{
				return super::at(i);
			}
		}

		std::cerr << "Error: column " << name << " doesn't exist.";
		return data_column<type>(name, _stem);
	}

	data_column<type> get_numbered_item(int i)
	{
		if(0 <= i && i < super::size())
		{
				return super::at(i);
		}

		PyErr_SetString(PyExc_StopIteration, "Error: tried to get invalid index");
		boost::python::throw_error_already_set();

		return data_column<type>("invalid", _stem);
	}

//ARGH!
	data_table<type> get_slice(const boost::python::slice & s)
	{
		auto ret = data_table<type>(this);

		int start = 0;
		int stop  = super::size();
		int step  = 1;

		if(boost::python::extract<long>(s.start()).check())
		{
			start = boost::python::extract<long>(s.start()) % stop;
		}
		if(boost::python::extract<long>(s.step()).check())
		{
			step = boost::python::extract<long>(s.step()) % stop;
		}
		if(boost::python::extract<long>(s.stop()).check())
		{
			stop = boost::python::extract<long>(s.stop()) % stop;
		}

		for(int i = start; i <= stop && i < super::size(); i += step)
		{
			ret.push_back(super::at(i));
		}

		return ret;
	}


#define basic_operation(token)\
	template<typename T, typename return_type = type>\
	data_table<return_type> basic_##token(T v)\
	{\
		auto r_value = data_table<return_type>(this);\
		\
		for(int i = 0; i < super::size(); ++i)\
		{\
			r_value.push_back(super::at(i).basic_##token<T, return_type>(v));\
		}\
		\
		return r_value;\
	}

#define column_operation(token) basic_operation(token)\
	template<typename T, typename return_type = type>\
	data_table<return_type> column_##token(const data_column<T> & v)\
	{\
		auto r_value = data_table<return_type>(this);\
		\
		if(_time.size() != v.size())\
		{\
			std::cerr << "ERROR: trying to token a table and a column with different numbers of rows.";\
		}\
		else\
		{\
			for(int i = 0; i < super::size(); ++i)\
			{\
				r_value.push_back(super::at(i).column_##token<T, return_type>(v));\
			}\
		}\
		\
		return r_value;\
	}

#define table_operation(token) column_operation(token)\
	template<typename T, typename return_type = type>\
	data_table<return_type> table_##token(const data_table<T> & v)\
	{\
		auto r_value = data_table<return_type>(this);\
		\
		if(super::size() != v.size())\
		{\
			std::cerr << "ERROR: trying to token tables with different numbers of columns.";\
		}\
		else if(_time.size() != v._time.size())\
		{\
			std::cerr << "ERROR: trying to token tables with different numbers of rows.";\
		}\
		else\
		{\
			for(int i = 0; i < super::size(); ++i)\
			{\
				r_value.push_back(super::at(i).column_##token<T, return_type>(v.at(i)));\
			}\
		}\
		return r_value;\
	}

#define associative_operation(a) table_operation(a)
#define nonassociative_operation(a) \
	table_operation(a)\
	table_operation(r##a)

	associative_operation(add)
	nonassociative_operation(sub)
	associative_operation(mul)
	nonassociative_operation(div)

#undef nonassociative_operation
#undef associative_operation
#undef table_operation
#undef column_operation
#undef basic_operation

	data_table<type> set_reaction_rate(double base_time)
	{
		auto ret = data_table<type>(this);

		for(int i = 0; i < super::size(); ++i)
		{
			ret.push_back(data_column<type>(super::at(i).get_title(), _stem));
			ret.back().reserve(super::at(i).size());

			for(int j = 0; j < super::at(i).size(); ++j)
			{
				double rate = _time[j] - base_time;
				if(rate == 0)
				{
					ret.back().push_back(super::at(i)[j]);
				}
				else
				{
					ret.back().push_back(super::at(i)[j] / rate);
				}
			}
		}

		return ret;
	}

	int get_time_stamp(double time) const
	{
		int i;
		for(i = 0; i < _time.size(); ++i)
		{
			if(_time[i] > time)
			{
				return i;
			}
		}

		std::cerr << "Error: tried to select a time that doesn't exist.";
		return -1;
	}

	data_table<type> set_min_time(double time)
	{
		int row = get_time_stamp(time);
		if(row == -1)
		{
			return this;
		}

		auto ret = data_table<type>(this);

		for(int i = 0; i < super::size(); ++i)
		{
			ret.push_back(data_column<type>(super::at(i).get_title(), _stem));
			ret.back().reserve(_time.size() - row);
			ret.back()._min_row = super::at(i)._min_row + row;

			for(int j = row; j < super::at(i).size(); ++j)
			{
				ret[i].push_back(super::at(i)[j]);
			}
		}

		return ret;
	}

	double get_min_time() const
	{
		return _time.front();
	}

	data_column<double> get_seconds() const
	{
		auto dt = data_column<double>("seconds", _stem, super::at(0)._min_row);
		dt.insert(dt.begin(), _time.begin(), _time.end());
		return dt;
	}

	data_table<type> set_max_time(double time) 
	{
		int row = get_time_stamp(time);
		if(row == -1)
		{
			return this;
		}

		auto ret = data_table<type>(this);

		for(int i = 0; i < super::size(); ++i)
		{
			ret.push_back(data_column<type>(super::at(i).get_title(), _stem));
			ret.back().reserve(row);
			ret.back()._min_row = super::at(i)._min_row;

			for(int j = 0; j < row; ++j)
			{
				ret[i].push_back(super::at(i)[j]);
			}
		}

		return ret;
	}

	double get_max_time() const
	{
		return _time.back();
	}

	data_table<type> save()
	{
		return this;
	}

	data_table<type> negate()
	{
		auto ret = data_table<type>(this);

		for(int i = 0; i < super::size(); ++i)
		{
			ret.push_back(super::at(i).negate());
		}

		return this;
	}

	data_table<type> absolute()
	{
		auto ret = data_table<type>(this);

		for(int i = 0; i < super::size(); ++i)
		{
			ret.push_back(super::at(i).absolute());
		}

		return this;
	}

	template<int Operator, typename T>
	b_string get_basic_expression(T t)
	{
		std::stringstream message;

		for(unsigned int i = 0; i < super::size(); ++i)
		{
			message << super::at(i).document_comparison(Operator, t);
		}

		return b_string(message.str());
	}

	template<int Operator, typename T>
	b_string get_column_expression(data_column<T> t)
	{
		std::stringstream message;
		if(_time.size() != t.size())
		{
			std::cerr << "ERROR: trying to compare a table and a column with different numbers of rows.";
		}
		else
		{
			for(unsigned int i = 0; i < super::size(); ++i)
			{
				message << super::at(i).document_comparison(Operator, t);
			}
		}

		return b_string(message.str());
	}

	template<int Operator, typename T>
	b_string get_table_expression(data_table<T> t)
	{
		std::stringstream message;
		if(_time.size() != t._time.size())
		{
			std::cerr << "ERROR: trying to compare two tables with different numbers of rows.";
		}
		else if(super::size() != t.size())
		{
			std::cerr << "ERROR: trying to compare two tables with different numbers of columns.";
		}
		else
		{
			for(int i = 0; i < super::size(); ++i)
			{
				message << super::at(i).document_comparison(Operator, t.at(i));
			}
		}

		return b_string(message.str());
	}

	template<int Operator>
	b_string get_list_expression(const boost::python::list & t);
};


#ifndef _average_h_
#include "average.h"
#endif


template<typename type>
	template<int Operator>
	b_string data_table<type>::get_list_expression(const boost::python::list & t)
	{
		std::stringstream message;
		if(super::size() != len(t))
		{
			std::cerr << "ERROR: number of things in list does not equal number of columns.";
		}
		else
		{

			for(int i = 0; i < super::size(); ++i)
			{
				if(boost::python::extract<long>(t[i]).check())
				{
					message << super::at(i).document_comparison(Operator, (double) boost::python::extract<long>(t[i])); 
				}
				else if(boost::python::extract<double>(t[i]).check())
				{
					message << super::at(i).document_comparison(Operator, boost::python::extract<double>(t[i])); 
				}
				else if(boost::python::extract<average>(t[i]).check())
				{
					message << super::at(i).document_comparison(Operator, boost::python::extract<average>(t[i])); 
				}
				else 
				{
					std::cerr << "ERROR: objects in list must be ints, doubles or averages";
				}
			}
		}

		return b_string(message.str());
	}
#endif
