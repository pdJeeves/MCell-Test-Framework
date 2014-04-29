#include "script_file_parser.h"
#include <iostream>

const char script_file_parser::CommentStartOperator[4] = "\"\"\"";
const char script_file_parser::CommentEndOperator[4]   = "\"\"\"";

void script_file_parser::get_keywords()
{
	while(advance())
	{
		if(cur().is_operator(CommentEndOperator))
		{
			std::cout << "\nexited block comment." << std::endl;
			break;
		}
		else if(cur().is_operator('}'))
		{
			std::cout << "\nexited keyword braces." << std::endl;
			break;
		}
		else if(cur().is_string())
		{
			std::cout << "\nfound keyword: " << cur().to_string();
			_keywords.push_back(cur().to_string());
		}
		else if(cur().is_integer() 
		&& (peek().is_operator('/') || peek().is_operator('\\'))
		&& peek(2).is_integer()
		&& peek(3) == peek()
		&& peek(4).is_integer())
		{
			std::cout << "\nfound date: " << cur().get_integer() << "-" <<  peek(2).get_integer() << "-" << peek(4).get_integer();
			_date = (((cur().get_integer() << 4) | peek(2).get_integer()) << 5) | peek(4).get_integer();
			seek(4);
		}
		else
		{
			std::cout << "\nunidetified token.";
		}
	}
}

// file: '"""' keywords '"""'

void script_file_parser::read_file()
{
	if(!good())
	{
		std::cout << "\nfile unopened.";
		return;
	}

	if(!cur().is_operator(CommentStartOperator)) 
	{
		std::cout << "\nfile rejected.";
		return;
	}

	_valid = true;
	std::cout << "\nfile accepted.";
	std::cout << "\nin block comment.";

	while(advance())
	{
		if(cur().is_operator(CommentEndOperator))
		{
			std::cout << "no keywords found.";
			break;
		}

		if(cur().is_operator('{'))
		{
			std::cout << "in keyword braces.";
			get_keywords();
			break;
		}
	}
}

