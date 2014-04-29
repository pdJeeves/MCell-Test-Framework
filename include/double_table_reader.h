#ifndef _double_table_reader_h_
#define _double_table_reader_h_
#include <boost/filesystem/path.hpp>
#include "data_table.hpp"
#include "table_lexer.h"

class double_table_reader : public table_lexer
{
public:
	static data_table<double> load_file(const boost::filesystem::path & path);

private:
	double_table_reader(FILE * fp);

	std::vector< std::vector < double > > double_map;
	void commit_double();
};

class average_table_reader : public table_lexer
{
public:
	static data_table<average> load_file(const boost::filesystem::path & path);

private:
	average_table_reader(FILE * fp);

	typedef std::vector<average >          table_type;
	typedef std::pair<double, table_type > pair_type;
	typedef std::map<double, table_type >  map_type;

	map_type average_map;
	void commit_average();
};

class plural_double_table_reader : public table_lexer
{
public:
	plural_double_table_reader();
	void load_file(const boost::filesystem::path & s);
	data_table<average> get_data_table(const boost::filesystem::path & s);

private:
	typedef std::vector<std::vector < double > > table_type;
	typedef std::pair<double, table_type > pair_type;
	typedef std::map<double, table_type >  map_type;

	map_type double_map;
	void commit_double();
};


#endif
