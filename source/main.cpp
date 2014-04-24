#include <iostream>
#include <iomanip>
#include "query_parser.h"
#include "parser/script_file_parser.h"
#include "script_manager.h"
#include "support/config.h"

#define USING_PYTHON

#ifdef USING_PYTHON
#include "python_interface.hpp"
#endif

#define _name "McellTestFramework"
cstring config::name        = _name;
path    config::executable	= "/usr/local/bin/mcell";
path    config::tests 	    = "~/mcell/testsuite/tests";
path    config::modules	    = "~/mcell/testsuite/extensions";
path    config::cache 	    = "~/.cache/" _name "/";
path    config::data  	    = "~/.share/" _name "/";
uint    config::threads     = 4;
bool    config::force 	    = false;
bool    config::run_all	    = false;

const char * help =
		"NAME"
		"\n\tMCellTest - test framework for the Mcell stochastic diffusion simulator"
		"\n\nSYNOPSIS"
		"\n\tMCellTest [--threads] --query `jed wing` and before 2011-12-12"
		"\n\nDescription"
		"\n\tMCellTest reads in python scripts from the folder specified in it's config"
		"\n\tfile and runs ones which begin with a \"\"\" marker, if they match the given"
		"\n\tquery.  If they have already been run after this version of mcell was compiled"
		"\n\tthen the data from the previous run is used rather than running it again."
		"\n\tIt will be run anyway if the python script is more recent than the last run"
		"\n\tdata. However if that is the case it will rerun the python script--it will"
		"\n\trerun Mcell rather than using data from a previous run iff the .mdl is more"
		"\n\trecent than the data."
		"\n\n\tit is not capable of determining if the .mdl is more recent without running"
		"\n\tthe python script--which is not done because syntax erros in python scripts"
		"\n\tcause segfaults iff more than one thread is used."
		"\n\tit's config file should contain (all in quotes):"
		"\n\t\t-- mcell's path."
		"\n\t\t-- path to the runnable scripts."
		"\n\t\t-- the runnable script's import search path."
		"\n\t\t-- where finished runs should output data to."
		"\n\tand it must be located at '~/.config/McellTestFramework/config'."
		"\n\nOPTIONS"
		"\n\t--force\trun all python files regardless of if there is data from previous runs."
		"\n\n\t--threads= thread_number"
		"\n\t\trun the framework with the maximum number of mcell processes spawned simultaniously."
		"\n\t\tdefaults to 4"
		"\n\n\t--all\trun all of the python files in the directory."
		"\n\n\t--query the query to search keywords in the script files against."
		"\n\t\tSnytax (keywords are not case sensitive)."
	"\n\t\tQuery            ::= OrExpression"
	"\n\n\tOrExpression     ::= XorExpression"
	  "\n\t                   |   OrExpression  '|'   XorExpression"
	  "\n\t                   |   OrExpression  '||'  XorExpression"
	  "\n\t                   |   OrExpression  'or'  XorExpression"
	"\n\n\tXorExpression    ::= AndExpression"
	  "\n\t                   |   XorExpression '^'   AndExpression"
	  "\n\t                   |   XorExpression '^^'  AndExpression"
	  "\n\t                   |   XorExpression 'xor' AndExpression"
	"\n\n\tAndExpression    ::= PrefixExpression"
	  "\n\t                   |   AndExpression '&'   PrefixExpression"
	  "\n\t                   |   AndExpression '&&'  PrefixExpression"
	  "\n\t                   |   AndExpression 'and' PrefixExpression"
	"\n\n\tPrefixExpression  ::= PrefixExpression"
	  "\n\t                   |   '!'      KeywordExpression"
	  "\n\t                   |   'not'    KeywordExpression"
	  "\n\t                   |   'before' DateExpression"
	  "\n\t	                  |   'after'  DateExpression"
	"\n\n\tKeywordExpression ~= [^ ]*"
	  "\n\t                   |  \"[^\"]*\""
	"\n\n\tDateExpression ~= [0-9]{4}[-\\/][0-9]{2}[-\\/][0-9]{2}";

