"""
  Regression test 11: MCell cannot parse quoted-tickmark orientations if the
     molecule name is a single character long.  This is a stripped down
     version of the test case Boris sent when he reported the bug.

     Failure: Parse error on the count statements.
     Success: Run completes successfully.

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-03-27
"""

McellTest("11-quoted_tickmark_counts_parse_error")

