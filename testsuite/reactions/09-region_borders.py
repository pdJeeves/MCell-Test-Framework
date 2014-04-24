"""
  Test 09: Region borders properties tested on cube.

      The test is done on the six cubes with simple region 
      border geometry. Region r1 is TOP, and region r2 consist
      of all other sides. For each of the above cubes region 
      r1 is assigned a surface class that has properties such 
      as TRANSPARENT/REFLECTIVE/ABSORPTIVE for ALL_MOLECULES 
      or ALL_SURFACE_MOLECULES. Initially we place surface 
      molecules A and B in the amount of 100 molecules of each
      type inside region r1. We count molecules A and B in the 
      regions r1 and r2.

      REFLECTIVE/ABSORPTIVE/TRANSPARENT properties of REGION border tested 
      on the cube for ALL_MOLECULES and ALL_SURFACE_MOLECULES keywords.
 
   Author: Boris Kaminsky <borisk@psc.edu>
   Date: 2011-03-04
"""

t = McellTest("09-region_borders")

refl_all_mols = t.reaction_output("refl_all_mols")
refl_all_mols[0:1] == 100
refl_all_mols[2:3] == 0

refl_all_surface_mols = t.reaction_output("refl_all_surface_mols")
refl_all_surface_mols[0:1] == 100
refl_all_surface_mols[2:3] == 0

t.reaction_output("absorb_all_mols") == 0
t.reaction_output("absorb_all_surface_mols") == 0

transp_all_mols = t.reaction_output("transp_all_mols");
transp_all_mols[0] + transp_all_mols[1] + transp_all_mols[2] + transp_all_mols[3] == 200;

transp_all_surface_mols = t.reaction_output("transp_all_surface_mols");
transp_all_surface_mols[0] + transp_all_surface_mols[1] + transp_all_surface_mols[2] + transp_all_surface_mols[3] == 200

