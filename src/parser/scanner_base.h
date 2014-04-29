#ifndef _scanner_base_h_
#define _scanner_base_h_
#include "types.h"
#include <cstdlib>
#include <cstdio>

/**
	almost all processing is here, it is a seperate base class
	so the functions don't have to be defined in a header file,
	as the full scanner is a tempalte.
*/

class scanner_base
{
public:
	scanner_base();
	~scanner_base();

protected:
	void 	_set_file(FILE * fp);

	point   get_point();

	int 	_loc;
	int 	_end_loc;
	int     _position;

	int     _col;
	int     _line;
	int     _tabs;

	bool	_EOF;
	bool    _good;

private:
	static const int buffer_size      = 1024;

	FILE * 	_file;
	point   _next;

	enum
	{
		LE		= 0,
		BE		= 1,
		ASCII	= 1 << 1,
		UTF8	= 1 << 2,
		UTF16	= 1 << 3,
		UTF32	= 1 << 4,

		UTF16BE = UTF16 | BE,
		UTF16LE = UTF16 | LE,
		UTF32BE = UTF32 | BE,
		UTF32LE = UTF32 | LE
	};
	uchar 	_type;

	int leading_bits(uchar) const;

	char 	get_char();
	point   get_utf16();
	point	get_utf32();
	void 	expected_continuation(const char * s);
	void	unexpected_continuation(const char * s);
};

#endif

