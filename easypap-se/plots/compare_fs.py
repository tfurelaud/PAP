#!/usr/bin/env python3
from expTools import *

options = {}
options["--label "] = ["final"]
options["-k "] = ["sable"]
options["-of "] = ["./plots/data/perf_data.csv"]
options["-a "] = ["aleai"]
options["-s "] = [480, 960, 1920, 3840]
ompenv = {}
ompenv["OMP_NUM_THREADS="] = [1]
nbrun = 5
# OMP_TILED GRAIN 8 16 32

execute('./run -o -n ', ompenv, options, nbrun, verbose=True, easyPath=".")

options["-a "] = ["4partouti"]
options["-s "] = [480]

execute('./run -o -n ', ompenv, options, nbrun, verbose=True, easyPath=".")

options["-v "] = ["tiled"]
options["-g "] = [8]
execute('./run ', ompenv, options, nbrun, verbose=True, easyPath=".")

options["-s "] = [480, 960, 1920, 3840]
options["-a "] = ["aleai"]
options["-g "] = [8]
ompenv["OMP_SCHEDULE="] = ["dynamic"]

execute('./run ', ompenv, options, nbrun, verbose=True, easyPath=".")
#execute('./run ', ompenv, options, nbrun, verbose=False, easyPath=".")
# OMP_LINE static et dynamic




