#include "table_lexer.h"

void table_lexer::commit_comment()
{
	std::string result;

	while(cur() != '\n')
	{
		if(result.size())
		{
			result += " ";
		}

		result += cur().to_string();
		advance();
	}

	advance();

	comment_range range = _comments->equal_range(_time);
	
	for(comment_iterator i(range.first); i != range.second; ++i)
	{
		if(i->first == _time && result.compare(i->second) == 0)
		{
			return;
		}
	}

	_comments->insert(comment_pair(_time, result));
}

void table_lexer::set_default_header(int length)
{
	char buffer[8] = { 0 };
	for(int pos = 1; pos <= length; ++pos)
	{
		sprintf(buffer, "col_%d", pos);
	}
}

/*
trigger_row ::= exact_time location orient_count

exact_time  ::= 
            |    double

location    ::= double double double

orient_count::= 
            | int 
            | int int
*/
bool table_lexer::get_trigger_header()
{
	if(!cur().is_double())
	{
		std::cerr << "ERROR: expected double at " << cur().line() << ":" << cur().col(); 
		return false;
	}

	if(peek().is_double()
	&& peek(1).is_double()
	&& peek(2).is_double())
	{
		int pos = 3;
		if(peek(pos).is_double() && !peek(pos).is_integer())
		{
			++pos;
			_header.push_back("exact_time");
		}

		_header.push_back("loc.x");
		_header.push_back("loc.y");
		_header.push_back("loc.z");

		if(peek(pos).is_integer())
		{
			++pos;
			_header.push_back("orient");
			if(peek(pos).is_integer())
			{
				++pos;
				_header.push_back("count");
			}
		}
	
		if(!peek(pos).is_double())
		{
			return true;
		}
		else
		{ 
			_header.clear();
		}
	}

	return false;
}

void table_lexer::get_double_table()
{
	do
	{
		commit_double();
		commit_comment();
	} while(get_double_row());
}

bool table_lexer::get_double_row()
{
	if(!cur().is_double())
	{
		return false;
	}

	if(store_time_seperately)
	{
		_time = cur().get_double();

		while(advance() && cur().is_double())
		{
			_read_doubles.push_back(cur().get_double());
		}
	}
	else
	{
		do
		{
			_read_doubles.push_back(cur().get_double());
		} while(advance() && cur().is_double());
	}

	return true;
}

/* 
average  ::= '(' double ',' double ')'
         |   '(' double ',' double ',' int ')'
*/
bool table_lexer::get_average()
{
	if(cur()   == '('
	&& peek( ).is_double()
	&& peek(2) == ','
	&& peek(3).is_double())
	{
		if(peek(4) == ')')
		{
			_read_averages.push_back(average(peek().get_double(), peek(3).get_double()));
			seek(4);
			return true;
		}
		else if(peek(4) == ','
			 && peek(5).is_integer()
			 && peek(6) == ')')
		{
			_read_averages.push_back(average(peek().get_double(), peek(3).get_double(), peek(5).get_integer()));
			seek(6);
			return true;
		}
	}

	return false;
}

bool table_lexer::get_average_row()
{
	if(!cur().is_double())
	{
		return false;
	}

	_time = cur().get_double();

	while(get_average()) ;

	return true;
}

void table_lexer::get_average_table()
{
	do
	{
		commit_average();
		commit_comment();
	} while(get_average_row());
}

std::string table_lexer::get_comment()
{
	std::string result;

	while(cur() != '\n')
	{
		if(result.size())
		{
			result += " ";
		}

		result += cur().to_string();
		advance();
	}

	advance();
	return result;
}


