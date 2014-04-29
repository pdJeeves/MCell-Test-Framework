/**
	scanner class for a regex lexer,

	it reads all unicode formats, and outputs full codepoints only.

	EOF  ::= physical EOF
	     |   0x0000
	     |   0x001A

	'\n' ::= EOF
		 |   0x000A         (Line Feed)
	     |   0x000B         (Vertical Tab)
		 |   0x000C         (Form Feed)
	     |   0x000D         (Carriage Return)
         |   0x000D 0x000A 
	     |   0x0085         (Line Separator)
         |   0x2028         (Line Separator)
	     |   0x2029         (Paragraph Separator)
*/
#ifndef _scanner_h_
#define _scanner_h_
#include "scanner_base.h"
#include <string>

template<int lookahead=8>
class scanner : private scanner_base
{
private:
	point	_peek[lookahead];

public:
/** basic constructor, sets the file being read to NULL and EOF to true */
	scanner()        
	{ 
		for(int i = 0; i < lookahead; ++i)
		{
			_peek[i] = 0;
		}
	}
/** takes a file directly, if it doesn't exist then EOF is set to true.
	the scanner will close the file on it's own, so the user must ensure
	that the file will remain open for the duration */
	scanner(FILE * fp)			
	{
		for(int i = 0; i < lookahead; ++i)
		{
			_peek[i] = 0;
		}

		set_file(fp);
	}
/** takes the named file, if it doesn't exist then EOF is set to true. */
	scanner(const char * path) 
	{
		for(int i = 0; i < lookahead; ++i)
		{
			_peek[i] = 0;
		}

		set_file(fopen(path, "r"));
	}
	scanner(std::string & path)
	{
		for(int i = 0; i < lookahead; ++i)
		{
			_peek[i] = 0;
		}

		set_file(fopen(path.c_str(), "r"));
	}

/** set the file being read to the file specified by the path given. */
	void 	set_file(std::string & path) { set_file(fopen(path.c_str(), "r")); }
	void 	set_file(const char * path) {  set_file(fopen(path, "r")); }
/** takes a file directly, if it doesn't exist then EOF is set to true.
	the scanner will close the file on it's own, so the user must ensure
	that the file will remain open for the duration */
	void    set_file(FILE * fp)
	{
		if(fp == NULL)
		{
			return;
		}

		_set_file(fp);

		for(_loc = 0; _loc < lookahead; ++_loc)
		{
			_peek[_loc] = get_point();
		}

		_loc     = 0;
		_end_loc = -1;

		_col     = _peek[0] == '\n'? 0 : 1;
		_line    = 1 - _col;
		_tabs    = _peek[0] == '\t';
	}
	int   advance()
	{
		if(_loc == _end_loc || _EOF)
		{
			_EOF = true;
			return 0;
		}

		++_col;
		++_position;

		if(_end_loc == -1)
		{
			_peek[_loc] = get_point();
		}
		_loc = (_loc + 1) % lookahead;

		if(_peek[0] == '\t')
		{
			++_tabs;
		}
		else if(_peek[0] == '\n')
		{
			++_line;
			_tabs = _col = 0;
		}

		return _peek[0];
	}



/** advance cur() by one position and return the previous value. */
	int     consume()
	{
		int ret = _peek[_loc];
		advance();
		return ret;
	}
/** advance cur() by N+1 positions, so cur() becomes 1 after peek(N).
    it works by calling advance() repeatedly, because
    seeking by incrementing the position would result in
    line() col() and tabs() being wrong.
 */
	void    seek(int N)
	{
		for(; N >= 0; --N)
		{
			advance();
		}
	}

/** return the current codepoint. */
	int		cur()            const { return _peek[_loc]; }
/** return the codepoint up to 8 ahead without advancing cur(). 
    more accurately, it returns the peek character N % 8 ahead. */
	int   peek(int N = 1) const
	{
		return _peek[(_loc + N) % lookahead];
	}

/** return the number of '\n' characters encountered so far. */
	int		line()     const  { return _line; }
/** return the number of advance() calls since the last '\n' */
	int     col()      const  { return _col; }
/** return number of '\t' characters since the last '\n' */
	int     tabs()     const  { return _tabs; }
/** return absolute number of advance() calls */
	int     position() const  { return _position; }

/** return true if there hasn't been a unicode error and we aren't at the end of file. */
	bool	good()   const  { return _good && !_EOF; }
/** return true if at the EOF. */
	bool    at_EOF() const  { return _EOF; }
/** make at_EOF() true. */
	void	set_EOF()       { _EOF = true; }

	point * const get_buffer() { return _peek; }
};

#endif

