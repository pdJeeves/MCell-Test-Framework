"""
  Regression test 09: Checkpoint file contains incorrect simulation times.

     This is a bug encountered by Jacob Czech (2009-01-16).  When a checkpoint
     file is created, if the time step has been changed between saving and
     loading the checkpoint, an incorrect "current time" is stored into the
     checkpoint file.  This will not have any immediate adverse effects; if,
     however, the simulation, after loading from the checkpoint, checkpoints
     again and reloads, times listed in the reaction data output files will be
     incorrect!

     Failure: Times jump from 1.5e-6 to 2.2e-6
     Success: Times jump from 1.5e-6 to 1.7e-6

  Author: Jacob Czech <jczech@psc.edu> (adapted by Jed Wing <jed@salk.edu>)
  Date:   2009-01-16
"""

McellTest("09-incorrect_times_in_chkpt_A")
t = McellTest("09-incorrect_times_in_chkpt_B", iterations=2)
A_World = t.reaction_output("A_World.dat")
A_World["Seconds"] == [i*1e-7 for i in range(0,6)] + [(2*i+1)*1e-7 for i in range(3, 13)]


