# PAP

![Snapshot](Capture.png?raw=true "snapshot")

A simulation of pile abelian sandfall using Parallel Programming (OpenMP, OpenCL).

Before looking at this github project, please take a look at the basic code made by our teachers on [easypap](https://gforgeron.gitlab.io/easypap/).

You can also read the README.md that you'll find in the easypap-se folder, which was created by our teachers. 

## Our work

As you can see on easypap, the sequential code is already done. Here, we had to do a parallel version using OpenMP. 

But first of all we've optimized the sequential code to gain performance in sequential mode, that is the very important first step, and it must to be done.

The file we modified is sable.c in kernel/c/sable.c. Its is mainly an OpenMP version (tiled, ...). We have also modified kernel/ocl/sable.cl to make GPU optimization with OpenCL. 

## Our results

You can find our report at the root of the github. It's in French but it shows the result we get using different optimizations. It also shows all the different ways we work on to optimize calculations. 


