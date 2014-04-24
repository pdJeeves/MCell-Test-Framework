"""
 Test 01: Simple volume macromolecules.

    This case is simple enough that it may be solved analytically.  When I
    was originally working on the macromolecules, I compared against the
    analytically derived solution.

    An analyically derived solution is:
        d_0 ~31.1
        d_1 ~30.0
        d_2 ~28.9

    where d_0 is the number of dimer partners in the doubly-unbound state,
    d_1 is the number in the singly-bound state, and d_2 is the number in the
    doubly-bound state.  The counts below relate in the following way:

      00 -> 2*d_0     (number of subunits in doubly unbound dimer pairs)
      01 -> d_1       (number of bound subunits with unbound partners)
      10 -> d_1       (number of unbound subunits with bound partners)
      11 -> 2*d_2     (number of subunits in doubly bound dimer pairs)
      us -> 2*d_0+d_1 (number of unbound subunits)
      bs -> 2*d_2+d_1 (number of unbound subunits)
      cam -> 800 - 2*d_2 - d_1    (number of free Cam)

    01 and 10 should be in agreement at all times.

 Author: Jed Wing <jed@salk.edu>
 Date:   2008-04-04
"""

t = McellTest("01-macro.mdl")
counts = t.reaction_output("counts")
counts[0] + counts[1] + counts[2] + counts[3] == 180
counts[1] + counts[3] + counts[6] == 800
counts[1] == counts[2]
counts[0] + counts[1] == counts[4]
counts[2] + counts[3] == counts[5]

counts.min_time = 0.001
counts[0] == equilibrium(62.2, 3.0)
counts[1:2] == equilibrium(30.0, 1.5)
counts[3] == equilibrium(57.8, 3.0)
counts[4] == equilibrium(92.1, 4.5)
counts[5] == equilibrium(87.9, 4.5)
counts[6] == equilibrium(712.1, 10.0)

