"""  
  Regression test 27: Two walls are coincident with accuracy of EPS_C.


  Failure: no errors
  Success: run exits with afailure code and prints a message

  Author: Boris Kaminsky
  Date: 2011-10-17
"""

t = McellTest("27-walls_coincident")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name);

