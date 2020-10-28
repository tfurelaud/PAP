#!/usr/bin/env python3
from expTools import *

options = {}
options["--label "] = ["final"]
options["-k "] = ["sable"]
options["-a "] = ["4partout"]
options["-of "] = ["./plots/data/perf_data.csv"]
options["-s "] = [480]
ompenv = {}
ompenv["OMP_NUM_THREADS="] = [1,8,12,24]
nbrun = 1
# OMP_TILED GRAIN 8 16 32
options["-v "] = ["tiled_stable_omp"]
options["-g "] = [8]
ompenv["OMP_SCHEDULE="] = ["dynamic"]
4
#execute('./run ', ompenv, options, nbrun, verbose=False, easyPath=".")
# OMP_LINE static et dynamic
options["-v "] = ["tiled_omp_vect","tiled_stable_omp","tiled"]
execute('./run ', ompenv, options, nbrun, verbose=False, easyPath=".")