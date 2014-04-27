"""
   Test-14: Surface products placement in the presence of restrictive 
           region borders.
    
      The policy of surface products placement is described in detail 
      in the comments to the function "outcome_products_random()" and 
      "outcome_products_trimol_reaction_random()" and also in policy 
      document in "docs" folder.

      The test is done on 19 cubes with simple region border geometry.
      Unimolecular reaction is tested on the boxes 1 and 2, bimolecular
      reaction is tested on boxes 3 and 4, and trimolecular reaction on 
      boxes 5 and 6. For these boxes we define region border properties
      as restrictive for some reaction products and in some cases for 
      the reactant(s). For boxes 1, 3, and 5 there should be no products 
      outside restrictive region r1 boundary. For boxes 2, 4, and 6 pro-
      ducts F and G should be placed outside region r1 since its borders 
      are not restrictive for them.


    Author: Boris Kaminsky <borisk@psc.edu>
    Date: 2011-08-24
"""

t = McellTest("14-restricted_border_product_placement")
t.reaction_output("box_1_r1") >  0
t.reaction_output("box_1_r3") == 0
t.reaction_output("box_2_r2") == 0
t.reaction_output("box_2_r3") >  0
t.reaction_output("box_3_r1") >  0
t.reaction_output("box_3_r2") >  0
t.reaction_output("box_4_r1") >  0
t.reaction_output("box_4_r2") == 0
t.reaction_output("box_5_r3") >  0
t.reaction_output("box_5_r4") == 0
t.reaction_output("box_6_r1") >  0
t.reaction_output("box_6_r5") == 0
t.reaction_output("box_7_r1") >  0
t.reaction_output("box_7_r3") == 0
t.reaction_output("box_8_r1") >  0
t.reaction_output("box_8_r4") == 0
t.reaction_output("box_9_r1") >  0
t.reaction_output("box_9_r2") == 0
t.reaction_output("box_10_r1") >  0
t.reaction_output("box_10_r2") >  0
t.reaction_output("box_11_r1") >  0
t.reaction_output("box_11_r2") == 0
t.reaction_output("box_12_r5") >= 0
t.reaction_output("box_12_r6") == 0
t.reaction_output("box_13_r1") >  0
t.reaction_output("box_13_r3") == 0
t.reaction_output("box_14_r1") >  0
t.reaction_output("box_14_r3") == 0
t.reaction_output("box_15_r1") >  0
t.reaction_output("box_15_r2") == 0
t.reaction_output("box_16_r1") >  0
t.reaction_output("box_16_r2") == 0
t.reaction_output("box_17_r1") >  0
t.reaction_output("box_17_r2") == 0
t.reaction_output("box_18_r1") >  0
t.reaction_output("box_18_r2") == 0
t.reaction_output("box_19_r1") >  0
t.reaction_output("box_19_r2") == 0

