#ifndef _MCellInstance_h_
#define _MCellInstance_h_
#include <string>

class MCell
{
private:
	int       & _ret_code;
	int       & _signal;
	std::string _logfile;
	std::string _errfile;
	std::string _mdl;
	std::string _seed;
	bool        _quiet;

public:
	MCell(const std::string & mdl, int & ret_code, int & signal_code);
	~MCell();

	MCell & logfile(const std::string & a);
	MCell & errfile(const std::string & a);
	MCell & seed(int  i);
	MCell & seed(const std::string & s);
	MCell & verbose(bool value);
};



#endif
