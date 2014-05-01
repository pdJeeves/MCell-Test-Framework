#ifndef _token_h_
#define _token_h_
#include "types.h"
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <string>

template<typename Derived>
class token_interface 
{
public:
	token_interface()
	{
		_line   = -1;
		_col    = -1;
		_tab    = -1;
		_num    = -1;
		_flags  =  0;
		_type   = -1;
		_integer  =  0;
	}
	token_interface(int line, int col, int tab)
	{
		_line      = line;
   		_col       = col;
   		_tab       = tab;
		_num       = 0;
		_flags     = 0;
		_type      = 0;
		_integer   = 0;
	}

	token_interface(const token_interface & it)
	{
		_line      = it._line;
	   	_col       = it._col;
	   	_tab       = it._tab;
		_num       = it._num;
		_flags     = it._flags;
		_type      = it._type;
		if(_type == Double)
		{
			_double = it._double;
		}
		else
		{
			_integer  = it._integer;
		}
		_integer   = it._integer;
		_self      = it._self;
	}

	virtual ~token_interface()
	{
	}
	Derived & operator=(const Derived & it)
	{
		_line   = it._line;
    	_col    = it._col;
    	_tab    = it._tab;

		_flags  = it._flags;
		_type   = it._type;
		if(_type == Double)
		{
			_double = it._double;
		}
		else
		{
			_integer  = it._integer;
		}
		_self     = it._self;

		return *(static_cast<Derived *>(this));
	}

/** get the line where this token was found.
	or where the first token that this was 
	reduced from was found. 
*/
	int       line()   const { return _line; }
/** get the position in the line where this token was found.
	or where the first token that this was 
	reduced from was found. 
*/
	int       col()    const { return _col; }
/** get the number of tabs prior to this token in said line.
	or where the first token that this was 
	reduced from was found. 
*/
	int       tab()    const { return _tab; }

/** get the number of this token.
	where 0 is the first token created, 1 is the second, etc.
	if it was created via a reduction then this is the position 
	of the first token it was reduced from.
*/
	int       number()    const { return _num; }
/** get our bitflags */
	int       get_flags() const { return _flags; }

	bool operator==(const Derived & it) const
	{
		return _flags     == it._flags
			&& _type      == it._type
		    && _integer   == it._integer
		    && _self      == it._self;
	}
	bool operator!=(const Derived & it) const
	{
		return _flags     != it._flags
			|| _type      != it._type
		    || _integer   != it._integer
		    || _self      != it._self;
	}

	bool operator==(char i)          const
	{
		if(_type == Operator)
		{
			return _integer == i;
		}

		return false;
	}

	bool operator!=(char i)          const
	{
		if(_type == Operator)
		{
			return _integer != i;
		}

		return true;
	}

/** if the token is an integer or double,
	 then return if it is equal to the given integer,
	otherwise false **/
	bool operator==(int i)          const
	{
		if(_type == Double)
		{
			return _double == i;
		}
		else if((_type & Integer) || _type == Operator)
		{
			return _integer == i;
		}

		return false;
	}

/** if the token is an integer or double,
	then return if it is not equal to the given integer,
	otherwise true **/
	bool operator!=(int i)          const
	{
		if(_type == Double)
		{
			return _double != i;
		}
		else if((_type & Integer) || _type == Operator)
		{
			return _integer != i;
		}

		return true;
	}

/** if the token is an operator or a string, 
	then return if it is not equal to the given string,
	otherwise true **/
	bool operator==(const char * c) const
	{
		if(_type == Operator)
		{
			return is_operator(c);
		}
		else if(_type == String)
		{
			return _self.compare(c) != 0;
		}

		return false;
	}

/** if the token is an integer then return if it is not equal to the given integer,
	otherwise true **/
	bool operator!=(const char * c) const
	{
		if(_type == Operator)
		{
			return !is_operator(c);
		}
		else if(_type == String)
		{
			return _self.compare(c) != 0;
		}

		return true;
	}
/** if the token is an integer or double,
	then return if it is not equal to the given number,
	otherwise true **/
	bool operator==(double i)       const
	{
		if(_type == Double)
		{
			return _double == i;
		}
		else if((_type & Integer) || _type == Operator)
		{
			return _integer == i;
		}

		return false;
	}
/** if the token is an integer or double,
	then return if it is not equal to the given number,
	otherwise true **/
	bool operator!=(double i)       const
	{
		if(_type == Double)
		{
			return _double != i;
		}
		else if((_type & Integer) || _type == Operator)
		{
			return _integer != i;
		}

		return true;
	}

