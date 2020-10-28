# PAP

![Snapshot](Capture.png?raw=true "snapshot")

A simulation of pile sand abelien fall by using Parallel Archi Programming (OpenMP, OpenCL).

Before looking at this github project, please have a look on the basic code did by our teachers on [easypap](https://gforgeron.gitlab.io/easypap/).

You can also read the README.md that you'll find in the easypap-se folder, that has been made by our teachers. 

## Our work

As you can see on easypap, the sequential code is already done. Here, we had to do a parallel version by using OpenMP. 

But first of all we've optimized the sequential code to win performance in sequential mode, that is the first very important step, and that has to be do.

The file that we modified is sable.c in kernel/c/sable.c. This is a lot of a OpenMP version (tiled, ...). We also have modified kernel/ocl/sable.cl to make GPU optimization with OpenCL. 

## Our results

You can find our report at the root of the github. It's in french but it shows the result we obtains by using different optimization. also its shows all the different ways we work on to optimize calculations. 


