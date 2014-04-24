#include <boost/filesystem.hpp>
#include "../data_tables/double_table_reader.h"
#include "test_interface.h"
#include "MCellInstance.h"

average_test::average_test(std::string name, int iterations)
{
	boost::filesystem::path p = script_scheduler()->get_current_directory();
	p /= name;
	name = p.native();

	if(!p.has_extension())
	{
		name += ".mdl";
	}

	if(boost::filesystem::exists(name))
	{
		initialize(name, iterations, false);
	}
}

average_test::average_test(std::string name, int iterations, bool verbose)
{
	boost::filesystem::path p = script_scheduler()->get_current_directory();
	p /= name;
	name = p.native();

	if(!p.has_extension())
	{
		name += ".mdl";
	}

	if(boost::filesystem::exists(name))
	{
		initialize(name, iterations, verbose);
	}
}

void average_test::initialize(const std::string & name, int iterations, bool verbose)
{
	typedef std::pair<std::string, plural_double_table_reader> file_pair;
	typedef std::map<std::string, plural_double_table_reader> file_map;
	file_map _read_files;

	for(int i = 0; i < iterations; ++i)
	{
		char buffer[16];
		sprintf(buffer, "%d", i);
		std::string seed(buffer);

		boost::filesystem::path working_dir = script_scheduler()->get_working_directory();
		working_dir /= seed;
		boost::filesystem::create_directory(working_dir);
		boost::filesystem::current_path(working_dir);

		{ //ensure the destructor gets called before the next line.
			MCell(name, _return_code, _signal_code)
											.logfile(_logfile)
											.errfile(_errfile)
											.verbose(verbose)
											.seed(buffer);
		}

		boost::filesystem::directory_iterator begin(working_dir);
		boost::filesystem::directory_iterator end;
		for( ; begin != end; ++begin)
		{
			if(boost::filesystem::is_regular_file(*begin))
			{
				boost::filesystem::path p = *begin;
				file_map::iterator i = _read_files.find(p.stem().native());
				if(i == _read_files.end())
				{
					_read_files.insert(file_pair(p.stem().native(), plural_double_table_reader()));
					i = _read_files.find(p.stem().native());
				}

				i->second.load_file(p.native());
			}
		}

		for(file_map::iterator i(_read_files.begin()); i != _read_files.end(); ++i)
		{
			boost::filesystem::path p = script_scheduler()->get_working_directory();
			p /= i->first;
			_loaded.push_back(i->second.get_data_table(i->first, p.native()));
		}
	}
}

double_test::double_test(std::string name)
{
	boost::filesystem::path p = script_scheduler()->get_current_directory();
	p /= name;
	name = p.native();

	if(!p.has_extension())
	{
		name += ".mdl";
	}

	if(boost::filesystem::exists(name))
	{
		initialize(name, false);
	}
}

double_test::double_test(std::string name, bool verbose)
{
	boost::filesystem::path p = script_scheduler()->get_current_directory();
	p /= name;
	name = p.native();

	if(!p.has_extension())
	{
		name += ".mdl";
	}

	if(boost::filesystem::exists(name))
	{
		initialize(name, verbose);
	}
}

void double_test::initialize(const std::string & name, bool verbose)
{
	script_scheduler()->set_working_directory();

	{ //ensure the destructor gets called before the next line.
		MCell(name, _return_code, _signal_code)
					.logfile(_logfile)
					.errfile(_errfile)
					.seed(rand() % 50000)
					.verbose(verbose);
	}

	boost::filesystem::directory_iterator begin(script_scheduler()->get_working_directory());
	boost::filesystem::directory_iterator end;
	for( ; begin != end; ++begin)
	{
		if(boost::filesystem::is_regular_file(*begin))
		{
			boost::filesystem::path p = *begin;
			_loaded.push_back(double_table_reader::load_file(p.stem().native(), p.native()));
		}
	}
}


