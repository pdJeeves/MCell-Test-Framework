"""
  Test 03: All surface rxn types, simple test of equilibrium and reaction
           rates.
 
      A quantitative test of the basic *-surface reaction types 
      (uni, bi, tri) in various combinations with surface (and 
      some volume) molecules.
 
      Note that the bi-directional pathways are in here twice.  
      This is so that they can be tested via both the non-
      trimolecular and trimolecular code pathways.  The latter 
      is accomplished by adding a bogus trimolecular reaction 
      involving the relevant molecules.

   Author: Jed Wing <jed@salk.edu>,  Boris Kaminsky <borisk@psc.edu>
   Date:   2008-12-05, 2012-06-20
"""


t = McellTest("03-surface")
V_out = t.reaction_output("V_out")
V_out == equilibrium(500, 25)

V_out[1] == V_out[2]
V_out[3] == V_out[4]
V_out[5] == V_out[6]
V_out[7] == V_out[8]
V_out[9] == V_out[10]
V_out[9] == V_out[11]
V_out[12] == V_out[13]
V_out[12] == V_out[14]
V_out[15] == V_out[16]
V_out[15] == V_out[17]
V_out[18] == V_out[19]
V_out[20] == V_out[21]
V_out[22] == V_out[23]
V_out[24] == V_out[25]
V_out[26] == V_out[27]
V_out[26] == V_out[28]
V_out[29] == V_out[30]
V_out[29] == V_out[31]
V_out[32] == V_out[33]
V_out[32] == V_out[34]
V_out[35] == V_out[36]
V_out[35] == V_out[37]
V_out[38] == V_out[39]
V_out[38] == V_out[40]
V_out[41] == V_out[42]
V_out[41] == V_out[43]
V_out[0] + V_out[44] == 1000
V_out[1] + V_out[45] == 1000
V_out[3] + V_out[46] == 1000
V_out[5] + V_out[47] == 1000
V_out[7] + V_out[48] == 1000
V_out[9] + V_out[49] == 1000
V_out[12] + V_out[50] == 1000
V_out[15] + V_out[51] == 1000
V_out[18] + V_out[52] == 1000
V_out[20] + V_out[53] == 1000
V_out[22] + V_out[54] == 1000
V_out[24] + V_out[55] == 1000
V_out[26] + V_out[56] == 1000
V_out[29] + V_out[57] == 1000
V_out[32] + V_out[58] == 1000
V_out[35] + V_out[59] == 1000
V_out[38] + V_out[60] == 1000
V_out[41] + V_out[61] == 1000

rxn_out = t.reaction_output("rxn_out").rate(0.0).after(5e-3)
rxn_out[:14]   == equilibrium(1e5, 1.5e4)
rxn_out[15:17] == equilibrium(1e4, 7.0e3)
rxn_out[18:33] == equilibrium(1e5, 1.5e4)
rxn_out[34:36] == equilibrium(1e4, 7.0e3)

t.reaction_output("b19_rxn_out").rate(0.0).after(5e-3) == equilibrium(3.3e4, 0.99e4)

b20 = t.reaction_output("b20_rxn_out").rate(0.0).after(5e-3)
b20[0]   == equilibrium(5.0e4, 1.0e4)
b20[1:3] == equilibrium(2.5e4, 0.5e4)
