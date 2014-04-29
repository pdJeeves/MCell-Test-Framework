"""
  Regression test 06: Misreporting reaction products.

     This is in response to a bug report from Boris, who noticed that recent
     versions of MCell, when given reactions:

       a+b -> c [rate1]
       a+b -> d [rate2]
       a+b -> e [rate3]

      would give three probability reports with different probabilities, as
      appropriate, but the reports would all reference the same set of
      products.


     Failure: output includes lines which look like:
       Reaction probabilities generated for the following reactions:
         Probability 3.6790e-04 set for a{0} + b{0} -> c{0}
         Probability 7.3581e-04 set for a{0} + b{0} -> c{0}
         Probability 1.1037e-03 set for a{0} + b{0} -> c{0}

     Success: output includes lines which look like:
       Reaction probabilities generated for the following reactions:
         Probability 3.6790e-04 set for a{0} + b{0} -> c{0}
         Probability 7.3581e-04 set for a{0} + b{0} -> d{0}
         Probability 1.1037e-03 set for a{0} + b{0} -> e{0}

  Author: Jed Wing <jed@salk.edu>
  Date:   2008-09-16
"""

t = McellTest("06-misreporting_rxn_products", quiet=False)
c = 0
d = 0
e = 0
for line in open(t.stdout):
	if 'a\{0\} \+ b\{0\} ->' in line:
		if 'c\{0\}' in line:
			c = c + 1
		if 'd\{0\}' in line:
			d = d + 1
		if 'e\{0\}' in line:
			e = e + 1

if c != 1 or d != 1 or e != 1:
	print "Test '06-misreporting_rxn_products' failed."

