"""
  Regression test 04: When two surface molecules undergo a reaction in which
        both reagents are preserved, and in which both reagents change
        orientation, counting does not occur properly.

        Failure: as flippy increases, a000- and b000- remain 0

        Success: a000- and b000- increase to non-zero values
                 flippy increases
                 at all times a000- + a000+ == 300
                 at all times b000- + b000+ == 300

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-09-10
"""

t = McellTest("04-rx_flipflip")
counts = t.reaction_output("counts.txt");
counts[0] + counts[1] == 300
counts[2] + counts[3] == 300

counts.min_time = 5e-5
counts[0:3] > 40
counts[0:3] < 260
counts[4] > 10000
counts[4] < 1e300

