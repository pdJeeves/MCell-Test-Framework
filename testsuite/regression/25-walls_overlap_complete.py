"""
  Regression test 25: Two walls of the object Cylinder are completely
                      overlapped (one wall is inside another)

  Failure: no errors
  Success: run exits with a failure code and prints a message

  Author: Boris Kaminsky and Jacob Czech
  Date: 2011-05-25
"""

t = McellTest("25-walls_overlap_complete")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)
