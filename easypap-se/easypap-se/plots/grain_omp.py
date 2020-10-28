#!/usr/bin/env python3
from expTools import *



options = {}
options["--label "] = ["final"]
options["-k "] = ["sable"]
options["-a "] = ["4partout"]
options["-of "] = ["./plots/data/perf_data.csv"]
options["-s "] = [480]
ompenv = {}
ompenv["OMP_NUM_THREADS="] = [1]
nbrun = 5
# OMP_TILED GRAIN 8 16 32
options["-v "] = ["tiled_stable"]
options["-g "] = [8,16,32,48,96]

execute('./run ', ompenv, options, nbrun, verbose=True, easyPath=".")

ompenv["OMP_NUM_THREADS="] = [1]
options["-v "] = ["tiled"]

execute('./run ', ompenv, options, nbrun, verbose=True, easyPath=".")

options["-a "] = ["alea"]
options["-of "] = ["./plots/data/perf_data.csv"]
options["-s "] = [960,1920,3840]
ompenv = {}
ompenv["OMP_NUM_THREADS="] = [1]
nbrun = 5
# OMP_TILED GRAIN 8 16 32
options["-v "] = ["tiled_stable"]
options["-g "] = [8,16,32,48,96]

execute('./run ', ompenv, options, nbrun, verbose=True, easyPath=".")

ompenv["OMP_NUM_THREADS="] = [1]
options["-v "] = ["tiled"]

execute('./run ', ompenv, options, nbrun, verbose=True, easyPath=".")
