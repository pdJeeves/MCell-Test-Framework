#include "double_table_reader.h"
#include "../parser/file_lexer.h"

data_table<double> double_table_reader::load_file(const std::string & name, const std::string & path)
{
	FILE * fp = fopen(name.c_str(), "r");
	if(fp == NULL)
	{
		perror("fopen error:");
		return data_table<double>(name, path);
	}

	auto dt = double_table_reader(fp);

	return data_table<double>( dt._row_count
							 , dt._col_count
							 , name
							 , path
							 , dt._header
							 , dt.double_map
							 , dt._comments);
}

double_table_reader::double_table_reader(FILE * fp) {
	store_time_seperately = false;

	table_lexer::super::set_lexer(new file_lexer(fp));

	while(good() && !cur().is_double())
	{
		do {
			_header.push_back(cur().to_string());
		} while(advance() && cur() != '\n');

		advance();
	}

	if(_header.empty())
	{
		get_trigger_header();
	}

	if(get_double_row())
	{
		get_double_table();
	}
}

void double_table_reader::commit_double()
{
	++_row_count;
	if(_col_count == 0)
	{
		_col_count = _read_doubles.size();
	}
	else if(_col_count != _read_doubles.size())
	{
		std::cerr << "ERROR: inconsistent number of columns in data table.";
	}

	double_map.push_back(_read_doubles);
	_read_doubles.clear();
}

data_table<average> plural_double_table_reader::get_data_table(const std::string & name, const std::string & path)
{
	return data_table<average>(_row_count
							 , _col_count
							 , name
							 , path
							 , _header
							 , double_map
							 , _comments);
}


void plural_double_table_reader::load_file(const std::string & name) {
	FILE * fp = fopen(name.c_str(), "r");
	if(fp == NULL)
	{
		perror("fopen error:");
		return;
	}

	table_lexer::super::set_lexer(new file_lexer(fp));

	while(good() && !cur().is_double())
	{
		do {
			_header.push_back(cur().to_string());
		} while(advance() && cur() != '\n');

		advance();
	}

	if(_header.empty())
	{
		get_trigger_header();
	}

	if(get_double_row())
	{
		get_double_table();
	}
}


void plural_double_table_reader::commit_double()
{
	if(_col_count == 0)
	{
		_col_count = _read_doubles.size();
	}
	else if(_col_count != _read_doubles.size())
	{
		std::cerr << "ERROR: inconsistent number of columns in data table.";
	}

	auto itr = double_map.find(_time);

	if(itr == double_map.end())
	{
		double_map.insert(pair_type(_time, table_type()));
		itr = double_map.find(_time);

		itr->second.reserve(_col_count);
		for(int i = 0; i < _col_count; ++i)
		{
			itr->second.push_back(std::vector<double>());
		}
	}

	for(int i = 0; i < _col_count; ++i)
	{
		itr->second[i].push_back(_read_doubles[i]);
	}

	_read_doubles.clear();
}

