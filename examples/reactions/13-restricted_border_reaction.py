"""
  Test-13: Surface reactants cannot search for reaction partners
           across restrictive region borders.

      If all of the reactants are behind restrictive region 
      border (REFLECTIVE/ABSORPTIVE) the reaction should not 
      happen. Restrictive border is considered "inside-out" 
      when it is e.g. REFLECTIVE relative to the molecule's 
      own region, or "outside-in" when REFLECTIVE to the 
      neighbor region.

      We test here the property that reactant cannot look for 
      potential reaction partners across the restrictive region 
      border.  Static surface molecules occupy all available 
      slots on the meshes.

      The test is done on five cubes with simple region border 
      geometry. For bimolecular reaction molecules b_A are 
      placed on the RIGHT side of the cubes, and molecules b_B 
      are placed on the TOP side. For trimolecular reaction 
      molecules t_A and t_B are placed on RIGHT side of the cube
      and molecules t_C are placed on TOP side of the cube. We 
      declare the region border between TOP and RIGHT restrictive 
      in several different ways.  There should be no reactions 
      and therefore no reaction products D for boxes 1-4. For 
      box_5 the region border is not restrictive and so reactions
      happened and reaction products are created.

   Author: Boris Kaminsky <borisk@psc.edu>
   Date: 2011-06-27
"""

t = McellTest("13-restricted_border_reaction")
    
t.reaction_output("box_1") == 0
t.reaction_output("box_2") == 0
t.reaction_output("box_3") == 0
t.reaction_output("box_4") == 0
t.reaction_output("box_6") == 0
t.reaction_output("box_7") == 0
t.reaction_output("box_5") > 0
t.reaction_output("box_8") > 0

