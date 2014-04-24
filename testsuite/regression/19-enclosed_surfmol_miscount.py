"""
  Regression test 19: surface molecule diffusion gives incorrect counts for
     "enclosed" counts.

     Failure: Counts remain 50/50/0
     Success: Counts equilibrate to around 50/25/25

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-07-31
  Note:  As of 5/23/2011 we disallow overlapped walls, so the coordinates
         of "counter_right" box are slightly shifted to allow for
         the overlap test to pass.
         Modified by Boris Kaminsky
         Date: 2011-05-23
"""

t = McellTest("19-enclosed_surfmol_miscount")
A = t.reaction_output("A")
A[0] == 50
A[0] == A[1] + A[2]

A[1:2] > 0
A[1:2] < 50
A[1:2] == equilibrium(25.0, 5.0)

