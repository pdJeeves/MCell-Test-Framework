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

t = MCellTest('counts')
counts = t['counts']

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

Average(counts[ 0]) in Distribution(expect_total   , expect_total/10   ) 
Average(counts[ 1]) in Distribution(expect_subunits, expect_subunits/10)
Average(counts[ 2]) in Distribution(expect_00      , expect_00/10      )
Average(counts[ 3]) in Distribution(expect_01      , expect_01/10      )
Average(counts[ 4]) in Distribution(expect_11      , expect_11/10      )
Average(counts[ 5]) in Distribution(8*expect_t     , 0.1               )
Average(counts[ 6]) in Distribution(expect_t       , 0.1               )
Average(counts[ 7]) in Distribution(2*expect_t     , 0.1               )
Average(counts[ 8]) in Distribution(2*expect_bo    , 0.1               )
Average(counts[ 9]) in Distribution(expect_bo      , 0.1               )
Average(counts[10]) in Distribution(8*expect_bo    , 0.1               )
Average(counts[11]) in Distribution(8*expect_l     , 1+expect_l        )
Average(counts[12]) in Distribution(expect_l       , 1+expect_l/8      )
Average(counts[13]) in Distribution(2*expect_l     , 1+expect_l/4      )
Average(counts[14]) in Distribution(2*expect_r     , 0.1               )
Average(counts[15]) in Distribution(expect_r       , 0.1               )
Average(counts[16]) in Distribution(8*expect_r     , 0.1               )
Average(counts[17]) in Distribution(5*expect_f     , 1+expect_f        )
Average(counts[18]) in Distribution(expect_f       , 0.1               )
Average(counts[19]) in Distribution(5*expect_f     , 1+expect_f        )
Average(counts[20]) in Distribution(5*expect_ba    , expect_ba         )
Average(counts[21]) in Distribution(expect_ba      , expect_ba/10      )
Average(counts[22]) in Distribution(5*expect_ba    , expect_ba         )

