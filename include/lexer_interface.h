#ifndef _lexer_interface_h_
#define _lexer_interface_h_
#include "token.h"
#include "scanner.h"
#include <iostream>

template<typename token_type>
class lexer_interface
{
public:
	virtual ~lexer_interface() { };
	virtual token_type MOVE fetch_token() = 0;
};

#endif