    bool      is_operator(int op)          const
	{ return (_type == Character || _type == Operator) && _integer == op; }
	bool      is_operator(const char op[]) const
	{
		char _op[4] = { 0 };
		for(int i = 0; i < 4 && op[i]; ++i)
		{
			_op[i] = op[i];
		}

		return is_operator(*((int *) _op));
	}

/** returns if we are the EOF, or more accurately undefined. */
	bool      is_EOF()       const { return _type == -1; }
	bool      is_operator()  const { return (_type == Character || _type == Operator); }
	bool      is_integer()   const { return (_type == Integer); } 
	bool      is_double()    const { return (_type == Integer || _type == Double); }
    bool      is_character() const { return (_type == Character || _type == Operator); }
    bool      is_string()    const { return (_type == String); }

/** named getters */
	bool          get_flag(int i)   const { return (i & _flags); }
	int           get_operator()    const { return _integer; }
	long long int get_integer()     const { return _integer; }
    long double   get_double()      const 
	{
		return  _type == Double
		?
			_double
		:
			_integer
		;
	}
    int           get_character()   const { return _integer; }
	std::string	  to_string()       const { return _self; }

/** implied getters */
	operator long long int( void )  const { return get_integer(); }
	operator long double  ( void )  const { return get_double();  }
	operator std::string  ( void )  const { return to_string(); }

/** set type to operator, (needs to be < 4 characters) */
	Derived &     set_operator (int op)
	{
		_type   = Operator;
		_integer = op;

		return *(static_cast<Derived *>(this));
	}
	Derived &     set_operator (const char op[])
	{
		return set_operator(int_from_string(op));
	}
/** set type to character and value to i */
	Derived &     set_string   (const std::string & vec)
	{
		_type = String;
		_self = vec;
		return *(static_cast<Derived *>(this));
	}
/** set type to character and value to i */
	Derived &     set_character(int           i)
	{
		_type    = Character;
		_integer = i;

		return *(static_cast<Derived *>(this));
	}
/** set type to int and value to i */
	Derived &     set_integer  (const std::string & s)
	{
		_type    = Integer;
		_integer = atoll(s.c_str());
		_self    = s;
		return *(static_cast<Derived *>(this));
	}
/** set type to float and value to i */
	Derived &     set_double   (const std::string & s)
	{
		_type    = Double;
		_double  = atof(s.c_str());
		_self    = s;

		return *(static_cast<Derived *>(this));
	}

protected:
	int int_from_string(const char op[])
	{
		char _op[4] = { 0 };
		for(int i = 0; i < 4 && op[i]; ++i)
		{
			_op[i] = op[i];
		}
		
		return *((int *) _op);
	}

	enum types
	{
		Operator  = 1 << 0,
		Character = 1 << 1,
		Integer   = 1 << 2 | Character,
		Double    = 1 << 3 | Integer,
		String    = 1 << 4
	};

	int	          _line;
    int           _col;
    int           _tab;

	int           _num;
	int	          _flags;
	uchar         _type;
//only have kinds of content that are integers.
	union {
		long long int  _integer;
		long double    _double;
	};
	std::string   _self;
};

struct token : public token_interface<token> {
	token() { };
	token(int line, int col, int tab)
	: token_interface<token>(line, col, tab) { };
};

#undef basic_constructor
#undef copy_constructor
#undef define_constructors

#endif

