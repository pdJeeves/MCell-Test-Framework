#ifndef _test_interface_h_
#define _test_interface_h_
#include "../support/average.h"
#include "../scheduler_interface.h"
#include "../data_tables/data_table.hpp"

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

		boost::filesystem::path location = script_scheduler()->get_output_directory();
		location /= "std";
		_logfile = location.native();
		_errfile = _logfile + "err";
		_logfile += "out";
	}

	int get_logfile() const { return _logfile; }
	int get_errfile() const { return _errfile; }

	int get_return_code() const { return _return_code; }
	int get_singal_code() const { return _signal_code; }

	data_table<T> * GetOutput(std::string name)
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

		return _loaded[cur].second;
	}
};


class double_test : private test_interface<double>
{
public:
	double_test(std::string name);
	double_test(std::string name, bool verbose);

	void initialize(const std::string & name, bool verbose);
};

class average_test : private test_interface<average>
{
public:
	average_test(std::string name, int iterations);
	average_test(std::string name, int iterations, bool verbose);

	void initialize(const std::string & name, int iterations, bool verbose);
};

#endif
