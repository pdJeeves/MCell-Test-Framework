#ifndef _file_lexer_h_
#define _file_lexer_h_
#include "token.h"
#include "scanner.h"
#include "lexer_interface.h"

class file_lexer : public virtual lexer_interface<token>, private virtual scanner<10>
{
public:
	file_lexer()               { _have_header = false; }
	file_lexer(FILE * fp)      : scanner<10>(fp) { _have_header = false; }
	file_lexer(const char * s) : scanner<10>(s)  { _have_header = false; }
	virtual ~file_lexer() { }

	token MOVE fetch_token();

private:
	bool _have_header;

//lexers get out of hand very easily, make it very verbose!
	bool is_digit(int i);
	bool is_numeric(int i);

	bool is_quote(int i);
	bool is_operator(int i);

	bool is_closure(int i);

	bool is_word_closure(int i);
};

#endif

