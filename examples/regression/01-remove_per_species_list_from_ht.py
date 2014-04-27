"""
  Regression test 01: Memory corruption when attempting to remove a
     per-species list from the hash table.

     This is a bug encountered by Shirley Pepke (2008-04-24).  When a
     per-species list is removed from the hash table, if the hash table has a
     collision for the element being removed, and the element being removed
     was not the first element (i.e. was the element which originally
     experienced the collision), memory could be corrupted due to a bug in the
     hash table removal code.

     Failure: MCell crashes
     Success: MCell does not crash (eventually all molecules should be
              consumed, and the sim should run very fast)

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-04-24
"""

McellTest("01-remove_per_species_list_from_ht")

