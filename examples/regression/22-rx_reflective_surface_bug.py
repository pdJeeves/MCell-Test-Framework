"""
  Test 22: Testing reaction between volume molecule and surface
           when surface class is declared REFLECTIVE and when it is
           REFLECTIVE by default.
 
 	Volume molecules A are released inside boxes "box_refl", and B
        inside  "box_default".
 	Let one box "box_refl" interior be REFLECTIVE through SURFACE_CLASS
        "my_refl".  Let another box "box_default" also has interior
        REFLECTIVE but through the SURFACE_CLASS "my_default" (in fact
        default property).  
 
        Reflective surfaces we can specify directly through
        SURFACE_CLASS "my_refl{REFLECTIVE = A}" or indirectly
        through SURFACE_CLASS "my_default {} " using the fact that 
        all surfaces by default are REFLECTIVE.
 
        The bug in the code prevented reaction 
        of the type vol_mol' @ my_refl; -> ... [rate] to happen, while
        reaction of the type vol_mol' @ my_default; -> ...[rate]
        did happened.
 
        Here we test the bug fix.
 
  Author: Boris Kaminsky <borisk@psc.edu>
  Date: 2011-01-14
"""

t = McellTest("22-rx_reflective_surface_bug")
t.reaction_output("refl") > 0

