#ifndef _data_table_h_
#define _data_table_h_
#include "data_column.hpp"
#include <boost/shared_ptr.hpp>
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
	data_table(const std::string & name,
		       const std::string & path)
	: _file(name), _path(path) { };

	template<typename U>
	data_table(int row_count,
			   int col_count,
			   const std::string & name,
			   const std::string & path,
			   const std::vector<std::string> & _header,
			   const U & vec,
			   const shared_ptr<comment_map> & comments)
		: _comments(comments)
		, _file(name)
		, _path(path)
    {
		if(vec.empty())
		{
			return;
		}

		super::reserve(col_count);
		_time.reserve(row_count);

		int offset = _header.size() - col_count;

		for(int i = 1; i < col_count; ++i)
		{
			if(offset >= 0)
			{
				super::push_back(data_column<type>(_header[offset+i], _file, _path));
			}
			else
			{
				char buffer[16];
				sprintf(buffer, "column[%d]", i);
				super::push_back(data_column<type>(std::string(buffer), _file, _path));
			}
		}

		insert_data(vec);
	}

	template<typename U>
	void insert_data(const U & vec)
	{
	}

	std::string & get_file() const
	{
		return _file;
	}

	std::string & get_path() const
	{
		return _path;
	}

	data_column<type> get_item(std::string name)
	{
		for(int i = 0; i < super::size(); ++i)
		{
			if(name.compare(super::at(i)._title) == 0)
			{
				return super::at(i);
			}
		}

		std::cerr << "Error: column " << name << " doesn't exist.";
		return data_column<type>();
	}

#define declare_operation(trans, token)\
	template<typename T, typename return_type = type>\
	data_table<return_type> basic_##trans##_##token(T v)\
	{\
		auto r_value = data_table<return_type>(_file, _time);\
		\
		for(int i = 0; i < super::size(); ++i)\
		{\
			r_value.push_back(super::at(i).basic_##trans##_##token<return_type, T>(v));\
		}\
		\
		return r_value;\
	}\
	template<typename T, typename return_type = type>\
	data_table<return_type> column_##trans##_##token(const data_column<T> & v)\
	{\
		auto r_value = data_table<return_type>(_file, _time);\
		\
		if(_time.size() != v.size())\
		{\
			std::cerr << "ERROR: trying to token a table and a column with different numbers of rows.";\
		}\
		else\
		{\
			for(int i = 0; i < super::size(); ++i)\
			{\
				r_value.push_back(super::at(i).column_##trans##_##token<return_type, T>(v));\
			}\
		}\
		\
		return r_value;\
	}\
	template<typename T, typename return_type = type>\
	data_table<return_type> table_##trans##_##token(const data_table<T> & v)\
	{\
		auto r_value = data_table<return_type>(_file, _time);\
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
				r_value.push_back(super::at(i).column_##trans##_##token<return_type, T>(v.at(i)));\
			}\
		}\
		return r_value;\
	}

#define transitive_operation(a) declare_operation(transitive, a)
#define intransitive_operation(a) \
	declare_operation(transitive, a)\
	declare_operation(intransitive, a)

	transitive_operation(add)
	transitive_operation(multiply)

	intransitive_operation(subtract)
	intransitive_operation(divide)

#undef transitive_operation
#undef intransitive_operation
#undef declare_operation

	data_table<type> & set_reaction_rate(double base_time)
	{
		for(int i = 0; i < super::size(); ++i)
		{
			for(int j = 0; j < super::at(i).size(); ++j)
			{
				double rate = _time[j] - base_time;
				if(rate == 0)
				{
					super::at(i)[j] = 0;
				}
				else
				{
					super::at(i)[j] = super::at(i)[j] / rate;
				}
			}
		}

		return this;
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

	data_table<type> & set_min_time(double time)
	{
		int row = get_time_stamp(time);
		if(row == -1)
		{
			return *this;
		}

		for(int j = 0, k = row; k < _time.size(); ++j, ++k)
		{
			_time[j] = _time[k];
		}

		_time.resize(_time.size() - row);

		for(int i = 0; i < super::size(); ++i)
		{
			for(int j = 0, k = row; k < super::at(i).size(); ++j, ++k)
			{
				super::at(i)[j] = super::at(i)[k];
			}

			super::at(i).resize(super::at(i).size - row);
		}

		return *this;
	}

	double get_min_time() const
	{
		return _time.front();
	}

	data_table<type> & set_max_time(double time)
	{
		int row = get_time_stamp(time);
		if(row == -1)
		{
			return *this;
		}

		_time.resize(row);

		for(int i = 0; i < super::size(); ++i)
		{
			super::at(i).resize(row);
		}

		return *this;
	}

	double get_max_time() const
	{
		return _time.back();
	}

	data_table<type> & save()
	{
		return *this;
	}

	data_table<type> & negate()
	{
		for(int i = 0; i < super::size(); ++i)
		{
			super::at(i).negate();
		}

		return *this;
	}

	data_table<type> & absolute()
	{
		for(int i = 0; i < super::size(); ++i)
		{
			super::at(i).absolute();
		}

		return *this;
	}

	template<int Operator, typename T>
	expression && get_expression(T t)
	{
		return expression(bind(&data_table<type>::comparison<string, T>, *this, Operator, t),
						  bind(&data_table<type>::comparison<bool, T>, *this, Operator, t));
	}

	template<int Operator, typename T>
	expression && get_table_expression(data_table<T> t)
	{
		return expression(bind(&data_table<type>::table_comparison<string, T>, *this, Operator, t),
						  bind(&data_table<type>::table_comparison<bool, T>, *this, Operator, t));
	}

	template<int Operator, typename T>
	expression && get_column_expression(data_column<T> t)
	{
		return expression(bind(&data_table<type>::column_comparison<string, T>, *this, Operator, t),
						  bind(&data_table<type>::column_comparison<bool, T>, *this, Operator, t));
	}

private:
	shared_ptr<comment_map> _comments;
	std::vector<double>     _time;
	std::string             _file;
	std::string				_path;

	template<typename return_type, typename T>
	return_type comparison(int operation, T t) const
	{
		b_string message;

		for(int i = 0; i < super::size(); ++i)
		{
			message << super::at(i).template document_comparison<return_type>(operation, t);
		}

		return (return_type) message;
	}

	template<typename return_type, typename T>
	return_type table_comparison(int operation, const data_table<T> & t) const
	{
		b_string message;
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
				message << super::at(i).template document_comparison<return_type>(operation, t.at(i));
			}
		}

		return (return_type) message;
	}

	template<typename return_type, typename T>
	return_type column_comparison(int operation, const data_column<T> & t) const
	{
		b_string message;
		if(_time.size() != t.size())
		{
			std::cerr << "ERROR: trying to compare a table and a column with different numbers of rows.";
			return (return_type) message;
		}

		for(int i = 0; i < super::size(); ++i)
		{
			message << super::at(i).template document_comparison<return_type>(operation, t);
		}

		return (return_type) message;
	}

};


#endif
