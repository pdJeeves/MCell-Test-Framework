"""
  Test 03: Surface macromolecule list release.
 
     This test is to ensure that surface macromolecule list releases work
     properly.  The information is validated based on the time 0 viz and
     reaction data output.  The complexes are at the center of each face, the
     middle of each edge, and on each corner of the box.  The subunits should
     conform to the corners of the box as appropriate.  Molecule positions
     will be validated, as will the counts of molecules actually released in
     each orientation, and the fraction of dimer pairs exhibiting each
     possible orientation combination.
 
  Author: { 'Jed Wing' } <jed@salk.edu>
  Date:   { 2008/04/04 }
"""

t = MCellTest("03-macro_surface_listrelease")
t['counts'] == [26, 312, 208, 26, 52]

#    t.add_extra_check(CheckListReleasePositions('molecules.ascii.0.dat'))
#?????

