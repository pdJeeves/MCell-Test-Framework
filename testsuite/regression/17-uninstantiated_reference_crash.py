"""
  Regression test 17: An instantiated release site can reference an
     uninstantiated object as its shape, leading to a crash.

     Failure: crash (INTERNAL ERROR or seg fault)
     Success: run exits with normal failure code

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-06-15
"""

t = McellTest("17-uninstantiated_reference_crash")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)
else:
	found = False
	for line in open(t.stderr):
		if r"Region neither instanced nor grouped with release site." not in line:
			found = True
	if found is False:
		print "Expected an error message in test '{0}'...".format(t.name)

