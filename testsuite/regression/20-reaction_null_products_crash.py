"""
  Regression test 20: A bug in the function
      "check_reaction_for_duplicate_pathways()"  results in segfault when
       presented with reaction with pathways (A->NULL[rate1], A->B[rate2]).
       Important here is that one of the pathways has zero products.
 
       Failure: *crash*
       Success: run exits successfully
 
  Author: Jacob Czech, Boris Kaminsky
  Date: 2010-11-04
"""

t = McellTest("20-reaction_null_products_crash")


