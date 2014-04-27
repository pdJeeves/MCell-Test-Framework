"""
  Regression test 23: Walls of the two objects are overlapped.
                     In fact two walls (0 and 1) of the "small_box" are
                     completely inside the wall 3 of the "box".
 
   Failure: no errors
   Success: run exits with a failure code and prints a message
 
   Author: Boris Kaminsky <borisk@psc.edu>
   Date: 2011-05-20
"""

t = McellTest("23-walls_overlap_two_objects")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)

