#ifndef _script_file_parser_h_
#define _script_file_parser_h_
#include "memoized_lexer.hpp"
#include "file_lexer.h"
#include <vector>

class script_file_parser : public virtual memoized_lexer<token>
{
typedef std::vector<std::string> key_list;
public:
typedef key_list::const_iterator       iterator;
	script_file_parser(const std::string & file)
	{
		_valid = false;
		set_lexer(new file_lexer(file.c_str()));
		read_file();
	}
	script_file_parser(const char * file)
	{
		_valid = false;
		set_lexer(new file_lexer(file));
		read_file();
	}

	bool is_valid() const { return _valid; }
	int get_date()  const { return _date;  }

	std::vector<std::string>::const_iterator begin() const
	{
		return _keywords.begin();
	}
	std::vector<std::string>::const_iterator end() const
	{
		return _keywords.end();
	}

private:
	static const char CommentStartOperator[4];
	static const char CommentEndOperator[4];

	bool      _valid;
	int       _date;
	key_list  _keywords;

//keywords: '{' keyword_list '}'
//keyword_list: keyword | keyword_list | keyword
//keyword: date | string | word
//date: int '/' int '/' int

	void get_keywords();

// file: '"""' keywords '"""'

	void read_file();
};


#endif

