"""
  Regression test 05: When a surface molecule undergoes a reaction to become a
        volume molecule, orientation must be respected.

        Failure: as g decreases, mi increases faster than m

        Success: at all times m == mi
                 at all times m + g == 1000

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-09-10
"""

t = McellTest("05-rx_dissociate_inwards")
molecules = t.reaction_output("molecules.txt")
molecules[0] + molecules[1] == 1000
molecules[1] == molecules[2]

