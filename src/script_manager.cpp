#include <boost/filesystem.hpp>
#include "python_interface.hpp"
#include "support/platform.h"
#include "support/config.h"
#include "script_manager.h"
#include <fstream>

#ifdef _Windows
#include <system.h>
#else
#include <sys/types.h>
#include <sys/wait.h>
#endif


script_manager * script_manager::_instance = NULL;

script_manager * script_scheduler()
{
	return script_manager::_instance;
}

script_manager::script_manager(script_interface * internal)
{
	interface = internal;
#ifdef PCL_H
	main_thread = co_current();
#endif
	_std_out = -1;
	_std_err = -1;

	out = NULL;

	return_code = 0;
	signal_code = 0;

	import(config::modules);
}

script_manager::~script_manager()
{
	wait();
	delete interface;
}

void script_manager::DELETE_INSTANCE()
{
	delete _instance;
}


void script_manager::block(p_data a, int & ret_code, int & sig_code)
{
#ifdef PCL_H
	if(config::threads > 1)
	{
		_fibers.push_back(subprocess_data(a));
		//clean up output/error pipes
#ifdef pipe_data
		fflush(out);
		fclose(out);

		_dup2(_std_out, STDOUT_FILENO);
		_dup2(_std_err, STDERR_FILENO);


		_close(_std_out);
		_close(_std_err);

		std::string std_out = out_current;
		std::string w_dir = working_dir;
#endif
		co_call(main_thread);
#ifdef pipe_data

		out_current = std_out;
		working_dir = w_dir;

		_std_out = _dup(STDOUT_FILENO);
		_std_err = _dup(STDERR_FILENO);

		out = fopen(out_current.c_str(), "a");
		if(out)
		{
			_dup2(_fileno(out), STDOUT_FILENO);
			_dup2(_fileno(out), STDERR_FILENO);
		}
#endif
	}
	else
	{
#endif
#ifndef _Windows
		int status;
		if(-1 == waitpid(a, &status, 0))
		{
			std::cerr << "waitpid error." << std::endl;
		}

		if(WIFEXITED(status))
		{
			return_code = WEXITSTATUS(status);
		}
		else if(WIFSIGNALED(status))
		{
			signal_code = WTERMSIG(status);
		}
#endif
#ifdef PCL_H
	}
#endif
	ret_code = return_code;
	sig_code = signal_code;
	return_code = 0;
	signal_code = 0;
};

void script_manager::try_unblock(bool force)
{
	int size = _fibers.size();
	if(size == 0) { return; }

#ifdef PCL_H
#ifdef _Windows
	HANDLE * handles = new HANDLE[size];
	for(plist::iterator i(_fibers.begin()); i != _fibers.end(); ++j, ++i)
	{
		handles[j] = i->_waiting_on;
	}

	DWORD process = WaitForMultipleObjects(size, handles, false, force? INFINITE : 0);
	if(process < WAIT_OBJECT_0 || process > WAIT_OBJECT_0 + size)
	{
		return;
	}
	process -= WAIT_OBJECT_0;
#else
	int status;
	pid_t process = waitpid(-1, &status, force? 0 : WNOHANG);

	if(process == -1 || process == 0)
	{
		return;
	}
#endif

#ifdef _Windows

#else
	if(WIFEXITED(status))
	{
		return_code = WEXITSTATUS(status);
	}
	else if(WIFSIGNALED(status))
	{
		signal_code = WTERMSIG(status);
	}
	else
	{
		return;
		std::cout << "didn't exit or signal?" << std::endl;
	}
#endif

#ifdef _Windows
	j = 0;
	for(plist::iterator j(_fibers.begin()); j != _fibers.end(); ++i, ++j)
	{
		if(j == process)
		{
#else
	for(plist::iterator i(_fibers.begin()); i != _fibers.end(); ++i)
	{
		if(i->_waiting_on == process)
		{
#endif
			coroutine_t coro = i->coro;
			_fibers.erase(i);

			co_call(coro);
			return;
		}
	}
#endif
}

void script_manager::schedule_file(const path & file, const path & working, const path & output)
{
#ifndef DEBUG
	if(config::force == false && boost::filesystem::exists(output))
	{
		time_t write_time = boost::filesystem::last_write_time(output);

		if(write_time > boost::filesystem::last_write_time(file) && write_time > config::version)
		{
			print_file(out_current.c_str());
			return;
		}
	}
#endif

	_schedule.push(schedule_data(file, working, output));

#ifdef PCL_H
	if(_fibers.size() < config::threads )
	{
		coroutine_t coro = co_create(&script_manager::co_execute, NULL, NULL, 4096*4);
		if(coro != NULL)
		{
			co_call(coro);
		}
	}
	else if(config::threads > 1)
	{
		try_unblock(false);
	}
#endif
}

void script_manager::execute(const char * path)
{
#ifdef pipe_data
	out = fopen(out_current.c_str(), "w");

	if(out)
	{
		_std_out = _dup(STDOUT_FILENO);
		_std_err = _dup(STDERR_FILENO);

		_dup2(_fileno(out), STDOUT_FILENO);
		_dup2(_fileno(out), STDERR_FILENO);
#endif
		interface->run_script(path);
#ifdef pipe_data
		fflush(out);
		fclose(out);

		_dup2(_std_out, STDOUT_FILENO);
		_dup2(_std_err, STDERR_FILENO);

		_close(_std_out);
		_close(_std_err);

		print_file(out_current.c_str());
	}
	else
	{
		interface->run_script((char *) data);
	}
#endif
};

void script_manager::run_scheduled_files()
{
	while(_schedule.size())
	{
		working_dir = _schedule.top()._working;
		out_current = _schedule.top()._output.native();
		std::string f = _schedule.top()._file.native();

		_schedule.pop();

		execute(f.c_str());
	}
}

void script_manager::wait()
{
	while(_fibers.size())
	{
		try_unblock(true);
	}

	run_scheduled_files();
}

void script_manager::co_execute(void * data)
{
	script_scheduler()->run_scheduled_files();

#ifdef PCL_H
	co_exit();
#endif
};

void script_manager::print_file(const char * address)
{
	FILE * fp = fopen(address, "r");
	if(fp == NULL)
	{
		return;
	}

	char BUFFER[128];
	long int rc;
	do
	{
		rc = fread((void *) BUFFER, 1, sizeof(BUFFER)-1, fp);
		if(rc == 0)
		{
			break;
		}
		BUFFER[rc] = '\0';
		std::cout << BUFFER;
	} while(rc == sizeof(BUFFER)-1);

	fclose(fp);
}

void script_manager::import(const boost::filesystem::path & p)
{
	if(!is_directory(p))
	{
		return;
	}

	insert_path(p);

	boost::filesystem::directory_iterator begin(p);
	boost::filesystem::directory_iterator end;
	for( ; begin != end; ++begin)
	{
		import(*begin);
	}
}

