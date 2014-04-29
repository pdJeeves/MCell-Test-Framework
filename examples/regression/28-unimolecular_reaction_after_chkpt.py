"""
 Regression test 28: The results of unimolecular reaction with variable 
                     rection rate were incorrect due to the bug.

  This is a bug encountered by Jun Ma.  It represents itself as molecule's
  negative lifetime. It happened under two simultaneous conditions:
  checkpointing and unimolecular reactions with variable rates.
  It appeared that variable rates files were not properly read and
  and molecule's scheduling times were not properly set up.
  For this test file the bug showed up as number of molecules B
  equal to 10 right after checkpoint.

  Failure: final number of molecules B is 10
  Success: final number of molecules B is less than 10
"""

t = McellTest("28-unimolecular_reaction_after_chkpt", iterations=2)
t.reaction_output("B_World") < 10
