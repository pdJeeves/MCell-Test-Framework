"""
  Reactions test-12: Testing of the REACTION_DATA_OUTPUT statement
                     COUNT[name, region, ALL_ENCLOSED]
 
      Surface molecules A are placed on the surfaces of both "outer" 
      and "inner" cubes, such that "inner" cube is completely inside 
      "outer" one. The COUNT statement above written for "outer" 
      cube will effectively calculate number of molecules A for the 
      inner cube.

      The test is done on the collection of two cubes, one enclosed 
      inside the another. The grid molecules A are placed on the 
      surfaces of both cubes. The statement:
          COUNT[A,outer[whole_mesh], ALL_ENCLOSED]
      counts number of molecules A on the "inner" cube.

   Author: Boris Kaminsky <borisk@psc.edu>
   Date: 2011-03-25
"""

t = McellTest("12-all_enclosed_test")
t.reaction_output("A") == 100 

