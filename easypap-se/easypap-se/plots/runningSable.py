#!/usr/bin/env python3
from graphTools import *
from expTools import *
import os

#8 THREADS tiled alea
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#8 THREADS tiled alea 
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#8 THREADS tiled_stable_omp alea

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -g 16 -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [8]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#12 THREADS tiled alea 480
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#12 THREADS tiled alea 
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#12 THREADS tiled_stable_omp alea 
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [12]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

#24 THREADS tiled alea 
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#24 THREADS tiled alea
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#24 THREADS tiled_stable_omp alea
easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 480"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 960"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 1920"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)

easyspap_options = {}
easyspap_options["--kernel "] = ["sable"]
easyspap_options["--variant "] = ["tiled_stable_omp -a alea -s 3840"]

omp_icv = {} 
omp_icv["OMP_NUM_THREADS="] = [24]
omp_icv["OMP_SCHEDULE="] = ["static"]
execute('./run', omp_icv, easyspap_options, nbrun=1)


#4partout

#easyspap_options = {}
#easyspap_options["--kernel "] = ["sable"]
#easyspap_options["--variant "] = ["tiled -a 4partout -s 480"]

#omp_icv = {} 
#omp_icv["OMP_NUM_THREADS="] = [12]
#omp_icv["OMP_SCHEDULE="] = ["static"]
#execute('./run', omp_icv, easyspap_options, nbrun=1)

#easyspap_options = {}
#easyspap_options["--kernel "] = ["sable"]
#easyspap_options["--variant "] = ["tiled_stable -a 4partout -s 480"]

#omp_icv = {} 
#omp_icv["OMP_NUM_THREADS="] = [12]
#omp_icv["OMP_SCHEDULE="] = ["static"]
#execute('./run', omp_icv, easyspap_options, nbrun=1)
