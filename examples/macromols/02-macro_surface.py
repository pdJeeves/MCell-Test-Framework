"""
  Test 02: Simple surface macromolecules.
 
     This test is analogous to test #1, but using surface molecules.  Counting
     encompasses considerably more options for surface complexes, due to the
     additional dimensions added by orientations.  As a result, this includes
     a much more comprehensive collection of counts than test #1.
 
     See test_macromols.py for a matrix giving the static constraints which
     this simulation must obey.  (For instance, the number of subunits in any
     state must never differ from the number initially placed, regardless how
     we sum over the states.  This gives rise to quite a number of validity
     constraints over the set of counts we produce here.)
 
     An equilibrium derived via a first order iterative "state machine" model
     is:
 
         Subunits in doubly-unbound dimers: 15.5348 in each orientation combo
         Subunits in singly-bound dimers:   14.9938 in each orientation combo
         Subunits in doubly-bound dimers:   14.4715 in each orientation combo
 
     All equilibria in the test suite for this test may be derived from these
     numbers.  Note that the numbers for the doubly-bound or doubly-unbound
     cases will appear to be roughly twice as large as those for the
     singly-bound cases because the bound and unbound are counted as two
     separate rules for the singly-bound case, whereas they are counted
     together if the subunits in the dimer are both bound or both unbound.
 
  Author: Jed Wing <jed@salk.edu>
  Date:   2008-04-04
"""

t = McellTest("02-macro_surface")
counts = t['counts']

counts[0] == counts[1] + counts[2]
counts[1] == counts[3]
counts[2] == counts[4]
counts[1] == counts[5] + counts[7]
counts[2] == counts[6] + counts[8]
counts[0] == counts[9]
counts[1] == counts[10]
counts[2] == counts[11]
counts[12] == counts[0] + counts[22]
counts[13] == counts[0] + counts[21]
counts[14] == counts[1] + counts[25]
counts[15] == counts[2] + counts[26]
counts[16] == counts[1] + counts[23]
counts[17] == counts[2] + counts[24]
counts[18] == counts[19] + counts[20]
counts[18] == counts[21] + counts[22]
counts[19] == counts[39]
counts[20] == counts[40]
counts[21] == counts[37]
counts[22] == counts[38]
counts[19] == counts[23] + counts[25]
counts[20] == counts[24] + counts[26]
counts[18] == counts[27]
counts[19] == counts[28]
counts[20] == counts[29]
counts[30] == counts[4] + counts[18]
counts[31] == counts[3] + counts[18]
counts[32] == counts[7] + counts[19]
counts[33] == counts[8] + counts[20]
counts[34] == counts[5] + counts[19]
counts[35] == counts[6] + counts[20]
counts[36] == counts[37] + counts[38]
counts[36] == counts[39] + counts[40]
counts[37] == counts[41] + counts[43]
counts[38] == counts[42] + counts[44]
counts[36] == counts[45]
counts[37] == counts[46]
counts[38] == counts[47]
counts[48] == counts[36] + counts[58]
counts[49] == counts[36] + counts[57]
counts[50] == counts[37] + counts[61]
counts[51] == counts[38] + counts[62]
counts[52] == counts[37] + counts[59]
counts[53] == counts[38] + counts[60]
counts[54] == counts[55] + counts[56]
counts[55] == counts[57]
counts[56] == counts[58]
counts[55] == counts[59] + counts[61]
counts[56] == counts[60] + counts[62]
counts[54] == counts[63]
counts[55] == counts[64]
counts[56] == counts[65]
counts[66] == counts[40] + counts[54]
counts[67] == counts[39] + counts[54]
counts[68] == counts[43] + counts[55]
counts[69] == counts[44] + counts[56]
counts[70] == counts[41] + counts[55]
counts[71] == counts[42] + counts[56]
counts[72] == counts[0] + counts[18]
counts[73] == counts[2] + counts[20]
counts[74] == counts[1] + counts[19]
counts[75] == counts[36] + counts[54]
counts[76] == counts[38] + counts[56]
counts[77] == counts[37] + counts[55] 
counts[36] == counts[18]
counts[0] + counts[18] + counts[36] + counts[54] == 180
counts[36] + counts[54] + counts[78] == 800


Average(counts[23:26]) in Distribution(7.4969, 1.2)
Average(counts[41:44]) in Distribution(7.4969, 1.2)
Average(counts[59:62]) in Distribution(14.4715, 1.8)
Average(counts[28:29]) in Distribution(14.9938, 1.2)
Average(counts[37:40]) in Distribution(14.9938, 1.2)
Average(counts[46:47]) in Distribution(14.9938, 1.2)
Average(counts[19:22]) in Distribution(14.9938, 1.2)
Average(counts[ 5: 8]) in Distribution(15.5348, 1.8)
Average(counts[64:65]) in Distribution(28.943, 1.8)
Average(counts[55:58]) in Distribution(28.943, 1.8)
Average(counts[50:53]) in Distribution(29.4653, 1.2)
Average(counts[18   ]) in Distribution(29.9876, 1.2)
Average(counts[27   ]) in Distribution(29.9876, 1.2)
Average(counts[36   ]) in Distribution(29.9876, 1.2)
Average(counts[45   ]) in Distribution(29.9876, 1.2)
Average(counts[32:35]) in Distribution(30.5286, 1.2)
Average(counts[1 : 4]) in Distribution(31.0696, 1.8)
Average(counts[10:11]) in Distribution(31.0696, 1.8)
Average(counts[68:71]) in Distribution(36.4399, 1.8)
Average(counts[14:17]) in Distribution(38.5665, 1.8)
Average(counts[76:77]) in Distribution(43.9368, 1.2)
Average(counts[73:74]) in Distribution(46.0634, 1.2)
Average(counts[54   ]) in Distribution(57.886, 1.8)
Average(counts[63   ]) in Distribution(57.886, 1.8)
Average(counts[48:49]) in Distribution(58.9306, 1.2)
Average(counts[30:31]) in Distribution(61.0572, 1.2)
Average(counts[ 0   ]) in Distribution(62.1392, 1.8)
Average(counts[ 9   ]) in Distribution(62.1392, 1.8)
Average(counts[66:67]) in Distribution(72.8798, 1.8)
Average(counts[12:13]) in Distribution(77.133, 1.8)
Average(counts[75   ]) in Distribution(87.8736, 1.2)
Average(counts[72   ]) in Distribution(92.1268, 1.2)
Average(counts[78   ]) in Distribution(712.1264, 2.4)

