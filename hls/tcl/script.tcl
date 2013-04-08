############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project zynqBTC
set_top sha256_top
add_files sha256.c
add_files -tb sha256_tb.c
open_solution "solution2"
set_part  {xc7z020clg484-1}
create_clock -period 5

source "./zynqBTC/solution2/directives.tcl"
csynth_design
