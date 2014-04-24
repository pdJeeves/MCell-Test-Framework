"""
   Test 04: All surface rxn types, simple test of equilibrium and reaction
            rates.

      A quantitative test of the basic *-surface reaction types (uni, bi)
      in various combinations with surface (and some volume) molecules and
      always with surface class as one of the reactants.

      This test is completely identical to the test "03-surface"
      except that we add surface class to all reactions and we do not
      test for trimolecular reactions.

   Authors: Jed Wing <jed@salk.edu> and Boris Kaminsky <borisk@psc.edu>
   Date:   2008-12-05, 2010-11-29, 2012-06-20
"""

t = McellTest("04-surface")

V_out = t.reaction_output("V_out");
V_out["S_b1"]	== V_out["S_b2"]
V_out["S_b3"]	== V_out["S_b4"]
V_out["S_bt1"]	== V_out["S_bt2"]
V_out["V_b1"]	== V_out["S_vb2"]
V_out["V_bt1"]	== V_out["S_vbt2"]
V_out["V_b3"]	== V_out["S_vb4"]
V_out["V_bt3"]	== V_out["S_vbt4"]
V_out["S_u"]	+  V_out["B_u"]    == 1000
V_out["S_b1"]	+  V_out["B_b"]    == 1000
V_out["S_b3"]   +  V_out["B_b2"]   == 1000
V_out["S_bt1"]  +  V_out["B_bt"]   == 1000
V_out["S_bt3"]  +  V_out["B_bt2"]  == 1000
V_out["V_b1"]   +  V_out["B_vb"]   == 1000
V_out["V_bt1"]  +  V_out["B_vbt"]  == 1000
V_out["V_b3"]   +  V_out["B_vb2"]  == 1000
V_out["V_bt3"]  +  V_out["B_vbt2"] == 1000

V_out == equilibrium(500, 25)

t.reaction_output("rxn_out").rate(0.0).after(5e-3) == equilibrium(1e5,1.5e4)
t.reaction_output("b13_u_out") == equilibrium(500, 25)
t.reaction_output("b13_u_rxn_out").rate(0.0).after(5e-3) == equilibrium(3.3e4,0.99e4)
t.reaction_output("b14_u_out") == equilibrium(1000, 50)
b14 = t.reaction_output("b14_u_rxn_out").rate(0.0).after(5e-3)
b14[0:1] == equilibrium(8.0e4, 2.4e4)
b14[2:3] == equilibrium(4.0e4, 1.2e4)
b14[4:5] == equilibrium(2.4e4, 0.72e4)
t.reaction_output("b15_u_out") == equilibrium(1000, 50)
b15 = t.reaction_output("b15_u_rxn_out").rate(0.0).after(5e-3)
b15[0:1] == equilibrium(2.4e4, 0.72e4)
b15[2:3] == equilibrium(4.0e4, 1.2e4)
b15[4:5] == equilibrium(8.0e4, 2.4e4)
t.reaction_output("b16_b_rxn_out").rate(0.0).after(5e-3) == equilibrium(5.1e4, 1.02e4)
b17 = t.reaction_output("b17_b_rxn_out").rate(0.0).after(5e-3)
b17[0] == equilibrium(7.26e4, 1.04e4)
b17[1] == equilibrium(3.63e4, 0.73e4)
b17[2] == equilibrium(2.18e4, 0.44e4)
  


