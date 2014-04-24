"""
  Test 07: Region borders properties tested on cube.

      REFLECTIVE/ABSORPTIVE/TRANSPARENT properties of 
      REGION border tested on the cube.

      The test is done on the cube with simple region 
      border geometry. Region r1 borders are REFLECTIVE 
      for molecule A, ABSORPTIVE for B, and TRANSPARENT 
      for C.  Initially we place 100 molecules of each 
      type inside region r1. We check that all molecules 
      A are contained within r1 and there are none of 
      them in the surrounding region r2. Because B has 
      high diffusion coefficient, they all get absorbed
      at the border, and after some time we should reg-
      ister none of them inside either r1 or r2. The 
      total count of C as sum across regions r1 and r2 
      is constant over the simulation. Region r3 borders 
      are REFLECTIVE for molecules D and E. We place 100 
      molecules D there that also undergo unimolecular 
      reaction D @ surface_class -> E[rate].  We check 
      that total sum of D and E within the region r3 is 
      equal to 100. 

   Author: Boris Kaminsky <borisk@psc.edu>
   Date: 2010-11-30
"""
    
t = McellTest("07-region_borders")
    
box_1_A = t.reaction_output("box_1_A")
box_1_A[0] == 100
box_1_A[1] == 0
t.reaction_output("box_1_B") == 0
box_1_C = t.reaction_output("box_1_C")
box_1_C[0] + box_1_C[1] == 100

box_1_r3 = t.reaction_output("box_1_r3")
box_1_r3[0] + box_1_r3[1] == 100 

t.reaction_output("box_2_r4.") == 0
t.reaction_output("box_2_A.") > 0
t.reaction_output("box_3_r5") == 200
t.reaction_output("box_3_r6") == 0

