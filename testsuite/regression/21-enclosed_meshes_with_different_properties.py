"""
   Volume molecules are released inside a set of enclosed meshes.
   Let some of the meshes be located very close one to another
   so that they are within the molecule random walk distance and 
   they have different surface properties, say REFLECTIVE and TRANSPARENT.
   The bug in the diffusion code have resulted in leaking molecules 
   through the REFLECTIVE mesh.   
"""

t = McellTest("21-enclosed_meshes_with_different_properties")
t.reaction_output("A") == 0

