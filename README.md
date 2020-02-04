# VDF

## Meta
The goal of this project was to implement the Verifible Delay Function proposed by [Benjamin Wesolowski](http://bweso.com) in his [paper](https://eprint.iacr.org/2018/623).

Together with [Alessio Attanasio](alessio.attanasio@epfl.ch) and under the supervision of Novak Kaluderovic and Aymeric Genet in Arjen Lenstra's [LACAL](https://www.epfl.ch/labs/lacal/) laboratory at EPFL, 
we worked on this implementation in the context of our Bachelor's project.

We also wrote a report detailing our work and a few of our finding and is available upon request.

This code is available _as is_ and is no longer being worked on.

## Readme

The main code is contained in VDF/, the other python files and graphs were included for the purpose of benchmarking the speed.

#### VDF

Implements the original algrithm proposed in the paper. We also included the proposed speedup by parallelizing the proof generation.

#### Extra

Contains a few specific functions not directly related to groups or the VDF.

#### Group

This contains the implementation of the $Z/nZ$ group. To allow the use of other group structures such as class groups, we made this into a seperate file that can be swapped at compilation.

#### main

We hadn't had time to fully implement an interface to the VDF and thus the code must be changed to allow for desired values.
