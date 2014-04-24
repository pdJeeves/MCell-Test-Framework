"""
  Regression test 03: When two surfaces coincide and one of them is
        transparent, odd things happen.  Counting, for one, can become
        confused.  Essentially, the transparent surface may be re-collided in
        certain cases.

        Failure: counts go negative, or extremely positive
                 total count of cannonballs != 500

        Success: for entire run, count 122L+122R == 122 == 500  AND
                 for entire run, count r122L+r122R == r122 == 500

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-09-05
  Note: As of 05/23/2011 we disallow overlapped walls, so the coodinates
        of box122R are slightly shifted.
        Modified by Boris Kaminsky
        Date: 2011-05-23
"""

t = McellTest("03-coincident_surfaces")
cannonballs = t.reaction_output("cannonballs.txt")
cannonballs[0] + cannonballs[1] == cannonballs[2]
cannonballs[2] == 500
cannonballs[3] + cannonballs[4] == cannonballs[5]
cannonballs[5] == 500

