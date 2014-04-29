#ifndef _table_lexer_h_
#define _table_lexer_h_
#include <boost/shared_ptr.hpp>
#include "average.h"
#include <map>
#include "memoized_lexer.hpp"

using boost::shared_ptr;

class table_lexer : public memoized_lexer<token, 10>
{
public:
typedef memoized_lexer<token, 10> super;
	table_lexer();
	virtual ~table_lexer();
	void read_file(const std::string & s);

public:
#define map_definition(key, value, name)\
typedef std::pair<key, value>	 	                name##_pair;\
typedef std::vector <name##_pair>                   name##_map;\
typedef name##_map::iterator                        name##_iterator;
#define multimap_definition(key, value, name)\
typedef std::pair     <key, value>	                name##_pair;\
typedef std::multimap <key, value>                  name##_map;\
typedef name##_map::iterator                        name##_iterator;\
typedef std::pair<name##_iterator, name##_iterator> name##_range;
	//map times to comments
	multimap_definition(double, std::string, comment)
	//map times to doubles
	map_definition(double, std::vector< std::vector<double> >, double)
	map_definition(double, std::vector< average >, average)

	shared_ptr<comment_map>	 _comments;

	std::vector<average>     _read_averages;
	std::vector<double>      _read_doubles;
	std::vector<std::string> _header;
	double                   _time;
	int						 _row_count;
	int						 _col_count;
	bool 					 store_time_seperately;

	void push_word(std::string);

	virtual void commit_double() { };
	virtual void commit_average() { };

	void commit_comment();
	bool is_average();

	void set_default_header(int length);
	bool get_trigger_header();

	std::string get_comment();

	bool get_double_row();
	void get_double_table();

	bool get_average();
	bool get_average_row();
	void get_average_table();
};

#undef map_definition
#undef multimap_definition


#endif

