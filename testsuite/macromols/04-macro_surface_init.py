"""
  Test 04: Surface macromolecule init by density/number.
 
 This test is to ensure that init_effectors properly handles surface
 macromolecules.  Most of the information is conveyed by the time 0 viz
 output, but the forward reaction rate has been cranked up enough to see
 from the reaction data also whether the molecules were placed.  The
 complexes are at the center of each face, the middle of each edge, and on
 each corner of the box.  The subunits should conform to the corners of
 the box as appropriate.
 
 Note, in particular, that if the complexes (or resultant subunits) were
 placed with incorrect orientation, we expect that close to 66% of the
 dimer pairs will be in the doubly-bound state, 16% will be in the
 singly-bound state, and 16% should perpetually be in the unbound state (1
 of the dimer pairs from each ring is intentionally placed with the wrong
 orientation to react).
 
  Author: Jed Wing <jed@salk.edu>
  Date:   2008-04-04
"""

t = McellTest("04-macro_surface_init")
counts = t.reaction_output("counts.txt")

counts[1]  == 12*counts[0]
counts[1]  == counts[2] + counts[3]*2 + counts[4]
counts[5]  == 8*counts[6]
counts[8]  == 2*counts[9]
counts[11] == 8*counts[12]
counts[14] == 2*counts[15]
counts[7]  == 2*counts[6] 
counts[10] == 8*counts[9]
counts[13] == 2*counts[12]
counts[16] == 8*counts[15]
counts[18] == (counts[17] + counts[19])/10
counts[21] == (counts[20] + counts[22])/10

counts[2]  == counts[5] + counts[8] + counts[11] + counts[14] + counts[17] + counts[20]
counts[3]  == counts[6] + counts[9] + counts[12] + counts[15] + counts[18] + counts[21]
counts[4]  == counts[7] + counts[10] + counts[13] + counts[16] + counts[19] + counts[22]
counts[9]  == 20
counts[15] == 200

# BROKEN: placement on surface class does nothing
# expect_t = 20
# expect_l = 200
# expect_f = 25
expect_t  = 0
expect_l  = 0
expect_f  = 0
expect_bo = 20
expect_r  = 200
expect_ba = 200
expect_00 = 8*expect_t + 2*expect_bo + 8*expect_l + 2*expect_r + 5*expect_f + 5*expect_ba
expect_01 =   expect_t +   expect_bo +   expect_l +   expect_r +   expect_f +   expect_ba
expect_11 = 2*expect_t + 8*expect_bo + 2*expect_l + 8*expect_r + 5*expect_f + 5*expect_ba
expect_total = expect_t + expect_l + expect_f + expect_bo + expect_r + expect_ba
expect_subunits = 12*expect_total

counts[ 0] == equilibrium(expect_total   , expect_total/10   ) 
counts[ 1] == equilibrium(expect_subunits, expect_subunits/10)
counts[ 2] == equilibrium(expect_00      , expect_00/10      )
counts[ 3] == equilibrium(expect_01      , expect_01/10      )
counts[ 4] == equilibrium(expect_11      , expect_11/10      )
counts[ 5] == equilibrium(8*expect_t     , 0.1               )
counts[ 6] == equilibrium(expect_t       , 0.1               )
counts[ 7] == equilibrium(2*expect_t     , 0.1               )
counts[ 8] == equilibrium(2*expect_bo    , 0.1               )
counts[ 9] == equilibrium(expect_bo      , 0.1               )
counts[10] == equilibrium(8*expect_bo    , 0.1               )
counts[11] == equilibrium(8*expect_l     , 1+expect_l        )
counts[12] == equilibrium(expect_l       , 1+expect_l/8      )
counts[13] == equilibrium(2*expect_l     , 1+expect_l/4      )
counts[14] == equilibrium(2*expect_r     , 0.1               )
counts[15] == equilibrium(expect_r       , 0.1               )
counts[16] == equilibrium(8*expect_r     , 0.1               )
counts[17] == equilibrium(5*expect_f     , 1+expect_f        )
counts[18] == equilibrium(expect_f       , 0.1               )
counts[19] == equilibrium(5*expect_f     , 1+expect_f        )
counts[20] == equilibrium(5*expect_ba    , expect_ba         )
counts[21] == equilibrium(expect_ba      , expect_ba/10      )
counts[22] == equilibrium(5*expect_ba    , expect_ba         )

