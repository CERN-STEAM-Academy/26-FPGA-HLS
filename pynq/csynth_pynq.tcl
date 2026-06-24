# Project
open_project -reset "METProjAcceleratorHLS"
# TODO: replace this with your own MET implementation!
add_files ../MET/dummy_met.cpp -cflags "-I/home/sioni/Work/fpga_teaching/MET"
add_files met_pynq.cpp -cflags "-I/home/sioni/Work/fpga_teaching/MET"
set_top met_accelerator

# Solution
open_solution -reset "solution" -flow_target vivado
set_part {xc7z020clg400-1}
create_clock -period 10.0 -name default

config_interface -m_axi_addr64=false

# Synthesis
csynth_design

# Export
export_design -vendor cern.ch -library MET -ipname METAccelerator -version 0.0 -format ip_catalog

exit

