"""
  Regression test 13: Due to an error in the error-reporting code, if
     find_corresponding_region failed, a crash can be produced instead of a
     diagnostic.

     Failure: crash
     Success: run exits with a failure code and prints a message

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-04-12
"""

t = McellTest("13-find_corresponding_region_error_crash_1")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)
else:
	found = False
	for line in open(t.stderr):
		if r"Can't find new region corresponding to foo\.boxB,ALL for world\.bar\.rs \(copy of foo\.bar\.rs\)" not in line:
			found = True
	if found is False:
		print "Expected an error message in test '{0}'...".format(t.name)
