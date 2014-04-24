#include "file_lexer.h"
#include <iostream>

token MOVE file_lexer::fetch_token()
{
	utf_string str;

	if(!good())
	{
		return token();
	}

	while(good() && (cur() == ' ' || cur() == '\t'))
	{
		advance();
	}

	token t = token(line(), col(), tabs());

	if(is_operator(cur()))
	{
		return t.set_operator(consume());
	}

	if(cur() == '"' && peek() == '"' && peek(2) == '"')
	{
		seek(2);
		return t.set_operator("\"\"\"");
	}

	if(is_quote(cur()))
	{
		int closure = cur();

		while(good() && advance() != closure)
		{
			str += cur();
		}
		advance();

		return t.set_string(str);
	}

	if(is_numeric(cur()))
	{
		std::string num;

//	int : [+-][0-9]+
		do {
			str += (char) cur();
			num += (char) cur();
		} while(is_digit(advance()));

		if(is_closure(cur()))
		{
			return t.set_integer(atoi(num.c_str()));;
		}

// float: {int}\.?[0-9]*
		if(cur() == '.')
		{
			do {
				str += (char) cur();
				num += (char) cur();
			} while(is_digit(advance()));
		}

//exponent: {float}[eE][-+]?[0-9]+
		if(cur() | 32 == 'e')
		{
			str += (char) cur();
			num += (char) cur();

			advance();
			if(cur() == '-' || cur() == '+' || is_digit(cur()))
			{
				do
				{
					str += (char) cur();
					num += (char) cur();
				} while(is_digit(advance()));
			}
		}
		
		if(is_closure(cur()))
		{
			return t.set_double(atof(num.c_str()));
		}
	}

	do
	{
		str += cur();
	} while(good() && !is_word_closure(advance()));

	
	return t.set_string(str);
}

inline
void file_lexer::set_got_header()
{
	_have_header = true;
}

inline
bool file_lexer::is_digit(int i)
{
	return ('0' <= i && i <= '9');
}

inline
bool file_lexer::is_numeric(int i)
{
	return is_digit(i) || i == '-' || i == '+' || i == '.';
}

inline
bool file_lexer::is_quote(int i)
{
	return i == '\'' || i == '"' || i == '`';
}

inline
bool file_lexer::is_operator(int i)
{
	return i == '(' || i == ')'
	    || i == ',' || i == '\n'
	    || i == '/' || i == '\\'
	    || i == '{' || i == '}';
}

inline
bool file_lexer::is_closure(int i)
{
	return i == ')' || i == ',' || i == ' ' || i == '\t' || i == '\n'
		|| i == '\\' || i == '/';
}

inline
bool file_lexer::is_word_closure(int i)
{
	if(_have_header == true)
	{
		return i == '\n';
	}

	return is_quote(i) || i == ' ' || i == '\t' || i == '\n'
		|| i == '(' || i == ')'
	    || i == ',' || i == '"'
	    || i == '/' || i == '\\'
	    || i == '{' || i == '}';
}


