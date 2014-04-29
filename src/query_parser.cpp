#include "query_parser.h"
#include "script_file_parser.h"

query_interface * query_interface::parse(int & begin, int argc, const char * args[])
{
	return query_or::parse(begin, argc, args);
}

query_before::query_before(int d)
{
	_date = d;
}

bool query_before::evaluate(const key_list & vec) const
{
	return vec.get_date() < _date;
}

query_after::query_after(int d)
{
	_date = d;
}

bool query_after::evaluate(const key_list & vec) const
{
	return vec.get_date() > _date;
}

query_and::query_and(query_interface * i, query_interface * j)
{
	lhs = i;
	rhs = j;
}
query_and::~query_and()
{
	delete lhs;
	delete rhs;
}

bool query_and::evaluate(const key_list & vec) const
{
	return lhs->evaluate(vec) && rhs->evaluate(vec);
}

query_interface * query_and::parse(int & begin, int end, const char * args[])
{
	query_interface * l = query_not::parse(begin, end, args);

	while(begin+2 < end)
	{
		if((args[begin][0] == '&'
		&& (args[begin][1] == '\0'
		|| (args[begin][1] == '&' && args[begin][2] == '\0')))
		|| (   (args[begin][0] | 32) == 'a'
			&& (args[begin][1] | 32) == 'n'
			&& (args[begin][2] | 32) == 'd'
			&& (args[begin][3] | 32) == '\0'))
		{
			++begin;
			l = new query_and(l, query_not::parse(begin, end, args));
		}
		else
		{
			break;
		}
	}

	return l;
}

query_xor::query_xor(query_interface * i, query_interface * j)
{
	lhs = i;
	rhs = j;
}
query_xor::~query_xor()
{
	delete lhs;
	delete rhs;
}

bool query_xor::evaluate(const key_list & vec) const
{
	bool l = lhs->evaluate(vec);
	bool r = rhs->evaluate(vec);
	return (l ^ r);
}

query_interface * query_xor::parse(int & begin, int end, const char * args[])
{
	query_interface * l = query_and::parse(begin, end, args);

	while(begin+2 < end)
	{
		if((args[begin][0] == '^'
		&& (args[begin][1] == '\0'
		|| (args[begin][1] == '^' && args[begin][2] == '\0')))
		|| (   (args[begin][0] | 32) == 'x'
			&& (args[begin][1] | 32) == 'o'
			&& (args[begin][2] | 32) == 'r'
			&& (args[begin][3] | 32) == '\0'))
		{
			++begin;
			l = new query_xor(l, query_and::parse(begin, end, args));
		}
		else
		{
			break;
		}
	}

	return l;
}

query_or::query_or(query_interface * i, query_interface * j)
{
	lhs = i;
	rhs = j;
}
query_or::~query_or()
{
	delete lhs;
	delete rhs;
}

bool query_or::evaluate(const key_list & vec) const
{
	return lhs->evaluate(vec) || rhs->evaluate(vec);
}

query_interface * query_or::parse(int & begin, int end, const char * args[])
{
	query_interface * l = query_xor::parse(begin, end, args);

	while(begin+2 < end)
	{
		if((args[begin][0] == '|'
		&& (args[begin][1] == '\0'
		|| (args[begin][1] == '|' && args[begin][2] == '\0')))
		|| (   (args[begin][0] | 32) == 'o'
			&& (args[begin][1] | 32) == 'r'
			&& (args[begin][2] | 32) == '\0'))
		{
			++begin;
			l = new query_or(l, query_xor::parse(begin, end, args));
		}
		else
		{
			break;
		}
	}

	return l;
}

query_not::query_not(query_interface * it)
{
	_self = it;
}
query_not::~query_not()
{
	delete _self;
}

bool query_not::evaluate(const key_list & vec) const
{
	return !_self->evaluate(vec);
}

int query_not::date_parse(const char * arg)
{
	int year = 2014, month = 0, day = 0;
	if(is_digit(arg[0])	&& is_digit(arg[1])
	&& is_digit(arg[2])	&& is_digit(arg[3])
	&& is_separator(arg[4])
	&& is_digit(arg[5]) && is_digit(arg[6])
	&& is_separator(arg[7])
	&& is_digit(arg[8]) && is_digit(arg[9])
	&& arg[9] == '\0')
	{
		year  = atoi(arg);
		month = atoi(arg+5);
		day   = atoi(arg+8);
	}

	return (((year << 4) | month) << 5) | day;
}

query_interface * query_not::parse(int & begin, int end, const char * args[])
{
	if(begin+1 < end)
	{
		if(args[begin][0] == '!'
		|| (   (args[begin][0] | 32) == 'n'
			&& (args[begin][1] | 32) == 'o'
			&& (args[begin][2] | 32) == 't'
			&& (args[begin][3] | 32) == '\0'))
		{
			++begin;
			return new query_not(new query_keyword(args[begin++]));
		}
		else if((args[begin][0] | 32) == 'b'
			 && (args[begin][1] | 32) == 'e'
			 && (args[begin][2] | 32) == 'f'
			 && (args[begin][3] | 32) == 'o'
			 && (args[begin][4] | 32) == 'r'
			 && (args[begin][5] | 32) == 'e'
			 &&	(args[begin][6] | 32) == '\0')
		{
			++begin;
			return new query_before(date_parse(args[begin++]));
		}
		else if((args[begin][0] | 32) == 'a'
			 && (args[begin][1] | 32) == 'f'
			 && (args[begin][2] | 32) == 't'
			 && (args[begin][3] | 32) == 'e'
			 && (args[begin][4] | 32) == 'r'
			 && (args[begin][5] | 32) == '\0')
		{
			++begin;
			return new query_after(date_parse(args[begin++]));
		}
	}
	else if(begin < end)
	{
		return new query_keyword(args[begin++]);
	}

	return NULL;
}

query_keyword::query_keyword(const char * k)
: _keyword(k)
 {
 }

bool query_keyword::evaluate(const key_list & vec) const
{
	for(key_list::iterator i(vec.begin()); i != vec.end(); ++i)
	{
		if(_keyword.compare(*i) == 0)
		{
			return true;
		}
	}

	return false;
}

