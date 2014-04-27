"""
  Regression test 18: VIZ_OUTPUT can reference an uninstantiated object,
     leading to a crash.

     Failure: crash (INTERNAL ERROR or seg fault)
     Success: run exits with normal failure code

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-07-17
"""

t = McellTest("18-uninstantiated_reference_crash_2")

if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)
else:
	found = False
	for line in open(t.stderr):
		if r"Cannot produce visualization for the uninstantiated object 'uninstantiated'" not in line:
			found = True
	if found is False:
		print "Expected an error message in test '{0}'...".format(t.name)

