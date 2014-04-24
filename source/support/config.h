#ifndef _config_h_
#define _config_h_
#include <boost/filesystem.hpp>
#include <string>

using std::string;
using boost::filesystem::path;

typedef unsigned int uint;
typedef const char * cstring;

class query_interface;

extern string remove_quotes(string i);

struct config
{
	static bool read_configuration_file(const char * config_file_name);
	static void process_args(int argc, const char * argv[]);

	static query_interface * query;
	static cstring name;
	static path    executable;
	static time_t  version;
	static path    tests;
	static path    modules;
	static path    cache;
	static path    data;
	static uint    threads;
	static bool    force;
	static bool	   run_all;
};

#endif
