#ifndef _scheduler_interface_h_
#define _scheduler_interface_h_
#include <boost/filesystem/path.hpp>
#include "platform.h"

class scheduler_interface
{
public:
typedef boost::filesystem::path path;
	scheduler_interface() { }
	virtual ~scheduler_interface() { }
	virtual void block(p_data , int & ret_code, int & sig_code) = 0;
	virtual void try_unblock(bool) = 0;

	virtual void schedule_file(const path & file,
	                           const path & working_directory,
	                           const path & output_file) = 0;

	virtual void run_scheduled_files() = 0;

	virtual void set_working_directory() const = 0;
	virtual void wait() = 0;

	virtual path get_working_directory() const = 0;
	virtual path get_current_directory() const = 0;
};

extern scheduler_interface * script_scheduler();

#endif
