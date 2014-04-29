#ifndef _memoized_lexer_h_
#define _memoized_lexer_h_
#include "lexer_interface.h"

template<typename token_type=token, int lookahead=10>
class memoized_lexer
{
private:
	lexer_interface<token_type> * _self;	
	token_type      _peek[lookahead];
	int				_loc;
	int				_length;

public:
	memoized_lexer()
	{
		_length = lookahead;
		_self = NULL;
		_loc = 0;
	}
	memoized_lexer(lexer_interface<token_type> * it)
	{
		set_lexer(it);
	}
	~memoized_lexer()
	{
		if(_self != NULL)
		{
			delete _self;
		}
	}

	void set_lexer(lexer_interface<token_type> * it)
	{
		if(_self != NULL)
		{
			delete _self;
		}
		_self = it;
		_loc = 0;

		for(int i = 0; i < lookahead; ++i)
		{
			_peek[i] = _self->fetch_token();
		}
	}
	lexer_interface<token_type> * get_lexer()
	{
		return _self;
	}

	bool good()
	{
		return _length;
	}
	token_type & cur()
	{
		return _peek[_loc];
	}
	token_type & peek(int N = 1)
	{
		return _peek[(_loc + N) % lookahead];
	}
	bool advance()
	{
		if(_length == lookahead)
		{
			_peek[_loc] = _self->fetch_token();
			if(_peek[_loc].is_EOF())
			{
				--_length;
			}
		}
		else
		{
			--_length;
		}
		_loc = (_loc + 1) % lookahead;
		return good();
	}
	void seek(int N = 1)
	{
		for(; N >= 0; --N)
		{
			advance();
		}
	}
};

#endif