bool get_confirmation()
{
	string line;
	while(std::getline(std::cin, line))
	{
		switch(line[0] | 0x20)
		{
		case 'n': return false;
		case 'y': return true;
		default:
			std::cout << "please answer 'yes' or 'no'." << std::endl;
			break;
		}
	}
	return false;
}

/* current directory -- where the file is
 * working directory -- where mcell outputs it's files to
 * output directory  -- where we output our files to
 */
template<void (*callback)(const path & cur, const path & work, const path & out)>
void scan_filesystem(const path & cur, path mcell_output, path framework_out)
{
	if(!exists(cur))
	{
		return;
	}

	mcell_output /= cur.stem();
	boost::filesystem::create_directory(mcell_output);

	if(boost::filesystem::is_directory(cur))
	{
		framework_out  /= cur.stem();
		boost::filesystem::create_directory(framework_out);

		boost::filesystem::directory_iterator begin(cur);
		boost::filesystem::directory_iterator end;
		for( ; begin != end; ++begin)
		{
			scan_filesystem<callback>(*begin, mcell_output, framework_out);
		}
	}
	else if(boost::filesystem::is_regular_file(cur))
	{
		framework_out  /= (cur.stem().native() + ".out");
		callback(cur,mcell_output,framework_out);
	}
}

void run_file(const boost::filesystem::path & cur,
			  const boost::filesystem::path & work,
			  const boost::filesystem::path & out)
{
	script_scheduler()->schedule_file(cur, work, out);
}

void try_run_file(const boost::filesystem::path & cur,
		  	  	  const boost::filesystem::path & work,
		  	  	  const boost::filesystem::path & out)
{
	if(cur.extension().native().compare(".py"))
	{
		return;
	}

	script_file_parser script_file(cur.native());

	if(script_file.is_valid() && config::query->evaluate(script_file))
	{
		script_scheduler()->schedule_file(cur, work, out);
	}
}


int main(int argc, const char * argv[])
{
	config::process_args(argc, argv);
	script_manager::initialize<SCRIPTING_ENGINE>(argc, argv);

	if(config::run_all)
	{
		scan_filesystem<run_file>(config::tests, config::cache, config::data);
		script_scheduler()->wait();
	}
	else if(config::query == NULL)
	{
		std::cout << help;
	}
	else
	{
		scan_filesystem<try_run_file>(config::tests, config::cache, config::data);
		script_scheduler()->wait();
	}

	return 0;
}
/*

	do
	{
		std::string query;
		std::cout << "Input Query:\n$ ";
		std::getline(std::cin, query);
		if(0 == query.compare("exit")
		|| 0 == query.compare("quit"))
		{
			return 0;
		}

		Xapian::Enquire enquire = database::instance()->search(query);
		Xapian::MSet matches = enquire.get_mset(0, 10);
		query.clear();

		int total = matches.get_matches_estimated();
		if(total > 10)
		{
			std::cout << "Matches 1 - 10 of " << total << ":\n";
		}
		else if(total == 0)
		{
			std::cout << "No matches.\n";
			continue;
		}
		else
		{
			std::cout << "Matches: " << "\n";
		}

	    for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
	    	string data = i.get_document().get_data();
	    	std::cout << std::setw(2) << i.get_rank() + 1 << " " << data.substr(0, data.find('\n')) << "\n";
	    }

	    std::cout << "\nRun this set of tests? ";
	    if(get_confirmation())
	    {
	    	std::vector<string> vec;
	    	matches = enquire.get_mset(0, database::instance()->get_doccount());
	    	for (Xapian::MSetIterator i = matches.begin(); i != matches.end(); ++i) {
	    		  	vec.push_back(i.get_document().get_data());
	    	}
	    	script_manager::instance()->run_files(vec);
	    }
	} while(1);

	return 0;
}
*/
