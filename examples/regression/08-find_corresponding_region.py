"""
  Regression test 08: Assertion fails when deep copying certain objects.

     This is a bug encountered by Daniel Sherwood (2008-04-24).  When a
     release site is cloned into a metaobject and the release site referred to
     a region in the original template object, and the fully qualified name of
     the release site was longer than the fully qualified name of the region,
     an assertion failure would occur due to an incorrect length calculation.

     Failure: MCell aborts (SIGABRT)
     Success: MCell does not crash

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-12-05
"""

McellTest("08-find_corresponding_region")

