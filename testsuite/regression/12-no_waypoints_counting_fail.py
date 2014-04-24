"""
  Regression test 12: MCell fails to count oriented surface molecule reaction
     products in a simulation which has no waypoints.  This is a modified
     version of the test case Boris sent when he reported the bug.

     Failure: Any of the first 7 columns in the count differ at any time
              Any of the next 14 columns in the count differ at any time
              Any of the last 12 columns are non-zero at any time
              The first and 8th column ever sum to anything other than 500
              The first column does not eventually decrease from 500
     Success: Run completes successfully, counts do not trigger the
              above failure criteria.

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-03-28
"""

t = McellTest("12-no_waypoints_counting_fail")

counts = t.reaction_output("counts.txt")
counts[0] == counts[1:6]
counts[0] + counts[7] == 500
counts[7] == counts[8:20]
counts[21:32] == 0

counts.min_time = 9e-5
counts[0:20] > 1

