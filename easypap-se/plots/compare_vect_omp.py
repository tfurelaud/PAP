#!/usr/bin/env python3
from expTools import *

options = {}
options["--label "] = ["final"]
options["-k "] = ["sable"]
options["-a "] = ["4partout"]
options["-s "] = [480]
ompenv = {}
nbrun = 1
#execute('./run ', ompenv, options, nbrun, verbose=False, easyPath=".")
# OMP_LINE static et dynamic

ompenv["OMP_NUM_THREADS="] = [2,12,24]
options["-g "] = [8,32,48]

options["-v "] = ["vect_omp_long"]
execute('./run ', ompenv, options, nbrun, verbose=False, easyPath=".")


ompenv["OMP_NUM_THREADS="] = [1]
options["-g "] = [8]

options["-v "] = ["seq_vect","seq"]

execute('./run ', ompenv, options, nbrun, verbose=False, easyPath=".")