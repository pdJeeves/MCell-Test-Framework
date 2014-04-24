#include "../query_parser.h"
#include "config.h"
#include "cfgpath.h"
#include <fstream>

time_t config::version = 0;
query_interface * config::query = NULL;

string remove_quotes(string i)
{
	if (*(i.begin()) == '"')
	{
		if (*(i.rbegin()) == '"')
		{
			i = i.substr(1, i.length() - 2);
		}
		else
		{
			i = i.substr(1, i.length() - 1);
		}
	}
	else if (*(i.rbegin()) == '"')
	{
		i = i.substr(0, i.length() - 1);
	}

	return i;
}

void config::process_args(int argc, const char * argv[])
{
	for(int i = 1; i < argc; )
	{
		if(strcmp("--force", argv[i]) == 0)
		{
			force = true;
		}
		else if(strcmp("--all", argv[i]) == 0)
		{
			run_all = true;
		}
		else if(0 == strncmp(argv[i], "--threads=", 10))
		{
			threads = atoi(argv[i] + 11);
		}
		else if(0 == strcmp(argv[i], "--query"))
		{
			++i;
			query = query_interface::parse(i, argc, argv);
			continue;
		}

		++i;
	}

	char BUFFER[256];
	get_user_data_folder(BUFFER, sizeof(BUFFER),  name);
	data= BUFFER;
	cache = data;
	cache /= ".output";
	get_user_config_file(BUFFER, sizeof(BUFFER),  name);

	if(!read_configuration_file(BUFFER))
	{
		std::cerr << "unable to open configuration file: \"" << BUFFER << "\".\n";
		exit(-1);
	}
	version = boost::filesystem::last_write_time(executable);
}

bool config::read_configuration_file(const char * config_file_name)
{
		std::fstream fp;
		fp.open(config_file_name);

		if(!fp.is_open())
		{
			fp.open(config_file_name, std::ios_base::in |  std::ios_base::out |  std::ios_base::trunc);
			if(!fp.is_open())
			{
				return false;
			}
		}
		else if(fp.good())
		{
			string s;

			if(std::getline(fp, s).good())
			{
				executable = remove_quotes(s);

				if(std::getline(fp, s).good())
				{
					tests = remove_quotes(s);

					if(std::getline(fp, s).good())
					{
						modules = remove_quotes(s);
					}
				}
			}

			fp.clear();
			fp.seekp(0, std::ios_base::beg);
		}

		bool set_something = false;
		if(!boost::filesystem::exists(executable)
		|| !boost::filesystem::is_regular_file(executable))
		{
		std::cout << "Enter location of mcell executable: \n";
		for(;;)
		{
			executable.clear();
			std::cin >> executable;
			if(!boost::filesystem::exists(executable))
			{
				std::cout << "The given location does not exist, please enter a new one: \n";
				continue;
			}
			if(!boost::filesystem::is_regular_file(executable))
			{
				std::cout << "The given location exists, but is not a file, please enter a new one: \n";
				continue;
			}

			set_something = true;
			break;
		}
		}

		if(!boost::filesystem::exists(tests)
		|| !boost::filesystem::is_directory(tests))
		{
		std::cout << "Enter location to search for python scripts to run: \n";
		for(;;)
		{
			tests.clear();
			std::cin >> tests;
			if(!boost::filesystem::exists(tests))
			{
				std::cout << "The given location does not exist, please enter a new one: \n";
				continue;
			}
			if(!boost::filesystem::is_directory(tests))
			{
				std::cout << "The given location exists, but is not a directory, please enter a new one: \n";
				continue;
			}

			set_something = true;
			break;
		}
		}

		if(!boost::filesystem::exists(modules)
		|| !boost::filesystem::is_directory(modules))
		{
		std::cout << "Enter location to search for python extensions to run: \n";
		for(;;)
		{
			modules.clear();
			std::cin >> modules;
			if(!boost::filesystem::exists(modules))
			{
				std::cout << "The given location does not exist, please enter a new one: \n";
				continue;
			}
			if(!boost::filesystem::is_directory(modules))
			{
				std::cout << "The given location exists, but is not a directory, please enter a new one: \n";
				continue;
			}

			set_something = true;
			break;
		}
		}

		if(set_something)
		{
			fp << executable << "\n";
			fp << tests << "\n";
			fp << modules << "\n";
		}

		fp.flush();
		fp.close();

		return true;
}

