"""
  Test-15: Surface products placement done on sphere in the presence
           of restrictive regions.

      The test is done is done on sphere with three overlapping regions.
      Here we test the surface products placement that should follow
      the established policy.

   Author: Boris Kaminsky <borisk@psc.edu>
   Date: 2011-08-31

   Note: region "r4" is an intersection of three regions "r1", "r2", "r3"
         region "r5" consists of all walls on the sphere minus region "r4"
"""

t = McellTest("15-overlappingSRs")
t.reaction_output("r4") > 0
t.reaction_output("r5") == 0
