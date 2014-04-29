#ifndef _expression_h_
#define _expression_h_
#include <boost/bind.hpp>
#include <string>
#include <iostream>

class expression
{
private:
	bool _evaluate;
	std::string (*_string)();
	bool        (*_bool)();

public:
	expression(std::string (*s)(), bool (*b)())
	: _string(s),
	  _bool(b)
	{
		_evaluate = true;
	}
	expression(expression & it)
	: _string(it._string),
	  _bool(it._bool)
	{
		_evaluate = it._evaluate;
		it._evaluate = false;
	}
	expression(expression && it)
	: _string(it._string),
	  _bool(it._bool)
	{
		_evaluate = it._evaluate;
		it._evaluate = false;
	}
	~expression()
	{
		if(_evaluate)
		{
			std::cout << _string();
		}
	}

	bool get_bool()
	{
		_evaluate = true;
		return _bool();
	}
	std::string get_string()
	{
		_evaluate = true;
		return _string();
	}
};


#endif
