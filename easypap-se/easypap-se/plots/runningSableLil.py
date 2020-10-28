#!/usr/bin/env python3
from graphTools import *
from expTools import *
import os

#8 THREADS seq alea 480
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["seq -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 3840 -g 120"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#8 THREADS tiled alea 480