"""
  Test 08: Region border properties tested on sphere.

      The test is done on the sphere with complex region
      border geometry. Region  r1 boundary crosses the apex
      of the sphere. Region r1 borders are REFLECTIVE for
      molecule A, ABSORPTIVE for B, and TRANSPARENT for C.
      Initially we place 100 molecules of each type inside
      region r1. We check that all molecules A are contained
      within r1 and there are none of them in the surrounding
      region r2. Because B has high diffusion coefficient,
      they all get absorbed at the border, and after some 
      time we should register none of them inside either r1
      or r2. The total count of C as sum across regions r1
      and r2 is constant over the simulation. Region borders
      are REFLECTIVE for molecules D and E. We place 100
      molecules D there that also undergo unimolecular
      reaction D @ surface_class -> E[rate].  We check that
      total sum of D and E within the region r3 is equal to
      100. Also TRIGGER statements are checked.

      REFLECTIVE/ABSORPTIVE/TRANSPARENT properties of REGION
      border tested on sphere with much more complex shape
      of the region and boundary of the region crossing the
      sphere apex. Also tested HITS/CROSSINGS statements
      with COUNT/TRIGGER statements.


   Author: Boris Kaminsky <borisk@psc.edu>
   Date:  2010-12-08
"""

t = McellTest("08-region_borders")
    
A = t.reaction_output("A")
A[0] == 100
A[1] == 0
t.reaction_output("B") == 0
C = t.reaction_output("C")
C[0] + C[1] == 100
r2 = t.reaction_output("r2")
r2[0] + r2[1] == 100

hits_cross = t.reaction_output("A_hits_C_cross")
hits_cross[0] == hits_cross[2]
hits_cross[1] == 0
hits_cross.min_row = 1
hits_cross[3:5] > 0

for location in ["fr", "back", "all"]:
	for section in ["hits", "cross"]:
		file_name = "C_trigger_{0}_{1}".format(location, section)
		C_trigger = t.trigger_output(file_name)
		C_trigger["loc.x"] == equilibrium(0,10)
		C_trigger["loc.y"] == equilibrium(0,10)
		C_trigger["loc.z"] == equilibrium(0,10)


