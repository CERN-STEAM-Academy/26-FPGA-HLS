create_project project_1 METProjAcceleratorVivado -part xc7z020clg400-1 -force

set_property board_part tul.com.tw:pynq-z2:part0:1.0 [current_project]
set_property  ip_repo_paths METProjAcceleratorHLS [current_project]
update_ip_catalog

create_bd_design "design_1"

create_bd_cell -type ip -vlnv xilinx.com:ip:processing_system7:5.5 processing_system_0

apply_bd_automation -rule xilinx.com:bd_rule:processing_system7 -config {make_external "FIXED_IO, DDR" apply_board_preset "1" Master "Disable" Slave "Disable" }  [get_bd_cells processing_system_0]

set_property -dict [list CONFIG.PCW_USE_S_AXI_HP0 {1}] [get_bd_cells processing_system_0]

create_bd_cell -type ip -vlnv cern.ch:MET:METAccelerator:0.0 METAccelerator

apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/processing_system_0/M_AXI_GP0} Slave {/METAccelerator/s_axi_control} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins METAccelerator/s_axi_control]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {Auto} Clk_slave {Auto} Clk_xbar {Auto} Master {/METAccelerator/m_axi_gmem0} Slave {/processing_system_0/S_AXI_HP0} ddr_seg {Auto} intc_ip {New AXI Interconnect} master_apm {0}}  [get_bd_intf_pins processing_system_0/S_AXI_HP0]

make_wrapper -files [get_files ./METProjAcceleratorVivado/project_1.srcs/sources_1/bd/design_1/design_1.bd] -top

add_files -norecurse ./METProjAcceleratorVivado/project_1.srcs/sources_1/bd/design_1/hdl/design_1_wrapper.v

reset_run impl_1
reset_run synth_1
launch_runs impl_1 -to_step write_bitstream -jobs 6
wait_on_run -timeout 360 impl_1

write_hw_platform -fixed -include_bit -force -file ./METProjAcceleratorVivado/METAccelerator.xsa

open_run impl_1
report_utilization -file util.rpt -hierarchical -hierarchical_percentages