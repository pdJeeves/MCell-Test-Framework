"""
  Regression test 15: Under certain circumstances, mol-grid-grid reactions
     could lead to a crash.  The wrong variable was being used to index into
     the array of matching reactions in the mol-grid-grid case, causing mcell
     to access outside the bounds of the array.  This may not reliably fail,
     and may depend on the compiler and architecture.
 
     Failure: crash
     Success: run exits successfully
 
  Author: Jed Wing <jed@salk.edu>
  Date:   2009-04-12
"""

McellTest("15-mol_grid_grid_crash")

