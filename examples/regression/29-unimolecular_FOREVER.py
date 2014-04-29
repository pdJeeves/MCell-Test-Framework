"""
  Regression test 29: This test checks the correct use of FOREVER
                       as optimization design tool.

  There was a bug reported by Jacob Czech about reactions stalling.
  The study finds a bug in the use of FOREVER constant in the
  context of unimolecular reactions.

  Failure: The equilibrium number of molecules "S1{-1}" is exactly
           equal to the initial value (reactions stalled)
  Success: The equilibrium number of molecules "S1{-1}" is about the half
            of the initial value.

  Author: Boris Kaminsky and Jacob Czech
  Date:   2012-06-25
"""

t = McellTest("29-unimolecular_FOREVER")
S1_down = t.reaction_output("S1_down")
S1_down.min_time = 1e-3
S1_down.max_time = 1.5e-3
S1_down == equilibrium(10, 3)
