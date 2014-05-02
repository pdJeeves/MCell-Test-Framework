#ifndef _script_manager_h_
#define _script_manager_h_
#include "scheduler_interface.h"
#include "platform.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <queue>
#include <iostream>
#include <vector>
#include <string>

#include "pcl.h"
#define pipe_data

using std::vector;
using std::string;

extern string remove_quotes(string i);

#ifdef _Windows
typedef HANDLE p_data;
#else
typedef pid_t p_data;
#endif

class script_interface
{
public:
	script_interface() { }
	virtual ~script_interface() { };

	virtual void insert_path(const char * path) = 0;
	virtual void run_script(const char * path) = 0;
};

class script_manager : public scheduler_interface
{
typedef boost::filesystem::path path;
	struct schedule_data
	{
		schedule_data(const path & f, const path & w, const path & o)
		: _file(f), _working(w), _output(o)
		{
			std::string s = f.stem().native();
			const char * p = s.c_str();
			_id = 0;
			for(int i = 0; i < 4 && *p; ++i, ++p)
			{
				_id = (_id << 8) | *p;
			}
		}

		path _file;
		path _working;
		path _output;

		int  _id;
	};

	struct schedule_compare
	{
		bool operator()(const schedule_data & a, const schedule_data & b)
		{
			return a._id < b._id;
		}
	};

	struct subprocess_data
	{
		subprocess_data(p_data & a)
		{
#ifdef PCL_H
			coro = co_current();
#endif
			_waiting_on = a;
		}
		subprocess_data(const subprocess_data & s)
		{
#ifdef PCL_H
			coro = s.coro;
#endif
			_waiting_on = s._waiting_on;
		}

#ifdef PCL_H
		coroutine_t coro;
#endif
		p_data  _waiting_on;
	};

typedef std::priority_queue<schedule_data, std::vector<schedule_data>, schedule_compare> schedule_type;
typedef std::list<subprocess_data> plist;

private:
	static script_manager * _instance;
#ifdef PCL_H
		coroutine_t main_thread;
#endif

	int _std_out;
	int _std_err;

	path        current_dir;
	path        working_dir;
	std::string out_current;
	FILE * out;

	int return_code;
	int signal_code;

	plist _fibers;

	script_interface * interface;

	schedule_type      _schedule;

	static void print_file(const char * address);
	static void co_execute(void * data);
	void execute(const char * path);
	static void DELETE_INSTANCE();

	void import(const path &);

	void insert_path(const path & p)
	{
		interface->insert_path(p.native().c_str());
	}

	void run_scheduled_files();

public:
	template<class T>
	static void initialize(int argc, const char * argv[])
	{
		if(_instance == NULL)
		{
			_instance = new script_manager(new T(argc, argv));
			atexit(DELETE_INSTANCE);
		}
	}
	static script_manager * instance()
	{
		return _instance;
	}
	script_manager(script_interface * internal);
	~script_manager();

	void block(p_data , int & ret_code, int & sig_code);
	void try_unblock(bool);

	void schedule_file(const path & file,
	                   const path & working_directory,
	                   const path & output_file);

	void wait();

	void set_working_directory() const
	{
		boost::filesystem::current_path(working_dir);
	}

	void set_working_directory(const std::string & s) const
	{
		path temp = working_dir;
		boost::filesystem::create_directory(temp /= s);
		boost::filesystem::current_path(temp);
	}

	path get_working_directory() const
	{
		return working_dir;
	}
	path get_current_directory() const
	{
		return current_dir;
	}
};


#endif
