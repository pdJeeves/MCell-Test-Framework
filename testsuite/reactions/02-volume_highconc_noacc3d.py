"""
 Test 02: All volume rxn types, simple test of equilibrium and reaction
          rates, no accurate 3d reactions.
 
    A quantitative test of the basic volume-volume reaction types (uni, bi,
    tri).  Note that the bi-directional pathways are in here twice.  This is
    so that they can be tested via both the non-trimolecular and trimolecular
    code pathways.  The latter is accomplished by adding a bogus trimolecular
    reaction involving the relevant molecules.

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-11-07
"""

t = McellTest("02-volume_highconc_noacc3d.mdl")
v_out = t.reaction_output("V_out")
v_out == equilibrium(500, 25)

v_out[1]  == v_out[2]
v_out[3]  == v_out[4]
v_out[5]  == v_out[6]
v_out[7]  == v_out[8]
v_out[9]  == v_out[10:11]
v_out[12] == v_out[13:14]
v_out[15] == v_out[16:17]

v_out[0]  + v_out[18] == 1000
v_out[1]  + v_out[19] == 1000
v_out[3]  + v_out[20] == 1000
v_out[5]  + v_out[21] == 1000
v_out[7]  + v_out[22] == 1000
v_out[9]  + v_out[23] == 1000
v_out[12] + v_out[24] == 1000
v_out[15] + v_out[25] == 1000

rxn_out = t.reaction_output("rxn_out")
rxn_out.min_time = 5e-3
rxn_out.base_time= 0.0
rxn_out[:4]    == equilibrium(1e5, 1.5e4);
rxn_out[5:7]   == equilibrium(2e4, 6e3);
rxn_out[8:12]  == equilibrium(1e5, 1.5e4);
rxn_out[13:15] == equilibrium(2e4, 6e3);

