"""
  Regression test 24: Two walls of the object Cylinder are partially 
                      overlapped.
 
  Failure: no errors
  Success: run exits with a failure code and prints a message
  
  Author: Boris Kaminsky and Jacob Czech
  Date: 2011-05-23
"""

t = McellTest("24-walls_overlap_partial")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)
