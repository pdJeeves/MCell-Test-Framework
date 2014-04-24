"""
  Regression test 26: Two small walls partially share an edge with
                      a bigger wall and overlap bigger wall.

  Failure: no errors
  Success: run exits with a failure code and prints a message

  Author: Boris Kaminsky and Jacob Czech
  Date:   2011-05-25
"""

t = McellTest("26-walls_overlap_share_edge")
if(t.exit_code != 1):
	print "In test '{0}', expected exit code 1.".format(t.name)
