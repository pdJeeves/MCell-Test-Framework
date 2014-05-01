#include <iostream>
#include <iomanip>
#include <queue>
#include "query_parser.h"
#include "script_file_parser.h"
#include "script_manager.h"
#include "config.hpp"

#define USING_PYTHON

#ifdef USING_PYTHON
#include "python_interface.hpp"
#endif

#define _name "TestMCell"
cstring config::name        = _name;
path    config::executable	= "/usr/local/bin/mcell";
path    config::tests 	    = "~/mcell/testsuite/tests";
path    config::modules	    = "~/mcell/testsuite/extensions";
path    config::cache;
path    config::data;
uint    config::threads     = 4;
bool    config::force 	    = false;
bool    config::run_all	    = false;

/* current directory -- where the file is
 * working directory -- where mcell outputs it's files to
 * output directory  -- where we output our files to
 */
template<void (*callback)(const path & cur)>
void scan_filesystem(const path & cur)
{
	if(!exists(cur))
	{
		return;
	}

	if(boost::filesystem::is_directory(cur))
	{
/* can create an inifinte loop if we add a folder to the folder we're iterating in, so create a buffer queue */
		std::queue<path> BFS;

		boost::filesystem::directory_iterator begin(cur);
		boost::filesystem::directory_iterator end;
		for( ; begin != end; ++begin)
		{
			BFS.push(*begin);
		}

		while(!BFS.empty())
		{
			if(is_directory(BFS.front()))
			{
				scan_filesystem<callback>(BFS.front());
			}
			else if(boost::filesystem::is_regular_file(BFS.front()))
			{

				callback(BFS.front());
			}
			BFS.pop();
		}
	}
}

void create_directories(const boost::filesystem::path & cur, 
						      boost::filesystem::path & work,
							  boost::filesystem::path & out)
{
	auto parent = cur.parent_path();
	auto start  = parent.begin();

	for(auto p = config::tests.begin(); p != config::tests.end() && *p == *start; ++p, ++start) ;

	work = config::cache;
	out  = config::data;

	for(; start != parent.end(); ++start)
	{
		work /= *start;
		out  /= *start;
		boost::filesystem::create_directory(work);
		boost::filesystem::create_directory(out);
	}

	work /= cur.stem();
	out  /= (cur.stem().native() + ".out");

	boost::filesystem::create_directory(work);
}

void run_file(const boost::filesystem::path & cur)
{
	if(cur.extension().native().compare(".py"))
	{
		return;
	}

	script_file_parser script_file(cur.native());

	if(script_file.is_valid())
	{
		boost::filesystem::path work, out;
		create_directories(cur, work, out);
		script_scheduler()->schedule_file(cur, work, out);
	}
}

void try_run_file(const boost::filesystem::path & cur)
{
	if(cur.extension().native().compare(".py"))
	{
		return;
	}

	script_file_parser script_file(cur.native());

	if(script_file.is_valid())
	{
		boost::filesystem::path work, out;
		create_directories(cur, work, out);

		auto parent = cur.parent_path();
		auto start  = parent.begin();

		for(auto p = config::tests.begin(); p != config::tests.end() && *p == *start; ++p, ++start) ;

		for(; start != parent.end(); ++start)
		{
			script_file.add_keyword((*start).native());
		}

		script_file.add_title(cur.stem().native());

		if(config::query->evaluate(script_file))
		{
			script_scheduler()->schedule_file(cur, work, out);
		}
	}
}


int main(int argc, const char * argv[])
{
	config::process_args(argc, argv);
	script_manager::initialize<SCRIPTING_ENGINE>(argc, argv);

	if(config::run_all)
	{
		scan_filesystem<run_file>(config::tests);
		script_scheduler()->wait();
	}
	else if(config::query == NULL)
	{
		std::cout << "Use man MCellTest for help.";
	}
	else
	{
		scan_filesystem<try_run_file>(config::tests);
		script_scheduler()->wait();
	}

	std::cout << std::endl;

	return 0;
}

