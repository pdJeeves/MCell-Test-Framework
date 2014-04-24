#include "scanner_base.h"
#include <iostream>
#include <cstdio>

scanner_base::scanner_base()
{
	_loc      = 0;
	_end_loc  = 0;
	_position = 0;

	_col      = 0;
	_line     = 0;
	_tabs     = 0;

	_file     = NULL;
	_next     = 0;

	_EOF      = true;
	_good     = false;

	_type     = ASCII;
}

void scanner_base::_set_file(FILE * fp)
{
	if(_file != NULL && _file != stdin)
	{
		fclose(_file);
	}

	_good   = true;
	_file 	= fp; 
	_position = 0;

	if(_file == NULL)
	{
		_EOF  = true;
		return;
	}

	char _buffer[4];
	fread(_buffer, 1, 4, _file);
	rewind(_file);
	_EOF   = false;

	_type = ASCII;
	if(_buffer[0] == 0xEF && _buffer[1] == 0xBB && _buffer[2] == 0xBF)
	{
		_type = UTF8;
		fseek(_file, 3, SEEK_SET);
	}
	else if(_buffer[0] == 0xFF && _buffer[1] == 0xFE)
	{
		if(_buffer[2] == 0x00 && _buffer[3] == 0x00)
		{
			_type = UTF32LE;
			fseek(_file, 4, SEEK_SET);
		}
		else
		{
			_type = UTF16LE;
			fseek(_file, 2, SEEK_SET);
		}
	}
	else if(_buffer[0] == 0x00 && _buffer[1] == 0x00 && _buffer[2] == 0xFE && _buffer[3] == 0xFF)
	{
		_type = UTF32BE;
		fseek(_file, 4, SEEK_SET);
	}
	else if(_buffer[0] == 0xFE && _buffer[1] == 0xFF)
	{
		_type = UTF16BE;
		fseek(_file, 2, SEEK_SET);
	}

	_next = get_char();
}

scanner_base::~scanner_base()
{
	if(_file != NULL && _file != stdin)
	{
		fclose(_file);
	}
}

char scanner_base::get_char()
{
	int rc = fgetc(_file);
	return rc == -1? 0x001A : rc;
}

int scanner_base::get_utf16()
{
	char rhs = get_char();
	char lhs = get_char();

	if(_type & BE)
	{
		return (rhs << 8) | lhs;
	}

	return (lhs << 8) | rhs;
}

int scanner_base::leading_bits(uchar check) const
{
	int ret = 0;
	while(check & 0x80)
	{
		check << 1;
		++ret;
	}

	return ret;
}

int scanner_base::get_utf32()
{
	int ret = 0;
	if(_type & UTF32)
	{
		char _1 = get_char();
		char _2 = get_char();
		char _3 = get_char();
		char _4 = get_char();

		if(_type & BE)
		{
			return ((((_1 << 8) | _2) << 8 | _3) << 8) | _4;
		}

		return ((((_4 << 8) | _3) << 8 | _2) << 8) | _1;
	}
	else if(_type & UTF16)
	{
		ret = get_utf16();

		if(ret & 0xFC00 == 0xD800)
		{
			int temp = get_utf16();

			if(temp & 0xFC00 == 0xDC00)
			{
				ret  &= 0x3FF;
				ret <<= 10;
				ret  |= (temp & 0x3FF);
				ret  += 0x010000;
			}
			else
			{
				expected_continuation("0xDC");
			}
		}
		else if(ret & 0xFC00 == 0xDC00)
		{
				unexpected_continuation("0xDC");
		}

		return ret;
	}

	ret = get_char();

	int until = leading_bits((uchar) ret);
	if(until == 1)
	{
		unexpected_continuation("0b10");
	}
	else if(until > 1)
	{
		ret &= ((1 << (8 - until)) - 1);

		for(--until; until; --until)
		{
			uchar temp = get_char();
			if(leading_bits(temp) == 1)
			{
				ret <<= 6;
				ret  |= (get_char() & 0x3F);
			}
			else
			{
				expected_continuation("0b10");
			}
		}
	}

	return ret;
}

void scanner_base::expected_continuation(const char * s)
{
	_good = false;
	fprintf(stderr, "ERROR: expected a leading sequence %s at byte #%d.\n", s, _position);
}

void scanner_base::unexpected_continuation(const char * s)
{
	_good = false;
	fprintf(stderr, "ERROR: unexpected a leading sequence %s at byte #%d.\n", s, _position);
}

int scanner_base::get_point()
{
	int ret  = _next;
	_next    = get_char();

	switch(ret)
	{
	case 0x0000:
	case 0x001A:
		_end_loc = _loc;

	case 0x000D:
		if(_next == 0x000A)
		{
			_next = get_char();
		}
	case 0x000A:
	case 0x000B:
	case 0x000C:
	case 0x0085:
	case 0x2028:
	case 0x2029:
		return '\n';
	default:
		break;
	}

	return ret;
}

