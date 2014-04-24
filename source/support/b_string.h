#ifndef _b_string_h_
#define _b_string_h_
#include <sstream>
#include <string>

using std::string;

class b_string : public std::stringstream
{
public:
	b_string() { };
	~b_string() { };

	std::stringstream prefix;

	operator bool()   { return str().size(); }
	operator string()
	{
		if(str().size())
		{
			prefix << str();
			return prefix.str();
		}
		return "";
	}
};

#endif
