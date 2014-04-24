"""
   Test 010: Combination of ALL_MOLECULES with surface properties
             (REFLECTIVE/TRANSPARENT/ABSORPTIVE) 

      Volume molecules are released inside a set of enclosed meshes.
      Let some of the meshes be REFLECTIVE, or TRANSPARENT, or ABSORPTIVE.
      This test check validity of ALL_MOLECULES statement in combination
      with REFLECTIVE/TRANSPARENT/ABSORPTIVE properties of meshes.
       
      The test is done on the collection of two cubes, one enclosed inside
      the another.  The outer cube has REFLECTIVE mesh for ALL_MOLECULES.
      The properties of inner cubes change from REFLECTIVE to TRANSPARENT 
      to ABSORPTIVE for ALL_MOLECULES as we move from one collection 
      to another.  Volume molecules A,B,C are released inside the inner cube.
      We count molecules in the space between the cubes. 

  Author: Boris Kaminsky <borisk@psc.edu>
  Date: 2011-03-04
"""

t = McellTest("10-surf_properties_all_molecules")
t.reaction_output("refl") == 0
t.reaction_output("absorp") == 0
t.reaction_output("transp") >= 0

