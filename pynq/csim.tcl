# Project
open_project -reset "METProj"
add_files -tb ../MET/pt.csv
add_files -tb ../MET/phi.csv
add_files -tb testbench.cpp
# TODO replace with your implementation
add_files example_met.cpp
add_files met_pynq.cpp

# Solution
open_solution -reset "solution"
set_part {xc7z020clg400-1}
create_clock -period 10.0 -name default

# Simulation
csim_design -ldflags "-fuse-ld=gold"

exit

