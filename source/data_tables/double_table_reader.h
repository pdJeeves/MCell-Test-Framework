#ifndef _double_table_reader_h_
#define _double_table_reader_h_
#include "data_table.hpp"
#include "table_lexer.h"

class double_table_reader : public table_lexer
{
public:
	static data_table<double> load_file(const std::string & name, const std::string & path);

private:
	double_table_reader(FILE * fp);

	std::vector< std::vector < double > > double_map;
	void commit_double();

};

class plural_double_table_reader : public table_lexer
{
public:
	plural_double_table_reader();
	void load_file(const std::string & s);
	data_table<average> get_data_table(const std::string & name, const std::string & path);

private:
	typedef std::vector<std::vector < double > > table_type;
	typedef std::pair<double, table_type > pair_type;
	typedef std::map<double, table_type >  map_type;

	map_type double_map;
	void commit_double();

};

#endif
