# High Level Synthesis

High Level Synthesis is the techniques and tooling around writing FPGA code in C++ that can be synthesized down to Hardware Description Language (HDL) code.
This course teaches some of the key concepts of writing efficient high level FPGA code for physics.
First of all, you will be introduced to numerics and arithmetic for FPGA, focussing on fixed point.
Secondly, you will learn how to understand and optimize loops, about pipelining, and how to explore the tradeoff between area, throughput, and latency.
Both of these concepts will be taught with some simple examples, including some by-hand calculations for fixed point arithmetic in the companion slides.
You will put the techniques into practice by writing an algorithm to compute the missing transverse energy (MET) from a list of particles.
Finally, we will put together what you've learned to run the algorithm on a pynq-z2 (time permitting).

The companion slides are available at the [STEAM academy indico](https://indico.cern.ch/event/1697464/timetable/#b-677766-fpga-programming-high). These provide some important introduction to each topic, as well as the by-hand exercises for fixed point arithmetic.

The sections are, in order:
1. arithmetic
2. MET
3. pynq

Explore the readme files in the sub directories for details of how to get started!

Author: Sioni Summers
Based on material first created for an [introductory course to VHDL and HLS FPGA programming](https://agenda.infn.it/event/45908/timetable/) for INFN ICSC, 2025.
