"""
  Test-16: Test of part of the code related to surface products placement

      We test here some aspects of surface reactions in particular
      how surface products replace surface reactants. The detailed
      description of the policies is in the document
      "src/docs/policy_surf_products_placement.doc"
      
      The test is done is done on the number of cubes. Here we test 
      how surface products replace surface reactants that should 
      follow the established policy. Mostly here we look into product 
      generation and no segfaults during simulation.

  Author: Boris Kaminsky <borisk@psc.edu>
  Date: 2011-09-15
"""
 
t = McellTest("16-surf_products_replace_reactants")
t.reaction_output("box_1") > 0
t.reaction_output("box_2") > 0
t.reaction_output("box_3") > 0
t.reaction_output("box_4_I") > 0
t.reaction_output("box_4_b_A3") > 0
t.reaction_output("box_5") > 0
t.reaction_output("box_6") > 0
t.reaction_output("box_7") > 0
t.reaction_output("box_8") > 0
t.reaction_output("box_9") > 0
t.reaction_output("box_10") > 0
t.reaction_output("box_11") > 0
t.reaction_output("box_12") > 0

