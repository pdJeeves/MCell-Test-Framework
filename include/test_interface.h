#ifndef _test_interface_h_
#define _test_interface_h_
#include "average.h"
#include "scheduler_interface.h"
#include "data_table.hpp"

//working dir - where mcell puts files
//current dir - where we put files
//so only need to know working.

extern int get_least_edit_distance(const std::string & a, const std::string & b);

template<typename T>
class test_interface
{
protected:
	typedef std::vector<data_table<T> > table_map;

	table_map    _loaded;

	int         _return_code;
	int         _signal_code;

	std::string _logfile;
	std::string _errfile;

public:
	test_interface()
	{
		_return_code = 0;
		_signal_code = 0;

		boost::filesystem::path location = script_scheduler()->get_current_directory();
		location /= "std";
		_logfile = location.native();
		_errfile = _logfile + "err";
		_logfile += "out";
	}

	int get_logfile() const { return _logfile; }
	int get_errfile() const { return _errfile; }

	int get_return_code() const { return _return_code; }
	int get_singal_code() const { return _signal_code; }

	data_table<T> GetOutput(std::string name)
	{
		if(_loaded.empty())
		{
			return NULL;
		}

		int cur = 0;
		int min = 10000;

		for(int i = 0; i < _loaded.size() && min != 0; ++i)
		{
			int temp = least_edit_distance(name, _loaded[i].name);

			if(temp < min)
			{
				min = temp;
				cur = i;
			}
		}

		if(((double) min / name.size()) < 1)
		{
			return data_table<T>(_loaded[cur].second);
		}
		
		return NULL;
	}
};


class double_test : public test_interface<double>
{
public:
	static double_test create_double_test(std::string name);
	static double_test create_double_test_option(std::string name, bool verbose);

private:
	double_test(std::string name, bool verbose);

	void initialize(const std::string & name, bool verbose);
};

class average_test : public test_interface<average>
{
public:
	static average_test create_average_test(std::string name, int iterations);
	static average_test create_average_test_option(std::string name, int iterations, bool verbose);

private:
	average_test(std::string name, int iterations, bool verbose);

	void initialize(const std::string & name, int iterations, bool verbose);
};

#endif
