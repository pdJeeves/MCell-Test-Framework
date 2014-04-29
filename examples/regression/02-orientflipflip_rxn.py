"""
  Regression test 02: In a grid mol + vol mol reaction V' + G' -> V, + G,
     (i.e. where both molecules are preserved and change orientation), the
     updated orientation of the grid molecule is not counted.

     Failure: a_inner != b_outward or a_outer != b_inward

     Success: a_inner == b_outward && a_outer == b_inward
              a_inner + a_outer == a_total == 1000
              b_inward + b_outward == b_total == 1000

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-09-05
"""

t = McellTest("02-orientflipflip_rxn")
counts = t.reaction_output("counts.txt")
counts["a_inner"] == counts["b_outward"]
counts["a_outer"] == counts["b_inward"]
counts["a_inner"]  + counts["a_outer"]  == counts["a_total"]
counts["b_inward"] + counts["b_outward"]== counts["b_total"]
counts["a_total"] == 1000
counts["b_total"] == 1000
