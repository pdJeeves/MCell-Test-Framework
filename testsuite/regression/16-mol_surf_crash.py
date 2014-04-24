"""
  Regression test 16: Volume molecule + surface reactions, such as those used
     in absorptive or concentration clamped surfaces, were not properly
     checking whether any ENCLOSED (or other waypoint-requiring) counts had
     been requested.  This leads to a seg fault if no other waypoint-requiring
     features are needed, and some unnecessary computation otherwise.
     Also here we are testing keywords CLAMP_CONC, ESTIMATE_CONC,
     DIFFUSION_CONSTANT

     Failure: crash
     Success: run exits successfully

  Author: Jed Wing <jed@salk.edu>
  Date:   2009-06-15
"""

McellTest("16-mol_surf_crash")

