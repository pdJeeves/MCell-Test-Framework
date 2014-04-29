#include "../script_manager.h"
#include "../support/config.h"
#include "MCellInstance.h"
#ifdef _Windows
#include <system.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#endif

MCell::MCell(const std::string & mdl, int & ret_code, int & sig_code)
: _ret_code(ret_code), _signal(sig_code), _mdl(mdl)
{
}

MCell::~MCell()
{
	if(boost::filesystem::exists(_logfile) && config::force == false)
	{
		time_t write_time = boost::filesystem::last_write_time(_logfile);

		if(write_time > boost::filesystem::last_write_time(_mdl) && write_time > config::version)
		{
			return;
		}
	}

	int argc = 7;
	const char * args[10] = { NULL };
	args[0]	= config::executable.c_str();
	args[1]	= "-logfile"; args[2] = _logfile.c_str();
	args[3]	= "-errfile"; args[4] = _errfile.c_str();
	args[5]	= "-seed";    args[6] = _seed.c_str();
	if(_quiet)
	{
		args[argc++]	= "-quiet";
	}
	args[argc] = _mdl.c_str();

#ifdef _Windows
	HANDLE child = (HANDLE)_spawnv(_P_NOWAIT, config::executable.c_str(), ,const_cast<char * const *>(args));
#else
	pid_t child;

	child = fork();
	if(child == 0) { //child
		if(-1 == execvp(config::executable.c_str(),const_cast<char * const *>(args)))
		{
			perror("execvp");
			exit(-1);
		}
	}
	else if(child < 0)
	{
		std::cerr << "fork error." << std::endl;
	}
#endif
	script_scheduler()->block(child, _ret_code, _signal);
}

MCell & MCell::logfile(const std::string & a)
{
	_logfile = a;
	return *this;
}

MCell & MCell::errfile(const std::string & a)
{
	_errfile = a;
	return *this;
}

MCell & MCell::seed(int  i)
{
	char buffer[16];
	sprintf(buffer, "%d", i);
	_seed = std::string(buffer);
	return *this;
}

MCell & MCell::seed(const std::string & s)
{
	_seed = s;
	return *this;
}

MCell & MCell::verbose(bool value)
{
	_quiet = !value;
	return *this;
}

